#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <unistd.h>
#include <iostream>

#include "palette.h"
#include "interface.h"

#include "rendergl.h"
#include "app.h"
#include "macros.h"
#include "saveload2.h"
#include "setup.h"

// Per-file logging
#ifndef NDEBUG
// Debug
#define LOGGING 1
#else
// Release
#define LOGGING 0
#endif


#define RED_DEPTH 5
#define GREEN_DEPTH 6
#define BLUE_DEPTH 5
#define RENDER_DEPTH 16
#define FRAME_VALUES 10

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
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
EGLContext eglContextMain;
EGLContext eglContextPalette;
EGLSurface eglSurfaceMain;
EGLSurface eglSurfacePalette;
Display *x11Display;

void initOpenGL(SDL_Window* window, SDL_Window* palette) {
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

  eglSurfaceMain = eglCreateWindowSurface(eglDisplay, eglConfig,
                                      (EGLNativeWindowType)sysInfo.info.x11.window, 0);
  if (eglSurfaceMain == EGL_NO_SURFACE) {
    throwErr("Unable to create EGL main surface.");
  }

  if (SDL_GetWindowWMInfo(palette, &sysInfo) != SDL_TRUE) {
    sdlErr("Unable to get palette window handle.");
  }
  eglSurfacePalette = eglCreateWindowSurface(eglDisplay, eglConfig,
                                             (EGLNativeWindowType)sysInfo.info.x11.window, 0);
  if (eglSurfacePalette == EGL_NO_SURFACE) {
    throwErr("Unable to create EGL palette surface.");
  }

  eglBindAPI(EGL_OPENGL_ES_API);
  EGLint contextParams[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  eglContextMain = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextParams);
  if (eglContextMain == EGL_NO_CONTEXT) {
    throwErr("Unable to create GLES context.");
  }
  eglContextPalette = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextParams);
  if (eglContextPalette == EGL_NO_CONTEXT) {
    throwErr("Unable to create GLES context.");
  }

  // Start off with main as the current EGL context
  if (eglMakeCurrent(eglDisplay, eglSurfaceMain, eglSurfaceMain, eglContextMain) == EGL_FALSE) {
    throwErr("Unable to make GLES context current.");
  }
}

void makeMainCurrent() {
  if (eglMakeCurrent(eglDisplay, eglSurfaceMain, eglSurfaceMain, eglContextMain) == EGL_FALSE) {
    throwErr("Unable to make GLES context current.");
  }
}

void makePaletteCurrent() {
  if (eglMakeCurrent(eglDisplay, eglSurfacePalette, eglSurfacePalette, eglContextPalette) == EGL_FALSE) {
    throwErr("Unable to make GLES context current.");
  }
}

void termOpenGL() {
  eglMakeCurrent(eglDisplay, NULL, NULL, EGL_NO_CONTEXT);
  eglDestroySurface(eglDisplay, eglSurfaceMain);
  eglDestroySurface(eglDisplay, eglSurfacePalette);
  eglDestroyContext(eglDisplay, eglContextMain);
  eglDestroyContext(eglDisplay, eglContextPalette);
  eglSurfaceMain = NULL;
  eglSurfacePalette = NULL;
  eglContextMain = NULL;
  eglContextPalette = NULL;
  eglConfig = NULL;
  eglTerminate(eglDisplay);
  eglDisplay = NULL;
  XCloseDisplay(x11Display);
  x11Display = NULL;
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

void renderMain() {
  makeMainCurrent();
  nativeRender();
  eglSwapBuffers(eglDisplay, eglSurfaceMain);
}

void renderPalette() {
  makePaletteCurrent();

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  drawPalette();

  eglSwapBuffers(eglDisplay, eglSurfacePalette);
}

int main(int argc, char **argv) {
  SDL_Window* mainWindow = NULL;
  SDL_Window* paletteWindow = NULL;
  Uint32 mainID, paletteID;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    sdlErr("SDLInit failed");
  }

  // Make main window
  mainWindow = SDL_CreateWindow("The Elements",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (mainWindow == NULL) {
    sdlErr("Main window create failed");
  }
  mainID = SDL_GetWindowID(mainWindow);

  // Make palette window
  paletteWindow = SDL_CreateWindow("The Elements - Palette",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               PALETTE_WIDTH, PALETTE_HEIGHT, SDL_WINDOW_SHOWN);
  if (paletteWindow == NULL) {
    sdlErr("Palette window create failed");
  }
  paletteID = SDL_GetWindowID(paletteWindow);

  initOpenGL(mainWindow, paletteWindow);
  makePaletteCurrent(); // Palette GL must be initialized in the palette context
  initPalette();

  // Main drawing stuff -- uses JNI interface for now
  makeMainCurrent(); // Native init must happen in main context
  nativeInit();
  setElement(NORMAL_ELEMENT);
  setZoom(1);
  nativeResize(SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_Event event;
  bool running = true;
  Uint32 ticks = SDL_GetTicks();
  int frames = 0;
  while (running) {
    // Render main window
    renderMain();
    frames++;
    if (frames % 10 == 0) {
      Uint32 diff = SDL_GetTicks() - ticks;
      ticks += diff;
      cout << "Avg framerate: " << 10/(0.001*diff) << endl;
    }
    
    // Render palette
    renderPalette();

    // Handle events
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button = SDL_BUTTON_LEFT) {
          if (event.button.windowID == mainID) {
            brushStartLocation(event.button.x, event.button.y);
          }
          else if (event.button.windowID == paletteID) {
            paletteLeftClick(event.button.x, event.button.y);
          }
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if (event.button.button = SDL_BUTTON_LEFT) {
          if (event.button.windowID == mainID) {
            brushEndLocation(event.button.x, event.button.y);
          }
        }
        break;
      case SDL_MOUSEMOTION:
        if (event.motion.state & SDL_BUTTON_LMASK) {
          if (event.button.windowID == mainID) {
            brushMoveLocation(event.motion.x, event.motion.y);
          }
        }
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          running = false;
        }
        else if (event.key.keysym.sym == SDLK_F9) {
          // Save temp
          saveState2("temp.sav2");
        }
        else if (event.key.keysym.sym == SDLK_F10) {
          // Clear and reload temp
          gameSetup();
          loadState2("temp.sav2");
        }
        break;
      }
    }
  }

  // Cleanup
  termOpenGL();
  SDL_DestroyWindow(paletteWindow);
  SDL_DestroyWindow(mainWindow);
  SDL_Quit();
  return 0;
}
