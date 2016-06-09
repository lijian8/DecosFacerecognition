#include "openface/facealigner.hpp"
#include "openface/settings.hpp"

FaceAligner::FaceAligner(const std::string& shape_path) {
    load(shape_path);
}

void FaceAligner::load(const std::string& shape_path) {
    dlib::deserialize(shape_path) >> pose_model;

    initialized_ = true;
}

dlib::full_object_detection FaceAligner::predict(const Detection& d) const {
    return pose_model(d.face.asDLIBImage(), d.rect.asDLIBRect());
}

void FaceAligner::align(std::vector<Detection>& ds) const {
    for (size_t i = 0; i < ds.size(); i++) {
        align(ds[i]);
    }
}

void FaceAligner::align(Detection& d) const {
    // Find the pose of each face.
    dlib::full_object_detection shape = predict(d);

    cv::Point2f landmarks[] = {
        cv::Point2f(shape.part(36).x(), shape.part(36).y()),
        cv::Point2f(shape.part(45).x(), shape.part(45).y()),
        cv::Point2f(shape.part(33).x(), shape.part(33).y()),
    };
    cv::Mat H = cv::getAffineTransform(landmarks, OUTER_EYES_AND_NOSE);
    d.face.warpAffine(H, cv::Size(FACE_SIZE_CONSTRAINT, FACE_SIZE_CONSTRAINT));

    // private access because Aligner is friend class of Face
    // face.align();
}
