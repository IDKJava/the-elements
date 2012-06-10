/*
 * gl.h
 * -----------------------------
 * Declares the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#ifndef GL_H_INCLUDED
#define GL_H_INCLUDED

//Include the Gl importation functions
#include "importgl.h"
//Include the global variables
#include "app.h"
//Include the update function
#include "update.h"
//Include pthread functions
#include <pthread.h>

void glInit(void);
void glRender(void);

#endif //!GL_H_INCLUDED
