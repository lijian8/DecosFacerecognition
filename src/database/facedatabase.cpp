#include "database/facedatabase.hpp"
#include "core/support.hpp"

#include <sstream>

void ImageDatabase::load_subjects() {
    subjects_ = list_directories(root_);
}

Image ImageDatabase::load_sample(const std::string& path) {
    return Image(path);
}

std::vector<std::string> ImageDatabase::subject_samples(const std::string& subject) {
    std::stringstream subject_path;
    std::vector<std::string> out;

    subject_path << root_ << '/' << subject;
    out = list_files(subject_path.str());

    return out;
}

void FaceDatabase::load_subjects() {
    subjects_ = list_directories(root_);
}

Face FaceDatabase::load_sample(const std::string& path) {
    return Face(path, true);
}

std::vector<std::string> FaceDatabase::subject_samples(const std::string& subject) {
    std::stringstream subject_path;
    std::vector<std::string> out;

    subject_path << root_ << '/' << subject;
    out = list_files(subject_path.str());

    return out;
}

void FaceNetEmbedDatabase::load_subjects() {
    subjects_ = list_directories(root_);
}

FaceNetEmbed FaceNetEmbedDatabase::load_sample(const std::string& path) {
    FaceNetEmbed f;
    dlib::deserialize(path) >> f;
    return f;
}

std::vector<std::string> FaceNetEmbedDatabase::subject_samples(const std::string& subject) {
    std::stringstream subject_path;
    std::vector<std::string> out;

    subject_path << root_ << '/' << subject;
    out = list_files(subject_path.str());

    return out;
}
