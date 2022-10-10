#include <png.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    unsigned pixels[4] = { 0, 0xFF, 0xFFFF, 0xFFFFFF };
    unsigned char** pngRows = (unsigned char**)malloc(2 * sizeof(unsigned char*));
    pngRows[0] = (unsigned char*)pixels;

  FILE* fPNG = fopen("image.png", "wb");
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(png_ptr);
  png_set_IHDR(png_ptr, info_ptr,
          /*width=*/4, /*height=*/1,
          /*bid_depth=*/8, PNG_COLOR_TYPE_RGB,
          PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
          PNG_FILTER_TYPE_DEFAULT);
  png_init_io(png_ptr, fPNG);

  png_set_rows(png_ptr, info_ptr, pngRows);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  fclose(fPNG);
  printf("wrote image.png\n");
  png_destroy_write_struct(&png_ptr, &info_ptr); png_ptr = NULL; info_ptr = NULL;
}
