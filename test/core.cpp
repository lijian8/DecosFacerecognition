#include "core/image.hpp"
#include "core/rectangle.hpp"
#include "core/support.hpp"

#include <dlib/image_io.h>
#include <gtest/gtest.h>
#include <opencv2/core/cuda.hpp>

#include <algorithm>

/**
 *
 * Image Tests
 *
 */

/**
* @fn Image::Image()
*
* @test
* Constructing an empty image.
*/
TEST (ImageTest, ConstructingEmpty) {
    Image i;

    EXPECT_EQ(i.width(), 0);
    EXPECT_EQ(i.height(), 0);
    EXPECT_EQ(i.pixeldata(), nullptr);
    // TODO(Jan): Add Testing asDLIBImage() and try adding data to cv::Mat with dlib function
}

/**
 * @fn Image::Image(const std::string& path)
 *
 * @test
 * Constructing image from a jpg file.
 */
TEST (ImageTest, ConstructingFromDirectory) {
    Image i("test/resources/image.jpg");

    EXPECT_EQ(i.width(), 640);
    EXPECT_EQ(i.height(), 480);
}

/**
 * @fn Image::Image(const std::string& path)
 *
 * @test
 * Failing construction with non-existent image.
 */
TEST (ImageTest, ConstructingNonExisting) {
    std::string image_path = "test/image.jpg";
    try {
        Image i(image_path);
        FAIL();
    }
    catch (std::runtime_error &err) {
        EXPECT_EQ(err.what(),std::string("No such file or directory: ")+image_path);
    }
    catch (...) {
        FAIL() << "Expected different exception";
    }
}

/**
 * @fn Image::Image(cv::Mat&)
 *
 * @test
 * Constructing from a cv::Mat.
 */
TEST (ImageTest, LoadingFromMat) {
    cv::Mat mat = cv::imread("test/resources/image.jpg");
    Image i(mat);

    EXPECT_EQ(i.width(), 640);
    EXPECT_EQ(i.height(), 480);
}

/**
 * @fn Image::Image(cv::Mat&)
 *
 * @test
 * Failing construction from matrix with too many channels.
 */
TEST (ImageTest, LoadingOneChannelMat) {
    cv::Mat M = cv::Mat::eye(5, 5, CV_8UC1);
    try {
        Image i(M);
        FAIL();
    }
    catch (std::runtime_error &err) {
        EXPECT_STREQ(err.what(),"Given Mat has wrong number of channels. Expected 3.");
    }
    catch (...) {
        FAIL() << "Expected different exception";
    }
}

/**
 * @fn Image::load(const std::string& path)
 *
 * @test
 * Loading an image from a file using the load function.
 */
TEST (ImageTest, LoadingFromDirectory) {
    Image i;
    i.load("test/resources/image.jpg");

    EXPECT_EQ(i.width(), 640);
    EXPECT_EQ(i.height(), 480);
}

/**
 * @fn Image::load(const std::string& path)
 *
 * @test
 * Failing construction with non-existent image.
 */
TEST (ImageTest, LoadingNonExisting) {
    std::string image_path = "test/image.jpg";
    Image i;
    try {
        i.load(image_path);
        FAIL();
    }
    catch (std::runtime_error &err) {
        EXPECT_EQ(err.what(),std::string("No such file or directory: ")+image_path);
    }
    catch (...) {
        FAIL() << "Expected different exception";
    }
}

/**
 * @fn Image::load(const std::string& path)
 *
 * @test
 * Loading an image from a file after constructing from a different file.
 */
TEST (ImageTest, ConstructingandLoadingFromDirectory) {
    Image i("test/resources/face.png");
    i.load("test/resources/image.jpg");

    EXPECT_EQ(i.width(), 640);
    EXPECT_EQ(i.height(), 480);
}

/**
 * @fn Image::pixeldata()
 *
 * @test
 * Reading raw pixel data using a single pixel image of color red. Also tests
 * the bgr color space.
 */
