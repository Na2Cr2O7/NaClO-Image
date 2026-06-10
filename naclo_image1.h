#pragma once
/**
 * @file naclo_image1.h
 * @brief 适用于C的高易用性图像处理单文件库
 * @brief 可选择替换申请内存、释放内存函数，无符号整数和浮点数类型。
 */
// #include <cstddef>
#include <ctype.h>
#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef NACLO_IMAGE
#define NACLO_IMAGE

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <memory.h>

#define NaClO_CALLOC calloc
#define NaClO_FREE free

#include <assert.h>

#define TODO assert(false);

#ifdef __cplusplus
extern "C" {
#endif
typedef uint64_t NaClO_uint;
typedef int64_t NaClO_int;
typedef float NaClO_float;

static const float NaClO_PI = 3.141592653589793f;
static const NaClO_float NaClO_e = 2.718281828459f;

/// @brief 错误种类
typedef enum {
  NACLO_OK,                               //!< @brief 不存在错误
  NACLO_MEMORY_ALLOCATION_FAILED,         //!< @brief 内存分配失败
  NACLO_UNKNOWN_CHANNEL,                  //!< @brief 图像通道未知
  NACLO_NULL_POINTER,                     //!< @brief 传入了空指针
  NACLO_UNKNOWN_OR_UNSUPPORTED_EXTENSION, //!< @brief
                                          //!< 试图保存到未知或不支持的扩展名
  NACLO_UNKNOWN_ERROR,                    //!< @brief 未知错误
  NACLO_OUT_OF_BOUNDS,                    //!< @brief 越界访问
  NACLO_DISMATCHED_CHANNEL,               //!< @brief 通道数不匹配
  NACLO_ZERO_WIDTH_OR_HEIGHT,             //!< @brief 宽或高为0
  NACLO_NOT_A_SQUARE_MATRIX,              //!< @brief 不是方阵
  NACLO_SINGULAR_MATRIX,                  //!< @brief 奇异矩阵

} NaClO_ErrorType;

/// @brief 颜色通道种类

typedef enum {
  NaClO_RGB,  //!< @brief 彩色图片
  NaClO_RGBA, //!< @brief 带透明度的彩色图片
  NaClO_L,    //!< @brief 灰度图片
  NaClO_1     //!< @brief 二值图片
} NaClO_ColorMode;

/// @brief 保存一个像素点RGB的结构体
/// @see NaClO_PixelType
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} NaClO_ColorModeRGB;
/// @brief 保存一个像素点RGBA的结构体
/// @see NaClO_PixelType
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} NaClO_ColorModeRGBA;

/// @brief 单个像素点联合体
/// @note 读取数据前请判断图像通道
typedef union {
  NaClO_ColorModeRGB RGB;
  NaClO_ColorModeRGBA RGBA;
  NaClO_float L; //!< @brief 灰度
  bool value;    //!< @brief 二值
} NaClO_PixelType;

/// @brief 图像类
/// @note 采用二维数组记录每个像素，长在前宽在后
/// @see NaClO_NewImage
/// @see *NaClO_Pixel
/// @see NaClO_GetPixel
/// @see NaClO_PutPixel
typedef struct {
  NaClO_uint width;       //!< @brief 宽
  NaClO_uint height;      //!< @brief 高
  NaClO_ColorMode mode;   //!< @brief 颜色通道
  NaClO_PixelType **data; //!< @brief 像素点数据
} NaClO_Image;

/// @brief 类似rustResult<T,E>的可携带错误的返回值
typedef struct {
  NaClO_Image result;
  NaClO_ErrorType Error;
} NaClO_ImageResult;

/// @brief 类似rustResult<T,E>的可携带错误的返回值
typedef struct {
  NaClO_PixelType result;
  NaClO_ErrorType Error;
} NaClO_PixelResult;

/// @brief 翻转方法
typedef enum {
  NACLO_FLIP_X,          //!< @brief 沿着中心X自上而下翻转
  NACLO_FLIP_Y,          //!< @brief 沿着中心Y自左而右翻转
  NACLO_FLIP_NEGATIVE_X, //!< @brief 沿着中心X自下而上翻转
  NACLO_FLIP_NEGATIVE_Y, //!< @brief 沿着中心Y自右而左翻转
} NaClO_FlipDirection;

typedef struct {
  uint16_t hue;
  NaClO_float saturation;
  NaClO_float value;
} NaClO_HSV;

typedef struct {
  NaClO_float *data;
  uint8_t width;
  uint8_t height;
} NaClO_Matrix;

typedef struct {
  NaClO_Matrix result;
  NaClO_ErrorType Error;
} NaClO_MatrixResult;
typedef struct {
  NaClO_float result;
  NaClO_ErrorType Error;
} NaClO_FloatResult;
typedef enum {
  NACLO_EXPAND_DEFAULT,
  NACLO_EXPAND_MIRROR,
  NACLO_EXPAND_EDGE
} NaClO_ExpandMode;

typedef NaClO_PixelType (*NaClO_SetPixelFunctionPointer)(NaClO_PixelType p,
                                                         NaClO_ColorMode mode);

NaClO_ImageResult NaClO_Load(const char *filename);
NaClO_ErrorType NaClO_FreeImage(NaClO_Image *src);

NaClO_ErrorType NaClO_Save(NaClO_Image *data, const char *fileName);
NaClO_ErrorType NaClO_SaveAndFree(NaClO_Image *data, const char *fileName);

NaClO_ImageResult NaClO_Resize(NaClO_Image *data, NaClO_uint w, NaClO_uint h);
NaClO_ErrorType NaClO_Resized(NaClO_Image *data, NaClO_uint w, NaClO_uint h);

NaClO_PixelType *NaClO_Pixel(NaClO_Image *data, NaClO_uint x, NaClO_uint y);
NaClO_PixelResult NaClO_GetPixel(NaClO_Image *data, NaClO_uint x, NaClO_uint y);
NaClO_ErrorType NaClO_PutPixel(NaClO_Image *data, NaClO_uint x, NaClO_uint y,
                               NaClO_PixelType target);

NaClO_ImageResult NaClO_Crop(NaClO_Image *data, NaClO_uint x, NaClO_uint y,
                             NaClO_uint w, NaClO_uint h);
NaClO_ErrorType NaClO_Cropped(NaClO_Image *data, NaClO_uint x, NaClO_uint y,
                              NaClO_uint w, NaClO_uint h);

NaClO_ImageResult NaClO_CopyImage(NaClO_Image *data);

NaClO_ImageResult NaClO_ConvertE(NaClO_Image *data, NaClO_ColorMode mode);
NaClO_ImageResult NaClO_Convert(NaClO_Image *data, const char *mode_str);
NaClO_ErrorType NaClO_Converted(NaClO_Image *data, const char *mode_str);
NaClO_ErrorType NaClO_ConvertedE(NaClO_Image *data, NaClO_ColorMode mode);

NaClO_ImageResult NaClO_NewImage(NaClO_uint w, NaClO_uint h,
                                 NaClO_ColorMode mode,
                                 NaClO_PixelType fillColor);

NaClO_ImageResult NaClO_Flip(NaClO_Image *data, NaClO_FlipDirection direction);
NaClO_ErrorType NaClO_Flipped(NaClO_Image *data, NaClO_FlipDirection direction);

/**
 * @brief 按照一个指定轴翻转
 * @note 依照NaClO_FlipDirection direction选择轴 NACLO_FLIP_X，
 * NACLO_FLIP_NEGATIVE_X则pos为y轴高度，否则为x轴高度
 * @see  NaClO_FlipDirection
 * @param data 图像
 * @param pos 轴位置
 * @param direction 翻转方向
 * @return 图片结果
 */
NaClO_ImageResult NaClO_FlipAt(NaClO_Image *data, NaClO_uint pos,
                               NaClO_FlipDirection direction);

NaClO_ImageResult NaClO_Paste(NaClO_Image *canva, NaClO_Image *object,
                              NaClO_uint w, NaClO_uint h);
NaClO_ErrorType NaClO_Pasted(NaClO_Image *canva, NaClO_Image *object,
                             NaClO_uint w, NaClO_uint h);

NaClO_HSV NaClO_GetHSV(NaClO_ColorModeRGB src);
NaClO_HSV NaClO_GetHSVRGBA(NaClO_ColorModeRGBA src);
NaClO_ColorModeRGB NaClO_GetRGB(NaClO_HSV src);

void NaClO_EnumeratePixel(NaClO_Image *src, NaClO_SetPixelFunctionPointer f);
#define NaClO_EnumeratePixel2(src, f)                                          \
  for (int x = 0; x < src.width; ++x) {                                        \
    for (int y = 0; y < src.height; ++y) {                                     \
      f;                                                                       \
    }                                                                          \
  }
NaClO_MatrixResult NaClO_NewMatrix(uint8_t x, uint8_t y);
NaClO_ErrorType NaClO_FreeMatrix(NaClO_Matrix *matrix);
NaClO_float *NaClO_MatrixElementYX(NaClO_Matrix *matrix, uint8_t row,
                                   uint8_t col);
NaClO_float *NaClO_MatrixElementYX1(NaClO_Matrix *matrix, uint8_t row,
                                    uint8_t col);
NaClO_float *NaClO_MatrixElement(NaClO_Matrix *matrix, uint8_t col,
                                 uint8_t row);
NaClO_float *NaClO_MatrixElement1(NaClO_Matrix *matrix, uint8_t col,
                                  uint8_t row);

NaClO_MatrixResult NaClO_CopyMatrix(NaClO_Matrix *matrix);
NaClO_MatrixResult NaClO_TransposeMatrix(NaClO_Matrix *matrix);
NaClO_ErrorType NaClO_TransposedMatrix(NaClO_Matrix *matrix);
NaClO_MatrixResult NaClO_NewIdentityMatrix(uint8_t w);
NaClO_MatrixResult NaClO_GetRowReducedEchelonMatrix(NaClO_Matrix *matrix);
NaClO_ErrorType NaClO_RowReducedEchelonMatrix(NaClO_Matrix *matrix);
NaClO_MatrixResult NaClO_MatrixInversion(NaClO_Matrix *matrix);
NaClO_MatrixResult NaClO_MatrixMultiply(NaClO_Matrix *A, NaClO_Matrix *B);

NaClO_MatrixResult NaClO_DotProduct(NaClO_Matrix *matrix1,
                                    NaClO_Matrix *matrix2);

NaClO_MatrixResult NaClO_GetRotationMatrix2D(uint8_t centerX, uint8_t centerY,
                                             NaClO_float angleDeg,
                                             NaClO_float scale);
NaClO_ImageResult NaClO_WarpAffine(NaClO_Image *data, NaClO_Matrix *M);
NaClO_ErrorType NaClO_WarpedAffine(NaClO_Image *data, NaClO_Matrix *M);

NaClO_ImageResult NaClO_Expand(NaClO_Image *data, NaClO_uint newWidth,
                               NaClO_uint newHeight,
                               NaClO_ExpandMode expandMode);
NaClO_ErrorType NaClO_Expanded(NaClO_Image *data, NaClO_uint newWidth,
                               NaClO_uint newHeight,
                               NaClO_ExpandMode expandMode);

NaClO_ImageResult NaClO_Rotate(NaClO_Image *data, NaClO_float angleDeg);
NaClO_ErrorType NaClO_Rotated(NaClO_Image *data, NaClO_float angleDeg);
NaClO_ImageResult NaClO_RotateAt(NaClO_Image *data, NaClO_float angleDeg,
                                 NaClO_uint x, NaClO_uint y);
NaClO_ErrorType NaClO_RotatedAt(NaClO_Image *data, NaClO_float angleDeg,
                                NaClO_uint x, NaClO_uint y);

NaClO_ImageResult NaClO_Gamma(NaClO_Image *data, NaClO_float gamma);
NaClO_ErrorType NaClO_SetGamma(NaClO_Image *data, NaClO_float gamma);

NaClO_ImageResult NaClO_Scale(NaClO_Image *data, NaClO_float factor);

NaClO_ImageResult NaClO_Perspective(NaClO_Image *data, NaClO_uint x1,
                                    NaClO_uint y1, NaClO_uint x2, NaClO_uint y2,
                                    NaClO_uint x3, NaClO_uint y3, NaClO_uint x4,
                                    NaClO_uint y4,

                                    NaClO_uint x5, NaClO_uint y5, NaClO_uint x6,
                                    NaClO_uint y6, NaClO_uint x7, NaClO_uint y7,
                                    NaClO_uint x8, NaClO_uint y8);
NaClO_ErrorType NaClO_SetPerspective(
    NaClO_Image *data, NaClO_uint x1, NaClO_uint y1, NaClO_uint x2,
    NaClO_uint y2, NaClO_uint x3, NaClO_uint y3, NaClO_uint x4, NaClO_uint y4,

    NaClO_uint x5, NaClO_uint y5, NaClO_uint x6, NaClO_uint y6, NaClO_uint x7,
    NaClO_uint y7, NaClO_uint x8, NaClO_uint y8);

NaClO_ImageResult NaClO_Convolute(NaClO_Image *data, NaClO_Matrix kernel,
                                  uint8_t stride, uint8_t padding);
NaClO_ErrorType NaClO_Convoluted(NaClO_Image *data, NaClO_Matrix kernel,
                                 uint8_t stride, uint8_t padding);

NaClO_ImageResult NaClO_Laplace(NaClO_Image *data);
NaClO_ErrorType NaClO_SetLaplace(NaClO_Image *data);

NaClO_ImageResult NaClO_Sobel(NaClO_Image *data);
NaClO_ErrorType NaClO_SetSobel(NaClO_Image *data);

NaClO_ImageResult NaClO_Roberts(NaClO_Image *data);
NaClO_ErrorType NaClO_SetRoberts(NaClO_Image *data);

NaClO_ImageResult NaClO_Prewitt(NaClO_Image *data);
NaClO_ErrorType NaClO_SetPrewitt(NaClO_Image *data);

NaClO_ImageResult NaClO_AvgBlur(NaClO_Image *data, NaClO_uint strength);
NaClO_ErrorType NaClO_AvgBlurred(NaClO_Image *data, NaClO_uint strength);

NaClO_ImageResult NaClO_MedianBlur(NaClO_Image *data, NaClO_uint strength);
NaClO_ErrorType NaClO_MedianBlurred(NaClO_Image *data, NaClO_uint strength);

NaClO_ImageResult NaClO_GaussianBlur(NaClO_Image *data, NaClO_uint strength);
NaClO_ErrorType NaClO_GaussianBlurred(NaClO_Image *data, NaClO_uint strength);

NaClO_ImageResult NaClO_Blur(NaClO_Image *data, NaClO_uint strength);
NaClO_ErrorType NaClO_Blurred(NaClO_Image *data, NaClO_uint strength);

NaClO_ImageResult NaClO_HueSaturationValue(NaClO_Image *data, NaClO_float hue,
                                           NaClO_float saturation,
                                           NaClO_float value);
NaClO_ErrorType NaClO_SetHueSaturationValue(NaClO_Image *data, NaClO_float hue,
                                            NaClO_float saturation,
                                            NaClO_float value);
NaClO_ErrorType NaClO_GradientHistogram(NaClO_Image *data,
                                        NaClO_uint *outHistogram);
NaClO_ErrorType NaClO_RGBHistogram(NaClO_Image *data, NaClO_uint *outRHistogram,
                                   NaClO_uint *outGHistogram,
                                   NaClO_uint *outBHistogram

);
NaClO_ImageResult NaClO_HistogramEqualization(NaClO_Image *data);
NaClO_ErrorType NaClO_HistogramEqualized(NaClO_Image *data);
NaClO_ErrorType NaClO_PremultipliedAlpha(NaClO_Image *data);
NaClO_ImageResult NaClO_AlphaPremultiplication(NaClO_Image *data);
NaClO_ErrorType NaClO_Sharpened(NaClO_Image *data);
NaClO_ErrorType NaClO_Reversed(NaClO_Image *data);
NaClO_ImageResult NaClO_ReverseImage(NaClO_Image *data);
NaClO_ErrorType NaClO_Blended(NaClO_Image *I1, NaClO_Image *I2,
                              NaClO_float ratio);
NaClO_ImageResult NaClO_Blend(NaClO_Image *I1, NaClO_Image *I2,
                              NaClO_float ratio);
NaClO_ErrorType NaClO_Dissolved(NaClO_Image *I1, NaClO_Image *I2,
                                NaClO_float ratio);
NaClO_ImageResult NaClO_Dissolve(NaClO_Image *I1, NaClO_Image *I2,
                                 NaClO_float ratio);
NaClO_ImageResult NaClO_Lighten(NaClO_Image *I1, NaClO_Image *I2);
NaClO_ErrorType NaClO_Darkened(NaClO_Image *I1, NaClO_Image *I2);
NaClO_ImageResult NaClO_Darken(NaClO_Image *I1, NaClO_Image *I2);
NaClO_ErrorType NaClO_Multiplied(NaClO_Image *I1, NaClO_Image *I2);
NaClO_ImageResult NaClO_Multiply(NaClO_Image *I1, NaClO_Image *I2);
#define NaClO_Burnt(I1, I2) NaClO_Burned(I1, I2)
NaClO_ErrorType NaClO_Burned(NaClO_Image *I1, NaClO_Image *I2);
#define NaClO_Unwrap(r) (assert((r).Error == NACLO_OK), (r).result)
#define NaClO_EdgeDetect(data) NaClO_Sobel(data)
#define NaClO_MakeHistogram(name) NaClO_uint name[256]

#define NACLO_IMAGE_IMPL
#ifdef NACLO_IMAGE_IMPL

#define __NaClO__makeResult(T)                                                 \
  NaClO_ImageResult(T);                                                        \
  memset(&T, 0, sizeof(T));
#define __NaClO__makeError(T, E)                                               \
  memset(&T, 0, sizeof(T));                                                    \
  T.Error = E;                                                                 \
  return T;

