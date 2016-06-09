#include "learning/facerecognizer.hpp"
#include <dlib/matrix.h>
#include <gtest/gtest.h>

//! @cond HIDDEN_SYMBOLS
class FaceRecognizerTest : public ::testing::Test {
protected:
    void SetUp() {
        fr.load("facedatabase.dat");
    }
    FaceRecognizer fr;
};
//! @endcond

/**
 * @fn FaceRecognizer::recognize()
 *
 * @test
 * Correct recognition of face in an image.
 */
TEST_F (FaceRecognizerTest, RecognizeFace) {
    dlib::matrix<float, 128, 1> facenetembed;
    dlib::deserialize("test/resources/Jan.dat") >> facenetembed;
    auto recognition = fr.recognize(facenetembed);

    EXPECT_EQ(recognition.first, "Jan");
}
TEST (RecognizerTest, DISABLED_SavingAndLoadingFromFile) {}
