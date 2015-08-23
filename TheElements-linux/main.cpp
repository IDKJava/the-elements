#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <unistd.h>
#include <iostream>

#include "rendergl.h"

#define RED_DEPTH 5
#define GREEN_DEPTH 6
#define BLUE_DEPTH 5
#define RENDER_DEPTH 16

using namespace std;

const int SCREEN_WIDTH = 360;
const int SCREEN_HEIGHT = 540;
const EGLint configAttribs[] = {
  EGL_RED_SIZE, RED_DEPTH,
  EGL_GREEN_SIZE, GREEN_DEPTH,
  EGL_BLUE_SIZE, BLUE_DEPTH,
  EGL_DEPTH_SIZE, RENDER_DEPTH,
  EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
  EGL_BIND_TO_TEXTURE_RGBA, EGL_TRUE,
  EGL_NONE
};


void sdlErr(string err) {
  cerr << err << ": " << SDL_GetError() << endl;
  exit(1);
}
void throwErr(string err) {
  cerr << err << endl;
  exit(2);
}

EGLDisplay eglDisplay;
EGLConfig eglConfig;
EGLContext eglContext;
EGLSurface eglSurface;
Display *x11Display;

void initOpenGL(SDL_Window* window) {
  x11Display = XOpenDisplay(NULL);
  if (!x11Display) {
    throwErr("Unable to get display");
  }
  
  eglDisplay = eglGetDisplay((EGLNativeDisplayType)x11Display);
  if (eglDisplay == EGL_NO_DISPLAY) {
    throwErr("Unable to init EGL display.");
  }

  if (!eglInitialize(eglDisplay, NULL, NULL)) {
    throwErr("Unable to init EGL.");
  }

  EGLint numConfigsOut;
  if (eglChooseConfig(eglDisplay, configAttribs, &eglConfig, 1, &numConfigsOut) != EGL_TRUE) {
    throwErr("Unable to select EGL config.");
  }

  SDL_SysWMinfo sysInfo;
  SDL_VERSION(&sysInfo.version);
  if (SDL_GetWindowWMInfo(window, &sysInfo) != SDL_TRUE) {
    sdlErr("Unable to get window handle.");
  }

  eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig,
                                      (EGLNativeWindowType)sysInfo.info.x11.window, 0);
  if (eglSurface == EGL_NO_SURFACE) {
    throwErr("Unable to create EGL surface.");
  }

  eglBindAPI(EGL_OPENGL_ES_API);
  EGLint contextParams[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT,contextParams);
  if (eglContext == EGL_NO_CONTEXT) {
    throwErr("Unable to create GLES context.");
  }

  if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
    throwErr("Unable to make GLES context current.");
  }
}

void termOpenGL() {
  eglMakeCurrent(eglDisplay, NULL, NULL, EGL_NO_CONTEXT);
  eglDestroySurface(eglDisplay, eglSurface);
  eglDestroyContext(eglDisplay, eglContext);
  eglSurface = NULL;
  eglContext = NULL;
  eglConfig = NULL;
  eglTerminate(eglDisplay);
  eglDisplay = NULL;
  XCloseDisplay(x11Display);
  x11Display = NULL;
}

void swapBuffers() {
  eglSwapBuffers(eglDisplay, eglSurface);
}

static const char gTestVShader[] =
                 "attribute vec4 vPosition;\n"
                 "void main() {\n"
                 "  gl_Position = vPosition;\n"
                 "}\n";
static const char gTestFShader[] =
                 "precision mediump float;"
                 "void main() {"
                 "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
                 "}";

int main(int argc, char **argv) {
  SDL_Window* window = NULL;
  SDL_Surface* screen = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    sdlErr("SDLInit failed");
  }

  window = SDL_CreateWindow("The Elements",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    sdlErr("Window create failed");
  }

  screen = SDL_GetWindowSurface(window);
  
  initOpenGL(window);

  // Main drawing stuff
  printGLString("Version", GL_VERSION);
  printGLString("Vendor", GL_VENDOR);
  printGLString("Renderer", GL_RENDERER);
  printGLString("Extensions", GL_EXTENSIONS);

  glClearColor(0.0, 1.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  GLuint prog = buildProgram(gTestVShader, gTestFShader);
  glUseProgram(prog);
  float vertices[] = { 0.0, 0.5, 0.0,
                       -0.5, -0.5, 0.0,
                       0.5, -0.5, 0.0 };
  GLuint posAttrib = glGetAttribLocation(prog, "vPosition");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  swapBuffers();

  SDL_Delay(2000);

  // Cleanup
  termOpenGL();
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
