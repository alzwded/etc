#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <png.h>
#include <fcntl.h>

#ifndef HAVE_GBM
# undef EGL_MESA_platform_gbm
#endif
#ifdef EGL_MESA_platform_gbm
# include <gbm.h>
#endif

#define BUFSIZE 1024

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

static const EGLint ctx_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE };


static const int pbufferWidth = 9;
static const int pbufferHeight = 9;

static const EGLint pbufferAttribs[] = {
      EGL_WIDTH, pbufferWidth,
      EGL_HEIGHT, pbufferHeight,
      EGL_NONE,
};

GLuint loadProgram(std::string const& name)
{
    std::string vshader = "v" + name + ".glsl";
    std::string fshader = "f" + name + ".glsl";

    GLuint vShader, fShader;
    {
        std::fstream fv(vshader, std::ios::in);
        auto s = std::string(std::istreambuf_iterator<char>(fv), std::istreambuf_iterator<char>());
        vShader = glCreateShader(GL_VERTEX_SHADER);
        const char* ssss[] = { s.c_str() };
        int sizes[] = { (int)s.size() };
        glShaderSource(vShader, 1, ssss, sizes);
        glCompileShader(vShader);
        GLint status;
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
        if(!status) {
            printf("%s\n", s.c_str());
            char buf[512];
            int sz;
            glGetShaderInfoLog(vShader, 512, &sz, buf);
            printf("vertex shader: %d %s\n", sz, buf);
        }
    }
    {
        std::fstream ff(fshader, std::ios::in);
        auto s = std::string(std::istreambuf_iterator<char>(ff), std::istreambuf_iterator<char>());
        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* ssss[] = { s.c_str() };
        int sizes[] = { (int)s.size() };
        glShaderSource(fShader, 1, ssss, sizes);
        glCompileShader(fShader);
        GLint status;
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
        if(!status) {
            printf("%s\n", s.c_str());
            char buf[512];
            int sz;
            glGetShaderInfoLog(fShader, 512, &sz, buf);
            printf("fragment shader: %s\n", buf);
        }
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    GLint pstatus;
    glGetProgramiv(program, GL_LINK_STATUS, &pstatus);
    if(!pstatus) {
        char buf[512];
        int sz;
        glGetProgramInfoLog(program, 512, &sz, buf);
        printf("program: %s\n", buf);
    }
    return program;
}

int main(int argc, char *argv[])
{
  // 1. Initialize EGL
  EGLDisplay eglDpy;

  if(argc > 1) {
      if(strcmp(argv[1], "help") == 0) {
          printf("Usage: %s [list|N]\n", argv[0]);
          printf("\n");
          printf("    list      list available devices\n");
          printf("    N         run on that device\n");
          printf("              The following device extensions are supported:\n");
          printf("              EGL_NV_device_cuda, EGL_MESA_device_software,\n");
          printf("              EGL_EXT_device_drm with gbm\n");
          printf("    -         Run against the default device\n");
          exit(2);
      } else if(strcmp(argv[1], "list") == 0) {
          EGLDeviceEXT eglDevices[4];
          EGLint numDevices;
          auto eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
          auto eglQueryDeviceStringEXT = (PFNEGLQUERYDEVICESTRINGEXTPROC)eglGetProcAddress("eglQueryDeviceStringEXT");
          eglQueryDevicesEXT(sizeof(eglDevices)/sizeof(eglDevices[0]), eglDevices, &numDevices);
          for(int i = 0; i < numDevices; ++i) {
              printf("device %d: %s\n", i, eglQueryDeviceStringEXT(eglDevices[i], EGL_EXTENSIONS));
          }
          exit(0);
      } else {
          EGLDeviceEXT eglDevices[4];
          EGLint numDevices;
          auto eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
          auto eglQueryDeviceStringEXT = (PFNEGLQUERYDEVICESTRINGEXTPROC)eglGetProcAddress("eglQueryDeviceStringEXT");
          auto eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
          eglQueryDevicesEXT(sizeof(eglDevices)/sizeof(eglDevices[0]), eglDevices, &numDevices);
          int idx = atoi(argv[1]);
          const char* deviceString = eglQueryDeviceStringEXT(eglDevices[idx], EGL_EXTENSIONS);
          if(idx > numDevices || idx < 0) {
              abort();
          } else {
              printf("Using device %d: %s\n", idx, deviceString);
          }

          if(strstr(deviceString, "EGL_NV_device_cuda")) {
              eglDpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, eglDevices[idx], 0);
          } else if(strstr(deviceString, "EGL_EXT_device_drm")) {
              int fd = open(eglQueryDeviceStringEXT(eglDevices[idx], EGL_DRM_DEVICE_FILE_EXT), O_RDWR);
              if(fd < 0) {
                  int err = errno;
                  printf("Error opening device: %d/%s\n", err, strerror(err));
                  abort();
              }
#ifdef HAVE_GBM
              printf("Using gbm\n");
              auto gbm = gbm_create_device(fd);
              pConfigAttribs += 2; // the pbuffer bit isn't supported on GBM
              eglDpy = eglGetDisplay((EGLNativeDisplayType)gbm);
#else
              printf("Using EGL_DRM_MASTER_FD_EXT\n");
# if !defined(EGL_DRM_MASTER_FD_EXT) && defined(EGL_EXT_platform_device) && EGL_EXT_platform_device
# define EGL_DRM_MASTER_FD_EXT 0x333C
# endif
              int attr1[] = { EGL_DRM_MASTER_FD_EXT, fd, EGL_NONE };
              eglDpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, eglDevices[idx], attr1);
              int err = eglGetError();
              printf("eglGetPlatformDisplayEXT: %d %x\n", err, err);
#endif
          } else if(strstr(deviceString, "EGL_MESA_device_software")) {
#ifdef EGL_MESA_platform_gbm
              pConfigAttribs += 2; // the pbuffer bit isn't supported on GBM
              //int fd = open("/dev/dri/card0", O_RDWR);
              int fd = open(eglQueryDeviceStringEXT(eglDevices[idx], EGL_DRM_DEVICE_FILE_EXT), O_RDWR);
              auto gbm = gbm_create_device(fd);
              eglDpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, gbm, NULL);
#else
              abort();
#endif
          }
      }
  } else {
       eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  }

  EGLint major, minor;

  EGLBoolean ok = eglInitialize(eglDpy, &major, &minor);
  if(!ok) {
      int err = eglGetError();
      printf("Failed to initialize EGL: %d %x\n", err, err);
      abort();
  }
  printf("major/minor: %d.%d\n", major, minor);


  // 2. Select an appropriate configuration
  EGLint numConfigs = 0;
  EGLConfig eglCfg;

  auto configOK = eglChooseConfig(eglDpy, pConfigAttribs, &eglCfg, 1, &numConfigs);
  if(!configOK) {
      int err = eglGetError();
      printf("Failed to choose a config: %d %x\n", err, err);
      abort();
  }
  if(numConfigs <= 0) {
      printf("No configs available\n");
      abort();
  }

  // 4. Bind the API
  eglBindAPI(EGL_OPENGL_ES_API);

  // 5. Create a context and make it current
  EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, ctx_attribs);

