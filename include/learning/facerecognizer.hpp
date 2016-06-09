#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

#include "../database/facedatabase.hpp"
#include "../openface/neuralnetwork.hpp"

#include <dlib/svm.h>
#include <dlib/svm/one_vs_one_trainer.h>
#include <dlib/svm/one_vs_all_trainer.h>

typedef dlib::linear_kernel<FaceNetEmbed> linear_kernel;
typedef dlib::one_vs_all_trainer<dlib::any_trainer<FaceNetEmbed, float>, std::string> ova_trainer;

/**
 * @brief Class to train and use a decision function for face recognition.
 *
 * The class provides methods to train and use a decision function for recognition
 * of faces based on a set of labeled facerepresentations. The recognizer uses
 * FaceNet embeddings (128-byte vectors) for the decision function and as input
 * and outputs a name.
 * The decision function is trained using an SVM with a linear kernel.
 */
class FaceRecognizer {
public:
    /**
     * @brief Default constructor.
     */
    FaceRecognizer();

    /**
     * @brief Default destructor.
     */
    ~FaceRecognizer() {};

    /**
     * @brief Trains a decision function based on the given faces and labels.
     *
     * @param faces  Faces used for training
     * @param labels Labels used for training (same size as faces)
     */
    void train(std::vector<FaceNetEmbed> faces,  std::vector<std::string> labels);

    /**
     * @brief Use the decision function to make a best guess recognition.
     * @param  face Face representation use for recognition
     * @return      Name of the person recognized
     */
    std::pair<std::string, float> recognize(FaceNetEmbed face);

    /**
     * @brief Returns the decision function itself.
     * TODO(Jan): Only used temporarily to serialize df in the webcam example.
     * 			  This should be implemented as a save function.
     */
    dlib::one_vs_all_decision_function<ova_trainer, dlib::probabilistic_function< dlib::decision_function<linear_kernel> > > df() {return df_;}

    /**
     * @brief Loads a serialized decision function from file.
     *
     * In order to not have to retrain the decision function each time, the function
     * can be trained once and then saved to file. Afterwards it can be loaded from file.
     *
     * @param file File path to the serialized decision function
     */
    void load(const std::string& file);

    /**
     * @brief Cross-validate the decision function to determine it's quality.
     *
     * TODO(Jan): Add this function
     */
    void cross_validate() {}
private:

    /**
     * @brief Internal decision function.
     */
    dlib::one_vs_all_decision_function<ova_trainer, dlib::probabilistic_function< dlib::decision_function<linear_kernel> > > df_;
};

#endif
