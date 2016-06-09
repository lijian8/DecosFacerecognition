#include "learning/facerecognizer.hpp"

FaceRecognizer::FaceRecognizer() {}

void FaceRecognizer::train(std::vector<FaceNetEmbed> faces, std::vector<std::string> labels) {
    ova_trainer trainer;

    dlib::svm_c_trainer<linear_kernel> linear_trainer;
    linear_trainer.set_kernel(linear_kernel());
    linear_trainer.set_c(10);
    trainer.set_trainer(probabilistic(linear_trainer, 3));

    df_ = trainer.train(faces, labels);
}

std::pair<std::string, float> FaceRecognizer::recognize(FaceNetEmbed s) {
    return df_.predict(s);
}

void FaceRecognizer::load(const std::string& path) {
    dlib::deserialize(path) >> df_;
}
