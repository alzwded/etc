#include <stdio.h>
#include <stdlib.h>
#include <png.h>

void fillImage(png_byte** row_pointers, int l1w, int l2w, unsigned int c1, unsigned int c2, int width, int height)
{
#define R(X, Y) (row_pointers[Y][X * 3 + 0])
#define G(X, Y) (row_pointers[Y][X * 3 + 1])
#define B(X, Y) (row_pointers[Y][X * 3 + 2])
#define SET(X, Y, CL)\
                R(X, Y) = (CL & 0x00FF0000) >> 16; \
                G(X, Y) = (CL & 0x0000FF00) >> 8; \
                B(X, Y) = (CL & 0x000000FF);
    size_t i, j, k;

    k = 0;
    for(i = 0; i < height; ++i) {
        for(j = 0; j < width; ++j) {
            if(k < l1w) {
                SET(j, i, c1);
            } else if(k < l1w + l2w) {
                SET(j, i, c2);
            }
        }
        k = (k + 1) % (l1w + l2w);
    }
    
#undef R
#undef G
#undef B
#undef SET
}

int main(int argc, char* argv[])
{
    FILE* fp = NULL;
    size_t i = 0;
    png_bytep* row_pointers = NULL;
    int rc = 0;
    int width;
    int height;
    int const bit_depth = 8;

    if(argc != 8) {
        printf("usage: %s file w h l1w l2w c1 c2\n", argv[0]);
        return 255;
    }

    fp = fopen(argv[1], "wb"); 
    if(!fp) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 1;
    }

    png_structp png_ptr = png_create_write_struct
        (PNG_LIBPNG_VER_STRING, png_voidp_NULL,
         png_error_ptr_NULL, png_error_ptr_NULL); 
    if (!png_ptr) {
        fclose(fp);
        return 2;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr,
                (png_infopp)NULL);
        fclose(fp);
        return 2;
    }

    width = atoi(argv[2]);
    height = atoi(argv[3]);

    row_pointers = png_malloc(png_ptr, height * png_sizeof(png_bytep));
    for(i = 0; i < width; ++i) {
        row_pointers[i] = png_malloc(png_ptr, width * 3 * (bit_depth / 8));
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        rc = 3;
    } else {

        png_init_io(png_ptr, fp);

#define color_type PNG_COLOR_TYPE_RGB
#define interlace_type PNG_INTERLACE_NONE
#define compression_type PNG_COMPRESSION_TYPE_DEFAULT
#define filter_method PNG_FILTER_TYPE_DEFAULT
        png_set_IHDR(png_ptr, info_ptr, width, height,
                bit_depth, color_type, interlace_type,
                compression_type, filter_method);

        fillImage(row_pointers, atoi(argv[4]), atoi(argv[5]), strtol(argv[6], NULL, 0), strtol(argv[7], NULL, 0), width, height);

        //png_write_image(png_ptr, row_pointers);
        png_set_rows(png_ptr, info_ptr, row_pointers);
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

        png_write_end(png_ptr, info_ptr);
    }

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    for(i = 0; i < width; ++i) {
        free(row_pointers[i]);
    }
    free(row_pointers);

    return rc;
}