#if 0
  // if we're not setting up our own framebuffer, eglSurf fills that role
  // also, this was step 3. in the tutorial
  EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, 
                                               pbufferAttribs);

  eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);
#else
  eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx);
#endif

  // from now on use your OpenGL context

#if 0
  GLenum err = glewInit();
  if(err != GLEW_OK) {
      printf("Error initializing GL: %s\n", glewGetErrorString(GLEW_VERSION));
      abort();
  }
#endif
  GLuint program = loadProgram("shader");
  glUseProgram(program);
  GLuint buffers[1];
  glGenBuffers(sizeof(buffers)/sizeof(buffers[0]), buffers);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  const int NUM_TRIAS = 100000;
  GLfloat s_coords[] = {
      -0.5f, -0.5f, 0.0f,
      +0.5f, -0.5f, 0.0f,
      -0.0f, +0.5f, 0.0f,
  };
  GLfloat* coords = (GLfloat*)malloc(sizeof(GLfloat) * 3 * 3 * NUM_TRIAS);
  for(int i = 0; i < NUM_TRIAS; ++i) {
      memcpy(&coords[i * 9], s_coords, 9 * sizeof(GLfloat));
  }
  glBufferData(GL_ARRAY_BUFFER, NUM_TRIAS * 3 * 3 * 4, coords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  GLuint textures[1], framebuffers[1];
  glGenTextures(sizeof(textures)/sizeof(textures[0]), textures);
  glGenFramebuffers(sizeof(framebuffers)/sizeof(framebuffers[0]), framebuffers);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BUFSIZE, BUFSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
  glViewport(0, 0, BUFSIZE, BUFSIZE);

  //glClearColor(1, 1, 1, 1);
  glClearColor(1, 1, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  //for(int i = 0; i < 10; ++i)
  glDrawArrays(GL_TRIANGLES, 0, NUM_TRIAS * 3);

#if 0
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glFlush();

  // abgr or rgba depending on how you look at endianness
  // Okay, honestly, I don't know how this will map into libpng, but the alpha comes first in memory xD
  int pixels[BUFSIZE * BUFSIZE];
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#else
  glFlush();
  int pixels[BUFSIZE * BUFSIZE];
  glReadPixels(0, 0, BUFSIZE, BUFSIZE, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

#if 0
  for(int i = 0; i < 100; ++i) {
    for(int j = 0; j < 100; ++j) {
        printf("%08x ", pixels[i*100 + j]);
    }
    printf("\n");
  }
#endif
  unsigned char** pngRows = (unsigned char**)malloc(BUFSIZE * sizeof(unsigned char*));
  for(int i = 0; i < BUFSIZE; ++i) {
      pngRows[i] = (unsigned char*) (&pixels[i * BUFSIZE]);
  }

  FILE* fPNG = fopen("image.png", "wb");
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(png_ptr);
  png_set_IHDR(png_ptr, info_ptr,
          /*width=*/BUFSIZE, /*height=*/BUFSIZE,
          /*bid_depth=*/8, PNG_COLOR_TYPE_RGB_ALPHA,
          PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
          PNG_FILTER_TYPE_DEFAULT);
  png_init_io(png_ptr, fPNG);

  png_set_rows(png_ptr, info_ptr, pngRows);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  fclose(fPNG);
  printf("wrote image.png\n");
  png_destroy_write_struct(&png_ptr, &info_ptr); png_ptr = NULL; info_ptr = NULL;

  // 6. Terminate EGL when finished
  eglTerminate(eglDpy);
  return 0;
}
