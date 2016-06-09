#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <dlib/opencv.h>

typedef cv::Rect CVRect;
typedef dlib::rectangle DLIBRect;

/**
 * @brief Class for 2d rectangles, wraps around dlib and OpenCV rectangles.
 *
 * The Rectangle is defined by the top-left corner by it's coordinates x and y
 * and the width and height of the rectangle. The class provides export to
 * OpenCV and dlib rectangle format.
 */
class Rectangle {
public:
    /**
     * @brief Default constructor.
     *
     * Constructs a Rectangle at position 0, 0 and of width and height 0.
     */
    Rectangle() : x_(0), y_(0), width_(0), height_(0) {}

    /**
     * @brief Constructor from a dlib rectangle object.
     *
     * @param rect dlib rectangle object
     */
    Rectangle(const DLIBRect& rect) :
        x_(rect.left()), y_(rect.top()),
        width_(rect.width()), height_(rect.height()) {}

    /**
     * @brief Constructor from given parameters.
     *
     * @param x x-position of top-left corner
     * @param y y-position of top-left corner
     * @param width Width of the Rectangle
     * @param height Height of the Rectangle
     */
    Rectangle(int x, int y, int width, int height) : x_(x), y_(y), width_(width), height_(height) {}

    /**
     * @brief Constructor from an OpenCV rectangle object.
     *
     * @param rect OpenCV rectangle object
     */
    Rectangle(const CVRect& rect) :
        x_(rect.x), y_(rect.y), width_(rect.width), height_(rect.height) {}

    /**
     * @brief Returns x-position of top-left corner.
     * @return x-position of top-left corner
     */
    int x() const {return x_;}

    /**
     * @brief Returns y-position of top-left corner.
     * @return y-position of top-left corner
     */
    int y() const {return y_;}

    /**
     * @brief Returns width of Rectangle
     * @return width of Rectangle
     */
    int width() const {return width_;}

    /**
     * @brief Returns height of Rectangle
     * @return height of Rectangle
     */
    int height() const {return height_;}

    /**
     * @brief Returns the product of width and height of Rectangle.
     * @return Area of Rectangle
     */
    long area() const {return width_ * height_;}

    /**
     * @brief Returns the Rectanlge as OpenCV's cv::Rect structure.
     *
     * This provides support for OpenCV operations on the Rectangle class.
     *
     * @return A copy of this rectangle in form of a cv::Rect
     */
    const CVRect asCVRect() const;

    /**
     * @brief Returns the Rectanlge as dlibs's dlib::rectangle structure.
     *
     * This provides support for dlib operations on the Rectangle class.
     *
     * @return A copy of this rectangle in form of a dlib::rectangle
     */
    const DLIBRect asDLIBRect() const;


private:
    int x_, y_, width_, height_;
};

inline const CVRect Rectangle::asCVRect() const {
    return CVRect(x_, y_, width_, height_);
}

inline const DLIBRect Rectangle::asDLIBRect() const {
    return DLIBRect(x_, y_, x_+width_-1, y_+height_-1);
}

#endif