void __NaClO_stbdata_to_array(NaClO_Image *T, int width, int height, int comp,
                              uint8_t *data) {
  for (NaClO_uint x = 0; x < width; ++x) {
    for (NaClO_uint y = 0; y < height; ++y) {
      uint8_t *pixelOffset = data + (x + y * width) * comp;

      switch (comp) {
      case 1: // L
              // printf("%f\n\n",T->data[x][y].L);
        T->data[x][y].L = (NaClO_float)(pixelOffset[0]) / (NaClO_float)(255);
        break;
      case 3:
        T->data[x][y].RGB.r = (pixelOffset[0]);
        T->data[x][y].RGB.g = (pixelOffset[1]);
        T->data[x][y].RGB.b = (pixelOffset[2]);
        break;

      case 4:
        T->data[x][y].RGBA.r = (pixelOffset[0]);
        T->data[x][y].RGBA.g = (pixelOffset[1]);
        T->data[x][y].RGBA.b = (pixelOffset[2]);
        T->data[x][y].RGBA.a = (pixelOffset[3]);
        break;
      }
    }
  }
}
uint8_t __naclo_get_channel_count(NaClO_Image *data) {
  if (data == NULL) {
    return 255;
  }
  uint8_t channel = 114;
  switch (data->mode) {
  case NaClO_RGB:
    channel = 3;
    break;
  case NaClO_RGBA:
    channel = 4;
    break;
  case NaClO_L:
  case NaClO_1:
    channel = 1;
    break;
  }
  return channel;
}
NaClO_ErrorType __naclo_make_memory(NaClO_Image *T, int comp) {

  if (T == NULL) {
    return NACLO_NULL_POINTER;
  }
  int width = T->width;
  int height = T->height;
  T->data = (NaClO_PixelType **)NaClO_CALLOC(sizeof(NaClO_PixelType *), width);
  if (T->data == NULL) {
    return NACLO_MEMORY_ALLOCATION_FAILED;
  }
  for (NaClO_uint i = 0; i < width; ++i) {
    T->data[i] =
        (NaClO_PixelType *)NaClO_CALLOC(sizeof(NaClO_PixelType), height);
    if ((T->data[i]) == NULL) {
      for (NaClO_uint c = 0; c < i; c++) {
        NaClO_FREE(T->data[c]);
      }
      NaClO_FREE(T->data);
      return NACLO_MEMORY_ALLOCATION_FAILED;
    }
  }
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Load(const char *filename) {
  // printf("const char * _Nonnull, ...");
  // NaClO_ImageResult T;
  // memset(&T, 0, sizeof(T));
  __NaClO__makeResult(T);
  int width;
  int height;
  int comp;
  uint8_t *data = stbi_load(filename, &width, &height, &comp, 0);
  if (data == NULL) {
  ERR_MALLOC_FAILED:
    T.Error = NACLO_MEMORY_ALLOCATION_FAILED;

    return T;
  }

  T.result.width = width;
  T.result.height = height;

  switch (comp) {
  case 1: // L
    T.result.mode = NaClO_L;
    break;
  case 2: // UNK
    T.Error = NACLO_UNKNOWN_CHANNEL;
    return T;
  case 3:
    T.result.mode = NaClO_RGB;
    break;
  case 4:
    T.result.mode = NaClO_RGBA;
    break;
  }

  if (__naclo_make_memory(&T.result, comp) == NACLO_MEMORY_ALLOCATION_FAILED) {
    T.Error = NACLO_MEMORY_ALLOCATION_FAILED;
    return T;
  }
  __NaClO_stbdata_to_array(&T.result, width, height, comp, data);
  stbi_image_free(data);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_FreeImage(NaClO_Image *src) {
  if (!src)
    return NACLO_NULL_POINTER;
  if (src->data) {
    for (NaClO_uint x = 0; x < src->width; x++) {
      NaClO_FREE(src->data[x]);
    }
    NaClO_FREE(src->data);
  } else {
    return NACLO_NULL_POINTER;
  }
  src->data = NULL;
  src->width = src->height = 0;
  return NACLO_OK;
}
// NaClO_Image NaClO_Unwrap(NaClO_ImageResult result) {
//   assert(result.Error == NACLO_OK);
//   return result.result;
// }

uint8_t *__NaClO_to_stb_image(NaClO_Image *data, int *comp) {
  if (data == NULL)
    return NULL;

  uint8_t channel = __naclo_get_channel_count(data);
  uint8_t *d = (uint8_t *)NaClO_CALLOC(data->width * data->height * channel,
                                       sizeof(uint8_t));
  if (d == NULL) {
    return NULL;
  }

  for (NaClO_uint x = 0; x < data->width; ++x) {
    for (NaClO_uint y = 0; y < data->height; ++y) {
      uint8_t *pixelOffset = d + (x + y * data->width) * channel;
      switch (channel) {
      case 1: {
        // 👇 关键：根据 mode 决定如何解释单通道数据
        if (data->mode == NaClO_L) {
          // L is NaClO_float in [0,1]
          pixelOffset[0] = (uint8_t)(data->data[x][y].L * 255.0f + 0.5f);
        } else if (data->mode == NaClO_1) {
          // 1-bit: true → 255 (white), false → 0 (black)
          pixelOffset[0] = data->data[x][y].value ? 255 : 0;
        } else {
          pixelOffset[0] = 0; // fallback
        }
        break;
      }
      case 3:
        pixelOffset[0] = data->data[x][y].RGB.r;
        pixelOffset[1] = data->data[x][y].RGB.g;
        pixelOffset[2] = data->data[x][y].RGB.b;
        break;
      case 4:
        pixelOffset[0] = data->data[x][y].RGBA.r;
        pixelOffset[1] = data->data[x][y].RGBA.g;
        pixelOffset[2] = data->data[x][y].RGBA.b;
        pixelOffset[3] = data->data[x][y].RGBA.a;
        break;
      }
    }
  }

  if (comp != NULL) {
    *comp = channel;
  }
  return d;
}
NaClO_ErrorType NaClO_Save(NaClO_Image *data, const char *fileName) {

  if (!data || !fileName)
    return NACLO_NULL_POINTER;
  if (data->data == NULL)
    return NACLO_NULL_POINTER;
  size_t fnlen = strlen(fileName);
  char *fileNameLower = (char *)NaClO_CALLOC(fnlen + 1, sizeof(char));
  if (!fileNameLower)
    return NACLO_MEMORY_ALLOCATION_FAILED;

  for (size_t i = 0; i < fnlen; ++i) {
    fileNameLower[i] = tolower((unsigned char)fileName[i]);
  }

  int channel;
  uint8_t *stbdata = __NaClO_to_stb_image(data, &channel);
  if (!stbdata) {
    NaClO_FREE(fileNameLower);
    return NACLO_MEMORY_ALLOCATION_FAILED;
  }

  bool success = false;
  if (strstr(fileNameLower, ".png")) {
    success = stbi_write_png(fileName, data->width, data->height, channel,
                             stbdata, 0);
  } else if (strstr(fileNameLower, ".bmp")) {
    success =
        stbi_write_bmp(fileName, data->width, data->height, channel, stbdata);
  } else if (strstr(fileNameLower, ".tga")) {
    success =
        stbi_write_tga(fileName, data->width, data->height, channel, stbdata);
  } else if (strstr(fileNameLower, ".jpg") || strstr(fileNameLower, ".jpeg")) {
    success = stbi_write_jpg(fileName, data->width, data->height, channel,
                             stbdata, 95);
  }

  NaClO_FREE(fileNameLower);
  NaClO_FREE(stbdata);

  return success ? NACLO_OK : NACLO_UNKNOWN_OR_UNSUPPORTED_EXTENSION;
}
NaClO_ErrorType NaClO_SaveAndFree(NaClO_Image *data, const char *fileName) {
  NaClO_ErrorType r = NaClO_Save(data, fileName);
  if (r != NACLO_OK) {
    return r;
  }
  return NaClO_FreeImage(data);
}

NaClO_ImageResult NaClO_Resize(NaClO_Image *data, NaClO_uint w, NaClO_uint h) {
  if (data->data == NULL) {
    __NaClO__makeResult(T);
    T.Error = NACLO_NULL_POINTER;
    return T;
  }
  int comp;

  uint8_t *stbdata = __NaClO_to_stb_image(data, &comp);

  // TODO;
  // printf("stb_image");

  uint8_t *result = (uint8_t *)NaClO_CALLOC(sizeof(uint8_t), w * h * comp);
  if (result == NULL) {
    NaClO_FREE(stbdata);
    __NaClO__makeResult(T);
    T.Error = NACLO_MEMORY_ALLOCATION_FAILED;
    return T;
    //  NACLO_NULL_POINTER;
  }
  // printf("  stbir_pixel_layout layout;");

  stbir_pixel_layout layout;
  switch (data->mode) {

  case NaClO_RGB:
    layout = STBIR_RGB;

    break;
  case NaClO_RGBA:
    layout = STBIR_RGBA;
    break;

  case NaClO_L:
  case NaClO_1:
    layout = STBIR_1CHANNEL;
    break;
  }
  // printf("11");
  stbir_resize(stbdata, data->width, data->height, 0, result, w, h, 0, layout,
               STBIR_TYPE_UINT8, STBIR_EDGE_CLAMP, STBIR_FILTER_CUBICBSPLINE);
  __NaClO__makeResult(T);
  if (result == NULL) {
    T.Error = NACLO_MEMORY_ALLOCATION_FAILED;
    return T;
  }
  T.result.width = w;
  T.result.height = h;

  if (__naclo_make_memory(&T.result, comp) == NACLO_MEMORY_ALLOCATION_FAILED) {
    NaClO_FREE(stbdata); // ← 修复点2
    NaClO_FREE(result);  // ← 修复点3
    T.Error = NACLO_MEMORY_ALLOCATION_FAILED;
    return T;
  }

  __NaClO_stbdata_to_array(&T.result, w, h, comp, result);
  NaClO_FREE(stbdata);
  NaClO_FREE(result);

  return T;
  // __NaClO_to_array(NaClO_Image *T, int width, int height, int comp, uint8_t
  // *data);
}

NaClO_ErrorType NaClO_Resized(NaClO_Image *data, NaClO_uint w, NaClO_uint h) {
  NaClO_ImageResult r = NaClO_Resize(data, w, h);
  if (r.Error != NACLO_OK) {
    // NaClO_FreeImage(&r.result);
    return r.Error;
  }
  NaClO_ErrorType err = NaClO_FreeImage(data);
  if (err != NACLO_OK) {
    return err;
  }
  *data = r.result;
  return NACLO_OK;
}

NaClO_PixelType *NaClO_Pixel(NaClO_Image *data, NaClO_uint x, NaClO_uint y) {
  // 没有任何边界检查
  return &data->data[x][y];
}
NaClO_PixelResult NaClO_GetPixel(NaClO_Image *data, NaClO_uint x,
                                 NaClO_uint y) {
  NaClO_PixelResult R;
  if (x >= data->width or y >= data->height) {
    R.Error = NACLO_OUT_OF_BOUNDS;
    return R;
  }
  R.result = data->data[x][y];
  R.Error = NACLO_OK;
  return R;
}
NaClO_ErrorType NaClO_PutPixel(NaClO_Image *data, NaClO_uint x, NaClO_uint y,
                               NaClO_PixelType target) {
  if (x >= data->width or y >= data->height) {
    return NACLO_OUT_OF_BOUNDS;
  }
  data->data[x][y] = target;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Crop(NaClO_Image *data, NaClO_uint x, NaClO_uint y,
                             NaClO_uint w, NaClO_uint h) {

  __NaClO__makeResult(T);
  if (data->data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }

  T.result.width = w;
  T.result.height = h;
  uint8_t channel = __naclo_get_channel_count(data);
  T.result.mode = data->mode;
  if (channel == 255) {
    T.Error = NACLO_UNKNOWN_CHANNEL;
    return T;
  }
  __naclo_make_memory(&T.result, channel);
  for (NaClO_uint x1 = 0; x1 < w; x1++) {
    for (NaClO_uint y1 = 0; y1 < h; y1++) {
      // NaClO_PixelType f=
      T.result.data[x1][y1] = *NaClO_Pixel(data, x1 + x, y1 + y);
    }
  }
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_Cropped(NaClO_Image *data, NaClO_uint x, NaClO_uint y,
                              NaClO_uint w, NaClO_uint h) {
  NaClO_ImageResult r = NaClO_Crop(data, x, y, w, h);
  if (r.Error != NACLO_OK) {
    return r.Error;
  }
  NaClO_ErrorType err = NaClO_FreeImage(data);
  if (err != NACLO_OK) {
    return err;
  }
  *data = r.result;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_CopyImage(NaClO_Image *data) {
  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
    return T;
  }
  if (data->data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
    return T;
  }
  // T.result=*data;
  T.result.height = data->height;
  T.result.width = data->width;
  T.result.mode = data->mode;
  uint8_t comp = __naclo_get_channel_count(&T.result);
  NaClO_ErrorType E = __naclo_make_memory(&T.result, comp);
  // printf("E");
  if (E != NACLO_OK) {
    T.Error = E;
    return T;
  }

  for (NaClO_uint x = 0; x < data->width; ++x) {
    for (NaClO_uint y = 0; y < data->height; ++y) {
      assert(NaClO_GetPixel(data, x, y).Error == NACLO_OK);
      T.result.data[x][y] = data->data[x][y];
      // if (NaClO_GetPixel(data, x, y).Error!=NACLO_OK)
      // {
      //   printf("%ld,%ld\n\n",data->width,data->height);
      // }
    }
  }
  T.Error = NACLO_OK;
  return T;
}

static void __naclo_add_alpha_channel(NaClO_Image *dst, NaClO_Image *src) {
  for (NaClO_uint x = 0; x < src->width; ++x) {
    for (NaClO_uint y = 0; y < src->height; ++y) {
      dst->data[x][y].RGBA.r = src->data[x][y].RGB.r;
      dst->data[x][y].RGBA.g = src->data[x][y].RGB.g;
      dst->data[x][y].RGBA.b = src->data[x][y].RGB.b;
      dst->data[x][y].RGBA.a = 255;
    }
  }
}

// Helper: Convert L (NaClO_float [0,1]) to RGB (uint8_t)
static void __naclo_L_to_RGB(NaClO_Image *dst, NaClO_Image *src) {
  uint8_t val;
  for (NaClO_uint x = 0; x < src->width; ++x) {
    for (NaClO_uint y = 0; y < src->height; ++y) {
      val = (uint8_t)(src->data[x][y].L * 255.0f + 0.5f);
      dst->data[x][y].RGB.r = val;
      dst->data[x][y].RGB.g = val;
      dst->data[x][y].RGB.b = val;
    }
  }
}

// Helper: Convert bool (1-bit) to RGB
static void __naclo_1_to_RGB(NaClO_Image *dst, NaClO_Image *src) {
  uint8_t val;
  for (NaClO_uint x = 0; x < src->width; ++x) {
    for (NaClO_uint y = 0; y < src->height; ++y) {
      val = src->data[x][y].value ? 255 : 0;
      dst->data[x][y].RGB.r = val;
      dst->data[x][y].RGB.g = val;
      dst->data[x][y].RGB.b = val;
    }
  }
}

// Helper: RGB/RGBA to L (luminance)
static void __naclo_RGB_to_L(NaClO_Image *dst, NaClO_Image *src,
                             bool has_alpha) {
  for (NaClO_uint x = 0; x < src->width; ++x) {
    for (NaClO_uint y = 0; y < src->height; ++y) {
      NaClO_float r =
          has_alpha ? src->data[x][y].RGBA.r : src->data[x][y].RGB.r;
      NaClO_float g =
          has_alpha ? src->data[x][y].RGBA.g : src->data[x][y].RGB.g;
      NaClO_float b =
          has_alpha ? src->data[x][y].RGBA.b : src->data[x][y].RGB.b;
      NaClO_float L = 0.299f * r + 0.587f * g + 0.114f * b;
      if (has_alpha && src->mode == NaClO_RGBA) {
        // Optional: premultiply alpha? Your original code did this.
        // But typically L ignores alpha. Let's follow standard luminance.
        // Remove alpha multiplication unless you specifically want it.
        // L = L * (src->data[x][y].RGBA.a / 255.0f); // ← your old way
      }
      dst->data[x][y].L = L / 255.0f; // normalize to [0,1]
    }
  }
}

// Helper: L or RGB/RGBA to 1-bit (threshold at 127/255 ≈ 0.5)
static void __naclo_any_to_1(NaClO_Image *dst, NaClO_Image *src) {
  NaClO_ImageResult L_img = NaClO_ConvertE(src, NaClO_L);
  if (L_img.Error != NACLO_OK) {
    // Caller should handle error; this helper assumes success
    return;
  }
  for (NaClO_uint x = 0; x < src->width; ++x) {
    for (NaClO_uint y = 0; y < src->height; ++y) {
      dst->data[x][y].value = (L_img.result.data[x][y].L > 0.5f);
    }
  }
  NaClO_FreeImage(&L_img.result);
}

NaClO_ImageResult NaClO_ConvertE(NaClO_Image *data, NaClO_ColorMode mode) {
  __NaClO__makeResult(T);
  if (data == NULL || data->data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  if (data->mode == mode) {
    return NaClO_CopyImage(data);
  }

  T.result.width = data->width;
  T.result.height = data->height;
  T.result.mode = mode;

  uint8_t target_comp = 0;
  switch (mode) {
  case NaClO_RGB:
    target_comp = 3;
    break;
  case NaClO_RGBA:
    target_comp = 4;
    break;
  case NaClO_L:
  case NaClO_1:
    target_comp = 1;
    break;
  default:
    __NaClO__makeError(T, NACLO_UNKNOWN_CHANNEL);
  }

  NaClO_ErrorType err = __naclo_make_memory(&T.result, target_comp);
  if (err != NACLO_OK) {
    T.Error = err;
    return T;
  }

  // Dispatch conversion
  if (mode == NaClO_RGB) {
    switch (data->mode) {
    case NaClO_RGBA: {
      // Copy RGB, ignore A (or copy and later set alpha if needed)
      for (NaClO_uint x = 0; x < data->width; ++x)
        for (NaClO_uint y = 0; y < data->height; ++y) {
          T.result.data[x][y].RGB.r = data->data[x][y].RGBA.r;
          T.result.data[x][y].RGB.g = data->data[x][y].RGBA.g;
          T.result.data[x][y].RGB.b = data->data[x][y].RGBA.b;
        }

      break;
    }
    case NaClO_L:
      __naclo_L_to_RGB(&T.result, data);
      break;
    case NaClO_1:
      __naclo_1_to_RGB(&T.result, data);
      break;
    default:
      __NaClO__makeError(T, NACLO_UNKNOWN_CHANNEL);
    }
  } else if (mode == NaClO_RGBA) {
    switch (data->mode) {
    case NaClO_RGB:
      __naclo_add_alpha_channel(&T.result, data);
      break;
    case NaClO_L: {
      __naclo_L_to_RGB(&T.result, data);
      // then set alpha
      for (NaClO_uint x = 0; x < data->width; ++x)
        for (NaClO_uint y = 0; y < data->height; ++y)
          T.result.data[x][y].RGBA.a = 255;
      break;
    }
    case NaClO_1: {
      __naclo_1_to_RGB(&T.result, data);
      for (NaClO_uint x = 0; x < data->width; ++x)
        for (NaClO_uint y = 0; y < data->height; ++y)
          T.result.data[x][y].RGBA.a = 255;
      break;
    }
    default:
      __NaClO__makeError(T, NACLO_UNKNOWN_CHANNEL);
    }
  } else if (mode == NaClO_L) {
    switch (data->mode) {
    case NaClO_RGB:
      __naclo_RGB_to_L(&T.result, data, false);
      break;
    case NaClO_RGBA:
      __naclo_RGB_to_L(&T.result, data, true);
      break;
    case NaClO_1: {
      for (NaClO_uint x = 0; x < data->width; ++x)
        for (NaClO_uint y = 0; y < data->height; ++y)
          T.result.data[x][y].L = data->data[x][y].value ? 1.0f : 0.0f;
      break;
    }
    default:
      __NaClO__makeError(T, NACLO_UNKNOWN_CHANNEL);
    }
  } else if (mode == NaClO_1) {
    __naclo_any_to_1(&T.result, data);
  }

  if (T.Error == NACLO_OK)
    T.Error = NACLO_OK;
  return T;
}
char *__naclo_get_lower_str(const char *src) {
  NaClO_uint u = strlen(src);
  char *c = (char *)NaClO_CALLOC(sizeof(char), u + 1);
  c[u] = '\0';
  for (NaClO_uint i = 0; i < u; ++i) {
    c[i] = tolower(src[i]);
  }
  return c;
}
NaClO_ImageResult NaClO_Convert(NaClO_Image *data, const char *mode_str) {
  if (!data || !mode_str) {
    __NaClO__makeResult(T);
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }

  char *lowerstr = __naclo_get_lower_str(mode_str);
  NaClO_ColorMode mode;
  size_t len = strlen(mode_str);
  if (len == 0)
    goto unknown;

  // Case-insensitive compare
  if ((len == 3 && strcmp(lowerstr, "rgb") == 0)) {
    mode = NaClO_RGB;
  } else if ((len == 4 && strcmp(lowerstr, "rgba") == 0)) {
    mode = NaClO_RGBA;
  } else if ((len == 1 && (mode_str[0] == 'l' || mode_str[0] == 'L'))) {
    mode = NaClO_L;
  } else if ((len == 1 && (mode_str[0] == '1'))) {
    mode = NaClO_1;
  } else {
  unknown:
    NaClO_FREE(lowerstr);
    __NaClO__makeResult(T);
    __NaClO__makeError(T, NACLO_UNKNOWN_OR_UNSUPPORTED_EXTENSION);
  }

  NaClO_FREE(lowerstr);

  return NaClO_ConvertE(data, mode);
}
NaClO_ErrorType NaClO_Converted(NaClO_Image *data, const char *mode_str) {
  NaClO_ImageResult r = NaClO_Convert(data, mode_str);
  if (r.Error != NACLO_OK) {
    // NaClO_FreeImage(&r.result);
    return r.Error;
  }
  NaClO_ErrorType err = NaClO_FreeImage(data);
  if (err != NACLO_OK) {
    return err;
  }
  *data = r.result;
  return NACLO_OK;
}
NaClO_ErrorType NaClO_ConvertedE(NaClO_Image *data, NaClO_ColorMode mode) {
  NaClO_ImageResult r = NaClO_ConvertE(data, mode);
  if (r.Error != NACLO_OK) {
    // NaClO_FreeImage(&r.result);
    return r.Error;
  }
  NaClO_ErrorType err = NaClO_FreeImage(data);
  if (err != NACLO_OK) {
    return err;
  }
  *data = r.result;
  return NACLO_OK;
}

NaClO_ImageResult NaClO_NewImage(NaClO_uint w, NaClO_uint h,
                                 NaClO_ColorMode mode,
                                 NaClO_PixelType fillColor) {
  __NaClO__makeResult(T);
  T.result.width = w;
  T.result.height = h;
  T.result.mode = mode;
  if (w == 0 or h == 0) {
    __NaClO__makeError(T, NACLO_ZERO_WIDTH_OR_HEIGHT);
  }
  int comp = __naclo_get_channel_count(&T.result);
  NaClO_ErrorType err = __naclo_make_memory(&T.result, comp);
  if (err != NACLO_OK) {
    T.Error = err;
    return T;
  }
  for (NaClO_uint x = 0; x < w; ++x) {
    for (NaClO_uint y = 0; y < h; ++y) {
      *NaClO_Pixel(&T.result, x, y) = fillColor;
    }
  }
  T.Error = NACLO_OK;
  return T;
}

NaClO_ImageResult NaClO_Flip(NaClO_Image *data, NaClO_FlipDirection direction) {
  __NaClO__makeResult(T);
  T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    // __NaClO__makeError(T, T.);
    return T;
  }
  for (int x = 0; x < data->width; ++x) {
    for (int y = 0; y < data->height; ++y) {
      switch (direction) {

      case NACLO_FLIP_X:
        *NaClO_Pixel(&T.result, x, y) = *NaClO_Pixel(
            data, x, abs((NaClO_int)data->height - 1 - (NaClO_int)y));
        break;

      case NACLO_FLIP_Y:
        *NaClO_Pixel(&T.result, x, y) = *NaClO_Pixel(
            data, abs((NaClO_int)data->width - 1 - (NaClO_int)x), y);
        break;

      case NACLO_FLIP_NEGATIVE_X:
        *NaClO_Pixel(&T.result, x,
                     abs((NaClO_int)data->height - 1 - (NaClO_int)y)) =
            *NaClO_Pixel(data, x, y);
        break;

      case NACLO_FLIP_NEGATIVE_Y:
        *NaClO_Pixel(&T.result, abs((NaClO_int)data->width - 1 - (NaClO_int)x),
                     y) = *NaClO_Pixel(data, x, y);

        break;
      }
    }
  }
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_Flipped(NaClO_Image *data,
                              NaClO_FlipDirection direction) {
  NaClO_ImageResult r = NaClO_Flip(data, direction);
  if (r.Error != NACLO_OK) {
    // NaClO_FreeImage(&r.result);
    return r.Error;
  }
  NaClO_ErrorType err = NaClO_FreeImage(data);
  if (err != NACLO_OK) {
    return err;
  }
  *data = r.result;
  return NACLO_OK;
}
// typedef enum {
//   NACLO_FLIP_X,
//   NACLO_FLIP_Y,
//   NACLO_FLIP_NEGATIVE_X,
//   NACLO_FLIP_NEGATIVE_Y,
// } NaClO_FlipDirection;

NaClO_ImageResult NaClO_FlipAt(NaClO_Image *data, NaClO_uint pos,
                               NaClO_FlipDirection direction) {
  __NaClO__makeResult(T);
  // T = NaClO_CopyImage(data);
  if (data == NULL) {

    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  switch (direction) {

  case NACLO_FLIP_X:
  case NACLO_FLIP_NEGATIVE_X:

  {

    NaClO_uint sizeY = pos * 2;
    if (sizeY > data->height) {
      T = NaClO_Expand(data, data->width, sizeY, NACLO_EXPAND_DEFAULT);
    } else {
      T = NaClO_Crop(data, 0, 0, data->width, sizeY);
    }
    if (T.Error != NACLO_OK) {
      // T.Error=NACLO_ZERO_WIDTH_OR_HEIGHT;
      return T;
    }
  } break;

  case NACLO_FLIP_Y:
  case NACLO_FLIP_NEGATIVE_Y:

  {

    NaClO_uint sizeX = pos * 2;
    if (sizeX > data->width) {
      T = NaClO_Expand(data, sizeX, data->width, NACLO_EXPAND_DEFAULT);
    } else {
      T = NaClO_Crop(data, 0, 0, sizeX, data->height);
    }
    if (T.Error != NACLO_OK) {
      return T;
    }
  } break;
  }
  NaClO_Flipped(&T.result, direction);
  return T;
}
NaClO_ErrorType NaClO_FlipedAt(NaClO_Image *data, NaClO_uint pos,
                               NaClO_FlipDirection direction) {
  NaClO_ImageResult T = NaClO_FlipAt(data, pos, direction);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FREE(data);
  *data = T.result;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Paste(NaClO_Image *canva, NaClO_Image *object,
                              NaClO_uint w, NaClO_uint h) {
  __NaClO__makeResult(T);

  if (canva == NULL or canva->data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  if (object == NULL or object->data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  if (w > canva->width or h > canva->height) {
    __NaClO__makeError(T, NACLO_OUT_OF_BOUNDS);
  }
  T = NaClO_CopyImage(canva);
  if (T.Error != NACLO_OK) {
    return T;
  }
  NaClO_ImageResult convertedImage = NaClO_ConvertE(object, canva->mode);
  if (convertedImage.Error != NACLO_OK) {
    NaClO_FreeImage(&T.result);
    __NaClO__makeError(T, convertedImage.Error);
  }

  NaClO_uint newWidth = canva->width - w;
  NaClO_uint newHeight = canva->height - h;

  if (newWidth > object->width) {
    newWidth = object->width;
  }
  if (newHeight > object->height) {
    newHeight = object->height;
  }

  NaClO_ErrorType E =
      NaClO_Cropped(&convertedImage.result, 0, 0, newWidth, newHeight);

  if (E != NACLO_OK) {
    NaClO_FreeImage(&convertedImage.result);
    __NaClO__makeError(T, E);
  }

  for (int x = 0; x < convertedImage.result.width; ++x) {
    for (int y = 0; y < convertedImage.result.height; ++y) {
      // NaClO_PixelResult P = NaClO_GetPixel(&convertedImage.result, x, y);
      // if (P.Error != NACLO_OK) {
      //   NaClO_FreeImage(&T.result);
      //   NaClO_FreeImage(&convertedImage.result);

      //   __NaClO__makeError(T, P.Error);
      // }
      // NaClO_PutPixel(&T.result, x + w, y + h, P.result);
      *NaClO_Pixel(&T.result, x + w, y + h) =
          *NaClO_Pixel(&convertedImage.result, x, y);
    }
  }

  NaClO_FreeImage(&convertedImage.result);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_Pasted(NaClO_Image *canva, NaClO_Image *object,
                             NaClO_uint w, NaClO_uint h) {
  NaClO_ImageResult r = NaClO_Paste(canva, object, w, h);
  if (r.Error != NACLO_OK) {
    // NaClO_FreeImage(&r.result);
    return r.Error;
  }
  NaClO_ErrorType err = NaClO_FreeImage(canva);
  if (err != NACLO_OK) {
    return err;
  }
  *canva = r.result;
  return NACLO_OK;
}
NaClO_ErrorType NaClO_DrawCircle(NaClO_Image *canva, NaClO_int x, NaClO_int y,
                                 NaClO_PixelType color, NaClO_uint radius,
                                 bool filled, NaClO_uint radius2) {
  if (filled and radius2 > radius) {
    return NACLO_ZERO_WIDTH_OR_HEIGHT;
  }
  if (canva == NULL) {
    return NACLO_NULL_POINTER;
  }
  NaClO_int startX = x - (NaClO_int)radius;
  NaClO_int endX = x + (NaClO_int)radius;
  NaClO_int startY = y - (NaClO_int)radius;
  NaClO_int endY = y + (NaClO_int)radius;
  NaClO_int r2 = (NaClO_int)(radius * radius);
  NaClO_int r2_inner = filled ? 0 : (NaClO_int)(radius2 * radius2);

  for (NaClO_int ny = startY; ny <= endY; ++ny) {
    for (NaClO_int nx = startX; nx <= endX; ++nx) {
      if (nx < 0 or (NaClO_uint) nx >= canva->width) {
        continue;
      }
      if (ny < 0 or (NaClO_uint) ny >= canva->height) {
        continue;
      }
      NaClO_int dx = nx - x;
      NaClO_int dy = ny - y;
      NaClO_int dist2 = dx * dx + dy * dy;
      if (filled) {
        if (dist2 <= r2) {
          *NaClO_Pixel(canva, (NaClO_uint)nx, (NaClO_uint)ny) = color;
        }
      } else {
        if (dist2 <= r2 and dist2 >= r2_inner) {
          *NaClO_Pixel(canva, (NaClO_uint)nx, (NaClO_uint)ny) = color;
        }
      }
    }
  }
  return NACLO_OK;
}
NaClO_ErrorType NaClO_Dot(NaClO_Image *canva, NaClO_int x, NaClO_int y,
                          NaClO_PixelType color, NaClO_uint radius) {
  return NaClO_DrawCircle(canva, x, y, color, radius, true, 0);
}

NaClO_ErrorType NaClO_DrawLine(NaClO_Image *canva, NaClO_int x1, NaClO_int y1,
                               NaClO_int x2, NaClO_int y2,
                               NaClO_PixelType color, NaClO_uint radius) {
  if (canva == NULL) {
    return NACLO_NULL_POINTER;
  }

  NaClO_int deltax = x1 - x2;
  NaClO_int deltay = y1 - y2;
  NaClO_float L = sqrtf(deltax * deltax + deltay * deltay);
  NaClO_float tanx = (NaClO_float)deltay / deltax;
  for (NaClO_uint x = x1; x < x2; x += radius / 4) {
    NaClO_uint y = (NaClO_float)(x * tanx);
    NaClO_Dot(canva, x, y, color, radius);
  }
  return NACLO_OK;
}

bool __naclo_NaClO_float_compare(NaClO_float a, NaClO_float b) {
  return (fabs(a - b) < 0.000001);
}
NaClO_HSV NaClO_GetHSV(NaClO_ColorModeRGB src) {
  NaClO_float r = (NaClO_float)src.r / 255.0f;
  NaClO_float g = (NaClO_float)src.g / 255.0f;
  NaClO_float b = (NaClO_float)src.b / 255.0f;

  NaClO_float max_val = fmaxf(fmaxf(r, g), b);
  NaClO_float min_val = fminf(fminf(r, g), b);
  NaClO_float delta = max_val - min_val;

  NaClO_HSV ret;
  ret.value = max_val;
  ret.saturation = (max_val == 0.0f) ? 0.0f : (delta / max_val);

  if (delta == 0.0f) {
    // Gray case: undefined hue, set to 0
    ret.hue = 0;
  } else {
    NaClO_float hue;
    if (max_val == r) {
      hue = 60.0f * fmodf((g - b) / delta, 6.0f);
    } else if (max_val == g) {
      hue = 60.0f * ((b - r) / delta + 2.0f);
    } else { // max_val == b
      hue = 60.0f * ((r - g) / delta + 4.0f);
    }
    if (hue < 0)
      hue += 360.0f;
    ret.hue = (uint16_t)(hue + 0.5f); // round to nearest integer
  }

  return ret;
}
NaClO_HSV NaClO_GetHSVRGBA(NaClO_ColorModeRGBA src) {
  NaClO_ColorModeRGB s;
  s.r = src.r;
  s.g = src.g;
  s.b = src.b;
  return NaClO_GetHSV(s);
}
NaClO_ColorModeRGB NaClO_GetRGB(NaClO_HSV src) {
  NaClO_float h = (NaClO_float)src.hue;
  NaClO_float s = src.saturation;
  NaClO_float v = src.value;

  // Handle grayscale
  if (s == 0.0f) {
    uint8_t gray = (uint8_t)(v * 255.0f + 0.5f);
    return (NaClO_ColorModeRGB){gray, gray, gray};
  }

  h = fmodf(h, 360.0f); // Ensure in [0, 360)
  if (h < 0)
    h += 360.0f;

  NaClO_float h_i = floorf(h / 60.0f);
  int i = (int)((int)h_i % 6);
  NaClO_float f = (h / 60.0f) - h_i;
  NaClO_float p = v * (1.0f - s);
  NaClO_float q = v * (1.0f - f * s);
  NaClO_float t = v * (1.0f - (1.0f - f) * s);

  NaClO_float r_f, g_f, b_f;
  switch (i) {
  case 0:
    r_f = v;
    g_f = t;
    b_f = p;
    break;
  case 1:
    r_f = q;
    g_f = v;
    b_f = p;
    break;
  case 2:
    r_f = p;
    g_f = v;
    b_f = t;
    break;
  case 3:
    r_f = p;
    g_f = q;
    b_f = v;
    break;
  case 4:
    r_f = t;
    g_f = p;
    b_f = v;
    break;
  case 5:
    r_f = v;
    g_f = p;
    b_f = q;
    break;
  default:
    r_f = v;
    g_f = v;
    b_f = v;
    break; // fallback
  }

  NaClO_ColorModeRGB ret;
  ret.r = (uint8_t)(r_f * 255.0f + 0.5f);
  ret.g = (uint8_t)(g_f * 255.0f + 0.5f);
  ret.b = (uint8_t)(b_f * 255.0f + 0.5f);
  return ret;
}

void NaClO_EnumeratePixel(NaClO_Image *src, NaClO_SetPixelFunctionPointer f) {
  if (f == NULL) {
    return;
  }
  for (int x = 0; x < src->width; ++x) {
    for (int y = 0; y < src->height; ++y) {
      NaClO_PixelType pixel = *NaClO_Pixel(src, x, y);
      *NaClO_Pixel(src, x, y) = f(pixel, src->mode);
    }
  }
}

NaClO_MatrixResult NaClO_NewMatrix(uint8_t x, uint8_t y) {
  NaClO_MatrixResult M;
  memset(&M, 1, sizeof(M));
  if (x == 0 or y == 0) {
    M.Error = NACLO_ZERO_WIDTH_OR_HEIGHT;
    return M;
  }
  M.result.data = (NaClO_float *)NaClO_CALLOC(sizeof(NaClO_float), x * y);
  // memset(M.result.data, x*y, sizeof(NaClO_float));
  if (M.result.data == NULL) {
    M.Error = NACLO_MEMORY_ALLOCATION_FAILED;
    return M;
  }
  M.result.height = y;
  M.result.width = x;
  M.Error = NACLO_OK;
  return M;
}
NaClO_ErrorType NaClO_FreeMatrix(NaClO_Matrix *matrix) {
  if (matrix == NULL) {
    return NACLO_NULL_POINTER;
  }
  NaClO_FREE(matrix->data);
  matrix->data = NULL;
  matrix->width = 0;
  matrix->height = 0;
  return NACLO_OK;
}

NaClO_float *NaClO_MatrixElementYX(NaClO_Matrix *matrix, uint8_t row,
                                   uint8_t col) {
  return &matrix->data[(NaClO_uint)row * matrix->width + col];
}
NaClO_float *NaClO_MatrixElementYX1(NaClO_Matrix *matrix, uint8_t row,
                                    uint8_t col) {
  return NaClO_MatrixElementYX(matrix, row - 1, col - 1);
}

NaClO_float *NaClO_MatrixElement(NaClO_Matrix *matrix, uint8_t col,
                                 uint8_t row) {
  return &matrix->data[(NaClO_uint)row * matrix->width + col];
}

// 修改后的函数：参数顺序互换 (col, row)
NaClO_float *NaClO_MatrixElement1(NaClO_Matrix *matrix, uint8_t col,
                                  uint8_t row) {
  return NaClO_MatrixElement(matrix, col - 1, row - 1);
}
NaClO_MatrixResult NaClO_CopyMatrix(NaClO_Matrix *matrix) {
  NaClO_MatrixResult M;
  memset(&M, 1, sizeof(M));
  if (matrix == NULL) {
    M.Error = NACLO_ZERO_WIDTH_OR_HEIGHT;
    return M;
  }
  M = NaClO_NewMatrix(matrix->width, matrix->height);
  if (M.Error != NACLO_OK) {
    return M;
  }
  memcpy(M.result.data, matrix->data,
         matrix->width * matrix->height * sizeof(NaClO_float));
  M.Error = NACLO_OK;
  return M;
}
NaClO_MatrixResult NaClO_TransposeMatrix(NaClO_Matrix *matrix) {
  NaClO_MatrixResult M;
  memset(&M, 0, sizeof(M));
  if (matrix == NULL) {
    M.Error = NACLO_NULL_POINTER;
    return M;
  }
  M = NaClO_NewMatrix(matrix->height, matrix->width); // 转置后宽高互换
  if (M.Error != NACLO_OK) {
    return M;
  }
  for (uint8_t i = 0; i < matrix->height; ++i) {
    for (uint8_t j = 0; j < matrix->width; ++j) {
      *NaClO_MatrixElementYX(&M.result, j, i) =
          *NaClO_MatrixElementYX(matrix, i, j);
    }
  }
  M.Error = NACLO_OK;
  return M;
}

NaClO_ErrorType NaClO_TransposedMatrix(NaClO_Matrix *matrix) {
  NaClO_MatrixResult M = NaClO_TransposeMatrix(matrix);
  if (M.Error != NACLO_OK) {
    return M.Error;
  }
  NaClO_FreeMatrix(matrix);
  matrix = &M.result;
  return NACLO_OK;
}
NaClO_MatrixResult NaClO_NewIdentityMatrix(uint8_t w) {
  NaClO_MatrixResult M = NaClO_NewMatrix(w, w);
  // memset(&M, 1, sizeof(M));
  if (M.Error != NACLO_OK) {
    return M;
  }
  for (uint8_t x = 0; x < w; ++x) {
    *NaClO_MatrixElementYX(&M.result, x, x) = 1;
  }
  M.Error = NACLO_OK;
  return M;
}

NaClO_MatrixResult NaClO_GetRowReducedEchelonMatrix(NaClO_Matrix *matrix) {
  NaClO_MatrixResult M;
  memset(&M, 0, sizeof(M));
  if (matrix == NULL) {
    M.Error = NACLO_NULL_POINTER;
    return M;
  }
  if (matrix->width == 0 || matrix->height == 0) {
    M.Error = NACLO_ZERO_WIDTH_OR_HEIGHT;
    return M;
  }

  // 复制原矩阵以避免修改原始数据
  M = NaClO_CopyMatrix(matrix);
  if (M.Error != NACLO_OK) {
    return M;
  }

  NaClO_Matrix *result = &M.result;
  uint8_t rows = result->height;
  uint8_t cols = result->width;

  uint8_t lead = 0; // 主元列索引

  // 高斯-约旦消元法实现行最简阶梯形
  for (uint8_t r = 0; r < rows && lead < cols; r++) {
    // 寻找主元：在当前列中找到绝对值最大的元素
    uint8_t i_max = r;
    for (uint8_t i = r + 1; i < rows; i++) {
      if (fabs(*NaClO_MatrixElementYX(result, lead, i)) >
          fabs(*NaClO_MatrixElementYX(result, lead, i_max))) {
        i_max = i;
      }
    }

    // 如果主元为零，则尝试下一列
    if (__naclo_NaClO_float_compare(*NaClO_MatrixElementYX(result, lead, i_max),
                                    0.0f)) {
      // 找到下一个非零列
      bool found_nonzero = false;
      for (uint8_t c = lead + 1; c < cols; c++) {
        for (uint8_t i = r; i < rows; i++) {
          if (!__naclo_NaClO_float_compare(*NaClO_MatrixElementYX(result, c, i),
                                           0.0f)) {
            // 交换列c和列lead
            for (uint8_t row = 0; row < rows; row++) {
              NaClO_float temp = *NaClO_MatrixElementYX(result, lead, row);
              *NaClO_MatrixElementYX(result, lead, row) =
                  *NaClO_MatrixElementYX(result, c, row);
              *NaClO_MatrixElementYX(result, c, row) = temp;
            }
            i_max = i;
            found_nonzero = true;
            break;
          }
        }
        if (found_nonzero)
          break;
      }

      if (!found_nonzero) {
        // 没有更多非零元素，算法结束
        break;
      }
    }

    // 交换行r和行i_max（如果它们不同）
    if (i_max != r) {
      for (uint8_t j = 0; j < cols; j++) {
        NaClO_float temp = *NaClO_MatrixElementYX(result, j, r);
        *NaClO_MatrixElementYX(result, j, r) =
            *NaClO_MatrixElementYX(result, j, i_max);
        *NaClO_MatrixElementYX(result, j, i_max) = temp;
      }
    }

    // 将主元归一化为1
    NaClO_float pivot = *NaClO_MatrixElementYX(result, lead, r);
    if (!__naclo_NaClO_float_compare(pivot, 0.0f)) {
      for (uint8_t j = 0; j < cols; j++) {
        *NaClO_MatrixElementYX(result, j, r) /= pivot;
      }
    }

    // 将其他行的当前列元素消为0
    for (uint8_t i = 0; i < rows; i++) {
      if (i != r) {
        NaClO_float factor = *NaClO_MatrixElementYX(result, lead, i);
        for (uint8_t j = 0; j < cols; j++) {
          *NaClO_MatrixElementYX(result, j, i) -=
              factor * *NaClO_MatrixElementYX(result, j, r);
        }
      }
    }

    lead++; // 移动到下一列
  }

  M.Error = NACLO_OK;
  return M;
}
NaClO_ErrorType NaClO_RowReducedEchelonMatrix(NaClO_Matrix *matrix) {
  NaClO_MatrixResult M = NaClO_GetRowReducedEchelonMatrix(matrix);
  if (M.Error != NACLO_OK) {
    return M.Error;
  }
  *matrix = M.result;
  return NACLO_OK;
}
NaClO_MatrixResult NaClO_MatrixInversion(NaClO_Matrix *matrix) {
  NaClO_MatrixResult M;
  memset(&M, 0, sizeof(M));

  if (matrix == NULL) {
    M.Error = NACLO_NULL_POINTER;
    return M;
  }

  if (matrix->width == 0 || matrix->height == 0) {
    M.Error = NACLO_ZERO_WIDTH_OR_HEIGHT;
    return M;
  }

  if (matrix->width != matrix->height) {
    M.Error = NACLO_NOT_A_SQUARE_MATRIX;
    return M;
  }

  uint8_t n = matrix->width;

  // 创建增广矩阵 [A|I]
  NaClO_float *augmented =
      (NaClO_float *)NaClO_CALLOC(n * n * 2, sizeof(NaClO_float));
  if (augmented == NULL) {
    M.Error = NACLO_MEMORY_ALLOCATION_FAILED;
    return M;
  }

  // 初始化增广矩阵
  for (uint8_t y = 0; y < n; y++) {
    for (uint8_t x = 0; x < n; x++) {
      augmented[y * (n * 2) + x] = *NaClO_MatrixElement(matrix, x, y);
      augmented[y * (n * 2) + x + n] = (y == x) ? 1.0f : 0.0f;
    }
  }

  // 高斯-约旦消元法
  for (uint8_t i = 0; i < n; i++) {
    // 寻找主元（部分选主元）
    uint8_t pivot_row = i;
    NaClO_float max_val = fabs(augmented[i * (n * 2) + i]);

    for (uint8_t k = i + 1; k < n; k++) {
      NaClO_float val = fabs(augmented[k * (n * 2) + i]);
      if (val > max_val) {
        max_val = val;
        pivot_row = k;
      }
    }

    // 如果主元为0，矩阵不可逆
    if (__naclo_NaClO_float_compare(max_val, 0.0f)) {
      NaClO_FREE(augmented);
      M.Error = NACLO_SINGULAR_MATRIX; // 矩阵奇异
      return M;
    }

    // 交换行（如果需要）
    if (pivot_row != i) {
      for (uint8_t j = 0; j < n * 2; j++) {
        NaClO_float temp = augmented[i * (n * 2) + j];
        augmented[i * (n * 2) + j] = augmented[pivot_row * (n * 2) + j];
        augmented[pivot_row * (n * 2) + j] = temp;
      }
    }

    // 将主元归一化为1
    NaClO_float pivot = augmented[i * (n * 2) + i];
    for (uint8_t j = 0; j < n * 2; j++) {
      augmented[i * (n * 2) + j] /= pivot;
    }

    // 消去其他行
    for (uint8_t k = 0; k < n; k++) {
      if (k != i) {
        NaClO_float factor = augmented[k * (n * 2) + i];
        for (uint8_t j = 0; j < n * 2; j++) {
          augmented[k * (n * 2) + j] -= factor * augmented[i * (n * 2) + j];
        }
      }
    }
  }

  // 创建结果矩阵
  M = NaClO_NewMatrix(n, n);
  if (M.Error != NACLO_OK) {
    NaClO_FREE(augmented);
    return M;
  }

  // 从增广矩阵提取逆矩阵
  for (uint8_t x = 0; x < n; x++) {
    for (uint8_t y = 0; y < n; y++) {
      *NaClO_MatrixElement(&M.result, y, x) = augmented[x * (n * 2) + y + n];
    }
  }

  NaClO_FREE(augmented);
  M.Error = NACLO_OK;
  return M;
}
NaClO_MatrixResult NaClO_DotProduct(NaClO_Matrix *matrix1,
                                    NaClO_Matrix *matrix2) {
  NaClO_MatrixResult M;
  memset(&M, 1, sizeof(M));

  if (matrix1 == NULL) {
    M.Error = NACLO_NULL_POINTER;
    return M;
    // return NACLO_NULL_POINTER;
  }
  if (matrix2 == NULL) {
    return NaClO_CopyMatrix(matrix1);
  }
  M = NaClO_CopyMatrix(matrix2);
  if (M.Error != NACLO_OK) {
    return M;
  }
  if (matrix1->width != matrix2->width and matrix1->height != matrix2->height) {
    if (matrix1->height == 1) {
      for (uint8_t x = 0; x < matrix2->width; ++x) {
        for (uint8_t y = 0; y < matrix2->height; ++y) {
          (*NaClO_MatrixElement(&M.result, x, y)) *=
              (*NaClO_MatrixElement(matrix1, x, 0));
        }
      }
      M.Error = NACLO_OK;
      return M;
    }
    M.Error = NACLO_DISMATCHED_CHANNEL;
    return M;
  }
  for (uint8_t x = 0; x < matrix1->width; ++x) {
    for (uint8_t y = 0; y < matrix1->height; ++y) {
      (*NaClO_MatrixElement(&M.result, x, y)) *=
          (*NaClO_MatrixElement(matrix1, x, y));
    }
  }
  M.Error = NACLO_OK;
  return M;
}

NaClO_MatrixResult NaClO_MatrixMultiply(NaClO_Matrix *A, NaClO_Matrix *B) {
  // A: [m x n], B: [n x p] → C: [m x p]
  if (A->width != B->height) {
    NaClO_MatrixResult err;
    memset(&err, 0, sizeof(err));
    err.Error = NACLO_DISMATCHED_CHANNEL;
    return err;
  }

  NaClO_MatrixResult C =
      NaClO_NewMatrix(B->width, A->height); // width=p, height=m
  if (C.Error != NACLO_OK) {
    return C;
  }

  for (uint8_t i = 0; i < A->height; ++i) {  // A 的行
    for (uint8_t j = 0; j < B->width; ++j) { // B 的列
      NaClO_float sum = 0.0f;
      for (uint8_t k = 0; k < A->width; ++k) { // A 的列 == B 的行
        sum +=
            (*NaClO_MatrixElement(A, k, i)) * (*NaClO_MatrixElement(B, j, k));
      }
      *NaClO_MatrixElement(&C.result, j, i) = sum;
    }
  }
  return C;
}
NaClO_float __naclo_deg_to_rad(NaClO_float deg) {
  return (NaClO_float)deg * NaClO_PI / 180.0f;
}
NaClO_MatrixResult NaClO_GetRotationMatrix2D(uint8_t centerX, uint8_t centerY,
                                             NaClO_float angleDeg,
                                             NaClO_float scale) {
  NaClO_float angleRad = __naclo_deg_to_rad(angleDeg);

  // T1: 平移 (-centerX, -centerY)
  NaClO_MatrixResult T1R = NaClO_NewMatrix(3, 3);
  if (T1R.Error != NACLO_OK)
    return T1R;
  NaClO_Matrix *T1 = &T1R.result;
  *NaClO_MatrixElement(T1, 0, 0) = 1;
  *NaClO_MatrixElement(T1, 1, 1) = 1;
  *NaClO_MatrixElement(T1, 2, 2) = 1;
  *NaClO_MatrixElement(T1, 0, 2) = -centerX; // tx
  *NaClO_MatrixElement(T1, 1, 2) = -centerY; // ty

  // RS: 旋转 + 缩放
  NaClO_MatrixResult RSR = NaClO_NewMatrix(3, 3);
  if (RSR.Error != NACLO_OK) {
    NaClO_FreeMatrix(&T1R.result);
    return RSR;
  }
  NaClO_Matrix *RS = &RSR.result;
  *NaClO_MatrixElement(RS, 0, 0) = cos(angleRad) * scale;
  *NaClO_MatrixElement(RS, 0, 1) = -sin(angleRad) * scale;
  *NaClO_MatrixElement(RS, 1, 0) = sin(angleRad) * scale;
  *NaClO_MatrixElement(RS, 1, 1) = cos(angleRad) * scale;
  *NaClO_MatrixElement(RS, 2, 2) = 1;

  // T2: 平移 (+centerX, +centerY)
  NaClO_MatrixResult T2R = NaClO_NewMatrix(3, 3);
  if (T2R.Error != NACLO_OK) {
    NaClO_FreeMatrix(&T1R.result);
    NaClO_FreeMatrix(&RSR.result);
    return T2R;
  }
  NaClO_Matrix *T2 = &T2R.result;
  *NaClO_MatrixElement(T2, 0, 0) = 1;
  *NaClO_MatrixElement(T2, 1, 1) = 1;
  *NaClO_MatrixElement(T2, 2, 2) = 1;
  *NaClO_MatrixElement(T2, 0, 2) = centerX; // tx
  *NaClO_MatrixElement(T2, 1, 2) = centerY; // ty

  // M = T2 × RS × T1
  NaClO_MatrixResult temp1 = NaClO_MatrixMultiply(RS, T1);
  if (temp1.Error != NACLO_OK) {
    NaClO_FreeMatrix(&T1R.result);
    NaClO_FreeMatrix(&RSR.result);
    NaClO_FreeMatrix(&T2R.result);
    NaClO_MatrixResult err;
    memset(&err, 0, sizeof(err));
    err.Error = NACLO_UNKNOWN_ERROR;
    return err;
  }
  NaClO_MatrixResult M = NaClO_MatrixMultiply(T2, &temp1.result);
  NaClO_FreeMatrix(&temp1.result);
  NaClO_FreeMatrix(&T1R.result);
  NaClO_FreeMatrix(&RSR.result);
  NaClO_FreeMatrix(&T2R.result);
  return M;

cleanup:
  NaClO_FreeMatrix(&T1R.result);
  NaClO_FreeMatrix(&RSR.result);
  NaClO_FreeMatrix(&T2R.result);
  NaClO_MatrixResult err;
  memset(&err, 0, sizeof(err));
  err.Error = NACLO_UNKNOWN_ERROR;
  return err;
}
NaClO_PixelType __naclo_bilinear_sample(NaClO_Image *img, NaClO_float x,
                                        NaClO_float y) {
  if (x < 0 || y < 0 || x >= (NaClO_float)(img->width - 1) ||
      y >= (NaClO_float)(img->height - 1)) {
    // 越界返回黑色/透明
    NaClO_PixelType out;
    memset(&out, 1, sizeof(NaClO_PixelType));

    return out;
  }

  NaClO_uint x0 = (NaClO_uint)x;
  NaClO_uint y0 = (NaClO_uint)y;
  NaClO_uint x1 = x0 + 1;
  NaClO_uint y1 = y0 + 1;

  NaClO_float dx = x - x0;
  NaClO_float dy = y - y0;

  NaClO_PixelType p00 = *NaClO_Pixel(img, x0, y0);
  NaClO_PixelType p01 = *NaClO_Pixel(img, x0, y1);
  NaClO_PixelType p10 = *NaClO_Pixel(img, x1, y0);
  NaClO_PixelType p11 = *NaClO_Pixel(img, x1, y1);

  NaClO_PixelType result;
  memset(&result, 1, sizeof(result));

  switch (img->mode) {
  case NaClO_RGB:
    result.RGB.r =
        (uint8_t)(p00.RGB.r * (1 - dx) * (1 - dy) + p10.RGB.r * dx * (1 - dy) +
                  p01.RGB.r * (1 - dx) * dy + p11.RGB.r * dx * dy);
    result.RGB.g =
        (uint8_t)(p00.RGB.g * (1 - dx) * (1 - dy) + p10.RGB.g * dx * (1 - dy) +
                  p01.RGB.g * (1 - dx) * dy + p11.RGB.g * dx * dy);
    result.RGB.b =
        (uint8_t)(p00.RGB.b * (1 - dx) * (1 - dy) + p10.RGB.b * dx * (1 - dy) +
                  p01.RGB.b * (1 - dx) * dy + p11.RGB.b * dx * dy);
    break;
  case NaClO_RGBA:
    result.RGBA.r =
        (uint8_t)(p00.RGBA.r * (1 - dx) * (1 - dy) +
                  p10.RGBA.r * dx * (1 - dy) + p01.RGBA.r * (1 - dx) * dy +
                  p11.RGBA.r * dx * dy);
    result.RGBA.g =
        (uint8_t)(p00.RGBA.g * (1 - dx) * (1 - dy) +
                  p10.RGBA.g * dx * (1 - dy) + p01.RGBA.g * (1 - dx) * dy +
                  p11.RGBA.g * dx * dy);
    result.RGBA.b =
        (uint8_t)(p00.RGBA.b * (1 - dx) * (1 - dy) +
                  p10.RGBA.b * dx * (1 - dy) + p01.RGBA.b * (1 - dx) * dy +
                  p11.RGBA.b * dx * dy);
    result.RGBA.a =
        (uint8_t)(p00.RGBA.a * (1 - dx) * (1 - dy) +
                  p10.RGBA.a * dx * (1 - dy) + p01.RGBA.a * (1 - dx) * dy +
                  p11.RGBA.a * dx * dy);
    break;
  case NaClO_L:
    result.L = p00.L * (1 - dx) * (1 - dy) + p10.L * dx * (1 - dy) +
               p01.L * (1 - dx) * dy + p11.L * dx * dy;
    break;
  case NaClO_1:
    // 对二值图，使用阈值
    result.value = (p00.value && p01.value && p10.value && p11.value);
    break;
  }
  return result;
}
// 主函数：WarpAffine（逆映射）
NaClO_ImageResult NaClO_WarpAffine(NaClO_Image *data, NaClO_Matrix *M) {
  __NaClO__makeResult(T);
  if (!data || !M) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  if (M->width != 3 || M->height != 3) {
    __NaClO__makeError(T, NACLO_DISMATCHED_CHANNEL);
  }

  // 计算逆矩阵 M⁻¹
  NaClO_MatrixResult invR = NaClO_MatrixInversion(M);
  if (invR.Error != NACLO_OK) {
    __NaClO__makeError(T, invR.Error);
  }
  NaClO_Matrix *M_inv = &invR.result;

  // 创建与原图同尺寸的输出图像
  T = NaClO_NewImage(data->width, data->height, data->mode,
                     (NaClO_PixelType){0});
  if (T.Error != NACLO_OK) {
    NaClO_FreeMatrix(M_inv);
    return T;
  }

  // 逆映射：对输出图每个像素 (u,v)，计算其在原图中的位置
  for (NaClO_uint u = 0; u < data->width; ++u) {
    for (NaClO_uint v = 0; v < data->height; ++v) {
      // 构造目标点齐次坐标 [u, v, 1]^T
      NaClO_Matrix dst_pt;
      NaClO_float pt_data[3] = {(NaClO_float)u, (NaClO_float)v, 1.0f};
      dst_pt.data = pt_data;
      dst_pt.width = 1;
      dst_pt.height = 3;

      // 计算 src = M⁻¹ × dst
      NaClO_MatrixResult srcR = NaClO_MatrixMultiply(M_inv, &dst_pt);
      if (srcR.Error != NACLO_OK) {
        NaClO_FreeImage(&T.result);
        NaClO_FreeMatrix(M_inv);
        __NaClO__makeError(T, srcR.Error);
      }

      NaClO_float src_x = *NaClO_MatrixElementYX(&srcR.result, 0, 0);
      NaClO_float src_y = *NaClO_MatrixElementYX(&srcR.result, 1, 0);
      NaClO_FreeMatrix(&srcR.result);

      // 使用双线性插值采样
      NaClO_PixelType pixel = __naclo_bilinear_sample(data, src_x, src_y);
      *NaClO_Pixel(&T.result, u, v) = pixel;
    }
  }

  NaClO_FreeMatrix(M_inv);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_WarpedAffine(NaClO_Image *data, NaClO_Matrix *M) {
  NaClO_ImageResult F = NaClO_WarpAffine(data, M);
  if (F.Error != NACLO_OK) {
    return F.Error;
  }
  NaClO_FreeImage(data);
  *data = F.result;
  return NACLO_OK;
}
static void __naclo_calculate_rotated_size(NaClO_uint orig_w, NaClO_uint orig_h,
                                           NaClO_float angleDeg,
                                           NaClO_uint *new_w,
                                           NaClO_uint *new_h) {
  NaClO_float angleRad = __naclo_deg_to_rad(angleDeg);
  NaClO_float abs_cos = fabsf(cosf(angleRad));
  NaClO_float abs_sin = fabsf(sinf(angleRad));

  // 这与博客中的 MATLAB 代码逻辑一致:
  // new_m = ceil(abs(m*cosd(degree)) + abs(n*sind(degree)));
  // new_n = ceil(abs(n*cosd(degree)) + abs(m*sind(degree)));
  *new_w = (NaClO_uint)ceilf((NaClO_float)orig_w * abs_cos +
                             (NaClO_float)orig_h * abs_sin);
  *new_h = (NaClO_uint)ceilf((NaClO_float)orig_h * abs_cos +
                             (NaClO_float)orig_w * abs_sin);
}

NaClO_ImageResult NaClO_Expand(NaClO_Image *data, NaClO_uint newWidth,
                               NaClO_uint newHeight,
                               NaClO_ExpandMode expandMode) {

  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
    return T;
  }
  NaClO_PixelType Pt;
  memset(&Pt, 1, sizeof(Pt));
  NaClO_ImageResult T2 = NaClO_NewImage(newWidth, newHeight, data->mode, Pt);
  if (T2.Error != NACLO_OK) {
    __NaClO__makeError(T, T2.Error);
    return T;
  }
  NaClO_uint pasteWidth = (newWidth - data->width) / 2;
  NaClO_uint pasteHeight = (newHeight - data->height) / 2;
  T = NaClO_Paste(&T2.result, data, pasteWidth, pasteHeight);
  if (T.Error != NACLO_OK) {
    // __NaClO__makeError(T, T)
    return T;
  }
  NaClO_FreeImage(&T2.result);
  switch (expandMode) {

  case NACLO_EXPAND_DEFAULT:
    break;
  case NACLO_EXPAND_MIRROR: {

    NaClO_uint mirrorPosX = pasteWidth + 1;
    for (NaClO_uint x = pasteWidth - 1; x > 0; --x) {
      NaClO_uint deltaX = pasteWidth - x;
      mirrorPosX = pasteWidth + deltaX;
      if (mirrorPosX - pasteWidth > data->width) {
        mirrorPosX -= data->width;
      }
      for (NaClO_uint y = 0; y < data->height; ++y) {

        *NaClO_Pixel(&T.result, x, y + pasteHeight) =
            *NaClO_Pixel(&T.result, mirrorPosX, y + pasteHeight);
      }
    }

    for (NaClO_uint x = (pasteWidth + data->width) + 1; x < T.result.width;
         ++x) {
      NaClO_uint deltaX = x - (pasteWidth + data->width);
      mirrorPosX = (pasteWidth + data->width) - deltaX - 1;
      if (mirrorPosX - data->width < 0) {
        mirrorPosX += data->width;
      }
      for (NaClO_uint y = 0; y < data->height; ++y) {

        *NaClO_Pixel(&T.result, x, y + pasteHeight) =
            *NaClO_Pixel(&T.result, mirrorPosX, y + pasteHeight);
      }
    }

    NaClO_uint mirrorPosY;
    for (NaClO_uint y = pasteHeight; y > 0; --y) {
      NaClO_uint deltaY = y - pasteHeight;
      mirrorPosY = pasteHeight - deltaY;
      if (mirrorPosY - pasteHeight > data->height) {
        mirrorPosY -= data->height;
      }
      for (NaClO_uint x = 0; x < T.result.width; ++x) {
        *NaClO_Pixel(&T.result, x, y - 1) =
            *NaClO_Pixel(&T.result, x, mirrorPosY);
      }
    }

    for (NaClO_uint y = pasteHeight + data->height; y < T.result.height; ++y) {
      NaClO_uint deltaY = y - (pasteHeight + data->height);
      mirrorPosY = pasteHeight - deltaY - 1;
      if (mirrorPosY - pasteHeight < 0) {
        mirrorPosY += data->height;
      }
      for (NaClO_uint x = 0; x < T.result.width; ++x) {
        *NaClO_Pixel(&T.result, x, y) = *NaClO_Pixel(&T.result, x, mirrorPosY);
      }
    }

  } break;
  case NACLO_EXPAND_EDGE: {
    for (NaClO_uint x = pasteWidth - 1; x > 0; --x) {
      for (NaClO_uint y = 0; y < data->height; ++y) {
        *NaClO_Pixel(&T.result, x, y + pasteHeight) =
            *NaClO_Pixel(&T.result, pasteWidth, y + pasteHeight);
      }
    }
    for (NaClO_uint x = (pasteWidth + data->width) + 1; x < T.result.width;
         ++x) {
      for (NaClO_uint y = 0; y < data->height; ++y) {
        *NaClO_Pixel(&T.result, x, y + pasteHeight) = *NaClO_Pixel(
            &T.result, pasteWidth + data->width - 1, y + pasteHeight);
      }
    }
    for (NaClO_uint y = pasteHeight; y > 0; --y) {
      for (NaClO_uint x = 0; x < T.result.width; ++x) {
        *NaClO_Pixel(&T.result, x, y - 1) =
            *NaClO_Pixel(&T.result, x, pasteHeight);
      }
    }
    for (NaClO_uint y = pasteHeight + data->height; y < T.result.height; ++y) {
      for (NaClO_uint x = 0; x < T.result.width; ++x) {
        *NaClO_Pixel(&T.result, x, y) =
            *NaClO_Pixel(&T.result, x, pasteHeight + data->height - 1);
      }
    }
  } break;
  }
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_Expanded(NaClO_Image *data, NaClO_uint newWidth,
                               NaClO_uint newHeight,
                               NaClO_ExpandMode expandMode) {
  NaClO_ImageResult T = NaClO_Expand(data, newHeight, newHeight, expandMode);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}
// 主函数：修复后的 NaClO_Rotate
NaClO_ImageResult NaClO_Rotate(NaClO_Image *data, NaClO_float angleDeg) {
  __NaClO__makeResult(T);

  if (!data || !data->data) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }

  // 1. 计算新图像的尺寸
  NaClO_uint new_w, new_h;
  __naclo_calculate_rotated_size(data->width, data->height, angleDeg, &new_w,
                                 &new_h);

  // 2. 创建新尺寸的空白图像
  NaClO_PixelType fill_pixel = {0}; // 默认填充黑色/透明
  T = NaClO_NewImage(new_w, new_h, data->mode, fill_pixel);
  if (T.Error != NACLO_OK) {
    return T;
  }

  // 3. 构建完整的正向变换矩阵 M (从原图坐标到新图坐标)
  // 根据博客公式(9)的推导，我们需要组合三个变换：
  // a. 将原图坐标系平移到其中心 (T1)
  // b. 绕原点旋转 (R)
  // c. 将旋转后的坐标系平移回新图的中心 (T2)

  NaClO_float angleRad = __naclo_deg_to_rad(angleDeg);
  NaClO_float cos_a = cosf(angleRad);
  NaClO_float sin_a = sinf(angleRad);

  // 原图中心
  NaClO_float orig_cx = (NaClO_float)data->width / 2.0f;
  NaClO_float orig_cy = (NaClO_float)data->height / 2.0f;
  // 新图中心
  NaClO_float new_cx = (NaClO_float)new_w / 2.0f;
  NaClO_float new_cy = (NaClO_float)new_h / 2.0f;

  // 构建正向变换矩阵 M = T2 * R * T1
  // 我们直接计算最终的 3x3 矩阵元素
  NaClO_MatrixResult MR = NaClO_NewMatrix(3, 3);
  if (MR.Error != NACLO_OK) {
    NaClO_FreeImage(&T.result);
    __NaClO__makeError(T, MR.Error);
  }
  NaClO_Matrix *M = &MR.result;

  // 第一列
  *NaClO_MatrixElementYX(M, 0, 0) = cos_a;
  *NaClO_MatrixElementYX(M, 1, 0) = -sin_a;
  *NaClO_MatrixElementYX(M, 2, 0) = 0;

  // 第二列
  *NaClO_MatrixElementYX(M, 0, 1) = sin_a;
  *NaClO_MatrixElementYX(M, 1, 1) = cos_a;
  *NaClO_MatrixElementYX(M, 2, 1) = 0;

  // 第三列 (平移分量)
  *NaClO_MatrixElementYX(M, 0, 2) = -orig_cx * cos_a - orig_cy * sin_a + new_cx;
  *NaClO_MatrixElementYX(M, 1, 2) = orig_cx * sin_a - orig_cy * cos_a + new_cy;
  *NaClO_MatrixElementYX(M, 2, 2) = 1.0f;

  // 4. 计算逆变换矩阵 M_inv，用于反向映射
  NaClO_MatrixResult invR = NaClO_MatrixInversion(M);
  NaClO_FreeMatrix(M); // 正向矩阵不再需要
  if (invR.Error != NACLO_OK) {
    NaClO_FreeImage(&T.result);
    __NaClO__makeError(T, invR.Error);
  }
  NaClO_Matrix *M_inv = &invR.result;

  // 5. 反向映射：遍历新图像的每个像素
  for (NaClO_uint v = 0; v < new_h; ++v) {   // 行 (y)
    for (NaClO_uint u = 0; u < new_w; ++u) { // 列 (x)
      // 目标点齐次坐标 [u, v, 1]
      NaClO_float dst_pt_data[3] = {(NaClO_float)u, (NaClO_float)v, 1.0f};
      NaClO_Matrix dst_pt = {dst_pt_data, 1, 3};

      // 计算 src = M_inv * dst
      NaClO_MatrixResult srcR = NaClO_MatrixMultiply(M_inv, &dst_pt);
      if (srcR.Error != NACLO_OK) {
        NaClO_FreeMatrix(M_inv);
        NaClO_FreeImage(&T.result);
        __NaClO__makeError(T, srcR.Error);
      }

      NaClO_float src_x = *NaClO_MatrixElementYX(&srcR.result, 0, 0);
      NaClO_float src_y = *NaClO_MatrixElementYX(&srcR.result, 1, 0);
      NaClO_FreeMatrix(&srcR.result);

      // 6. 使用双线性插值从原图采样
      NaClO_PixelType pixel = __naclo_bilinear_sample(data, src_x, src_y);
      *NaClO_Pixel(&T.result, u, v) = pixel;
    }
  }

  NaClO_FreeMatrix(M_inv);
  T.Error = NACLO_OK;
  return T;
}

// 修复后的 NaClO_Rotated
NaClO_ErrorType NaClO_Rotated(NaClO_Image *data, NaClO_float angleDeg) {
  NaClO_ImageResult F = NaClO_Rotate(data, angleDeg);
  if (F.Error != NACLO_OK) {
    return F.Error;
  }
  NaClO_FreeImage(data);
  *data = F.result;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_RotateAt(NaClO_Image *data, NaClO_float angleDeg,
                                 NaClO_uint x, NaClO_uint y) {
  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  NaClO_uint deltaX = data->width / 2 - x;
  NaClO_uint deltaY = data->height / 2 - y;
  NaClO_uint newWidth = data->width + fabs(deltaX);
  NaClO_uint newHeight = data->height + fabs(deltaY);

  NaClO_PixelType pt;
  memset(&pt, 1, sizeof(pt));
  NaClO_ImageResult T2 = NaClO_NewImage(newWidth, newHeight, data->mode, pt);
  if (T2.Error != NACLO_OK) {
    __NaClO__makeError(T, T2.Error);
  }
  NaClO_uint pasteWidth = (newWidth - deltaX - data->width) / 2;
  NaClO_uint pasteHeight = (newHeight - deltaY - data->height) / 2;
  NaClO_Pasted(&T2.result, data, pasteWidth, pasteHeight);
  T = NaClO_Rotate(&T2.result, angleDeg);
  NaClO_FreeImage(&T2.result);

  // NaClO_uint newX=data->width+deltaX;
  // NaClO_uint  newY=data->height+deltaY  ;
  // T.Error=NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_RotatedAt(NaClO_Image *data, NaClO_float angleDeg,
                                NaClO_uint x, NaClO_uint y) {
  NaClO_ImageResult F = NaClO_RotateAt(data, angleDeg, x, y);
  if (F.Error != NACLO_OK) {
    return F.Error;
  }
  NaClO_FreeImage(data);
  *data = F.result;
  return NACLO_OK;
}

NaClO_ErrorType NaClO_SetGamma(NaClO_Image *data, NaClO_float gamma) {
  for (int x = 0; x < data->width; ++x) {
    for (int y = 0; y < data->height; ++y) {
      {
        NaClO_PixelType p = *NaClO_Pixel(data, x, y);

        switch (data->mode) {
        case NaClO_RGB: {
          NaClO_HSV hsvRGB = NaClO_GetHSV(p.RGB);
          // p.RGB.r = pow(p.RGB.r, gamma);
          // p.RGB.g = pow(p.RGB.g, gamma);
          // p.RGB.b = pow(p.RGB.b, gamma);
          hsvRGB.value = pow(hsvRGB.value, gamma);
          p.RGB = NaClO_GetRGB(hsvRGB);
        }

        break;
        case NaClO_RGBA: {
          NaClO_HSV hsvRGBA = NaClO_GetHSVRGBA(p.RGBA);
          hsvRGBA.value = pow(hsvRGBA.value, gamma);
          NaClO_ColorModeRGB rgb = NaClO_GetRGB(hsvRGBA);
          NaClO_ColorModeRGBA rgba;
          rgba.r = rgb.r;
          rgba.g = rgb.g;
          rgba.b = rgb.b;
          rgba.a = p.RGBA.a;
          p.RGBA = rgba;
          // p.RGBA.r = pow(p.RGBA.r, gamma);
          // p.RGBA.g = pow(p.RGBA.g, gamma);
          // p.RGBA.b = pow(p.RGBA.b, gamma);
        }

        break;
        case NaClO_L:
          p.L = pow(p.L, gamma);
          break;
        case NaClO_1:
          break;
        }
        *NaClO_Pixel(data, x, y) = p;
      };
    }
  };
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Gamma(NaClO_Image *data, NaClO_float gamma) {
  __NaClO__makeResult(T);
  T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    return T;
  }
  // NaClO_EnumeratePixel(&T.result, __naclo__gamma);
  if (data->mode == NaClO_1) {
    __NaClO__makeError(T, NACLO_DISMATCHED_CHANNEL);
  }

  NaClO_SetGamma(&T.result, gamma);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ImageResult NaClO_Scale(NaClO_Image *data, NaClO_float factor) {
  if (data == NULL) {
    __NaClO__makeResult(T);
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  return NaClO_Resize(data, data->width * factor, data->height * factor);
}
NaClO_ErrorType NaClO_Scaled(NaClO_Image *data, NaClO_float factor) {
  if (data == NULL) {
    return NACLO_NULL_POINTER;
  }
  return NaClO_Resized(data, data->width * factor, data->height * factor);
}

NaClO_ImageResult NaClO_Perspective(NaClO_Image *data, NaClO_uint x1,
                                    NaClO_uint y1, NaClO_uint x2, NaClO_uint y2,
                                    NaClO_uint x3, NaClO_uint y3, NaClO_uint x4,
                                    NaClO_uint y4,

                                    NaClO_uint x5, NaClO_uint y5, NaClO_uint x6,
                                    NaClO_uint y6, NaClO_uint x7, NaClO_uint y7,
                                    NaClO_uint x8, NaClO_uint y8) {
  __NaClO__makeResult(T);
  /*H=  [a b c]
        [d e f]
        [g h 1]

    x5=(ax1+by1+c) / (gx1+hy1+1)
    y5=(dx1+ey1+f) / (gx1+hy1+1)

    x6=(ax2+by2+c) / (gx2+hy2+1)
    y6=(dx2+ey2+f) / (gx2+hy2+1)

    x7=(ax3+by3+c) / (gx3+hy3+1)
    y7=(dx3+ey3+f) / (gx3+hy3+1)

    x8=(ax4+by4+c) / (gx4+hy4+1)
    y8=(dx4+ey4+f) / (gx4+hy4+1)




    (gx1+hy1+1) * x5=(ax1+by1+c)
    (gx1+hy1+1) * y5=(dx1+ey1+f)

    (gx2+hy2+1) * x6=(ax2+by2+c)
    (gx2+hy2+1) * y6=(dx2+ey2+f)

    (gx3+hy3+1) * x7=(ax3+by3+c)
    (gx3+hy3+1) * y7=(dx3+ey3+f)

    (gx4+hy4+1) * x8=(ax4+by4+c)
    (gx4+hy4+1) * y8=(dx4+ey4+f)


    gx1 * x5 +hy1 * x5 + x5=ax1 + by1 + c
    gx1 * y5 +hy1 * y5 + y5=dx1 + ey1 + f
    gx2 * x6 +hy2 * x6 + x6=ax2 + by2 + c
    gx2 * y6 +hy2 * y6 + y6=dx2 + ey2 + f
    gx3 * x7 +hy3 * x7 + x7=ax3 + by3 + c
    gx3 * y7 +hy3 * y7 + y7=dx3 + ey3 + f
    gx4 * x8 +hy4 * x8 + x8=ax4 + by4 + c
    gx4 * y8 +hy4 * y8 + y8=dx4 + ey4 + f


    ax1 + by1 + c-g(x1 * x5) -h(y1 * x5) = x5
    dx1 + ey1 + f-g(x1 * y5) -h(y1 * y5) = y5
    ax2 + by2 + c-g(x2 * x6) -h(y2 * x6) = x6
    dx2 + ey2 + f-g(x2 * y6) -h(y2 * y6) = y6
    ax3 + by3 + c-g(x3 * x7) -h(y3 * x7) = x7
    dx3 + ey3 + f-g(x3 * y7) -h(y3 * y7) = y7
    ax4 + by4 + c-g(x4 * x8) -h(y4 * x8) = x8
    dx4 + ey4 + f-g(x4 * y8) -h(y4 * y8) = y8

    [a    b   c   d   e   f       g          h        ]

    [x1   y1  1   0   0   0  (x1 * x5)   (y1 * x5)  x5]
    [0    0   0   x1  y1  1  (x1 * x5)   (y1 * y5)  y5]
    [x2   y2  1   0   0   0  (x2 * x6)   (y2 * x6)  x6]
    [0    0   0   x2  y2  1  (x2 * x6)   (y2 * y6)  y6]
    [x3   y3  1   0   0   0  (x3 * x7)   (y3 * x7)  x7]
    [0    0   0   x3  y3  1  (x3 * x7)   (y3 * y7)  y7]
    [x4   y4  1   0   0   0  (x4 * x8)   (y4 * x8)  x8]
    [0    0   0   x4  y4  1  (x4 * x8)   (y4 * y8)  y8]
   */
  NaClO_MatrixResult MR = NaClO_NewMatrix(9, 9);
  if (MR.Error != NACLO_OK) {
    __NaClO__makeError(T, MR.Error);
    return T;
  }
  /*
  [x1   y1  1   0   0   0  (x1 * x5)   (y1 * x5)  x5]
  [0    0   0   x1  y1  1  (x1 * x5)   (y1 * y5)  y5]
  */
  *NaClO_MatrixElement1(&MR.result, 1, 1) = x1;
  *NaClO_MatrixElement1(&MR.result, 1, 2) = y1;
  *NaClO_MatrixElement1(&MR.result, 1, 3) = 1;
  *NaClO_MatrixElement1(&MR.result, 1, 4) = 0;
  *NaClO_MatrixElement1(&MR.result, 1, 5) = 0;
  *NaClO_MatrixElement1(&MR.result, 1, 6) = 0;
  *NaClO_MatrixElement1(&MR.result, 1, 7) = x1 * x5;
  *NaClO_MatrixElement1(&MR.result, 1, 8) = y1 * x5;
  *NaClO_MatrixElement1(&MR.result, 1, 9) = x5;

  *NaClO_MatrixElement1(&MR.result, 2, 1) = 0;
  *NaClO_MatrixElement1(&MR.result, 2, 2) = 0;
  *NaClO_MatrixElement1(&MR.result, 2, 3) = 0;
  *NaClO_MatrixElement1(&MR.result, 2, 4) = x1;
  *NaClO_MatrixElement1(&MR.result, 2, 5) = y1;
  *NaClO_MatrixElement1(&MR.result, 2, 6) = 1;
  *NaClO_MatrixElement1(&MR.result, 2, 7) = x1 * x5;
  *NaClO_MatrixElement1(&MR.result, 2, 8) = y1 * y5;
  *NaClO_MatrixElement1(&MR.result, 2, 9) = y5;
  /*
[x2   y2  1   0   0   0  (x2 * x6)   (y2 * x6)  x6]
[0    0   0   x2  y2  1  (x2 * x6)   (y2 * y6)  y6]
  */
  *NaClO_MatrixElement1(&MR.result, 3, 1) = x2;
  *NaClO_MatrixElement1(&MR.result, 3, 2) = y2;
  *NaClO_MatrixElement1(&MR.result, 3, 3) = 1;
  *NaClO_MatrixElement1(&MR.result, 3, 4) = 0;
  *NaClO_MatrixElement1(&MR.result, 3, 5) = 0;
  *NaClO_MatrixElement1(&MR.result, 3, 6) = 0;
  *NaClO_MatrixElement1(&MR.result, 3, 7) = x2 * x6;
  *NaClO_MatrixElement1(&MR.result, 3, 8) = y2 * x6;
  *NaClO_MatrixElement1(&MR.result, 3, 9) = x6;

  *NaClO_MatrixElement1(&MR.result, 4, 1) = 0;
  *NaClO_MatrixElement1(&MR.result, 4, 2) = 0;
  *NaClO_MatrixElement1(&MR.result, 4, 3) = 0;
  *NaClO_MatrixElement1(&MR.result, 4, 4) = x2;
  *NaClO_MatrixElement1(&MR.result, 4, 5) = y2;
  *NaClO_MatrixElement1(&MR.result, 4, 6) = 1;
  *NaClO_MatrixElement1(&MR.result, 4, 7) = x2 * x6;
  *NaClO_MatrixElement1(&MR.result, 4, 8) = y2 * y6;
  *NaClO_MatrixElement1(&MR.result, 4, 9) = y6;

  /*
  [x3   y3  1   0   0   0  (x3 * x7)   (y3 * x7)  x7]
  [0    0   0   x3  y3  1  (x3 * x7)   (y3 * y7)  y7]
  */
  *NaClO_MatrixElement1(&MR.result, 5, 1) = x3;
  *NaClO_MatrixElement1(&MR.result, 5, 2) = y3;
  *NaClO_MatrixElement1(&MR.result, 5, 3) = 1;
  *NaClO_MatrixElement1(&MR.result, 5, 4) = 0;
  *NaClO_MatrixElement1(&MR.result, 5, 5) = 0;
  *NaClO_MatrixElement1(&MR.result, 5, 6) = 0;
  *NaClO_MatrixElement1(&MR.result, 5, 7) = x3 * x7;
  *NaClO_MatrixElement1(&MR.result, 5, 8) = y3 * x7;
  *NaClO_MatrixElement1(&MR.result, 5, 9) = x7;

  *NaClO_MatrixElement1(&MR.result, 6, 1) = 0;
  *NaClO_MatrixElement1(&MR.result, 6, 2) = 0;
  *NaClO_MatrixElement1(&MR.result, 6, 3) = 0;
  *NaClO_MatrixElement1(&MR.result, 6, 4) = x3;
  *NaClO_MatrixElement1(&MR.result, 6, 5) = y3;
  *NaClO_MatrixElement1(&MR.result, 6, 6) = 1;
  *NaClO_MatrixElement1(&MR.result, 6, 7) = x3 * x7;
  *NaClO_MatrixElement1(&MR.result, 6, 8) = y3 * y7;
  *NaClO_MatrixElement1(&MR.result, 6, 9) = y7;

  /*
  [x4   y4  1   0   0   0  (x4 * x8)   (y4 * x8)  x8]
  [0    0   0   x4  y4  1  (x4 * x8)   (y4 * y8)  y8]
  */
  *NaClO_MatrixElement1(&MR.result, 7, 1) = x4;
  *NaClO_MatrixElement1(&MR.result, 7, 2) = y4;
  *NaClO_MatrixElement1(&MR.result, 7, 3) = 1;
  *NaClO_MatrixElement1(&MR.result, 7, 4) = 0;
  *NaClO_MatrixElement1(&MR.result, 7, 5) = 0;
  *NaClO_MatrixElement1(&MR.result, 7, 6) = 0;
  *NaClO_MatrixElement1(&MR.result, 7, 7) = x4 * x8;
  *NaClO_MatrixElement1(&MR.result, 7, 8) = y4 * x8;
  *NaClO_MatrixElement1(&MR.result, 7, 9) = x8;

  *NaClO_MatrixElement1(&MR.result, 8, 1) = 0;
  *NaClO_MatrixElement1(&MR.result, 8, 2) = 0;
  *NaClO_MatrixElement1(&MR.result, 8, 3) = 0;
  *NaClO_MatrixElement1(&MR.result, 8, 4) = x4;
  *NaClO_MatrixElement1(&MR.result, 8, 5) = y4;
  *NaClO_MatrixElement1(&MR.result, 8, 6) = 1;
  *NaClO_MatrixElement1(&MR.result, 8, 7) = x4 * x8;
  *NaClO_MatrixElement1(&MR.result, 8, 8) = y4 * y8;
  *NaClO_MatrixElement1(&MR.result, 8, 9) = y8;

  NaClO_ErrorType Err = NaClO_RowReducedEchelonMatrix(&MR.result);
  if (Err != NACLO_OK) {
    NaClO_FreeMatrix(&MR.result);
    __NaClO__makeError(T, Err);
    return T;
  }
  NaClO_float a, b, c, d, e, f, g, h;

  //
  h = (*NaClO_MatrixElement1(&MR.result, 8, 9)) /
      *NaClO_MatrixElement1(&MR.result, 8, 8);
  g = ((*NaClO_MatrixElement1(&MR.result, 7, 9)) -
       h * (*NaClO_MatrixElement1(&MR.result, 7, 8))) /
      (*NaClO_MatrixElement1(&MR.result, 7, 7));
  f = (*NaClO_MatrixElement1(&MR.result, 6, 9)) -
      h * (*NaClO_MatrixElement1(&MR.result, 6, 8)) -
      g * (*NaClO_MatrixElement1(&MR.result, 6, 7));
  f /= (*NaClO_MatrixElement1(&MR.result, 6, 6));

  e = (*NaClO_MatrixElement1(&MR.result, 5, 9));
  e -= h * (*NaClO_MatrixElement1(&MR.result, 5, 8));
  e -= g * (*NaClO_MatrixElement1(&MR.result, 5, 7));
  e -= f * (*NaClO_MatrixElement1(&MR.result, 5, 6));
  e /= (*NaClO_MatrixElement1(&MR.result, 5, 5));

  d = (*NaClO_MatrixElement1(&MR.result, 4, 9));
  d -= h * (*NaClO_MatrixElement1(&MR.result, 4, 8));
  d -= g * (*NaClO_MatrixElement1(&MR.result, 4, 7));
  d -= f * (*NaClO_MatrixElement1(&MR.result, 4, 6));
  d -= e * (*NaClO_MatrixElement1(&MR.result, 4, 5));
  d /= (*NaClO_MatrixElement1(&MR.result, 4, 4));

  c = (*NaClO_MatrixElement1(&MR.result, 3, 9));
  c -= h * (*NaClO_MatrixElement1(&MR.result, 3, 8));
  c -= g * (*NaClO_MatrixElement1(&MR.result, 3, 7));
  c -= f * (*NaClO_MatrixElement1(&MR.result, 3, 6));
  c -= e * (*NaClO_MatrixElement1(&MR.result, 3, 5));
  c -= d * (*NaClO_MatrixElement1(&MR.result, 3, 4));
  c /= (*NaClO_MatrixElement1(&MR.result, 3, 3));

  b = (*NaClO_MatrixElement1(&MR.result, 2, 9));
  b -= h * (*NaClO_MatrixElement1(&MR.result, 2, 8));
  b -= g * (*NaClO_MatrixElement1(&MR.result, 2, 7));
  b -= f * (*NaClO_MatrixElement1(&MR.result, 2, 6));
  b -= e * (*NaClO_MatrixElement1(&MR.result, 2, 5));
  b -= d * (*NaClO_MatrixElement1(&MR.result, 2, 4));
  b -= c * (*NaClO_MatrixElement1(&MR.result, 2, 3));
  b /= (*NaClO_MatrixElement1(&MR.result, 2, 2));

  a = (*NaClO_MatrixElement1(&MR.result, 1, 9));
  a -= h * (*NaClO_MatrixElement1(&MR.result, 1, 8));
  a -= g * (*NaClO_MatrixElement1(&MR.result, 1, 7));
  a -= f * (*NaClO_MatrixElement1(&MR.result, 1, 6));
  a -= e * (*NaClO_MatrixElement1(&MR.result, 1, 5));
  a -= d * (*NaClO_MatrixElement1(&MR.result, 1, 4));
  a -= c * (*NaClO_MatrixElement1(&MR.result, 1, 3));
  a -= b * (*NaClO_MatrixElement1(&MR.result, 1, 2));
  a /= (*NaClO_MatrixElement1(&MR.result, 1, 1));

  NaClO_FreeMatrix(&MR.result);

  NaClO_PixelType pt;
  memset(&pt, 1, sizeof(pt));
  T = NaClO_NewImage(data->width, data->height, data->mode, pt);

  for (NaClO_uint dstX = 0; dstX < data->width; ++dstX) {
    for (NaClO_uint dstY = 0; dstY < data->height; ++dstY) {
      NaClO_float denom = g * dstX + h * dstY + 1;
      if (denom == 0) {
        continue;
      }
      NaClO_float srcXF = (a * dstX + b * dstY + c) / denom;
      NaClO_float srcYF = (d * dstX + e * dstY + f) / denom;
      if (srcXF < 0 || srcXF >= data->width || srcYF < 0 ||
          srcYF >= data->height) {
        continue;
      }
      NaClO_uint srcX = (NaClO_uint)(srcXF + 0.5f);
      NaClO_uint srcY = (NaClO_uint)(srcYF + 0.5f);
      if (srcX >= data->width || srcY >= data->height) {
        continue;
      }
      NaClO_PixelResult pixel = NaClO_GetPixel(data, srcX, srcY);
      if (pixel.Error != NACLO_OK) {
        continue;
      }
      NaClO_PutPixel(&T.result, dstX, dstY, pixel.result);
    }
  }
  return T;
}
NaClO_ErrorType NaClO_SetPerspective(
    NaClO_Image *data, NaClO_uint x1, NaClO_uint y1, NaClO_uint x2,
    NaClO_uint y2, NaClO_uint x3, NaClO_uint y3, NaClO_uint x4, NaClO_uint y4,

    NaClO_uint x5, NaClO_uint y5, NaClO_uint x6, NaClO_uint y6, NaClO_uint x7,
    NaClO_uint y7, NaClO_uint x8, NaClO_uint y8) {
  if (data == NULL) {
    return NACLO_NULL_POINTER;
  }
  __NaClO__makeResult(T);
  T = NaClO_Perspective(data, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6,
                        x7, y7, x8, y8);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}

NaClO_ImageResult NaClO_Convolute(NaClO_Image *data, NaClO_Matrix kernel,
                                  uint8_t stride, uint8_t padding) {

  __NaClO__makeResult(T);
  if (kernel.width != kernel.height) {
    __NaClO__makeError(T, NACLO_NOT_A_SQUARE_MATRIX);
  }
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }

  // 计算 kernel 的中心点偏移
  int8_t k_center = (int8_t)(kernel.width / 2);

  // 计算输出图像的尺寸
  NaClO_uint outputWidth =
      (data->width + 2 * padding - kernel.width) / stride + 1;
  NaClO_uint outputHeight =
      (data->height + 2 * padding - kernel.height) / stride + 1;

  // 创建输出图像
  NaClO_PixelType pt;
  memset(&pt, 0, sizeof(pt));
  T = NaClO_NewImage(outputWidth, outputHeight, data->mode, pt);
  if (T.Error != NACLO_OK) {
    NaClO_FreeImage(&T.result);
    __NaClO__makeError(T, T.Error);
  }

  // 计算 kernel 的总和（后续可以用于归一化，但现在先不做）
  NaClO_float kernelSum = 0.0f;
  for (uint8_t mx = 0; mx < kernel.width; mx++) {
    for (uint8_t my = 0; my < kernel.height; my++) {
      kernelSum += *NaClO_MatrixElement(&kernel, mx, my);
    }
  }

  // 避免除以 0
  if (kernelSum == 0.0f) {
    kernelSum = 1.0f;
  }

  // 遍历输出图像的每个像素
  for (NaClO_uint oy = 0; oy < outputHeight; oy++) {
    for (NaClO_uint ox = 0; ox < outputWidth; ox++) {
      // 计算输入图像中的对应位置（考虑 padding 和 stride）
      int64_t in_x = (int64_t)ox * stride - padding + k_center;
      int64_t in_y = (int64_t)oy * stride - padding + k_center;

      // 用 float 类型初始化累加器，避免溢出
      NaClO_float r_sum = 0.0f, g_sum = 0.0f, b_sum = 0.0f, a_sum = 0.0f,
                  l_sum = 0.0f;

      // 对 kernel 进行遍历
      for (int8_t mx = -k_center; mx <= k_center; mx++) {
        for (int8_t my = -k_center; my <= k_center; my++) {
          // 计算当前 kernel 元素在输入图像中的位置
          int64_t cur_x = in_x + mx;
          int64_t cur_y = in_y + my;

          // 边界检查，如果超出范围则跳过（等价于 padding 为 0）
          if (cur_x >= 0 && cur_x < (int64_t)data->width && cur_y >= 0 &&
              cur_y < (int64_t)data->height) {
            // 获取 kernel 中的权重
            NaClO_float weight =
                *NaClO_MatrixElement(&kernel, mx + k_center, my + k_center);

            switch (data->mode) {
            case NaClO_RGB: {
              NaClO_PixelType *src_pixel =
                  NaClO_Pixel(data, (NaClO_uint)cur_x, (NaClO_uint)cur_y);
              r_sum += (NaClO_float)src_pixel->RGB.r * weight;
              g_sum += (NaClO_float)src_pixel->RGB.g * weight;
              b_sum += (NaClO_float)src_pixel->RGB.b * weight;
              break;
            }
            case NaClO_RGBA: {
              NaClO_PixelType *src_pixel =
                  NaClO_Pixel(data, (NaClO_uint)cur_x, (NaClO_uint)cur_y);
              r_sum += (NaClO_float)src_pixel->RGBA.r * weight;
              g_sum += (NaClO_float)src_pixel->RGBA.g * weight;
              b_sum += (NaClO_float)src_pixel->RGBA.b * weight;
              a_sum += (NaClO_float)src_pixel->RGBA.a * weight;
              break;
            }
            case NaClO_L: {
              NaClO_PixelType *src_pixel =
                  NaClO_Pixel(data, (NaClO_uint)cur_x, (NaClO_uint)cur_y);
              l_sum += src_pixel->L * weight;
              break;
            }
            case NaClO_1: {
              // 二值图像不处理
              break;
            }
            }
          }
        }
      }

      // 将累加结果转换为输出像素
      NaClO_PixelType pixel;
      memset(&pixel, 0, sizeof(pixel));

      // 对结果进行饱和处理（clamp 到有效范围）
      switch (data->mode) {
      case NaClO_RGB: {
        // 饱和到 [0, 255]
        r_sum = r_sum < 0 ? 0 : (r_sum > 255 ? 255 : r_sum);
        g_sum = g_sum < 0 ? 0 : (g_sum > 255 ? 255 : g_sum);
        b_sum = b_sum < 0 ? 0 : (b_sum > 255 ? 255 : b_sum);

        pixel.RGB.r = (uint8_t)(r_sum + 0.5f);
        pixel.RGB.g = (uint8_t)(g_sum + 0.5f);
        pixel.RGB.b = (uint8_t)(b_sum + 0.5f);
        break;
      }
      case NaClO_RGBA: {
        r_sum = r_sum < 0 ? 0 : (r_sum > 255 ? 255 : r_sum);
        g_sum = g_sum < 0 ? 0 : (g_sum > 255 ? 255 : g_sum);
        b_sum = b_sum < 0 ? 0 : (b_sum > 255 ? 255 : b_sum);
        a_sum = a_sum < 0 ? 0 : (a_sum > 255 ? 255 : a_sum);

        pixel.RGBA.r = (uint8_t)(r_sum + 0.5f);
        pixel.RGBA.g = (uint8_t)(g_sum + 0.5f);
        pixel.RGBA.b = (uint8_t)(b_sum + 0.5f);
        pixel.RGBA.a = (uint8_t)(a_sum + 0.5f);
        break;
      }
      case NaClO_L: {
        l_sum = l_sum < 0.0f ? 0.0f : (l_sum > 1.0f ? 1.0f : l_sum);
        pixel.L = l_sum;
        break;
      }
      default:
        break;
      }

      // 将计算结果写入输出图像
      *NaClO_Pixel(&T.result, ox, oy) = pixel;
    }
  }

  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_Convoluted(NaClO_Image *data, NaClO_Matrix kernel,
                                 uint8_t stride, uint8_t padding) {
  NaClO_ImageResult T = NaClO_Convolute(data, kernel, stride, padding);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Laplace(NaClO_Image *data) {

  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  NaClO_MatrixResult MR = NaClO_NewMatrix(3, 3);
  if (MR.Error != NACLO_OK) {
    __NaClO__makeError(T, MR.Error);
  }
  // T=NaClO_Convert(data, "L");
  // if(T.Error!=NACLO_OK)
  // {
  //   return T;
  // }
  *NaClO_MatrixElement1(&MR.result, 1, 1) = 0;
  *NaClO_MatrixElement1(&MR.result, 2, 1) = 1;
  *NaClO_MatrixElement1(&MR.result, 3, 1) = 0;
  *NaClO_MatrixElement1(&MR.result, 1, 2) = 1;
  *NaClO_MatrixElement1(&MR.result, 2, 2) = -4;
  *NaClO_MatrixElement1(&MR.result, 3, 2) = 1;
  *NaClO_MatrixElement1(&MR.result, 1, 3) = 0;
  *NaClO_MatrixElement1(&MR.result, 2, 3) = 1;
  *NaClO_MatrixElement1(&MR.result, 3, 3) = 0;

  T = NaClO_Convolute(data, MR.result, 1, 1);
  NaClO_FreeMatrix(&MR.result);
  return T;
}
NaClO_ErrorType NaClO_SetLaplace(NaClO_Image *data) {
  NaClO_ImageResult T = NaClO_Laplace(data);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Sobel(NaClO_Image *data) {
  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  NaClO_MatrixResult Sobel1 = NaClO_NewMatrix(3, 3);
  NaClO_MatrixResult Sobel2 = NaClO_NewMatrix(3, 3);
  *NaClO_MatrixElement1(&Sobel1.result, 1, 1) = -1;
  *NaClO_MatrixElement1(&Sobel1.result, 2, 1) = -2;
  *NaClO_MatrixElement1(&Sobel1.result, 3, 1) = -1;
  *NaClO_MatrixElement1(&Sobel1.result, 1, 2) = 0;
  *NaClO_MatrixElement1(&Sobel1.result, 2, 2) = 0;
  *NaClO_MatrixElement1(&Sobel1.result, 3, 2) = 0;
  *NaClO_MatrixElement1(&Sobel1.result, 1, 3) = 1;
  *NaClO_MatrixElement1(&Sobel1.result, 2, 3) = 2;
  *NaClO_MatrixElement1(&Sobel1.result, 3, 3) = 1;

  *NaClO_MatrixElement1(&Sobel2.result, 1, 1) = -1;
  *NaClO_MatrixElement1(&Sobel2.result, 2, 1) = 0;
  *NaClO_MatrixElement1(&Sobel2.result, 3, 1) = 1;
  *NaClO_MatrixElement1(&Sobel2.result, 1, 2) = -2;
  *NaClO_MatrixElement1(&Sobel2.result, 2, 2) = 0;
  *NaClO_MatrixElement1(&Sobel2.result, 3, 2) = 2;
  *NaClO_MatrixElement1(&Sobel2.result, 1, 3) = -1;
  *NaClO_MatrixElement1(&Sobel2.result, 2, 3) = 0;
  *NaClO_MatrixElement1(&Sobel2.result, 3, 3) = 1;

  NaClO_ImageResult S1 = NaClO_Convolute(data, Sobel1.result, 1, 1);
  if (S1.Error != NACLO_OK) {
    __NaClO__makeError(T, S1.Error);
  }
  NaClO_ImageResult S2 = NaClO_Convolute(data, Sobel2.result, 1, 1);
  if (S2.Error != NACLO_OK) {
    NaClO_FreeImage(&S1.result);
    __NaClO__makeError(T, S2.Error);
  }
  assert(S1.result.width = S2.result.width);
  assert(S1.result.height = S2.result.height);
  assert(S1.result.width = data->width);
  assert(S1.result.height = data->height);

  T = NaClO_NewImage(data->width, data->height, data->mode, data->data[0][0]);
  if (T.Error != NACLO_OK) {
    return T;
  }
  for (NaClO_uint x = 0; x < data->width; ++x) {
    for (NaClO_uint y = 0; y < data->height; ++y) {
      NaClO_PixelType s1Pixel = *NaClO_Pixel(&S1.result, x, y);
      NaClO_PixelType s2Pixel = *NaClO_Pixel(&S2.result, x, y);
      NaClO_PixelType p;
      switch (data->mode) {

      case NaClO_RGB:

        p.RGB.r = s1Pixel.RGB.r > s2Pixel.RGB.r ? s1Pixel.RGB.r : s2Pixel.RGB.r;
        p.RGB.g = s1Pixel.RGB.g > s2Pixel.RGB.g ? s1Pixel.RGB.g : s2Pixel.RGB.g;
        p.RGB.b = s1Pixel.RGB.b > s2Pixel.RGB.b ? s1Pixel.RGB.b : s2Pixel.RGB.b;
        break;
      case NaClO_RGBA:
        p.RGBA.r =
            s1Pixel.RGBA.r > s2Pixel.RGBA.r ? s1Pixel.RGBA.r : s2Pixel.RGBA.r;
        p.RGBA.g =
            s1Pixel.RGBA.g > s2Pixel.RGBA.g ? s1Pixel.RGBA.g : s2Pixel.RGBA.g;
        p.RGBA.b =
            s1Pixel.RGBA.b > s2Pixel.RGBA.b ? s1Pixel.RGBA.b : s2Pixel.RGBA.b;
        p.RGBA.a = NaClO_Pixel(data, x, y)->RGBA.a;
        break;
      case NaClO_L:
        p.L = s1Pixel.L > s2Pixel.L ? s1Pixel.L : s2Pixel.L;
        break;
      case NaClO_1:
        p.value = s1Pixel.value > s2Pixel.value ? s1Pixel.value : s2Pixel.value;
        break;
      }
      *NaClO_Pixel(&T.result, x, y) = p;
    }
  }
  NaClO_FreeImage(&S1.result);
  NaClO_FreeImage(&S2.result);
  NaClO_FreeMatrix(&Sobel1.result);
  NaClO_FreeMatrix(&Sobel2.result);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_SetSobel(NaClO_Image *data) {
  NaClO_ImageResult T = NaClO_Sobel(data);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Roberts(NaClO_Image *data) {
  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  NaClO_MatrixResult Roberts1 = NaClO_NewMatrix(3, 3);
  NaClO_MatrixResult Roberts2 = NaClO_NewMatrix(3, 3);
  *NaClO_MatrixElement1(&Roberts1.result, 1, 1) = -1;
  *NaClO_MatrixElement1(&Roberts1.result, 2, 1) = 0;
  *NaClO_MatrixElement1(&Roberts1.result, 1, 2) = 0;
  *NaClO_MatrixElement1(&Roberts1.result, 2, 2) = 1;

  *NaClO_MatrixElement1(&Roberts2.result, 1, 1) = 0;
  *NaClO_MatrixElement1(&Roberts2.result, 2, 1) = -1;
  *NaClO_MatrixElement1(&Roberts2.result, 1, 2) = 1;
  *NaClO_MatrixElement1(&Roberts2.result, 2, 2) = 0;
  NaClO_ImageResult S1 = NaClO_Convolute(data, Roberts1.result, 1, 1);
  if (S1.Error != NACLO_OK) {
    __NaClO__makeError(T, S1.Error);
  }
  NaClO_ImageResult S2 = NaClO_Convolute(data, Roberts2.result, 1, 1);
  if (S2.Error != NACLO_OK) {
    NaClO_FreeImage(&S1.result);
    __NaClO__makeError(T, S2.Error);
  }
  assert(S1.result.width = S2.result.width);
  assert(S1.result.height = S2.result.height);
  assert(S1.result.width = data->width);
  assert(S1.result.height = data->height);

  T = NaClO_NewImage(data->width, data->height, data->mode, data->data[0][0]);
  if (T.Error != NACLO_OK) {
    return T;
  }
  for (NaClO_uint x = 0; x < data->width; ++x) {
    for (NaClO_uint y = 0; y < data->height; ++y) {
      NaClO_PixelType s1Pixel = *NaClO_Pixel(&S1.result, x, y);
      NaClO_PixelType s2Pixel = *NaClO_Pixel(&S2.result, x, y);
      NaClO_PixelType p;
      switch (data->mode) {

      case NaClO_RGB:

        p.RGB.r = s1Pixel.RGB.r > s2Pixel.RGB.r ? s1Pixel.RGB.r : s2Pixel.RGB.r;
        p.RGB.g = s1Pixel.RGB.g > s2Pixel.RGB.g ? s1Pixel.RGB.g : s2Pixel.RGB.g;
        p.RGB.b = s1Pixel.RGB.b > s2Pixel.RGB.b ? s1Pixel.RGB.b : s2Pixel.RGB.b;
        break;
      case NaClO_RGBA:
        p.RGBA.r =
            s1Pixel.RGBA.r > s2Pixel.RGBA.r ? s1Pixel.RGBA.r : s2Pixel.RGBA.r;
        p.RGBA.g =
            s1Pixel.RGBA.g > s2Pixel.RGBA.g ? s1Pixel.RGBA.g : s2Pixel.RGBA.g;
        p.RGBA.b =
            s1Pixel.RGBA.b > s2Pixel.RGBA.b ? s1Pixel.RGBA.b : s2Pixel.RGBA.b;
        p.RGBA.a = NaClO_Pixel(data, x, y)->RGBA.a;
        break;
      case NaClO_L:
        p.L = s1Pixel.L > s2Pixel.L ? s1Pixel.L : s2Pixel.L;
        break;
      case NaClO_1:
        p.value = s1Pixel.value > s2Pixel.value ? s1Pixel.value : s2Pixel.value;
        break;
      }
      *NaClO_Pixel(&T.result, x, y) = p;
    }
  }
  NaClO_FreeImage(&S1.result);
  NaClO_FreeImage(&S2.result);
  NaClO_FreeMatrix(&Roberts1.result);
  NaClO_FreeMatrix(&Roberts2.result);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_SetRoberts(NaClO_Image *data) {
  NaClO_ImageResult T = NaClO_Roberts(data);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}

NaClO_ImageResult NaClO_Prewitt(NaClO_Image *data) {
  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  NaClO_MatrixResult Prewitt1 = NaClO_NewMatrix(3, 3);
  NaClO_MatrixResult Prewitt2 = NaClO_NewMatrix(3, 3);
  *NaClO_MatrixElement1(&Prewitt1.result, 1, 1) = -1;
  *NaClO_MatrixElement1(&Prewitt1.result, 2, 1) = 0;
  *NaClO_MatrixElement1(&Prewitt1.result, 3, 1) = 1;
  *NaClO_MatrixElement1(&Prewitt1.result, 1, 2) = -1;
  *NaClO_MatrixElement1(&Prewitt1.result, 2, 2) = 0;
  *NaClO_MatrixElement1(&Prewitt1.result, 3, 2) = 1;
  *NaClO_MatrixElement1(&Prewitt1.result, 1, 3) = -1;
  *NaClO_MatrixElement1(&Prewitt1.result, 2, 3) = 0;
  *NaClO_MatrixElement1(&Prewitt1.result, 3, 3) = 1;

  *NaClO_MatrixElement1(&Prewitt2.result, 1, 1) = -1;
  *NaClO_MatrixElement1(&Prewitt2.result, 2, 1) = -1;
  *NaClO_MatrixElement1(&Prewitt2.result, 3, 1) = -1;
  *NaClO_MatrixElement1(&Prewitt2.result, 1, 2) = 0;
  *NaClO_MatrixElement1(&Prewitt2.result, 2, 2) = 0;
  *NaClO_MatrixElement1(&Prewitt2.result, 3, 2) = 0;
  *NaClO_MatrixElement1(&Prewitt2.result, 1, 3) = 1;
  *NaClO_MatrixElement1(&Prewitt2.result, 2, 3) = 1;
  *NaClO_MatrixElement1(&Prewitt2.result, 3, 3) = 1;

  NaClO_ImageResult S1 = NaClO_Convolute(data, Prewitt1.result, 1, 1);
  if (S1.Error != NACLO_OK) {
    __NaClO__makeError(T, S1.Error);
  }
  NaClO_ImageResult S2 = NaClO_Convolute(data, Prewitt2.result, 1, 1);
  if (S2.Error != NACLO_OK) {
    NaClO_FreeImage(&S1.result);
    __NaClO__makeError(T, S2.Error);
  }
  assert(S1.result.width = S2.result.width);
  assert(S1.result.height = S2.result.height);
  assert(S1.result.width = data->width);
  assert(S1.result.height = data->height);

  T = NaClO_NewImage(data->width, data->height, data->mode, data->data[0][0]);
  if (T.Error != NACLO_OK) {
    return T;
  }
  for (NaClO_uint x = 0; x < data->width; ++x) {
    for (NaClO_uint y = 0; y < data->height; ++y) {
      NaClO_PixelType s1Pixel = *NaClO_Pixel(&S1.result, x, y);
      NaClO_PixelType s2Pixel = *NaClO_Pixel(&S2.result, x, y);
      NaClO_PixelType p;
      switch (data->mode) {

      case NaClO_RGB:

        p.RGB.r = s1Pixel.RGB.r > s2Pixel.RGB.r ? s1Pixel.RGB.r : s2Pixel.RGB.r;
        p.RGB.g = s1Pixel.RGB.g > s2Pixel.RGB.g ? s1Pixel.RGB.g : s2Pixel.RGB.g;
        p.RGB.b = s1Pixel.RGB.b > s2Pixel.RGB.b ? s1Pixel.RGB.b : s2Pixel.RGB.b;
        break;
      case NaClO_RGBA:
        p.RGBA.r =
            s1Pixel.RGBA.r > s2Pixel.RGBA.r ? s1Pixel.RGBA.r : s2Pixel.RGBA.r;
        p.RGBA.g =
            s1Pixel.RGBA.g > s2Pixel.RGBA.g ? s1Pixel.RGBA.g : s2Pixel.RGBA.g;
        p.RGBA.b =
            s1Pixel.RGBA.b > s2Pixel.RGBA.b ? s1Pixel.RGBA.b : s2Pixel.RGBA.b;
        p.RGBA.a = NaClO_Pixel(data, x, y)->RGBA.a;
        break;
      case NaClO_L:
        p.L = s1Pixel.L > s2Pixel.L ? s1Pixel.L : s2Pixel.L;
        break;
      case NaClO_1:
        p.value = s1Pixel.value > s2Pixel.value ? s1Pixel.value : s2Pixel.value;
        break;
      }
      *NaClO_Pixel(&T.result, x, y) = p;
    }
  }
  NaClO_FreeImage(&S1.result);
  NaClO_FreeImage(&S2.result);
  NaClO_FreeMatrix(&Prewitt1.result);
  NaClO_FreeMatrix(&Prewitt2.result);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_SetPrewitt(NaClO_Image *data) {
  NaClO_ImageResult T = NaClO_Prewitt(data);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}

NaClO_ImageResult NaClO_AvgBlur(NaClO_Image *data, NaClO_uint strength) {
  NaClO_uint kernel = 2 * strength + 1;
  NaClO_ImageResult T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    return T;
  }
  for (NaClO_uint x = 0; x < data->width - kernel; ++x) {
    for (NaClO_uint y = 0; y < data->height - kernel; ++y) {
      NaClO_uint r = 0;
      NaClO_uint g = 0;
      NaClO_uint b = 0;
      NaClO_float L = 0;
      NaClO_uint value = 0;
      NaClO_PixelType pt;

      for (NaClO_uint kx = 0; kx < kernel; ++kx) {
        for (NaClO_uint ky = 0; ky < kernel; ++ky) {

          pt = *NaClO_Pixel(data, x + kx, y + ky);
          switch (data->mode) {

          case NaClO_RGB:
            r += pt.RGB.r;
            g += pt.RGB.g;
            b += pt.RGB.b;
            break;
          case NaClO_RGBA:
            r += pt.RGBA.r;
            g += pt.RGBA.g;
            b += pt.RGBA.b;
            break;
          case NaClO_L:
            L += pt.L;
            break;
          case NaClO_1:
            value += pt.value;
            break;
          }
        }
      }

      r /= (kernel * kernel);
      g /= (kernel * kernel);
      b /= (kernel * kernel);
      L /= (kernel * kernel);
      value /= (kernel * kernel);

      NaClO_PixelType pt2;
      switch (data->mode) {
      case NaClO_RGB:
        pt2.RGB.r = r;
        pt2.RGB.g = g;
        pt2.RGB.b = b;
        break;
      case NaClO_RGBA:
        pt2.RGBA.r = r;
        pt2.RGBA.g = g;
        pt2.RGBA.b = b;
        break;
      case NaClO_L:
        pt2.L = L;
        break;
      case NaClO_1:
        pt2.value = value > ((kernel * kernel) / 2);
        break;
      }
      for (NaClO_uint kx = 0; kx < kernel; ++kx) {
        for (NaClO_uint ky = 0; ky < kernel; ++ky) {
          *NaClO_Pixel(&T.result, x + kx, y + ky) = pt2;
        }
      }
    }
  }
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_AvgBlurred(NaClO_Image *data, NaClO_uint strength) {
  NaClO_ImageResult T = NaClO_AvgBlur(data, strength);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}
static int larger(const void *a, const void *b) {
  return (*(NaClO_uint *)a - *(NaClO_uint *)b);
}
NaClO_ImageResult NaClO_MedianBlur(NaClO_Image *data, NaClO_uint strength) {
  NaClO_uint kernel = 2 * strength + 1;
  NaClO_ImageResult T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    return T;
  }
  NaClO_uint *r = NULL;
  NaClO_uint *g = NULL;
  NaClO_uint *b = NULL;
  NaClO_float *L = NULL;
  bool *value = NULL;
  // (NaClO_uint*)  NaClO_CALLOC(sizeof(NaClO_uint),kernel);
  // (NaClO_uint*)  NaClO_CALLOC(sizeof(NaClO_uint),kernel);
  // (NaClO_uint*)  NaClO_CALLOC(sizeof(NaClO_uint),kernel);
  // (NaClO_float*) NaClO_CALLOC(sizeof(NaClO_float),kernel);
  // (NaClO_uint*)  NaClO_CALLOC(sizeof(NaClO_uint),kernel);
  switch (data->mode) {

  case NaClO_RGB:
  case NaClO_RGBA:
    r = (NaClO_uint *)NaClO_CALLOC(sizeof(NaClO_uint), kernel * kernel);
    g = (NaClO_uint *)NaClO_CALLOC(sizeof(NaClO_uint), kernel * kernel);
    b = (NaClO_uint *)NaClO_CALLOC(sizeof(NaClO_uint), kernel * kernel);
    break;
  case NaClO_L:
    L = (NaClO_float *)NaClO_CALLOC(sizeof(NaClO_float), kernel * kernel);
    break;
  case NaClO_1:
    value = (bool *)NaClO_CALLOC(sizeof(bool), kernel * kernel);
    break;
  }
  NaClO_uint ArraySubscript = 0;

  for (NaClO_uint x = 0; x < data->width - kernel; ++x) {
    for (NaClO_uint y = 0; y < data->height - kernel; ++y) {
      NaClO_PixelType pt;

      for (NaClO_uint kx = 0; kx < kernel; ++kx) {
        for (NaClO_uint ky = 0; ky < kernel; ++ky) {

          pt = *NaClO_Pixel(data, x + kx, y + ky);
          switch (data->mode) {

          case NaClO_RGB:
            r[ArraySubscript] = pt.RGB.r;
            g[ArraySubscript] = pt.RGB.g;
            b[ArraySubscript] = pt.RGB.b;
            break;
          case NaClO_RGBA:
            r[ArraySubscript] = pt.RGBA.r;
            g[ArraySubscript] = pt.RGBA.g;
            b[ArraySubscript] = pt.RGBA.b;
            break;
          case NaClO_L:
            L[ArraySubscript] = pt.L;
            break;
          case NaClO_1:
            value[ArraySubscript] = pt.value;
            break;
          }
        }
      }
      ArraySubscript++;
      qsort(r, sizeof(NaClO_uint), kernel * kernel, larger);
      qsort(g, sizeof(NaClO_uint), kernel * kernel, larger);
      qsort(b, sizeof(NaClO_uint), kernel * kernel, larger);
      qsort(L, sizeof(NaClO_float), kernel * kernel, larger);
      qsort(value, sizeof(bool), kernel * kernel, larger);

      NaClO_PixelType pt2;
      NaClO_uint sz = floorf((NaClO_float)(kernel * kernel) / 2.0f);
      switch (data->mode) {
      case NaClO_RGB:
        pt2.RGB.r = r[sz];
        pt2.RGB.g = g[sz];
        pt2.RGB.b = b[sz];
        break;
      case NaClO_RGBA:
        pt2.RGBA.r = r[sz];
        pt2.RGBA.g = g[sz];
        pt2.RGBA.b = b[sz];
        break;
      case NaClO_L:
        pt2.L = L[sz];
        break;
      case NaClO_1:
        pt2.value = value[sz];
        break;
      }
      for (NaClO_uint kx = 0; kx < kernel; ++kx) {
        for (NaClO_uint ky = 0; ky < kernel; ++ky) {
          *NaClO_Pixel(&T.result, x + kx, y + ky) = pt2;
        }
      }
    }
  }

  NaClO_FREE(r);
  NaClO_FREE(g);
  NaClO_FREE(b);
  NaClO_FREE(L);
  NaClO_FREE(value);

  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_MedianBlurred(NaClO_Image *data, NaClO_uint strength) {
  NaClO_ImageResult T = NaClO_MedianBlur(data, strength);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}

static NaClO_float NaClO_StandardNormalDistribution2D(NaClO_float x,
                                                      NaClO_float y) {
  return (1.0f / sqrtf(2 * NaClO_PI)) * powf(NaClO_e, (x * x + y * y) / 2);
}
NaClO_ImageResult NaClO_GaussianBlur(NaClO_Image *data, NaClO_uint strength) {
  NaClO_uint kernel = 2 * strength + 1;
  NaClO_uint kernelCenter = floorf((NaClO_float)kernel / 2);
  NaClO_ImageResult T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    return T;
  }
  for (NaClO_uint x = 0; x < data->width - kernel; ++x) {
    for (NaClO_uint y = 0; y < data->height - kernel; ++y) {
      NaClO_float r = 0;
      NaClO_float g = 0;
      NaClO_float b = 0;
      NaClO_float L = 0;
      NaClO_float value = 0;
      NaClO_PixelType pt;

      for (NaClO_uint kx = 0; kx < kernel; ++kx) {
        for (NaClO_uint ky = 0; ky < kernel; ++ky) {

          pt = *NaClO_Pixel(data, x + kx, y + ky);
          NaClO_float factor = NaClO_StandardNormalDistribution2D(
              kx - kernelCenter, ky - kernelCenter);
          switch (data->mode) {

          case NaClO_RGB:
            r += (NaClO_float)(pt.RGB.r) * factor;
            g += (NaClO_float)(pt.RGB.g) * factor;
            b += (NaClO_float)(pt.RGB.b) * factor;
            break;
          case NaClO_RGBA:
            r += (NaClO_float)(pt.RGBA.r) * factor;
            g += (NaClO_float)(pt.RGBA.g) * factor;
            b += (NaClO_float)(pt.RGBA.b) * factor;
            break;
          case NaClO_L:
            L += floorf(pt.L * factor);
            break;
          case NaClO_1:
            value += pt.value * factor;
            break;
          }
        }
      }

      NaClO_PixelType pt2;
      switch (data->mode) {
      case NaClO_RGB:
        pt2.RGB.r = r;
        pt2.RGB.g = g;
        pt2.RGB.b = b;
        break;
      case NaClO_RGBA:
        pt2.RGBA.r = r;
        pt2.RGBA.g = g;
        pt2.RGBA.b = b;
        break;
      case NaClO_L:
        pt2.L = L;
        break;
      case NaClO_1:
        pt2.value = value > ((NaClO_float)(kernel * kernel) / 2);
        break;
      }
      for (NaClO_uint kx = 0; kx < kernel; ++kx) {
        for (NaClO_uint ky = 0; ky < kernel; ++ky) {
          *NaClO_Pixel(&T.result, x + kx, y + ky) = pt2;
        }
      }
    }
  }
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_GaussianBlurred(NaClO_Image *data, NaClO_uint strength) {
  NaClO_ImageResult T = NaClO_GaussianBlur(data, strength);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Blur(NaClO_Image *data, NaClO_uint strength) {
  NaClO_float factor = 1.0f / (strength + 1);
  NaClO_ImageResult T = NaClO_Scale(data, factor);
  if (T.Error != NACLO_OK) {
    return T;
  }

  NaClO_ErrorType Err = NaClO_Resized(&T.result, data->width, data->height);
  T.Error = Err;
  return T;
}
NaClO_ErrorType NaClO_Blurred(NaClO_Image *data, NaClO_uint strength) {
  NaClO_ImageResult T = NaClO_Blur(data, strength);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}

NaClO_ImageResult NaClO_HueSaturationValue(NaClO_Image *data, NaClO_float hue,
                                           NaClO_float saturation,
                                           NaClO_float value) {

  NaClO_ImageResult T = NaClO_Convert(data, "rgba");
  if (T.Error != NACLO_OK) {
    return T;
  }
  for (int x = 0; x < T.result.width; ++x) {
    for (int y = 0; y < T.result.height; ++y) {
      NaClO_PixelType pt = *NaClO_Pixel(&T.result, x, y);
      NaClO_HSV hsv = NaClO_GetHSVRGBA(pt.RGBA);
      hsv.hue = (uint16_t)(hsv.hue * hue);
      hsv.saturation = fminf(hsv.saturation * saturation, 1.0f);
      hsv.value = fminf(hsv.value * value, 1.0f);
      NaClO_ColorModeRGB rgb = NaClO_GetRGB(hsv);
      NaClO_ColorModeRGBA rgba;
      rgba.r = rgb.r;
      rgba.g = rgb.g;
      rgba.b = rgb.b;
      rgba.a = pt.RGBA.a;
      NaClO_Pixel(&T.result, x, y)->RGBA = rgba;
    }
  }
  T.Error = NaClO_ConvertedE(&T.result, data->mode);

  return T;
}
NaClO_ErrorType NaClO_SetHueSaturationValue(NaClO_Image *data, NaClO_float hue,
                                            NaClO_float saturation,
                                            NaClO_float value) {
  NaClO_ColorMode mode = data->mode;
  NaClO_ErrorType Err = NaClO_Converted(data, "rgba");
  if (Err != NACLO_OK) {
    return Err;
  }
  for (int x = 0; x < data->width; ++x) {
    for (int y = 0; y < data->height; ++y) {
      NaClO_PixelType pt = *NaClO_Pixel(data, x, y);
      NaClO_HSV hsv = NaClO_GetHSVRGBA(pt.RGBA);
      hsv.hue = (uint16_t)(hsv.hue * hue);
      hsv.saturation = fminf(hsv.saturation * saturation, 1.0f);
      hsv.value = fminf(hsv.value * value, 1.0f);
      NaClO_ColorModeRGB rgb = NaClO_GetRGB(hsv);
      NaClO_ColorModeRGBA rgba;
      rgba.r = rgb.r;
      rgba.g = rgb.g;
      rgba.b = rgb.b;
      rgba.a = pt.RGBA.a;
      NaClO_Pixel(data, x, y)->RGBA = rgba;
    }
  }

  return NACLO_OK;
}
NaClO_ErrorType NaClO_GradientHistogram(NaClO_Image *data,
                                        NaClO_uint *outHistogram) {

  if (data == NULL or outHistogram == NULL) {
    return NACLO_NULL_POINTER;
  }
  memset(outHistogram, 256, sizeof(NaClO_uint));
  NaClO_ImageResult T = NaClO_Convert(data, "L");
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  for (NaClO_uint x = 0; x < T.result.width; ++x) {
    for (NaClO_uint y = 0; y < T.result.height; ++y) {
      NaClO_PixelType pt = *NaClO_Pixel(&T.result, x, y);
      uint8_t L = floorf(pt.L * 255);
      outHistogram[L]++;
    }
  }
  NaClO_FreeImage(&T.result);
  return NACLO_OK;
}
NaClO_ErrorType NaClO_RGBHistogram(NaClO_Image *data, NaClO_uint *outRHistogram,
                                   NaClO_uint *outGHistogram,
                                   NaClO_uint *outBHistogram

) {

  if (data == NULL or outRHistogram == NULL or outGHistogram == NULL or
      outBHistogram == NULL) {
    return NACLO_NULL_POINTER;
  }
  memset(outRHistogram, 256, sizeof(NaClO_uint));
  memset(outGHistogram, 256, sizeof(NaClO_uint));
  memset(outBHistogram, 256, sizeof(NaClO_uint));
  NaClO_ImageResult T = NaClO_Convert(data, "rgb");
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  for (NaClO_uint x = 0; x < T.result.width; ++x) {
    for (NaClO_uint y = 0; y < T.result.height; ++y) {
      NaClO_PixelType pt = *NaClO_Pixel(&T.result, x, y);
      outRHistogram[pt.RGB.r]++;
      outGHistogram[pt.RGB.g]++;
      outBHistogram[pt.RGB.b]++;
    }
  }
  NaClO_FreeImage(&T.result);
  return NACLO_OK;
}
static void __naclo__histogram_equalization(NaClO_uint hist[256],
                                            NaClO_uint lut[256]) {
  // 1. 计算总像素数
  uint64_t total_pixels = 0;
  for (int i = 0; i < 256; i++) {
    total_pixels += hist[i];
  }

  // 如果图片为空，直接返回
  if (total_pixels == 0)
    return;

  // 2. 计算累积分布函数 (CDF)，并找出 CDF 的最小值（非零的最小值）
  uint64_t cdf[256];
  cdf[0] = hist[0];
  uint64_t cdf_min = 0;

  for (int i = 1; i < 256; i++) {
    cdf[i] = cdf[i - 1] + hist[i];
  }

  // 寻找第一个非零的 CDF 值作为 cdf_min
  for (int i = 0; i < 256; i++) {
    if (cdf[i] > 0) {
      cdf_min = cdf[i];
      break;
    }
  }

  // 3. 生成映射表 (LUT)
  // 均衡化公式：lut[i] = (cdf[i] - cdf_min) * 255 / (total_pixels - cdf_min)
  uint64_t denominator = total_pixels - cdf_min;

  for (int i = 0; i < 256; i++) {
    if (cdf[i] == 0) {
      lut[i] = 0;
    } else {
      // 使用 64 位整数进行乘法防止溢出，最后映射回 0-255
      lut[i] = (uint8_t)(((cdf[i] - cdf_min) * 255) / denominator);
    }
  }
}
NaClO_ImageResult NaClO_HistogramEqualization(NaClO_Image *data) {

  __NaClO__makeResult(T);

  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  if (data->mode == NaClO_1) {
    __NaClO__makeError(T, NACLO_DISMATCHED_CHANNEL);
  }
  NaClO_MakeHistogram(oldHist);
  NaClO_MakeHistogram(newHist);
  T.Error = NaClO_GradientHistogram(data, oldHist);
  if (T.Error != NACLO_OK) {
    return T;
  }
  __naclo__histogram_equalization(oldHist, newHist);

  T = NaClO_ConvertE(data, NaClO_L);
  if (T.Error != NACLO_OK) {
    return T;
  }
  NaClO_Image gray = T.result;

  T = NaClO_NewImage(data->width, data->height, data->mode,
                     *NaClO_Pixel(data, 0, 0));
  if (T.Error != NACLO_OK) {
    NaClO_FreeImage(&gray);
    return T;
  }
  for (int x = 0; x < data->width; ++x) {
    for (int y = 0; y < data->height; ++y) {
      NaClO_float L = gray.data[x][y].L;
      NaClO_float newL = newHist[(uint8_t)(L * 255.0f + 0.5f)] / 255.0f;
      switch (data->mode) {
      case NaClO_RGB: {
        NaClO_PixelType pt = *NaClO_Pixel(data, x, y);
        NaClO_float oldLuminance =
            (0.299f * pt.RGB.r + 0.587f * pt.RGB.g + 0.114f * pt.RGB.b) /
            255.0f;
        if (oldLuminance > 0.0f) {
          NaClO_float scale = newL / oldLuminance;
          pt.RGB.r = (uint8_t)(fminf(pt.RGB.r * scale, 255.0f));
          pt.RGB.g = (uint8_t)(fminf(pt.RGB.g * scale, 255.0f));
          pt.RGB.b = (uint8_t)(fminf(pt.RGB.b * scale, 255.0f));
        }
        *NaClO_Pixel(&T.result, x, y) = pt;
      } break;
      case NaClO_RGBA: {
        NaClO_PixelType pt = *NaClO_Pixel(data, x, y);
        NaClO_float oldLuminance =
            (0.299f * pt.RGBA.r + 0.587f * pt.RGBA.g + 0.114f * pt.RGBA.b) /
            255.0f;
        if (oldLuminance > 0.0f) {
          NaClO_float scale = newL / oldLuminance;
          pt.RGBA.r = (uint8_t)(fminf(pt.RGBA.r * scale, 255.0f));
          pt.RGBA.g = (uint8_t)(fminf(pt.RGBA.g * scale, 255.0f));
          pt.RGBA.b = (uint8_t)(fminf(pt.RGBA.b * scale, 255.0f));
        }
        *NaClO_Pixel(&T.result, x, y) = pt;
      } break;
      case NaClO_L: {
        NaClO_Pixel(&T.result, x, y)->L = newL;
      } break;
      case NaClO_1:
        break;
      }
    }
  }
  NaClO_FreeImage(&gray);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_HistogramEqualized(NaClO_Image *data) {
  NaClO_ImageResult T = NaClO_HistogramEqualization(data);
  if (T.Error != NACLO_OK) {
    return T.Error;
  }
  NaClO_FreeImage(data);
  *data = T.result;
  return NACLO_OK;
}
NaClO_PixelType __naclo__premu(NaClO_PixelType p, NaClO_ColorMode mode) {
  NaClO_PixelType pt = p;
  pt.RGBA.r *= pt.RGBA.a;
  pt.RGBA.g *= pt.RGBA.a;
  pt.RGBA.b *= pt.RGBA.a;
  pt.RGBA.a = 0;
  return pt;
}
NaClO_ErrorType NaClO_PremultipliedAlpha(NaClO_Image *data) {
  if (data == NULL) {
    return NACLO_NULL_POINTER;
    // return T;
  }
  if (data->mode != NaClO_RGBA) {
    return NACLO_DISMATCHED_CHANNEL;
  }
  NaClO_EnumeratePixel(data, __naclo__premu);
  NaClO_ErrorType err = NaClO_Converted(data, "rgb");
  return err;
}
NaClO_ImageResult NaClO_AlphaPremultiplication(NaClO_Image *data) {
  __NaClO__makeResult(T);
  if (data == NULL) {
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }
  T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_PremultipliedAlpha(&T.result);
  return T;
}

NaClO_ErrorType NaClO_Sharpened(NaClO_Image *data) {
  if (data == NULL) {
    return NACLO_NULL_POINTER;
  }
  NaClO_MatrixResult M = NaClO_NewMatrix(3, 3);

  if (M.Error != NACLO_OK) {
    return M.Error;
  }
  *NaClO_MatrixElement1(&M.result, 1, 1) = -1;
  *NaClO_MatrixElement1(&M.result, 1, 2) = -1;
  *NaClO_MatrixElement1(&M.result, 1, 3) = -1;
  *NaClO_MatrixElement1(&M.result, 2, 1) = -1;
  *NaClO_MatrixElement1(&M.result, 2, 2) = 9;
  *NaClO_MatrixElement1(&M.result, 2, 3) = -1;
  *NaClO_MatrixElement1(&M.result, 3, 1) = -1;
  *NaClO_MatrixElement1(&M.result, 3, 2) = -1;
  *NaClO_MatrixElement1(&M.result, 3, 3) = -1;

  NaClO_ErrorType Err = NaClO_Convoluted(data, M.result, 1, 1);
  NaClO_FreeMatrix(&M.result);
  return Err;
}
NaClO_ImageResult NaClO_Sharpen(NaClO_Image *data) {
  NaClO_ImageResult T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_Sharpened(data);
  return T;
}
NaClO_PixelType NaClO_ReversePixel(NaClO_PixelType p, NaClO_ColorMode mode) {
  NaClO_PixelType result;
  switch (mode) {

  case NaClO_RGB:
    result.RGB.r = 255 - result.RGB.r;
    result.RGB.g = 255 - result.RGB.g;
    result.RGB.b = 255 - result.RGB.b;
    break;
  case NaClO_RGBA:
    result.RGBA.r = 255 - result.RGBA.r;
    result.RGBA.g = 255 - result.RGBA.g;
    result.RGBA.b = 255 - result.RGBA.b;
    break;
  case NaClO_L:
    result.L = 1 - result.L;
    break;
  case NaClO_1:
    result.value = !result.value;
    break;
  }
  return result;
}
NaClO_ErrorType NaClO_Reversed(NaClO_Image *data) {
  if (data == NULL) {
    return NACLO_NULL_POINTER;
  }
  for (NaClO_uint x = 0; x < data->width; ++x) {
    for (NaClO_uint y = 0; y < data->height; ++y) {
      NaClO_PixelType p = *NaClO_Pixel(data, x, y);
      switch (data->mode) {

      case NaClO_RGB:
        p.RGB.r = 255 - p.RGB.r;
        p.RGB.g = 255 - p.RGB.g;
        p.RGB.b = 255 - p.RGB.b;
        break;
      case NaClO_RGBA:
        p.RGBA.r = 255 - p.RGBA.r;
        p.RGBA.g = 255 - p.RGBA.g;
        p.RGBA.b = 255 - p.RGBA.b;
        // p.RGBA.a = 255 - p.RGBA.a;
        break;
      case NaClO_L:
        p.L = 1 - p.L;
      case NaClO_1:
        p.value = not p.value;
      }
    }
  }
  return NACLO_OK;
}
NaClO_ImageResult NaClO_ReverseImage(NaClO_Image *data) {

  NaClO_ImageResult T = NaClO_CopyImage(data);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_Reversed(&T.result);
  return T;
}

static NaClO_float __naclo__min(NaClO_float i1, NaClO_float i2) {
  return i1 > i2 ? i2 : i1;
}
static NaClO_float __naclo__max(NaClO_float i1, NaClO_float i2) {
  return i1 > i2 ? i1 : i2;
}
// NaClO_ErrorType

NaClO_ErrorType NaClO_Blended(NaClO_Image *I1, NaClO_Image *I2,
                              NaClO_float ratio) {
  if (I1 == NULL or I2 == NULL) {
    return NACLO_NULL_POINTER;
  }
  if (ratio > 1 or ratio < 0) {
    return NACLO_OUT_OF_BOUNDS;
  }
  NaClO_ImageResult I22 = NaClO_ConvertE(I2, I1->mode);
  if (I22.Error != NACLO_OK) {
    return I22.Error;
  }
  NaClO_ErrorType t = NaClO_Resized(&I22.result, I1->width, I1->height);
  if (t != NACLO_OK) {
    return t;
  }
  for (int x = 0; x < I1->width; ++x) {
    for (int y = 0; y < I1->height; ++y) {
      NaClO_PixelType p1 = *NaClO_Pixel(I1, x, y);
      NaClO_PixelType p2 = *NaClO_Pixel(&I22.result, x, y);
      switch (I1->mode) {
      case NaClO_RGB:
        p1.RGB.r = (NaClO_float)p1.RGB.r * (1 - ratio) +
                   (NaClO_float)p2.RGB.r * (ratio);
        p1.RGB.g = (NaClO_float)p1.RGB.g * (1 - ratio) +
                   (NaClO_float)p2.RGB.g * (ratio);
        p1.RGB.b = (NaClO_float)p1.RGB.b * (1 - ratio) +
                   (NaClO_float)p2.RGB.b * (ratio);
        break;
      case NaClO_RGBA:
        p1.RGBA.r = (NaClO_float)p1.RGBA.r * (1 - ratio) +
                    (NaClO_float)p2.RGBA.r * (ratio);
        p1.RGBA.g = (NaClO_float)p1.RGBA.g * (1 - ratio) +
                    (NaClO_float)p2.RGBA.g * (ratio);
        p1.RGBA.b = (NaClO_float)p1.RGBA.b * (1 - ratio) +
                    (NaClO_float)p2.RGBA.b * (ratio);
        p1.RGBA.a = (NaClO_float)p1.RGBA.a * (1 - ratio) +
                    (NaClO_float)p2.RGBA.a * (ratio);
        break;
      case NaClO_L:
        p1.L = (NaClO_float)p1.L * (1 - ratio) + (NaClO_float)p2.L * (ratio);
        break;
      case NaClO_1:
        p1.value = (NaClO_float)p1.value * (1 - ratio) +
                               (NaClO_float)p2.value * (ratio) >
                           0.5
                       ? true
                       : false;
        break;
      }
      *NaClO_Pixel(I1, x, y) = p1;
    }
  }
  NaClO_FreeImage(&I22.result);
  return NACLO_OK;
}

NaClO_ImageResult NaClO_Blend(NaClO_Image *I1, NaClO_Image *I2,
                              NaClO_float ratio) {
  NaClO_ImageResult T = NaClO_CopyImage(I1);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_Blended(&T.result, I2, ratio);
  return T;
}
NaClO_ErrorType NaClO_Dissolved(NaClO_Image *I1, NaClO_Image *I2,
                                NaClO_float ratio) {
  if (I1 == NULL or I2 == NULL) {
    return NACLO_NULL_POINTER;
  }
  if (ratio > 1 or ratio < 0) {
    return NACLO_OUT_OF_BOUNDS;
  }
  NaClO_ImageResult I22 = NaClO_ConvertE(I2, I1->mode);
  if (I22.Error != NACLO_OK) {
    return I22.Error;
  }
  NaClO_ErrorType t = NaClO_Resized(&I22.result, I1->width, I1->height);
  if (t != NACLO_OK) {
    return t;
  }
  for (int x = 0; x < I1->width; ++x) {
    for (int y = 0; y < I1->height; ++y) {
      NaClO_float rand01 = (NaClO_float)rand() / RAND_MAX;
      if (rand01 >= ratio) {
        *NaClO_Pixel(I1, x, y) = *NaClO_Pixel(&I22.result, x, y);
      }
    }
  }
  NaClO_FreeImage(&I22.result);
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Dissolve(NaClO_Image *I1, NaClO_Image *I2,
                                 NaClO_float ratio) {
  NaClO_ImageResult T = NaClO_CopyImage(I1);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_Dissolved(&T.result, I2, ratio);
  return T;
}

#define __naclo__max(a, b) (a) > (b) ? a : b
#define __naclo__min(a, b) (a) > (b) ? b : a
NaClO_ErrorType NaClO_Lightened(NaClO_Image *I1, NaClO_Image *I2) {
  if (I1 == NULL or I2 == NULL) {
    return NACLO_NULL_POINTER;
  }
  NaClO_ImageResult I22 = NaClO_ConvertE(I2, I1->mode);
  if (I22.Error != NACLO_OK) {
    return I22.Error;
  }
  NaClO_ErrorType t = NaClO_Resized(&I22.result, I1->width, I1->height);
  if (t != NACLO_OK) {
    return t;
  }
  for (int x = 0; x < I1->width; ++x) {
    for (int y = 0; y < I1->height; ++y) {
      NaClO_PixelType p1 = *NaClO_Pixel(I1, x, y);
      NaClO_PixelType p2 = *NaClO_Pixel(&I22.result, x, y);
      switch (I1->mode) {
      case NaClO_RGB:
        p1.RGB.r = __naclo__max(p1.RGB.r, p2.RGB.r);
        p1.RGB.g = __naclo__max(p1.RGB.g, p2.RGB.g);
        p1.RGB.b = __naclo__max(p1.RGB.b, p2.RGB.b);

        break;
      case NaClO_RGBA:
        p1.RGBA.r = __naclo__max(p1.RGBA.r, p2.RGBA.r);
        p1.RGBA.g = __naclo__max(p1.RGBA.g, p2.RGBA.g);
        p1.RGBA.b = __naclo__max(p1.RGBA.b, p2.RGBA.b);
        p1.RGBA.a = __naclo__max(p1.RGBA.a, p2.RGBA.a);
        break;
      case NaClO_L:
        p1.L = __naclo__max(p1.L, p2.L);
        break;
      case NaClO_1:
        p1.value = __naclo__max(p1.value, p2.value);
        break;
      }
      *NaClO_Pixel(I1, x, y) = p1;
    }
  }
  NaClO_FreeImage(&I22.result);
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Lighten(NaClO_Image *I1, NaClO_Image *I2) {
  NaClO_ImageResult T = NaClO_CopyImage(I1);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_Lightened(&T.result, I2);
  return T;
}
NaClO_ErrorType NaClO_Darkened(NaClO_Image *I1, NaClO_Image *I2) {
  if (I1 == NULL or I2 == NULL) {
    return NACLO_NULL_POINTER;
  }
  NaClO_ImageResult I22 = NaClO_ConvertE(I2, I1->mode);
  if (I22.Error != NACLO_OK) {
    return I22.Error;
  }
  NaClO_ErrorType t = NaClO_Resized(&I22.result, I1->width, I1->height);
  if (t != NACLO_OK) {
    return t;
  }
  for (int x = 0; x < I1->width; ++x) {
    for (int y = 0; y < I1->height; ++y) {
      NaClO_PixelType p1 = *NaClO_Pixel(I1, x, y);
      NaClO_PixelType p2 = *NaClO_Pixel(&I22.result, x, y);
      switch (I1->mode) {
      case NaClO_RGB:
        p1.RGB.r = __naclo__min(p1.RGB.r, p2.RGB.r);
        p1.RGB.g = __naclo__min(p1.RGB.g, p2.RGB.g);
        p1.RGB.b = __naclo__min(p1.RGB.b, p2.RGB.b);

        break;
      case NaClO_RGBA:
        p1.RGBA.r = __naclo__min(p1.RGBA.r, p2.RGBA.r);
        p1.RGBA.g = __naclo__min(p1.RGBA.g, p2.RGBA.g);
        p1.RGBA.b = __naclo__min(p1.RGBA.b, p2.RGBA.b);
        p1.RGBA.a = __naclo__min(p1.RGBA.a, p2.RGBA.a);
        break;
      case NaClO_L:
        p1.L = __naclo__min(p1.L, p2.L);
        break;
      case NaClO_1:
        p1.value = __naclo__min(p1.value, p2.value);
        break;
      }
      *NaClO_Pixel(I1, x, y) = p1;
    }
  }
  NaClO_FreeImage(&I22.result);
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Darken(NaClO_Image *I1, NaClO_Image *I2) {
  NaClO_ImageResult T = NaClO_CopyImage(I1);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_Darkened(&T.result, I2);
  return T;
}
NaClO_ErrorType NaClO_Multiplied(NaClO_Image *I1, NaClO_Image *I2) {
  if (I1 == NULL or I2 == NULL) {
    return NACLO_NULL_POINTER;
  }
  NaClO_ImageResult I22 = NaClO_ConvertE(I2, I1->mode);
  if (I22.Error != NACLO_OK) {
    return I22.Error;
  }
  NaClO_ErrorType t = NaClO_Resized(&I22.result, I1->width, I1->height);
  if (t != NACLO_OK) {
    return t;
  }
  for (int x = 0; x < I1->width; ++x) {
    for (int y = 0; y < I1->height; ++y) {
      NaClO_PixelType p1 = *NaClO_Pixel(I1, x, y);

      NaClO_PixelType p2 = *NaClO_Pixel(&I22.result, x, y);

      switch (I1->mode) {
      case NaClO_RGB:
        p1.RGB.r = ((NaClO_float)p1.RGB.r * (NaClO_float)p2.RGB.r) / (255.0f);
        p1.RGB.g = ((NaClO_float)p1.RGB.g * (NaClO_float)p2.RGB.g) / (255.0f);
        p1.RGB.b = ((NaClO_float)p1.RGB.b * (NaClO_float)p2.RGB.b) / (255.0f);
        break;
      case NaClO_RGBA:
        p1.RGBA.r =
            ((NaClO_float)p1.RGBA.r * (NaClO_float)p2.RGBA.r) / (255.0f);
        p1.RGBA.g =
            ((NaClO_float)p1.RGBA.g * (NaClO_float)p2.RGBA.g) / (255.0f);
        p1.RGBA.b =
            ((NaClO_float)p1.RGBA.b * (NaClO_float)p2.RGBA.b) / (255.0f);
        p1.RGBA.a =
            ((NaClO_float)p1.RGBA.a * (NaClO_float)p2.RGBA.a) / (255.0f);
        break;
      case NaClO_L:
        p1.L = p1.L * p2.L;
        break;
      case NaClO_1:
        p1.value = p1.value and p2.value;
        break;
      }
      *NaClO_Pixel(I1, x, y) = p1;
    }
  }
  NaClO_FreeImage(&I22.result);
  return NACLO_OK;
}
NaClO_ImageResult NaClO_Multiply(NaClO_Image *I1, NaClO_Image *I2) {
  NaClO_ImageResult T = NaClO_CopyImage(I1);
  if (T.Error != NACLO_OK) {
    return T;
  }
  T.Error = NaClO_Multiplied(&T.result, I2);
  return T;
}
NaClO_ErrorType NaClO_Burned(NaClO_Image *I1, NaClO_Image *I2) {
  if (I1 == NULL or I2 == NULL) {
    return NACLO_NULL_POINTER;
  }
  NaClO_ImageResult I22 = NaClO_ConvertE(I2, I1->mode);
  if (I22.Error != NACLO_OK) {
    return I22.Error;
  }
  NaClO_ErrorType t = NaClO_Resized(&I22.result, I1->width, I1->height);
  if (t != NACLO_OK) {
    return t;
  }
  for (int x = 0; x < I1->width; ++x) {
    for (int y = 0; y < I1->height; ++y) {
    }
  }
}

#ifdef __cplusplus
}
#endif
#endif

#endif
