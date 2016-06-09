#include "openface/openface.hpp"
#include "learning/facerecognizer.hpp"
#include "detection/facedetector.hpp"
#include "database/facedatabase.hpp"

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/String.h>


class OpenFaceRecognizer {
    FaceRecognizer fr_;
    OpenFace of_;
    FaceDetector fd_;

    ros::NodeHandle nh_;
    ros::Publisher name_pub;
    image_transport::ImageTransport it_;

    image_transport::Subscriber image_sub;

public:
    void imageCb(const sensor_msgs::ImageConstPtr& msg);

    OpenFaceRecognizer();
    ~OpenFaceRecognizer() {}
};


OpenFaceRecognizer::OpenFaceRecognizer() : it_(nh_) {
    image_sub = it_.subscribe("/usb_cam/image_raw", 1, &OpenFaceRecognizer::imageCb, this, image_transport::TransportHints::TransportHints("theora"));
    name_pub = nh_.advertise<std_msgs::String>("/facerecognizer/name", 1);
    fr_.load("facedatabase.dat");
}

void OpenFaceRecognizer::imageCb(const sensor_msgs::ImageConstPtr& msg) {
    cv_bridge::CvImageConstPtr cv_ptr; // for shared msg - data not mutable
    try {
        cv_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::Mat msg_img = cv_ptr->image;
    Image img(msg_img);
    std::cout << img.width() << std::endl;
    Detection detect = fd_.detect(img);
    if (detect.face.width() == 0)
        return;

    FaceNetEmbed rep = of_.facenet(detect.face);

    std::string label = fr_.recognize(rep);

    std::cout << "Recognized: " << label << std::endl;
    name_pub.publish(label);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "decos_openface");
    OpenFaceRecognizer ofr;
    ros::spin();
    return 0;
}
