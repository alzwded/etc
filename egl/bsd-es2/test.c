//#include <GL/gl.h>
#include <GL/glew.h>
#include <stdio.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

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
}
