#ifndef OPENFACE_HPP
#define OPENFACE_HPP

#include "face.hpp"
#include "neuralnetwork.hpp"
#include "facealigner.hpp"

/**
 * @brief Wrapper class for all OpenFace related classes.
 *
 * The OpenFace project is an implementation of a neural network bases face
 * recognition algorithm which high recognition rate on the standard data sets.
 * It uses face images, aligned on their outer eyes and nose-tip
 * features, as input to a deep neural network and outputs 128-byte representations
 * such that the distance between two vectors linearly translates to the similarity
 * of the two face images, simplifying classification and recognition tasks.
 *
 * This class is used wraps all tools to get from a simple rectangle face
 * detection to a FaceNet embedding that can be used to train a decision function.
 *
 * @see http://cmusatyalab.github.io/openface/
 */
class OpenFace {
    /**
     * @brief Neural network to use.
     */
    NeuralNetwork nn_;

    /**
     * @brief Face aligner to use.
     */
    FaceAligner fa_;

    bool initialized_;

public:
    /**
     * @brief Getter function for the internal NeuralNetwork.
     * @return Internal neural network
     */
    const NeuralNetwork& nn() {return nn_;}

    /**
     * @brief Getter function for the internal FaceAligner.
     * @return Internal face aligner.
     */
    const FaceAligner& fa() {return fa_;}

    void load(const std::string& shape_path, const std::string& script_path, const std::string& nn_path);

    /**
     * @brief Default constructor.
     */
    OpenFace() : initialized_(false) {}

    OpenFace(const std::string& shape_path, const std::string& script_path, const std::string& nn_path);

    /**
     * @brief Default destructor.
     */
    ~OpenFace() {}

    /**
     * @brief Get a FaceNet representation of the unaligned face
     * @param  face Aligned or unaligned Face to be forwarded to neural network
     * @return      FaceNet embedding of the given face.
     */
    FaceNetEmbed facenet(Detection& d);

    /**
     * @brief Calls facenet() on a set of faces.
     */
    std::vector<FaceNetEmbed> facenet(std::vector<Detection>&);
};

#endif
