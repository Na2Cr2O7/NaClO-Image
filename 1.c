#include "naclo_image1.h"
#include "stdio.h"
#include <stdbool.h>
#include <string.h>
void PrintMatrix(NaClO_Matrix M) {
  for (uint8_t y = 0; y < M.width; ++y) {
    for (uint8_t x = 0; x < M.height; ++x) {
      printf("%f ", *NaClO_MatrixElement(&M, x, y));
    }
    printf("\n");
  }
}
void printError(NaClO_ErrorType z) {
  switch (z) {
  case NACLO_OK:
    printf("NaClO_OK");
    break;
  case NACLO_MEMORY_ALLOCATION_FAILED:
    printf("NACLO_MEMORY_ALLOCATION_FAILED");

    break;
  case NACLO_UNKNOWN_CHANNEL:
    printf("NACLO_UNKNOWN_CHANNEL");

    break;
  case NACLO_NULL_POINTER:
    printf("NACLO_NULL_POINTER");
    break;
  case NACLO_UNKNOWN_OR_UNSUPPORTED_EXTENSION:
    printf("NACLO_UNKNOWN_OR_UNSUPPORTED_EXTENSION");
    break;
  case NACLO_UNKNOWN_ERROR:
    printf("NACLO_UNKNOWN_ERROR");
    break;
  case NACLO_OUT_OF_BOUNDS:
    printf("NACLO_OUT_OF_BOUNDS");
    break;
  case NACLO_DISMATCHED_CHANNEL:
    printf("NACLO_DISMATCHED_CHANNEL");

    break;
  case NACLO_ZERO_WIDTH_OR_HEIGHT:
    printf("NACLO_ZERO_WIDTH_OR_HEIGHT");

    break;
  case NACLO_NOT_A_SQUARE_MATRIX:
    printf("NACLO_NOT_A_SQUARE_MATRIX");

    break;
  case NACLO_SINGULAR_MATRIX:
    printf(" NACLO_SINGULAR_MATRIX");

    break;
  }
  printf("\n");
}
void PrintChannel(NaClO_Image c) {
  switch (c.mode) {

  case NaClO_RGB:
    printf("RGB\n");
    break;
  case NaClO_RGBA:
    printf("RGBA\n");

    break;
  case NaClO_L:
    printf("L\n");

    break;
  case NaClO_1:
    printf("1\n");

    break;
  }
}
int main() {

  NaClO_ColorModeRGB rgb;
  rgb.r = 0;
  rgb.g = 114;
  rgb.b = 224;
  NaClO_HSV hsv = NaClO_GetHSV(rgb);
  printf("%u,%f,%f\n", hsv.hue, hsv.saturation * 100, hsv.value * 100);
  rgb = NaClO_GetRGB(hsv);
  printf("%d,%d,%d\n", (int)rgb.r, (int)rgb.g, (int)rgb.b);

  // printf("11\n");

  // printf("Hello World\n");
  // NaClO_ImageResult z = NaClO_Load("color.png");
  // printf("NaClO_Load\n");
  // printError(z.Error);
  // // NaClO_Image z2=z.result;
  // //
  // printf("Channel:%d\n\nwidth:%ld\nheight:%ld\n\n",(int)z2.mode,z2.width,z2.height);
  // NaClO_Image unwrapped = NaClO_Unwrap(z);

  // printf("Copy\n");
  // NaClO_ImageResult copied = NaClO_CopyImage(&z.result);
  // printError(copied.Error);
  // NaClO_FreeImage(&copied.result);

  // PrintChannel(copied.result);
  // printf("convert1\n");
  // NaClO_ImageResult converted1 = NaClO_Convert(&z.result, "1");
  // PrintChannel(converted1.result);
  // printError(converted1.Error);
  // NaClO_Save(&converted1.result, "converted1.png");
  // NaClO_FreeImage(&converted1.result);

  // printf("convertL\n");
  // NaClO_ImageResult convertedL = NaClO_Convert(&z.result, "L");
  // PrintChannel(convertedL.result);
  // printError(convertedL.Error);
  // NaClO_Save(&convertedL.result, "convertedL.png");
  // NaClO_FreeImage(&convertedL.result);

  // printf("Resize\n");
  // NaClO_ImageResult resized =
  //     NaClO_Resize(&z.result, z.result.width * 2, z.result.height * 2);
  // NaClO_ErrorType saveResizedResult =
  //     NaClO_Save(&resized.result, "resized.png");
  // printError(saveResizedResult);
  // NaClO_FreeImage(&z.result);
  // printError(resized.Error);

  // printf("Flip\n");
  // NaClO_ImageResult flipped = NaClO_Flip(&resized.result, NACLO_FLIP_Y);
  // printError(flipped.Error);
  // NaClO_SaveAndFree(&flipped.result, "flip.png");

  // NaClO_ImageResult cropped = NaClO_Crop(&resized.result, 0, 0, 100, 100);
  // NaClO_FreeImage(&resized.result);
  // printError(cropped.Error);
  // NaClO_ErrorType saveCroppedResult = NaClO_Save(&cropped.result,
  // "resize.png"); NaClO_FreeImage(&cropped.result);
  // printError(saveCroppedResult);

  // NaClO_PixelType p;
  // p.RGBA.r = 0;
  // p.RGBA.g = 153;
  // p.RGBA.b = 192;

  // printf("newImage\n");
  // NaClO_ImageResult newImage = NaClO_NewImage(5000, 5000, NaClO_RGB, p);
  // // printf("newImage\n");
  // printError(newImage.Error);

  // printf("paste\n");
  // NaClO_ImageResult z2 = NaClO_Load("color.png");
  // // printError(z2.Error);
  // // NaClO_ImageResult converted2=NaClO_ConvertE(&z2.result, z.result.mode);
  // NaClO_ImageResult pasted =
  //     NaClO_Paste(&newImage.result, &z2.result, 1000, 1000);
  // // printf("pasted\n");
  // printError(pasted.Error);

  // NaClO_SaveAndFree(&pasted.result, "paste.png");
  // NaClO_FreeImage(&newImage.result);
  // NaClO_FreeImage(&z2.result);

  {
    printf("Resized\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ErrorType err = NaClO_Resized(&src.result, 50, 50);
    printError(err);
    NaClO_SaveAndFree(&src.result, "resized.png");
  }
  {
    printf("Flipped\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ErrorType err = NaClO_Flipped(&src.result, NACLO_FLIP_NEGATIVE_X);
    printError(err);
    NaClO_SaveAndFree(&src.result, "flipped.png");
  }
  // {
  //   NaClO_PixelType p;
  //   p.RGBA.r = 0;
  //   p.RGBA.g = 114;
  //   p.RGBA.b = 42;
  //   printf("pasted\n");
  //   NaClO_ImageResult src = NaClO_Load("color.png");
  //   NaClO_ImageResult newImage = NaClO_NewImage(5000, 5000, NaClO_RGB, p);

  //   NaClO_ErrorType err =
  //       NaClO_Pasted(&newImage.result, &src.result, 1000, 1000);
  //   printError(err);
  //   NaClO_SaveAndFree(&newImage.result, "pasted.png");
  //   NaClO_FreeImage(&src.result);
  // }
  {
    printf("Cropped\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ErrorType err = NaClO_Cropped(&src.result, 50, 50, 50, 50);
    printError(err);
    NaClO_SaveAndFree(&src.result, "cropped.png");
  }
  {
    printf("Rotated\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult Rotate = NaClO_Rotate(&src.result, 77);
    printError(Rotate.Error);
    NaClO_FreeImage(&src.result);
    NaClO_SaveAndFree(&Rotate.result, "Rotate1.png");
  }

  {
    NaClO_MatrixResult M1R = NaClO_NewMatrix(3, 3);
    NaClO_MatrixResult M2R = NaClO_NewMatrix(3, 3);
    NaClO_Matrix M1 = NaClO_Unwrap(M1R);
    NaClO_Matrix M2 = NaClO_Unwrap(M2R);
    *NaClO_MatrixElement(&M1, 0, 0) = 3;
    *NaClO_MatrixElement(&M1, 0, 1) = 4;
    *NaClO_MatrixElement(&M1, 0, 2) = 5;

    *NaClO_MatrixElement(&M1, 1, 0) = 6;
    *NaClO_MatrixElement(&M1, 1, 1) = 7;
    *NaClO_MatrixElement(&M1, 1, 2) = 8;

    *NaClO_MatrixElement(&M1, 2, 0) = 8;
    *NaClO_MatrixElement(&M1, 2, 1) = 9;
    *NaClO_MatrixElement(&M1, 2, 2) = 1;

    //

    *NaClO_MatrixElement(&M2, 0, 0) = 7;
    *NaClO_MatrixElement(&M2, 0, 1) = 8;
    *NaClO_MatrixElement(&M2, 0, 2) = 9;

    *NaClO_MatrixElement(&M2, 1, 0) = 0;
    *NaClO_MatrixElement(&M2, 1, 1) = 8;
    *NaClO_MatrixElement(&M2, 1, 2) = 9;

    *NaClO_MatrixElement(&M2, 2, 0) = 5;
    *NaClO_MatrixElement(&M2, 2, 1) = 6;
    *NaClO_MatrixElement(&M2, 2, 2) = 9;

    printf("M1\n");
    PrintMatrix(M1);

    printf("M2\n");

    PrintMatrix(M2);

    printf("Matrix multiply\n");
    NaClO_MatrixResult M3R = NaClO_MatrixMultiply(&M1, &M2);
    printError(M3R.Error);
    NaClO_Matrix M3 = NaClO_Unwrap(M3R);

    for (uint8_t y = 0; y < M3.width; ++y) {
      for (uint8_t x = 0; x < M3.height; ++x) {
        printf("%f ", *NaClO_MatrixElement(&M3, x, y));
      }
      printf("\n");
    }

    printf("Matrix Inversion\n");
    NaClO_MatrixResult M4R = NaClO_MatrixInversion(&M3);
    printError(M4R.Error);
    NaClO_Matrix M4 = NaClO_Unwrap(M4R);

    PrintMatrix(M4);
    NaClO_FreeMatrix(&M1);
    NaClO_FreeMatrix(&M2);
    NaClO_FreeMatrix(&M3);
    NaClO_FreeMatrix(&M4);
  }

  // {
  //   printf("Expand\n");
  //   NaClO_ImageResult src = NaClO_Load("color.png");
  //   NaClO_ImageResult defaultExpand =
  //       NaClO_Expand(&src.result, 2 * src.result.width, 2 *
  //       src.result.height,
  //                    NACLO_EXPAND_DEFAULT);
  //   printError(defaultExpand.Error);
  //   NaClO_SaveAndFree(&defaultExpand.result, "defaultExpand.png");
  //   NaClO_ImageResult mirrorExpand =
  //       NaClO_Expand(&src.result, 4 * src.result.width, 4 *
  //       src.result.height,
  //                    NACLO_EXPAND_MIRROR);
  //   printError(mirrorExpand.Error);
  //   NaClO_SaveAndFree(&mirrorExpand.result, "mirrorExpand.png");
  //   NaClO_ImageResult edgeExpand =
  //       NaClO_Expand(&src.result, 2 * src.result.width, 2 *
  //       src.result.height,
  //                    NACLO_EXPAND_EDGE);
  //   printError(edgeExpand.Error);
  //   NaClO_SaveAndFree(&edgeExpand.result, "edgeExpand.png");

  //   NaClO_FreeImage(&src.result);
  // }

  {
    printf("RotatedAt\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult Rotate = NaClO_RotateAt(&src.result, 77, 0, 0);
    printError(Rotate.Error);
    NaClO_FreeImage(&src.result);
    NaClO_SaveAndFree(&Rotate.result, "RotateAt.png");
  }
  {
    printf("Gamma\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    printError(NaClO_SetGamma(&src.result, 0.5f));
    NaClO_SaveAndFree(&src.result, "gamma.png");
  }
  {
    printf("Conv\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    if (src.Error != NACLO_OK) {
      printf("Failed to load image\n");
      return -1;
    }
    // printError(NaClO_SetGamma(&src.result, 0.5f));
    NaClO_MatrixResult mResult = NaClO_NewMatrix(3, 3);
    if (mResult.Error != NACLO_OK) {
      printf("Failed to create matrix\n");
      NaClO_FreeImage(&src.result);
      return -1;
    }
    NaClO_Matrix m = mResult.result;
    // *NaClO_MatrixElement(&m, 0, 0)=0;
    *NaClO_MatrixElement1(&m, 2, 1) = -1;
    *NaClO_MatrixElement1(&m, 2, 1) = -1;
    *NaClO_MatrixElement1(&m, 2, 2) = 5;
    *NaClO_MatrixElement1(&m, 2, 3) = -1;
    *NaClO_MatrixElement1(&m, 3, 2) = -1;

    NaClO_ErrorType convError = NaClO_Convoluted(&src.result, m, 1, 0);
    printError(convError);

    NaClO_FreeMatrix(&m);

    if (convError == NACLO_OK) {
      NaClO_SaveAndFree(&src.result, "Conv.png");
    } else {
      NaClO_FreeImage(&src.result);
    }
  }
  {
    printf("Laplace\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult lp = NaClO_Laplace(&src.result);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "Laplace.png");
    NaClO_FreeImage(&src.result);
  }
  {
    printf("Sobel\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_EdgeDetect(&src.result);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "sobel.png");
    NaClO_FreeImage(&src.result);
  }
  {
    printf("Roberts\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_Roberts(&src.result);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "Roberts.png");
    NaClO_FreeImage(&src.result);
  }

  {
    printf("Prewitt\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_Prewitt(&src.result);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "prewitt.png");
    NaClO_FreeImage(&src.result);
  }
  {
    printf("AvgBlur\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_AvgBlur(&src.result, 1);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "Avgblur.png");
    NaClO_FreeImage(&src.result);
  }
  {
    printf("MedianBlur\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_AvgBlur(&src.result, 8);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "MedianBlur.png");
    NaClO_FreeImage(&src.result);
    // NormalDistribution2D(2,2);
  }
  {
    printf("GaussianBlur\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_AvgBlur(&src.result, 8);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "GaussianBlur.png");
    NaClO_FreeImage(&src.result);
    // NormalDistribution2D(2,2);
  }

  {
    printf("Blur\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_Blur(&src.result, 1);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "blur.png");
    NaClO_FreeImage(&src.result);
  }

  {
    printf("HSV\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_HueSaturationValue(&src.result, 1, 2, 1);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "HueSaturationValue.png");
    NaClO_FreeImage(&src.result);
  }
  {
    printf("Histogram\n");

    NaClO_MakeHistogram(hist);
    NaClO_ImageResult src = NaClO_Load("re.jpg");

    NaClO_GradientHistogram(&src.result, hist);
    for (int x = 0; x < 255; ++x) {
      printf("%d,", (int)hist[x]);
    }
    uint8_t firstNonZero = 0, lastNonZero = 0;
    for (uint8_t x = 0; x < 255; ++x) {
      if (hist[x] > 0) {
        firstNonZero = x;
        break;
      }
    }
    for (uint8_t x = 255; x > 0; --x) {
      if (hist[x] > 0) {
        lastNonZero = x;
        break;
      }
    }

    NaClO_float scale = (NaClO_float)(lastNonZero - firstNonZero) / 255.0f;
    NaClO_MakeHistogram(Nhist);

    // uint8_t x2=0;
    for (uint8_t x = firstNonZero; x > lastNonZero; --x) {
      Nhist[(uint8_t)(x * scale)] = hist[x];
    }
    printf("\nNhist\n");

    for (int x = 0; x < 255; ++x) {
      printf("%d,", (int)Nhist[x]);
    }
    NaClO_MakeHistogram(rh);
    NaClO_MakeHistogram(gh);
    NaClO_MakeHistogram(bh);
    NaClO_RGBHistogram(&src.result, rh, gh, bh);
    printf("\nR\n");

    for (int x = 0; x < 255; ++x) {
      printf("%d,", (int)rh[x]);
    }

    printf("G\n");

    for (int x = 0; x < 255; ++x) {
      printf("%d,", (int)gh[x]);
    }
    printf("B\n");

    for (int x = 0; x < 255; ++x) {
      printf("%d,", (int)bh[x]);
    }

    NaClO_FreeImage(&src.result);
  }
  {
    printf("FlipAt\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_FlipAt(&src.result, 1000, NACLO_FLIP_X);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "FlipAt.png");
    NaClO_FreeImage(&src.result);
  }

  {
    printf("HistEq\n");
    NaClO_ImageResult src = NaClO_Load("re.jpg");
    src.Error = NaClO_Converted(&src.result, "L");
    NaClO_Save(&src.result, "L.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);
    NaClO_ImageResult lp = NaClO_HistogramEqualization(&src.result);
    printError(lp.Error);
    NaClO_SaveAndFree(&lp.result, "HistEq.png");
    NaClO_FreeImage(&src.result);
  }

  {
    printf("premul\n");
    NaClO_ImageResult src = NaClO_Load("alpha.png");
    // src.Error = NaClO_Converted(&src.result, "L");
    // NaClO_Save(&src.result, "L.png");
    // NaClO_ImageResult lp=NaClO_Sobel(&src.result);

    printError(NaClO_PremultipliedAlpha(&src.result));
    NaClO_SaveAndFree(&src.result, "premul.png");
    // NaClO_FreeImage(&src.result);
  }

  {
    printf("DrawCircle\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_PixelType pt;
    memset(&pt, 1, sizeof(pt));
    printError(NaClO_DrawCircle(&src.result, 100, 100, pt, 120, false, 50));
    NaClO_SaveAndFree(&src.result, "circle.png");
  }
  {
    printf("Sharpen\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_PixelType pt;
    memset(&pt, 1, sizeof(pt));
    printError(NaClO_Sharpened(&src.result));
    NaClO_SaveAndFree(&src.result, "sharpened.png");
  }

  {
    printf("Line\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_PixelType pt;
    memset(&pt, 1, sizeof(pt));
    printError(NaClO_DrawLine(&src.result, 0, 0, 300, 300, pt, 8));
    NaClO_SaveAndFree(&src.result, "line.png");
  }
  {
    printf("Blend\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Blended(&src.result, &src2.result, 0.5);
    NaClO_SaveAndFree(&src.result, "blended.png");
    NaClO_SaveAndFree(&src2.result, "blended.png");
  }
  {
    printf("Dissolve\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Dissolved(&src.result, &src2.result, 0.5);
    NaClO_SaveAndFree(&src.result, "dissolved.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("lighten\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Lightened(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "lighten.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("darken\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Darkened(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "darken.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("multiply\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Multiplied(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "multiply.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("burn\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Burned(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "burn.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("min\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_SetMin(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "min.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("screen\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Screened(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "screen.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("colorDodge\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_ColorDodged(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "colordodged.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("linearDodge\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_LinearDodged(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "lineardodged.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("overlay\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_Overlaid(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "overlay.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("softlight\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_SetSoftLight(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "softlight.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    printf("hardlight\n");
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_SetHardLight(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, "hardlight.png");
    NaClO_FreeImage(&src2.result);
  }
  {
    const char* f="vividlight.png";
    printf("%s\n",f);
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_SetVividLight(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, f);
    NaClO_FreeImage(&src2.result);
  }  {
    const char* f="linearlight.png";
    printf("%s\n",f);
    NaClO_ImageResult src = NaClO_Load("color.png");
    NaClO_ImageResult src2 = NaClO_Load("HistEq.png");
    NaClO_SetLinearLight(&src.result, &src2.result);
    NaClO_SaveAndFree(&src.result, f);
    NaClO_FreeImage(&src2.result);
  }
  //   {
  //       printf("Perspective\n");
  //     NaClO_ImageResult src = NaClO_Load("color.png");
  //   printError(NaClO_SetPerspective(&src.result, 50, 50, 100, 500, 300, 500,
  //   300, 100, 300, 12, 42, 45, 13, 45, 25, 66));
  //   NaClO_SaveAndFree(&src.result, "perspective.png");
  // }
}