TEST (ImageTest, ReadingPixel) {
    Image i("test/resources/red_pixel.png");

    const Pixel* data = i.pixeldata();
    EXPECT_EQ(data[0][2], 255);
    EXPECT_EQ(data[0][1], 0);
    EXPECT_EQ(data[0][0], 0);
}

/**
 * @fn Image::asCVImage()
 *
 * @test
 * Failing assertion on OpenCV transformation when using the unsafe method twice.
 */
TEST (ImageTest, UnsafeOpenCVTransformation) {
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    Image i("test/resources/red_pixel.png");

    EXPECT_DEATH(cv::cvtColor(i.asCVImage(), i.asCVImage(), CV_BGR2RGB), "Image is not safe to use.");
}

/**
 * @fn Image::asConstCVImage()
 *
 * @test
 * Transforming the color space of an Image with OpenCV library. By using the safe
 * asConstCVImage() function, a new cv::Mat can be assigned without changing the
 * original image.
 */
TEST (ImageTest, SafeOpenCVTransformation) {
    const Image i("test/resources/red_pixel.png");
    cv::Mat rgbimage;
    cv::cvtColor(i.asConstCVImage(), rgbimage, CV_BGR2RGB);

    const Pixel* data = i.pixeldata();
    EXPECT_EQ(data[0][2], 255);
    EXPECT_EQ(data[0][1], 0);
    EXPECT_EQ(data[0][0], 0);

    const Pixel* rgbdata = (Pixel*)rgbimage.data;
    EXPECT_EQ(rgbdata[0][0], 255);
    EXPECT_EQ(rgbdata[0][1], 0);
    EXPECT_EQ(rgbdata[0][2], 0);
}

/**
 *
 * Rectangle Tests
 *
 */


/**
 * @fn Rectangle::Rectangle()
 *
 * @test
 * Test zero initialization of default constructor for Rectanlge.
 */
TEST(RectangleTest, DefaultConstruction) {
    Rectangle rect;

    EXPECT_EQ(0, rect.x());
    EXPECT_EQ(0, rect.y());
    EXPECT_EQ(0, rect.width());
    EXPECT_EQ(0, rect.height());
    EXPECT_EQ(0, rect.area());
}

/**
 * @fn Rectangle::Rectangle(int, int, int, int)
 *
 * @test
 * Test parameter initialization of constructor for Rectanlge.
 */
TEST(RectangleTest, ParameterConstruction) {
    Rectangle rect(5, 5, 10, 10);

    EXPECT_EQ(5, rect.x());
    EXPECT_EQ(5, rect.y());
    EXPECT_EQ(10, rect.width());
    EXPECT_EQ(10, rect.height());
    EXPECT_EQ(100, rect.area());
}

/**
 * @fn Rectangle::Rectangle(DLIBRect&)
 *
 * @test
 * Test parameter initialization of dlib constructor for Rectanlge.
 */
TEST(RectangleTest, DLIBConstruction) {
    DLIBRect drect1;
    Rectangle rect1(drect1);

    EXPECT_EQ(0, rect1.x());
    EXPECT_EQ(0, rect1.y());
    EXPECT_EQ(0, rect1.width());
    EXPECT_EQ(0, rect1.height());
    EXPECT_EQ(0, rect1.area());

    // Initializes x, y, right-x, bottom-y
    DLIBRect drect2(5, 5, 10, 10);
    Rectangle rect2(drect2);

    EXPECT_EQ(5, rect2.x());
    EXPECT_EQ(5, rect2.y());
    EXPECT_EQ(6, rect2.width());
    EXPECT_EQ(6, rect2.height());
    EXPECT_EQ(36, rect2.area());
}

/**
 * @fn Rectangle::Rectangle(CVRect&)
 *
 * @test
 * Test parameter initialization of opencv constructor for Rectanlge.
 */
