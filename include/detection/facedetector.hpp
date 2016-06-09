#ifndef IMAGEPROCESSING_HPP
#define IMAGEPROCESSING_HPP

#include "../core/rectangle.hpp"
#include "../openface/face.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>

/**
 * @brief Container for a face detection.
 *
 * Contains a face and the bounding box of the detected face in the image.
 */
struct Detection {
    Image face;
    Rectangle rect;
};

/**
 * @brief Container for face detections of multiple images.
 *
 * Contains a list of faces and bounding boxes of each image for the given image.
 */
// struct Detections {
//     std::vector<Face> faces;
//     std::vector<Rectangle> rects;
// };


/**
 * @brief Detector class for faces in images.
 *
 * The FaceDetector class provides different algorithms to detect faces in a given
 * image. At this point it implements dlib's facedetecor and OpenCV face Detection
 * using Haarcascade classifiers.
 */
class FaceDetector {
public:
    /**
     * @brief Default constructor.
     *
     * Loads a haarcascade classifier file from resources/haarcascade_frontalface_alt.xml.
     *
     */
    FaceDetector();

    FaceDetector(const std::string& cpu_path, const std::string& gpu_path);

    /**
     * @brief Default destructor.
     */
    ~FaceDetector();

    void load(const std::string& cpu_path, const std::string& gpu_path);

    /**
     * @brief Detects an image using the a combination of the algorithms.
     * @param  img Image in which the face shall be detected.
     * @return     Detection of the face, if no face found the detection has an
     *             empty rectangle
     */
    Detection detect(const Image& img);
    /**
     * @brief Detects faces in multiple images.
     *
     * Internally calls detect() for each image.
     *
     * @param  imgs Images in which face shall be detected.
     * @return Detections of faces
     */
    std::vector<Detection> detect(const std::vector<Image>& imgs);

    /**
     * @brief Detects an image using the the dlib algorithms.
     * @param  img Image in which the face shall be detected.
     * @return     Detection of the face, if no face found the detection has an
     *             empty rectangle
     */
    Detection dlib_detect(const Image& img);

#ifdef CUDA_SUPPORT
    void gpu_detect(const cv::cuda::GpuMat& img);
#endif
    void nogpu_detect(const cv::Mat& img);

    /**
     * @brief Detects an image using the the Haarcascade approach.
     * @param  img Image in which the face shall be detected.
     * @return     Detection of the face, if no face found the detection has an
     *             empty rectangle
     */
    Detection cv_detect(const Image& img);
private:

    /**
     * @brief Dlib detector.
     */
    dlib::frontal_face_detector detector;

    /**
     * @brief OpenCV Haarcascade classifier.
     */
    cv::CascadeClassifier cv_detector;


#ifdef CUDA_SUPPORT
    cv::Ptr<cv::cuda::CascadeClassifier> gpu_detector;
#endif

    /**
     * @brief Ensures that the detection is valid.
     */
    bool verifyDetection(const Image& img, const Rectangle& rect);

    bool gpuEnabled;
    bool initialized_;
};

#endif
