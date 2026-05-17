#include "naclo_image.h"
#include "stdio.h"
void printError(NaClO_ErrorType z)
{
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
  }
    printf("\n");
}
void PrintChannel(NaClO_Image c)
{
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
  rgb.r=0;
  rgb.g=114;
  rgb.b=224;
  NaClO_HSV hsv=NaClO_GetHSV(rgb);
  printf("%u,%f,%f\n",hsv.hue,hsv.saturation*100,hsv.value*100);
  rgb=NaClO_GetRGB(hsv);
  printf("%d,%d,%d\n",(int)rgb.r,(int)rgb.g,(int)rgb.b);


  // printf("11\n");

  // printf("Hello World\n");
  NaClO_ImageResult z = NaClO_Load("color.png");
    printf("NaClO_Load\n");
  printError(z.Error);
  // NaClO_Image z2=z.result;
  // printf("Channel:%d\n\nwidth:%ld\nheight:%ld\n\n",(int)z2.mode,z2.width,z2.height);


  printf("Copy\n");
  NaClO_ImageResult copied=NaClO_CopyImage(&z.result);
  printError(copied.Error);
  NaClO_FreeImage(&copied.result);




  PrintChannel(copied.result);
  printf("convert1\n");
  NaClO_ImageResult converted1=NaClO_Convert(&z.result,"1");
  PrintChannel(converted1.result);
  printError(converted1.Error);
  NaClO_Save(&converted1.result, "converted1.png"); 
  NaClO_FreeImage(&converted1.result);


  printf("convertL\n");
  NaClO_ImageResult convertedL=NaClO_Convert(&z.result,"L");
  PrintChannel(convertedL.result);
  printError(convertedL.Error);
  NaClO_Save(&convertedL.result, "convertedL.png"); 
  NaClO_FreeImage(&convertedL.result);



  printf("Resize\n");
  NaClO_ImageResult resized=NaClO_Resize(&z.result, z.result.width*2, z.result.height*2);
  NaClO_ErrorType saveResizedResult=NaClO_Save(&resized.result, "resized.png");
  printError(saveResizedResult);
  NaClO_FreeImage(&z.result);
  printError(resized.Error);

  printf("Flip\n");
  NaClO_ImageResult flipped=NaClO_Flip(&resized.result, NACLO_FLIP_Y);
  printError(flipped.Error);
  NaClO_SaveAndFree(&flipped.result ,"flip.png");




  NaClO_ImageResult cropped= NaClO_Crop(&resized.result, 0, 0, 100, 100);
  NaClO_FreeImage(&resized.result);
  printError(cropped.Error);
  NaClO_ErrorType saveCroppedResult=NaClO_Save(&cropped.result, "resize.png");
  NaClO_FreeImage(&cropped.result);
  printError(saveCroppedResult);

  NaClO_PixelType p;
  p.RGBA.r=0;
  p.RGBA.g=153;
  p.RGBA.b=192;

  printf("newImage\n");
NaClO_ImageResult newImage= NaClO_NewImage(5000,5000,NaClO_RGB,p);
  // printf("newImage\n");
  printError(newImage.Error);



  printf("paste\n");
  NaClO_ImageResult z2 = NaClO_Load("color.png");
  // printError(z2.Error);
  // NaClO_ImageResult converted2=NaClO_ConvertE(&z2.result, z.result.mode);
  NaClO_ImageResult pasted=NaClO_Paste(&newImage.result, &z2.result, 1000,1000);
  // printf("pasted\n");
  printError(pasted.Error);


  NaClO_SaveAndFree(&pasted.result, "paste.png");
  NaClO_FreeImage(&newImage.result);
  NaClO_FreeImage(&z2.result);



{
    printf("Resized\n");
  NaClO_ImageResult src = NaClO_Load("color.png");
  NaClO_ErrorType err=NaClO_Resized(&src.result, 50, 50);
  printError(err);
  NaClO_SaveAndFree(&src.result, "resized.png");


}
{
  printf("Flipped\n");
  NaClO_ImageResult src = NaClO_Load("color.png");
  NaClO_ErrorType err=NaClO_Flipped(&src.result, NACLO_FLIP_NEGATIVE_X);
  printError(err);
  NaClO_SaveAndFree(&src.result, "flipped.png");
}
{
    NaClO_PixelType p;
  p.RGBA.r=0;
  p.RGBA.g=114;
  p.RGBA.b=42;
  printf("pasted\n");
  NaClO_ImageResult src = NaClO_Load("color.png");
NaClO_ImageResult newImage= NaClO_NewImage(5000,5000,NaClO_RGB,p);
  
  NaClO_ErrorType err=NaClO_Pasted(&newImage.result, &src.result, 1000, 1000);
  printError(err);
  NaClO_SaveAndFree(&newImage.result, "pasted.png");
  NaClO_FreeImage(&src.result);
}
{
  printf("Cropped\n");
  NaClO_ImageResult src = NaClO_Load("color.png");
  NaClO_ErrorType err=NaClO_Cropped(&src.result,50,50,50,50);
  printError(err);
  NaClO_SaveAndFree(&src.result, "cropped.png");
}
  // printf("newImage\n");
  // NaClO_SaveAndFree(&newImage.result, "newImage.jpg");
  // printError(NaClO_FreeImage(&newImage.result)); //NACLO_NULL_POINTER



  
}