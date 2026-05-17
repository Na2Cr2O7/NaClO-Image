#pragma once

// #include <cstddef>
// #include <cstddef>
// #include <cstddef>
// #include <cmath>
#include <ctype.h>
#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef enum {
  NACLO_OK,
  NACLO_MEMORY_ALLOCATION_FAILED,
  NACLO_UNKNOWN_CHANNEL,
  NACLO_NULL_POINTER,
  NACLO_UNKNOWN_OR_UNSUPPORTED_EXTENSION,
  NACLO_UNKNOWN_ERROR,
  NACLO_OUT_OF_BOUNDS,
  NACLO_DISMATCHED_CHANNEL,

} NaClO_ErrorType;
typedef enum { NaClO_RGB, NaClO_RGBA, NaClO_L, NaClO_1 } NaClO_ColorMode;
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} NaClO_ColorModeRGB;
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} NaClO_ColorModeRGBA;

typedef union {
  NaClO_ColorModeRGB RGB;
  NaClO_ColorModeRGBA RGBA;
  NaClO_float L;
  bool value;

} NaClO_PixelType;

typedef struct {
  NaClO_uint width;
  NaClO_uint height;
  NaClO_ColorMode mode;
  NaClO_PixelType **data;
} NaClO_Image;

typedef struct {
  NaClO_Image result;
  NaClO_ErrorType Error;
} NaClO_ImageResult;
typedef struct {
  NaClO_PixelType result;
  NaClO_ErrorType Error;
} NaClO_PixelResult;

typedef enum {
  NACLO_FLIP_X,
  NACLO_FLIP_Y,
  NACLO_FLIP_NEGATIVE_X,
  NACLO_FLIP_NEGATIVE_Y,
} NaClO_FlipDirection;
typedef struct {
  uint16_t hue;
  NaClO_float saturation;
  NaClO_float value;
} NaClO_HSV;

// #define __NaClO_CreateMatrix(name,w,h) NaClO_float name[w][h]
typedef struct
{
    NaClO_float** data;
    uint8_t width;
    uint8_t height;
} NaClO_Matrix;
typedef struct
{
    NaClO_Matrix result;
    NaClO_ErrorType Error;
} NaClO_MatrixResult;

typedef NaClO_PixelType (*NaClO_SetPixelFunctionPointer)(NaClO_PixelType p);

NaClO_ImageResult NaClO_Load(const char *filename);
NaClO_ErrorType NaClO_FreeImage(NaClO_Image *src);
NaClO_ErrorType NaClO_FreeImage(NaClO_Image *src);
NaClO_Image NaClO_Unwrap(NaClO_ImageResult result);
NaClO_ErrorType NaClO_Save(NaClO_Image *data, const char *fileName);

#define __NaClO__makeResult(T)                                                 \
  NaClO_ImageResult(T);                                                        \
  memset(&T, 0, sizeof(T));
#define __NaClO__makeError(T, E)                                               \
  memset(&T, 0, sizeof(T));                                                    \
  T.Error = E;                                                                 \
  return T;
#define NACLO_IMAGE_IMPL
#ifdef NACLO_IMAGE_IMPL

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
NaClO_Image NaClO_Unwrap(NaClO_ImageResult result) {
  assert(result.Error == NACLO_OK);
  return result.result;
}

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

