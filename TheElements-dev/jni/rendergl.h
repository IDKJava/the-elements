/*
 * gl.h
 * -----------------------------
 * Declares the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#ifndef GL_H_INCLUDED
#define GL_H_INCLUDED

#include <GLES2/gl2.h>

void glInit(void);
void glRender(void);
void glRenderThreaded(void);

void printGLString(const char *name, GLenum s);
void checkGlError(const char* op);
GLuint buildProgram(const char* pVertexSource, const char* pFragmentSource);


#endif //!GL_H_INCLUDED
