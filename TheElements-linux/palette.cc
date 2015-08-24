#include "palette.h"

#include <GLES2/gl2.h>

#include "interface.h"

#include "app.h"
#include "rendergl.h"

// Per-file logging
#ifndef NDEBUG
// Debug
#define LOGGING 1
#else
// Release
#define LOGGING 0
#endif

// Palette rendering
static GLuint prog;
static GLuint posHandle;
static GLuint colHandle;
static GLuint mvpHandle;
static const char gPaletteVShader[] =
  "attribute vec4 aPosition;"
  "uniform mat4 uMVPMatrix;"
  "void main() {"
  "  gl_Position = uMVPMatrix*aPosition;"
  "}";
static const char gPaletteFShader[] =
  "precision mediump float;"
  "uniform vec4 uColor;"
  "void main() {"
  "  gl_FragColor = uColor;"
  "}";

void initPalette() {
  prog = buildProgram(gPaletteVShader, gPaletteFShader);
  LOGI("Built palette GL program %d", prog);
  posHandle = glGetAttribLocation(prog, "aPosition");
  colHandle = glGetUniformLocation(prog, "uColor");
  mvpHandle = glGetUniformLocation(prog, "uMVPMatrix");
  LOGI("Uniform handle: %d %d", colHandle, mvpHandle);
}
void drawPalette() {
  GLfloat verts[12];
  GLfloat col[4];
  float cellWidth = CELL_SIZE/(float)PALETTE_WIDTH;
  float cellHeight = CELL_SIZE/(float)PALETTE_HEIGHT;

  glUseProgram(prog);

  // Transform from [-1,1]x[-1,1] to [0,1]x[1,0]
  // in the X,Y coordinates
  GLfloat proj[16] = {
    2, 0, 0, 0,
    0, -2, 0, 0,
    0, 0, 1, 0,
    -1, 1, 0, 1
  };
  glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &proj[0]);
  checkGlError("glUniformMatrix4fv");

  // Palette is a 10row x 4col matrix
  for (int i = 0; i < PALETTE_HEIGHT/CELL_SIZE; ++i) {
    for (int j = 0; j < PALETTE_WIDTH/CELL_SIZE; ++j) {
      float baseY = i*cellHeight;
      float baseX = j*cellWidth;
      verts[0] = baseX;
      verts[1] = baseY;
      verts[2] = baseX;
      verts[3] = baseY+cellHeight;
      verts[4] = baseX+cellWidth;
      verts[5] = baseY+cellHeight;
      verts[6] = baseX;
      verts[7] = baseY;
      verts[8] = baseX+cellWidth;
      verts[9] = baseY+cellHeight;
      verts[10] = baseX+cellWidth;
      verts[11] = baseY;

      int eltIndex = NORMAL_ELEMENT+(PALETTE_WIDTH/CELL_SIZE)*i + j;
      if (eltIndex < numElements) {
        struct Element* elt = elements[eltIndex];
        col[0] = (elt->red/255.0);
        col[1] = (elt->green/255.0);
        col[2] = (elt->blue/255.0);
        col[3] = 1.0;
      }
      else {
        col[0] = col[1] = col[2] = 0.0;
        col[3] = 1.0;
      }

      glEnableVertexAttribArray(posHandle);
      glVertexAttribPointer(posHandle, 2, GL_FLOAT, GL_FALSE, 0, verts);
      glUniform4fv(colHandle, 1, col);

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }
}

void paletteLeftClick(int x, int y) {
  LOGI("Palette click %d %d", x, y);
  int i = y/CELL_SIZE; // row
  int j = x/CELL_SIZE; // col
  int eltInd = i*PALETTE_WIDTH/CELL_SIZE + j + NORMAL_ELEMENT;
  if (eltInd >= NORMAL_ELEMENT && eltInd < numElements) {
    LOGI("Set element %d", eltInd);
    setElement(eltInd);
  }
}
