#ifndef ALIGNER_HPP
#define ALIGNER_HPP

#include "face.hpp"
#include "detection/facedetector.hpp"

#include <dlib/matrix.h>
#include <dlib/image_processing.h>

/**
 * @brief Class that can align faces to be in the correct form for the recognizer.
 *
 * In order to compare faces from multiple people in multiple environments, the
 * faces should be brought into a standard format to minimize influence of
 * head rotation and background.
 * This class uses dlib's shape predictor to find features in the face and align
 * the faces by calculating a transformation matrix.
 *
 * @see http://blog.dlib.net/2014/08/real-time-face-pose-estimation.html
 */
class FaceAligner {
    /**
     * @brief Pose model to find feature points in the face.
     */
    dlib::shape_predictor pose_model;

    /**
     * @brief Use the pose model to predict feature points in the face.
     *
     * @param  face Face to predict the shape of
     * @return      Dlib object detection object
     */
    dlib::full_object_detection predict(const Detection& d) const;

    bool initialized_;
public:
    /**
     * Default constructor.
     */
    FaceAligner() : initialized_(false) {}

    FaceAligner(const std::string& shape_path);

    void load(const std::string& shape_path);

    /**
     * Default destructor.
     */
    ~FaceAligner() {}

    /**
     * @brief Aligns a face according to pose model.
     *
     * First detects a shape using dlib's shape prediction. If the feature points
     * could not be found in the face, the face will not be aligned and #face's
     * Face::aligned_ parameter will be left at false.
     * If it succeeds, Face::aligned_ will be true
     *
     * @param face Face to be aligned.
     */
    void align(Detection& d) const;

    /**
     * @brief Calls align() on a set of faces.
     * @param faces Faces to be aligned.
     */
    void align(std::vector<Detection>& ds) const;
};

#endif
