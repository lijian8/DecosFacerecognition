#include "detection/facedetector.hpp"
#include "openface/facealigner.hpp"
#include <gtest/gtest.h>

//! @cond HIDDEN_SYMBOLS
class FaceDetectorTest : public ::testing::Test {
protected:
    void SetUp() {
        img.load("test/resources/image.jpg");
        noFace.load("test/resources/image_noface.jpg");
        fd.load("resources/haarcascade_frontalface_alt.xml", "");
    }

    FaceDetector fd;
    Image img;
    Image noFace;
};
//! @endcond

/**
 * @fn FaceDetector::FaceDetector()
 *
 * @test
 * Correct Initialization of the dlib and opencv detectors.
 */
TEST(FaceDetectorConstructorTest, DefaultConstruction) {
    try {
        FaceDetector fd;
        Image img("test/resources/image.jpg");
        fd.detect(img);
    }
    catch(...) {
        FAIL();
    }
}

/**
 * @fn FaceDetector::detect()
 *
 * @test
 * Correct detection of face in image.
 */
TEST_F (FaceDetectorTest, DetectFace) {
    Detection d = fd.detect(img);

    EXPECT_LT(0, d.rect.area());
}

/**
 * @fn FaceDetector::cv_detect()
 *
 * @test
 * Correct detection of face in image.
 */
TEST_F (FaceDetectorTest, DetectFaceOpenCV) {
    Detection d = fd.cv_detect(img);

    EXPECT_LT(0, d.rect.area());
}

/**
 * @fn FaceDetector::dlib_detect()
 *
 * @test
 * Correct detection of face in image.
 */
TEST_F (FaceDetectorTest, DetectFaceDLIB) {
    Detection d = fd.dlib_detect(img);

    EXPECT_LT(0, d.rect.area());
}

/**
 * @fn FaceDetector::detect()
 *
 * @test
 * Empty rectangle return when no face is found in the image.
 */
TEST_F (FaceDetectorTest, NoFace) {
    Detection d = fd.detect(noFace);

    EXPECT_EQ(0, d.rect.area());
}


// TODO(Jan): Add test for multiple detections and difficult cases(glasses, extreme angle)
