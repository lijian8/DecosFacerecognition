#include "openface/openface.hpp"

OpenFace::OpenFace(const std::string& shape_path, const std::string& script_path, const std::string& nn_path) {
    load(shape_path, script_path, nn_path);
}

void OpenFace::load(const std::string& shape_path, const std::string &script_path, const std::string &nn_path) {
    fa_.load(shape_path);
    nn_.load(script_path, nn_path);

    initialized_ = true;
}

FaceNetEmbed OpenFace::facenet(Detection& d) {
    assert(initialized_);

    fa_.align(d);

    return nn_.forward_nn(d.face);
}

std::vector<FaceNetEmbed> OpenFace::facenet(std::vector<Detection>& ds) {
    fa_.align(ds);

    std::vector<Image> faces;
    for (size_t i = 0; i < ds.size(); i++) {
        faces.push_back(ds[i].face);
    }

    return nn_.forward_nn(faces);
}
