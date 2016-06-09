#include "core/image.hpp"
#include "core/support.hpp"

CVImageBase* CVImageBase::fromFile(const std::string& path) {
    CVImageNew<cv::UMat>* ptr = new CVImageNew<cv::UMat>();
    ptr->load(path);
    return ptr;
}

Image::Image() {};

Image::Image(const std::string &path) {
    checkExistence(cv::imread(path, cv::IMREAD_COLOR), path).copyTo(mat_);
    dlibimg_ = DLIBImage(mat_.getMat(cv::ACCESS_READ | cv::ACCESS_WRITE));
    safe_ = true;
}

Image::Image(cv::Mat& mat) {
    checkCompatibility(mat).copyTo(mat_);
    dlibimg_ = DLIBImage(mat_.getMat(cv::ACCESS_READ | cv::ACCESS_WRITE));
    safe_ = true;
}

Image::Image(const Image& img, const Rectangle& rect) {
    mat_ = cv::UMat(img.cloneCVImage(), notEmpty(rectInsideImg(img, rect)).asCVRect());
    dlibimg_ = DLIBImage(mat_.getMat(cv::ACCESS_READ | cv::ACCESS_WRITE));
    safe_ = true;
}


const DLIBImage& Image::asDLIBImage() const {
    return dlibimg_;
}

const CVImage& Image::asCVImage() {
    ASSERT(safe_, "Image is not safe to use.");
    safe_ = false;
    return mat_;
}

ConstCVImage Image::asConstCVImage() const {
    return mat_;
}

CVImage Image::cloneCVImage() const {
    assert(safe_);
    return mat_.clone();
}

const Pixel* const Image::pixeldata() const {
    ASSERT(safe_, "Image is not safe to use.");
    return (Pixel*)mat_.getMat(cv::ACCESS_READ).data;
}

// TODO: Are there any exceptions this should throw? Path does not exist?
void Image::save(const std::string &path) {
    cv::imwrite(path, mat_);
}

void Image::load(const std::string &path) {
    checkExistence(cv::imread(path, cv::IMREAD_COLOR), path).copyTo(mat_);
    dlibimg_ = DLIBImage(mat_.getMat(cv::ACCESS_READ | cv::ACCESS_WRITE));
}

void Image::warpAffine(const cv::Mat& warpMat, cv::Size size) {
    cv::warpAffine(mat_, mat_, warpMat, size);
}

bool Image::safe() {
    return safe_;
}
