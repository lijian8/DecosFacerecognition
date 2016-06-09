#ifndef FACE_HPP
#define FACE_HPP

#include "../core/image.hpp"
#include "../core/rectangle.hpp"
#include "settings.hpp"

class FaceAligner;

/**
 * @brief Class to enforce (size-)constraints on Face images.
 *
 * In order to be used with a NeuralNetwork recognizer and some other recognizers
 * the faces all need to have the same size. This class applies these constraints
 * to the basic Image class. By inheriting from Image it can still
 * be used with all image operations.
 * Faces can also be aligned by the friend class FaceAligner which calls the
 * align method of the Face class to change the internal boolean #aligned_.
 */
class Face : public Image {
    friend class FaceAligner;
public:

    /**
     * @brief Default constructor, initializes the aligned_ variable.
     * TODO(Jan): Remove aligned argument
     */
    Face(bool aligned = false) : aligned_(aligned) {}

    /**
     * @brief Constructs a face from an Image, checks image dimensions.
     */
    Face(const Image&, bool aligned = false);

    /**
     * @brief Default destructor.
     */
    ~Face() {};

    /**
     * @brief Returns true if the face has been aligned.
     * @return true if the image has been aligned
     */
    bool aligned() {return aligned_;}

private:
    /**
     * @brief Control variable which is true if the face has been aligned.
     */
    bool aligned_;

    /**
     * @brief Set's #aligned_ to true.
     *
     * This function should only be called from the FaceAligner class.
     */
    void align();
};

#endif
