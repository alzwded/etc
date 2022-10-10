//#include <GL/gl.h>
#include <GL/glew.h>
#include <stdio.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

static const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, // enable if creating pbuffer
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
};    
const EGLint* pConfigAttribs = configAttribs;

int main(int argc, char* argv[])
{
    EGLDisplay eglDpy;
    GLenum err;
    EGLint major, minor;
    EGLBoolean ok;
    eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    ok = eglInitialize(eglDpy, &major, &minor);
    printf("ok: %d\n", ok);


  EGLConfig eglCfg;
  int numConfigs;

  eglChooseConfig(eglDpy, pConfigAttribs, &eglCfg, 1, &numConfigs);

  // 4. Bind the API
  eglBindAPI(EGL_OPENGL_API);

  // 5. Create a context and make it current
  EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, 
                                       NULL);

  eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);

    err = glewInit();
    printf("%d %d\n", err, GLEW_OK);
    printf("%s\n", glewGetErrorString(err));
    glLoadIdentity();

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
  glDrawArrays(GL_TRIANGLES, 0, 1 * 3);
}
//clang++ test.cpp -I/usr/X11R6/include -L/usr/X11R6/lib -I/usr/local/include/libpng16 -L/usr/local/lib -I/usr/local/include -lGL -lEGL -lGLEW -lpng
