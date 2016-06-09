#include "learning/facerecognizer.hpp"
#include "detection/facedetector.hpp"
#include "openface/openface.hpp"
#include "database/facedatabase.hpp"
#include <ctime>

using namespace std;
using namespace dlib;

int main()
{
    OpenFace of("resources/shape_predictor_68_face_landmarks.dat",
        "src/openface/forward_nn.lua", "resources/nn4.v2.t7");
    FaceDetector fd("resources/haarcascade_frontalface_alt.xml",
                    "");

    FaceRecognizer fr;
    FaceNetEmbedDatabase db(10000);
    db.load("database/vectorized");
    Image img("snapshot.jpg");

    Detection detect = fd.dlib_detect(img);
    if (detect.face.width() == 0) {
        return -1;
    }
    //fa.align(detect);
    FaceNetEmbed rep = of.facenet(detect);

    typedef one_vs_one_trainer<any_trainer<FaceNetEmbed, float>, string> ovo_trainer;
    typedef one_vs_all_trainer<any_trainer<FaceNetEmbed, float>, string> all_trainer;
    ovo_trainer trainer;
    svm_c_trainer<linear_kernel<FaceNetEmbed>> linear_trainer;
    linear_trainer.set_kernel(dlib::linear_kernel<FaceNetEmbed>());
    linear_trainer.set_c(10);
    trainer.set_trainer(probabilistic(linear_trainer, 3));

    all_trainer alltrainer;
    alltrainer.set_trainer(probabilistic(linear_trainer, 3));

    Batch<FaceNetEmbed> batch = db.batch(0);
    one_vs_one_decision_function<ovo_trainer, probabilistic_function< decision_function< dlib::linear_kernel<FaceNetEmbed> > > > df_ = trainer.train(batch.samples, batch.labels);
    one_vs_all_decision_function<all_trainer, probabilistic_function< decision_function< dlib::linear_kernel<FaceNetEmbed> > > > alldf_ = alltrainer.train(batch.samples, batch.labels);
    cout << "one vs one " <<  df_(rep) << endl;
    cout << "one vs all " << alldf_(rep) << endl;
    auto table = df_.get_binary_decision_functions();
    auto alltable = alldf_.get_binary_decision_functions();

    for (auto it = table.begin(); it != table.end(); it++) {
        std::cout << "Names: " << it->first.first << ", " << it->first.second << std::endl;
        std::cout << "second: " << it->second(rep) << std::endl;
        std::cout << "Decision: ";
        if (it->second(rep) > 0)
            std::cout << it->first.first << std::endl;
        else
            std::cout << it->first.second << std::endl;
    }

    for (auto it = alltable.begin(); it != alltable.end(); it++) {
        std::cout << "Name: " << it->first <<  std::endl;
        std::cout << "second: " << it->second(rep) << std::endl;
    }

}
