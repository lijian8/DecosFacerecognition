#include "openface/face.hpp"

Face::Face(const Image& img, bool aligned) : Image(img), aligned_(aligned) {
    if (aligned_ && (this->width() != FACE_SIZE_CONSTRAINT || this->height() != FACE_SIZE_CONSTRAINT))
        throw std::runtime_error("Face does satisfy size constraint");
}

void Face::align() {
    if (this->width() != FACE_SIZE_CONSTRAINT || this->height() != FACE_SIZE_CONSTRAINT) {
        throw std::runtime_error("Face does satisfy size constraint");
    }
    aligned_ = true;
}
