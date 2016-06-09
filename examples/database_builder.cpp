#include "detection/facedetector.hpp"
#include "openface/neuralnetwork.hpp"
#include "openface/facealigner.hpp"
#include <ctime>
#include <cstdlib>
#include <sstream>

#define DATABASE_DIR "data/"

using namespace dlib;
using namespace std;

int main(int argc, char const *argv[])
{
    try {
        cv::VideoCapture cap(0);
        image_window win;

        FaceDetector fd("resources/haarcascade_frontalface_alt.xml", "");
        FaceAligner fa("resources/shape_predictor_68_face_landmarks.xml");

        system((std::string("mkdir -p ")+std::string(DATABASE_DIR)+std::string("/raw/")+std::string(argv[1])).c_str());
        system((std::string("mkdir -p ")+std::string(DATABASE_DIR)+std::string("/aligned_faces/")+std::string(argv[1])).c_str());

        int i = 0;

        //Grab and process frames until the main window is closed by the user.
        while(!win.is_closed() && i < 65)
        {
            // Grab a frame
            cv::Mat temp;
            cap >> temp;
            stringstream raw_path;
            stringstream processed;
            raw_path << DATABASE_DIR << "/raw/" << argv[1] << "/" << i <<".png";
            processed << DATABASE_DIR << "/aligned_faces/" << argv[1] << "/" << i <<".png";
            Image frame(temp);
            frame.save(raw_path.str().c_str());
            Detection d = fd.detect(frame);
            fa.align(d);
            if (d.rect.area() > 0 ) {
                d.face.save(processed.str());
                i++;
            }

            win.clear_overlay();
            win.set_image(cv_image<rgb_pixel>(temp));
            win.add_overlay(d.rect.asDLIBRect(),rgb_pixel(255,0,0), std::to_string(65-i));
        }
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
}
