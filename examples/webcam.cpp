#include "learning/facerecognizer.hpp"
#include "detection/facedetector.hpp"
#include "openface/openface.hpp"
#include "database/facedatabase.hpp"
#include <ctime>
#include <sstream>

using namespace std;
using namespace dlib;

int main()
{
    try
    {
        cv::VideoCapture cap(0);
        cv::namedWindow( "Face window", cv::WINDOW_AUTOSIZE);
        image_window win;
        OpenFace of("resources/shape_predictor_68_face_landmarks.dat",
            "src/openface/forward_nn.lua", "resources/nn4.v2.t7");
        FaceDetector fd("resources/haarcascade_frontalface_alt.xml",
                        "");

        FaceRecognizer fr;
        try {
            fr.load("facedatabase.dat");
        }
        catch(serialization_error& err) {
            cout << "No learned database found." << endl;
            cout << "Please create a database with the ./examples/database_builder" << endl;
            cout << "Afterwards learn the decision function using ./examples/database_processor" << endl;
            cout << endl << err.what() << endl;
        }


        //Grab and process frames until the main window is closed by the user.
        while(!win.is_closed())
        {
            // Grab a frame
            cv::Mat temp;
            cap >> temp;

            Image img(temp);

            Detection detect = fd.dlib_detect(img);
            if (detect.face.width() == 0) {
                win.set_image(img.asDLIBImage());
                continue;
            }
            //fa.align(detect);
            FaceNetEmbed rep = of.facenet(detect);

            auto df = fr.df();
            auto result = df.predict(rep);
            if (result.second > 0.5) {
                std::stringstream ss;
                ss << result.first << ", " << result.second;
                std::string label = ss.str(); //fr.recognize(rep);
                // Display it all on the screen
                win.clear_overlay();
                win.set_image(img.asDLIBImage());
                win.add_overlay(detect.rect.asDLIBRect(),rgb_pixel(255,0,0), label);
            }
            else {
                // Display it all on the screen
                win.clear_overlay();
                win.set_image(img.asDLIBImage());
                win.add_overlay(detect.rect.asDLIBRect(),rgb_pixel(255,0,0), std::to_string(result.second));
            }
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
