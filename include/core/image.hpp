#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "rectangle.hpp"
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


typedef dlib::cv_image<dlib::bgr_pixel> DLIBImage;
typedef cv::UMat CVImage;
typedef const cv::UMat ConstCVImage;
typedef cv::Vec3b Pixel;

class CVImageBase {
public:
    virtual int width() =0;
    virtual void load(const std::string& path) =0;
    static CVImageBase* fromFile(const std::string& path);
};

template<typename T>
class CVImageNew : public CVImageBase {
private:
    T mat_;
public:
    CVImageNew () {}
    virtual ~CVImageNew () {}

    int width() { return mat_.cols; }
    void load(const std::string& path);
};

template<typename T>
void CVImageNew<T>::load(const std::string& path) {
    cv::imread(path, cv::IMREAD_COLOR).copyTo(mat_);
}

/**
 * @brief Contains image data and is a bridge between OpenCV's Mat and dlib's array2d.
 *
 * The image data is stored internally in a OpenCV Mat structure. It has assures
 * a CV_8UC3 layout, with BGR color space. It also stores a dlib::cv_image which
 * refers to the data of the internal cv::Mat.
 */
class Image {
public:

    /**
     * @brief Constructor of empty image.
     *
     * This creates a new cv::Mat with no data, such that only the header data exists.
     * Ensures that *this is properly initialized.
     * Ensures that width = 0, height = 0.
     * pixeldata() will return a **nullptr**.
     */
    Image();

    /**
     * @brief Loads an image from a path.
     *
     * Loads image data from a file given by path using the cv::imread function.
     * Assumes that the file exists, throws an exception otherwise. Always converts
     * the image to a 3-channel BGR image.
     *
     * @param path File path to the image file, including extension
     * @throw std::runtime_error "No such file or directory: ${path}"
     */
    Image(const std::string& path);

    /**
     * @brief Creates an image from an existing cv::Mat.
     *
     * Copies the cv::Mat to the internal cv::Mat by copying header **but not data**.
     * Requires that the matrix as 8UC3 data layout.
     *
     * @param mat Image matrix to be copied
     */
    Image(cv::Mat& mat);

    /**
     * @brief Creates an image from a subsection of an image.
     *
     * Creates an Image by cloning a region of the original image. In this case
     * data is actually copied using the OpenCV's clone function.
     * Requires the rectangle to be inside the Image.
     *
     * @param img Image from which the region will be extracted
     * @param rect Region to be extracted
     */
    Image(const Image& img, const Rectangle& rect);

    /** @brief Returns the width of the cv::Mat storage. */
    int width() const {return mat_.cols;}
    /** @brief Returns the height of the cv::Mat storage. */
    int height() const {return mat_.rows;}

    /**
     * @brief Returns the image as reference such that data can be edited.
     *
     * Enables the use of dlib functions on objects of the image class. The type is
     * dlib::cv_Image<bgr_pixel> which does not copy any data. Not all functions
     * can be used with this structure.
     * @see http://dlib.net/dlib/opencv/cv_image_abstract.h.html
     *
     * @return DLIBImage as constant reference
     */
    const DLIBImage& asDLIBImage() const;

    /**
     * @brief Returns the image as reference such that data can be edited.
     *
     * Enables the use of opencv function on objects of the image class. A cv::Mat
     * is returned which does not copy data, only header information.
     *
     * @return cv::Mat as constant reference
     */
    const CVImage& asCVImage();

    /**
     * @brief Makes image safely usable as **input** to OpenCV functions
     *
     * The _InputArray class is used to specify opencv matrices as input only,
     * so that they won't be changed by the operations inside the function. Therefore
     * this function will allow the application of OpenCV operations on const Image
     * objets.
     *
     * @return Returns the image underlying data as const cv::_InputArray
     */
    ConstCVImage asConstCVImage() const;

