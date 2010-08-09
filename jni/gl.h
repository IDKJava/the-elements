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

void appInit(void);
void appDeinit(void);
void appRender(void);

#endif
