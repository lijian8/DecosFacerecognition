#include "detection/facedetector.hpp"
#include "core/support.hpp"

FaceDetector::FaceDetector() {
    gpuEnabled = false;
    detector = dlib::get_frontal_face_detector();
}

FaceDetector::FaceDetector(const std::string& cpu_path, const std::string& gpu_path) {
    gpuEnabled = false;
    detector = dlib::get_frontal_face_detector();
    load(cpu_path, gpu_path);
}

void FaceDetector::load(const std::string &cpu_path, const std::string &gpu_path) {
    if (!cv_detector.load(cpu_path))
        throw std::runtime_error("No haarcascade classifier found. Run get_resources.sh");
#ifdef CUDA_SUPPORT
    if (cv::cuda::getCudaEnabledDeviceCount() > 0) {
        gpu_detector = cv::cuda::CascadeClassifier::create(gpu_path);
        gpuEnabled = true;
    }
#endif
    initialized_ = true;
}

FaceDetector::~FaceDetector() {
}

bool FaceDetector::verifyDetection(const Image& img, const Rectangle& rect) {
    return (rect.x() >= 0 && rect.y() >= 0 && rect.x() + rect.width() <= img.width()
        && rect.y() + rect.height() <= img.height() && rect.width() > 0 && rect.height() > 0);
}

Detection FaceDetector::detect(const Image& img) {
    return dlib_detect(img);
}

#ifdef CUDA_SUPPORT
void FaceDetector::gpu_detect(const cv::cuda::GpuMat& img) {
    ASSERT(gpuEnabled, "GPU detection cannot be used on this machine.");
    assert(initialized_);
    std::cout << "Succeed" << std::endl;
    std::vector<cv::Rect> faces;
    cv::cuda::GpuMat facesBuf;
    gpu_detector->setFindLargestObject(false);
    gpu_detector->setScaleFactor(1.1);
    gpu_detector->setMinNeighbors(2);
    gpu_detector->detectMultiScale(img, facesBuf);
    gpu_detector->convert(facesBuf, faces);
}
#endif

std::vector<Detection> FaceDetector::detect(const std::vector<Image>& images) {
    std::vector<Detection> ds;

    for (size_t i = 0; i < images.size(); i++) {
        Detection d = detect(images[i]);
        ds.push_back(d);
    }

    return ds;
}

Detection FaceDetector::dlib_detect(const Image& img) {
    std::vector<dlib::rectangle> faces = detector(img.asDLIBImage());

    if(faces.size() > 0 && verifyDetection(img, Rectangle(faces[0]))) {
        Detection d;
        d.face = img;
        d.rect = Rectangle(faces[0]);
        return d;
    }
    else {
        return Detection();
    }
}

void FaceDetector::nogpu_detect(const cv::Mat& img) {
    assert(initialized_);
    std::vector<cv::Rect> faces;
    cv_detector.detectMultiScale(img, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );
}

Detection FaceDetector::cv_detect(const Image& img) {
    assert(initialized_);
    std::vector<cv::Rect> faces;
    cv_detector.detectMultiScale(img.asConstCVImage(), faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );

    if(faces.size() > 0 && verifyDetection(img, Rectangle(faces[0]))) {
        Detection d;
        d.face = Face(Image(img, Rectangle(faces[0])));
        d.rect = Rectangle(faces[0]);
        return d;
    }
    else {
        return Detection();
    }
}
