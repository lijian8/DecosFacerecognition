#ifndef TORCHINTERFACE_HPP
#define TORCHINTERFACE_HPP

#include "../core/image.hpp"

#include <LuaState.h>
#include <LuaStack.h>
#include <TH/TH.h>
#include <luaT.h>

typedef THFloatTensor FloatTensor;
typedef THFloatStorage FloatStorage;

/**
 * @brief TorchInterface will be the class used to interpret torch scripts.
 *
 * lua::State is a small C++-11 library used to interface lua and C++. It
 * provides simple to use tools to load lua scripts and call lua functions.
 */
typedef lua::State TorchInterface;


#define FloatTensor_(x) THFloatTensor_##x
#define FloatStorage_(x) THFloatStorage_##x
#define TensorData FloatTensor_(data) // float* to data of the Tensor
#define TensorNew3d FloatTensor_(newWithSize3d)
#define TensorNew1d FloatTensor_(newWithSize1d)
#define TensorFree FloatTensor_(free)

/**
 * @brief Wrapper for torch Tensor type.
 *
 * Uses THFloatTensors to store data which can be send to torch scripts.
 */
class Tensor {
public:
    /**
     * @brief Constructs a torch Tensor from the raw data of the image.
     *
     * @param image Image of which the data will be copied into a tensor.
     */
    Tensor (const Image& image);

    /**
     * @brief Constructs tensor from an existing Tensor
     */
    Tensor (FloatTensor* tensor);

    /**
     * @brief Default destructor.
     */
    ~Tensor ();

    //TODO: Implement function for size
    /**
     * @brief Returns the raw pointer to the wrapped Tensor.
     * @return FloatTensor pointer
     */
    FloatTensor* raw() {return tensor_;}

private:

    /**
     * @brief Wrapped FloatTensor pointer.
     */
    FloatTensor* tensor_;
};


/**
 * Extension of the luaState library to be able to easily call functions with
 * torch tensors as input and output.
 */
namespace lua {

    namespace stack {
        template<>
        inline int push(lua_State* luaState, Tensor value) {
            luaT_pushudata(luaState, value.raw(), "torch.FloatTensor");
            return 1;
        }

        template<>
        inline Tensor read(lua_State* luaState, int index) {
            return Tensor((FloatTensor*)luaT_toudata(luaState, index, "torch.FloatTensor"));
        }
    }
}

inline Tensor::Tensor(const Image& img) {
    int w = img.width(), h = img.height();

    tensor_ = TensorNew3d(3, h, w);
    const Pixel* imgdata = img.pixeldata();
    float* data = TensorData(tensor_);
    for (int i = 0; i < w*h; ++i) {
        data[i      ] = 1./255 * imgdata[i][2];
        data[i+w*h  ] = 1./255 * imgdata[i][1];
        data[i+2*w*h] = 1./255 * imgdata[i][0];
     }
}

inline Tensor::Tensor(FloatTensor* tensor) : tensor_(tensor) {}

inline Tensor::~Tensor() {}

#endif