NaClO_ErrorType NaClO_Resized(NaClO_Image *data, NaClO_uint w, NaClO_uint h) 
{
    NaClO_ImageResult r=NaClO_Resize(data, w, h);
    if(r.Error!=NACLO_OK)
    {
      // NaClO_FreeImage(&r.result);
      return r.Error;    
    }
    NaClO_ErrorType err=NaClO_FreeImage(data);
    if(err!=NACLO_OK)
    {
      return err;
    }
    *data=r.result;
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
                             NaClO_uint w, NaClO_uint h)
{
   NaClO_ImageResult r=NaClO_Crop(data,x,y,w,h);
    if(r.Error!=NACLO_OK)
    {
      return r.Error;    
    }
    NaClO_ErrorType err=NaClO_FreeImage(data);
    if(err!=NACLO_OK)
    {
      return err;
    }
    *data=r.result;
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
NaClO_ImageResult NaClO_ConvertE(NaClO_Image *data, NaClO_ColorMode mode);
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
      NaClO_float r = has_alpha ? src->data[x][y].RGBA.r : src->data[x][y].RGB.r;
      NaClO_float g = has_alpha ? src->data[x][y].RGBA.g : src->data[x][y].RGB.g;
      NaClO_float b = has_alpha ? src->data[x][y].RGBA.b : src->data[x][y].RGB.b;
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

NaClO_ImageResult NaClO_Convert(NaClO_Image *data, const char *mode_str) {
  if (!data || !mode_str) {
    __NaClO__makeResult(T);
    __NaClO__makeError(T, NACLO_NULL_POINTER);
  }

  NaClO_ColorMode mode;
  size_t len = strlen(mode_str);
  if (len == 0)
    goto unknown;

  // Case-insensitive compare
  if ((len == 3 && strcasecmp(mode_str, "rgb") == 0)) {
    mode = NaClO_RGB;
  } else if ((len == 4 && strcasecmp(mode_str, "rgba") == 0)) {
    mode = NaClO_RGBA;
  } else if ((len == 1 && (mode_str[0] == 'l' || mode_str[0] == 'L'))) {
    mode = NaClO_L;
  } else if ((len == 1 && (mode_str[0] == '1'))) {
    mode = NaClO_1;
  } else {
  unknown:
    __NaClO__makeResult(T);
    __NaClO__makeError(T, NACLO_UNKNOWN_OR_UNSUPPORTED_EXTENSION);
  }

  return NaClO_ConvertE(data, mode);
}
NaClO_ErrorType NaClO_Converted(NaClO_Image *data, const char *mode_str) 
{
    NaClO_ImageResult r=NaClO_Convert(data,mode_str);
    if(r.Error!=NACLO_OK)
    {
      // NaClO_FreeImage(&r.result);
      return r.Error;    
    }
    NaClO_ErrorType err=NaClO_FreeImage(data);
    if(err!=NACLO_OK)
    {
      return err;
    }
    *data=r.result;
    return NACLO_OK;
}
NaClO_ErrorType NaClO_ConvertedE(NaClO_Image *data, NaClO_ColorMode mode)
{
      NaClO_ImageResult r=NaClO_ConvertE(data,mode);
    if(r.Error!=NACLO_OK)
    {
      // NaClO_FreeImage(&r.result);
      return r.Error;    
    }
    NaClO_ErrorType err=NaClO_FreeImage(data);
    if(err!=NACLO_OK)
    {
      return err;
    }
    *data=r.result;
    return NACLO_OK;
} 

NaClO_ImageResult NaClO_NewImage(NaClO_uint w, NaClO_uint h,
                                 NaClO_ColorMode mode,
                                 NaClO_PixelType fillColor) {
  __NaClO__makeResult(T);
  T.result.width = w;
  T.result.height = h;
  T.result.mode = mode;
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

NaClO_ImageResult NaClO_Flip(NaClO_Image *data, NaClO_FlipDirection direction) 
{
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
NaClO_ErrorType NaClO_Flipped(NaClO_Image *data, NaClO_FlipDirection direction) 
{
    NaClO_ImageResult r=NaClO_Flip(data,direction);
    if(r.Error!=NACLO_OK)
    {
      // NaClO_FreeImage(&r.result);
      return r.Error;    
    }
    NaClO_ErrorType err=NaClO_FreeImage(data);
    if(err!=NACLO_OK)
    {
      return err;
    }
    *data=r.result;
    return NACLO_OK;
}

NaClO_ImageResult NaClO_Paste(NaClO_Image *canva, NaClO_Image *object,
                              NaClO_uint w, NaClO_uint h)
                               {
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

  for (int x = 0; x < convertedImage.result.width; ++x) {
    for (int y = 0; y < convertedImage.result.height; ++y) {
      NaClO_PixelResult P = NaClO_GetPixel(&convertedImage.result, x, y);
      if (P.Error != NACLO_OK) {
        NaClO_FreeImage(&T.result);
        NaClO_FreeImage(&convertedImage.result);

        __NaClO__makeError(T, P.Error);
      }
      NaClO_PutPixel(&T.result, x + w, y + h, P.result);
    }
  }

  NaClO_FreeImage(&convertedImage.result);
  T.Error = NACLO_OK;
  return T;
}
NaClO_ErrorType NaClO_Pasted(NaClO_Image *canva, NaClO_Image *object,
                              NaClO_uint w, NaClO_uint h)
{
      NaClO_ImageResult r=NaClO_Paste(canva,object,w,h);
    if(r.Error!=NACLO_OK)
    {
      // NaClO_FreeImage(&r.result);
      return r.Error;    
    }
    NaClO_ErrorType err=NaClO_FreeImage(canva);
    if(err!=NACLO_OK)
    {
      return err;
    }
    *canva=r.result;
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

void NaClO_EnumeratePixel(NaClO_Image* src,NaClO_SetPixelFunctionPointer f)
{
    for (int x=0; x<src->width; ++x) {
      for (int y=0; y<src->height; ++y) {
          NaClO_PixelType pixel=*NaClO_Pixel(src,x,y);
          *NaClO_Pixel(src,x,y)=f(pixel);

      }
    }
}

NaClO_MatrixResult NaClO_DotProduct(NaClO_Matrix matrix1,NaClO_Matrix matrix2)
{

}
#ifdef __cplusplus
}
#endif 
#endif

#endif
