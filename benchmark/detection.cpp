#include <hayai/hayai.hpp>
#include <opencv2/core/ocl.hpp>

#include "detection/facedetector.hpp"

class FaceDetectorTest : public ::hayai::Fixture {
public:
    virtual void SetUp() {
	img = cv::imread("test/resources/image.jpg");
	gpuimg.upload(img);
    }

    cv::Mat img;
    cv::cuda::GpuMat gpuimg;
    FaceDetector fd;
};

BENCHMARK_F(FaceDetectorTest, OpenCVDetect, 1, 10) {
    fd.cv_detect(img);
}

#ifdef CUDA_SUPPORT
BENCHMARK_F(FaceDetectorTest, GPUDetect, 1, 10) {
    fd.gpu_detect(gpuimg);
}
#endif

int main()
{
    hayai::ConsoleOutputter consoleOutputter;

    hayai::Benchmarker::AddOutputter(consoleOutputter);
    hayai::Benchmarker::RunAllTests();
    return 0;
}
