#include <iostream>
#include <sstream>
#include <map>
#include <boost/filesystem.hpp>
#include "openface/openface.hpp"
#include "detection/facedetector.hpp"
#include "database/facedatabase.hpp"
#include "learning/facerecognizer.hpp"

using namespace std;

void align_faces(string root, string path) {
    FaceDetector fd("resources/haarcascade_frontalface_alt.xml", "");
    FaceAligner fa("resources/shape_predictor_68_face_landmarks.dat");
    ImageDatabase db(50);

    if (path.empty()) {
        stringstream ss;
        ss << root << "/../" << "aligned";
        path = ss.str();
    }

    db.load(root);
    Batch<Image> batch;
    map<string, int> files;

    // Make sure the directories exist
    for (size_t i = 0; i < db.subjects().size(); i++) {
        boost::filesystem::path subject_path(path+string("/")+db.subjects()[i]);
        boost::system::error_code returnedError;
        boost::filesystem::create_directories(subject_path, returnedError);
        if (returnedError) {
            cerr << "Error creating directory " << path << '/' << db.subjects()[i] << endl;
        }
        files[db.subjects()[i]] = 0;
    }

    cout << "Starting Face alignment" << endl;
    cout << db.batches() << " batches found. " << endl;

    for (size_t i = 0; i < db.batches(); i++) {
        cout << "Batch " << i << " ... ";
        batch = db.batch(i);
        std::vector<Detection> rawds = fd.detect(batch.samples);
        std::vector<Detection> ds;
        for(auto d : rawds) {
            if (d.rect.width() > 0)
                ds.push_back(d);
        }
        fa.align(ds);
        for (size_t j = 0; j < ds.size(); j++) {
            stringstream ss;
            ss << path << '/' << batch.labels[j] << '/' << files[batch.labels[j]]++ << ".png";
            ds[j].face.save(ss.str());
        }
        cout << "finished." << endl;
    }
}

void vectorize(string root, string path) {
    NeuralNetwork nn("src/openface/forward_nn.lua", "resources/nn4.v2.t7");
    ImageDatabase db(50);

    if (path.empty()) {
        stringstream ss;
        ss << root << "/../" << "vectorized";
        path = ss.str();
    }

    db.load(root);
    Batch<Image> batch;
    map<string, int> files;

    // Make sure the directories exist
    for (size_t i = 0; i < db.subjects().size(); i++) {
        boost::filesystem::path subject_path(path+string("/")+db.subjects()[i]);
        boost::system::error_code returnedError;
        boost::filesystem::create_directories(subject_path, returnedError);
        if (returnedError) {
            cerr << "Error creating directory " << path << '/' << db.subjects()[i] << endl;
        }
        files[db.subjects()[i]] = 0;
    }

    cout << "Starting Face vectorization" << endl;
    cout << db.batches() << " batches found. " << endl;

    for (size_t i = 0; i < db.batches(); i++) {
        cout << "Batch " << i << " ... ";
        batch = db.batch(i);
        std::vector<FaceNetEmbed> mappings = nn.forward_nn(batch.samples);
        for (size_t j = 0; j < mappings.size(); j++) {
            stringstream ss;
            ss << path << '/' << batch.labels[j] << '/' << files[batch.labels[j]]++ << ".dat";
            dlib::serialize(ss.str()) << mappings[j];
        }
        cout << "finished." << endl;
    }
}

void learn(string root, string path) {
    FaceRecognizer fr;
    FaceNetEmbedDatabase db(100000);

    db.load(root);
    Batch<FaceNetEmbed> batch = db.batch(0);

    fr.train(batch.samples, batch.labels);
    dlib::serialize(root+string("/../db.dat")) << fr.df();
}

void usage() {
    cout << "Usage: " << endl;
    cout << "./database_processor <root_dir> operation" << endl;
    cout << "[Operations]" << endl << endl;
    cout << "   align [dest]: Detects and alignes the faces and stores them in a directory " << endl;
    cout << "                 next to the root called aligned_faces or <dest> if given." << endl;
    cout << "   vectorize [dest]: Vectorizes aligned faces and stores them in a directory " << endl;
    cout << "                     called vectorized or <dest> if given." << endl;
    cout << "   learn [dest]: Trains a decision function based on the given vectorized faces" << endl;
    cout << "                 and saves a serialized function to [dest] if given." << endl;
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        usage();

        return 0;
    }

    string root(argv[1]);
    string operation(argv[2]);

    if (operation.compare("align") == 0) {
        string dest;
        if (argc >= 4)
            dest = string(argv[3]);
        align_faces(root, dest);
    }
    else if (operation.compare("vectorize") == 0) {
        string dest;
        if (argc >= 4)
            dest = string(argv[3]);
        vectorize(root, dest);
    }
    else if (operation.compare("learn") == 0) {
        string dest;
        if (argc >= 4)
            dest = string(argv[3]);
        learn(root, dest);
    }
    else
        usage();


    return 0;
}
