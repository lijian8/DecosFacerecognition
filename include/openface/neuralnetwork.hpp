#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include "torchinterface.hpp"
#include "face.hpp"

#include <iostream>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "dlib/matrix.h"

typedef dlib::matrix<float, 128, 1> FaceNetEmbed;

/**
 * @brief Class that converts the face image to a FaceNet embedding.
 *
 * The OpenFace project uses FaceNet embeddings, which are a 128-byte vector,
 * as representations for faces. Theses are created by forwarding a 96x96 px
 * image of a face to a neural network. This process is what this class provides.
 * The actual forwarding happens in a torch script which made available by the
 * TorchInterface.
 */
class NeuralNetwork {
public:
    /**
     * @brief Default constructor.
     *
     * This constructor automatically loads the torch script at the path
     * FORWARD_DEFINITION which is defined in openface/settings.hpp.
     */
    NeuralNetwork() : initialized_(false) {};

    NeuralNetwork(const std::string script_path, const std::string nn_path);

    /**
     * @brief Default destructor.
     */
    ~NeuralNetwork() {};

    /**
     * @brief Converts a face image to a FaceNet embedding.
     *
     * Calls the function "forward_nn" inside the loaded torch script with a
     * torch.FloatTensor containing the face's raw image data as only parameter.
     *
     * @param  face Face to be converted, must be *aligned*
     * @return      FaceNetEmbed (128-byte vector) representation of #face
     *
     * @see FaceAligner::align()
     */
    FaceNetEmbed forward_nn(const Image &img) const;

    void load(const std::string script_path, const std::string nn_path);

    /**
     * @brief Calls forward_nn() on a set of faces.
     */
    std::vector<FaceNetEmbed> forward_nn(const std::vector<Image> &imgs) const;
private:

    /**
     * @brief Provides interface to torch code.
     */
    TorchInterface torch;

    bool initialized_;
};

#endif