    /**
     * @brief Copies all image data to a new cv::Mat that can be edited.
     *
     * In order to be able to perform opencv operations without invalidating
     * the constness of the original image (which would happen when exported
     * by asCVImage()) we need to be able to clone the actual data and leave the
     * original unharmed.
     *
     * @return A deep copy of the image's underlying OpenCV Mat
     */
    CVImage cloneCVImage() const;

    /**
     * @brief Pointer to the raw pixel data.
     *
     * Pointer to be used by functions that work on raw pixel data. The pointer
     * is to the first element in the data array. It returns the data inside
     * the cv::Mat that stores the data. It is read-only at this moment.
     * @see http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
     *
     * @return BGR pixel pointer to the raw pixel data
     */
    const Pixel* const pixeldata() const;

    /**
     * @brief Performs an affine transformation on the image.
     *
     * Uses the OpenCV library function to perform an affine transformation on
     * the Image. It also scales the image to the specified size.
     *
     * @param warpMat 2x3 Warp matrix for rotation and translation
     * @param size Required size of the resulting image
     */
    void warpAffine(const cv::Mat &warpMat, cv::Size size);

    /**
     * @brief Saves the image to a file.
     *
     * The suffix of the path will determine the format of the file to be written.
     * The path will not create new directories so the directories have to exist,
     * otherwise an exception will be thrown.
     *
     * @param path File path to the image file, including extension
     * @throws std::runtime_error "Directory does not exist: ${path}"
     */
    void save(const std::string& path);

    /**
     * @brief Loads the image from a file.
     *
     * Loads image data from a file given by path using the cv::imread function.
     * Assumes that the file exists, throws an exception otherwise. Always converts
     * the image to a 3-channel BGR image.
     *
     * @param path File path to the image file, including extension
     * @throw std::runtime_error "No such file or directory: ${path}"
     */
    void load(const std::string& path);

    /**
     * @brief Returns true if it is safe to assume that the image has 3-channels in BGR color space.
     * @return True if image is 3-channel, BGR image
     */
    bool safe();
protected:
    /**
     *  @brief Actual datastorage.
     *
     *  All methods will use this object to access data and
     *  return image attributes such as width and height.
     */
    CVImage mat_;

    /**
     * @brief Dlib image strcture.
     *
     * A dlib wrapper around #mat_, data refers to #mat_. It is not used internally
     * but returned by the function asDLIBImage(), in order to be used with
     * dlib functions.
     */
    DLIBImage dlibimg_;

    /**
     * @brief Is true if it's safe to assume that the data is CV_8UC3.
     *
     * Due to the nature of the cv::Mat class of OpenCV that copies pointer to
     * the image data and provides access to the raw data pointer itself, even
     * returning a const reference to #mat_ will not prevent the change of the
     * underlying data. Therefore once the image is exported through asCVImage()
     * the assumption will be unsafe.
     */
    bool safe_;

    // TODO(Jan): Create OpenCVImage class that includes these checks in it's constructor
    //            and use that class in the Image class constructor
    const cv::Mat checkCompatibility(const cv::Mat& mat) {
        if (mat.channels() != 3)
            throw std::runtime_error(std::string("Given Mat has wrong number of channels. Expected 3."));
        return mat;
    }

    const cv::Mat checkExistence(const cv::Mat& mat, const std::string& path) {
        if (!mat.data)
            throw std::runtime_error(std::string("No such file or directory: ")+path);
        return mat;
    }

    const Rectangle notEmpty(const Rectangle& rect) {
        if (rect.area() == 0)
            throw std::runtime_error(std::string("Rectangle is empty so ROI cannot be extracted."));
        return rect;
    }

    const Rectangle rectInsideImg(const Image& img, const Rectangle& rect) {
        if (rect.x() < 0 || rect.y() < 0 || rect.x() + rect.width() > img.width()
            || rect.y() + rect.height() > img.height())
            throw std::runtime_error(std::string("Rectangle is outside Image."));
        return rect;
    }
};

#endif