TEST(RectangleTest, OpenCVConstruction) {
    CVRect drect1;
    Rectangle rect1(drect1);

    EXPECT_EQ(0, rect1.x());
    EXPECT_EQ(0, rect1.y());
    EXPECT_EQ(0, rect1.width());
    EXPECT_EQ(0, rect1.height());
    EXPECT_EQ(0, rect1.area());

    CVRect drect2(5, 5, 10, 10);
    Rectangle rect2(drect2);

    EXPECT_EQ(5, rect2.x());
    EXPECT_EQ(5, rect2.y());
    EXPECT_EQ(10, rect2.width());
    EXPECT_EQ(10, rect2.height());
    EXPECT_EQ(100, rect2.area());
}

/**
 * @fn Rectangle::asCVRect()
 *
 * @test
 * Test export as cv::Rect.
 */
TEST(RectangleTest, OpenCVExport) {
    Rectangle drect;
    cv::Rect rect1 = drect.asCVRect();

    EXPECT_EQ(0, rect1.x);
    EXPECT_EQ(0, rect1.y);
    EXPECT_EQ(0, rect1.width);
    EXPECT_EQ(0, rect1.height);

    Rectangle drect2(5, 5, 10, 10);
    cv::Rect rect2 = drect2.asCVRect();

    EXPECT_EQ(5, rect2.x);
    EXPECT_EQ(5, rect2.y);
    EXPECT_EQ(10, rect2.width);
    EXPECT_EQ(10, rect2.height);
}

/**
 * @fn Rectangle::asDLIBRect()
 *
 * @test
 * Test export as dlib::rectangle.
 */
TEST(RectangleTest, DLIBExport) {
    Rectangle drect;
    dlib::rectangle rect1 = drect.asDLIBRect();

    EXPECT_EQ(0, rect1.left());
    EXPECT_EQ(0, rect1.top());
    EXPECT_EQ(0, rect1.width());
    EXPECT_EQ(0, rect1.height());
    EXPECT_EQ(0, rect1.area());

    Rectangle drect2(5, 5, 10, 10);
    dlib::rectangle rect2 = drect2.asDLIBRect();

    EXPECT_EQ(5, rect2.left());
    EXPECT_EQ(5, rect2.top());
    EXPECT_EQ(10, rect2.width());
    EXPECT_EQ(10, rect2.height());
    EXPECT_EQ(100, rect2.area());
}

/**
 *
 * Support Tests
 *
 */

/**
 * @test
 * Tests the amount and name of files returned by the list_files function.
 */
TEST(SupportTest, ListFilesTest) {
    std::vector<std::string> files = list_files("test/resources/support_test");

    EXPECT_EQ(4, files.size());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "are") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), ".hiddenfile") == files.end());

    files = list_files("test/resources/support_test/hello");
    EXPECT_TRUE(files.empty());

    files = list_files("test/resources/support_test/hi");
    EXPECT_TRUE(files.empty());
}

/**
 * @test
 * Tests the amount and name of directories returned by the list_directories function.
 */
TEST(SupportTest, ListDirectoriesTest) {
    std::vector<std::string> dirs = list_directories("test/resources/support_test");

    EXPECT_EQ(2, dirs.size());
    EXPECT_TRUE(std::find(dirs.begin(), dirs.end(), "hello") != dirs.end());
    EXPECT_TRUE(std::find(dirs.begin(), dirs.end(), ".hidden") == dirs.end());

    dirs = list_directories("test/resources/support_test/hello");
    EXPECT_TRUE(dirs.empty());

    dirs = list_directories("test/resources/support_test/hi");
    EXPECT_TRUE(dirs.empty());
}

TEST(CVImageNewTest, LoadCPUCVImage) {
    CVImageNew<cv::Mat> a;
    a.load("test/resources/image.jpg");
}

TEST(CVImageNewTest, LoadOCLCVImage) {
    CVImageNew<cv::UMat> a;
    a.load("test/resources/image.jpg");
}

TEST(CVImageNewTest, LoadGPUCVImage) {
    CVImageBase* a = CVImageBase::fromFile("test/resources/image.jpg");
    EXPECT_EQ(a->width(), 640);
}
