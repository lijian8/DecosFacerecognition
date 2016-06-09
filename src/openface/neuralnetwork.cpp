#include "openface/neuralnetwork.hpp"
#include "openface/settings.hpp"

NeuralNetwork::NeuralNetwork(const std::string script_path, const std::string nn_path) {
    load(script_path, nn_path);
}

void NeuralNetwork::load(const std::string script_path, const std::string nn_path) {
    torch.doFile(script_path);
    torch["load"](nn_path);

    initialized_ = true;
}

FaceNetEmbed NeuralNetwork::forward_nn(const Image &img) const {
    assert(initialized_);
    
    Tensor face(img);
    Tensor output = torch["forward_nn"](face);
    FaceNetEmbed mapping = dlib::mat(TensorData(output.raw()), 128);

    return mapping;
}

std::vector<FaceNetEmbed> NeuralNetwork::forward_nn(const std::vector<Image> &imgs) const {
    std::vector<FaceNetEmbed> out;
    for (size_t i = 0; i < imgs.size(); i++) {
        out.push_back(forward_nn(imgs[i]));
    }

    return out;
}
