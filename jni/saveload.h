/*
 * saveload.h
 * --------------------------
 * Declares the function definitions for saver
 * and loader, the two functions which save and
 * load an element setup. Also defines macros
 * for file locations, and includes needed headers.
 */

#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

//Define the file locations so that they can be easily changed
#define SAVE_FILE "/sdcard/elementworks/save.txt"
#define QUICK_SAVE_FILE "/sdcard/elementworks/quicksave.txt"
#define DEMO_SAVE_FILE "/sdcard/elementworks/save2.txt"
#define CUSTOM_ELEMENT_FILE "/sdcard/elementworks/customele.txt"

//Include the FILE type
#include <stdio.h>
//Include the global variables
#include "app.h"
//Include the global macros
#include "macros.h"
//Include the initializer function
#include "setup.h"

int saver(int type);
int loader(int type);
void removeQuicksave(void);
int loadDemoFile(void);
int loadCustomFile(void);
int saveCustomFile(void);

#endif
