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

//File/folder locations
#define ROOT_FOLDER "/sdcard/thelements/"
#define SAVES_FOLDER "saves/"
#define ELEMENTS_FOLDER "elements/"
#define ATMOSPHERES_FOLDER "atmospheres/"
#define TEMP_SAVE "temp"
#define DEMO_SAVE "demo"
#define LIST_SAVE "eleList"
//File extensions
#define LIST_EXTENSION ".lst"
#define SAVE_EXTENSION ".sav"
#define ELEMENT_EXTENSION ".ele"
#define ATMOSPHERE_EXTENSION ".atm"

//Include the FILE type
#include <stdio.h>
//Include the date and time functions
#include <time.h>
//Include the global variables
#include "app.h"
//Include the global macros
#include "macros.h"
//Include points functions
#include "points.h"
//Include the initializer function
#include "setup.h"
//Include the string functions
#include <string.h>
//Include the directory functions
#include <dirent.h>
#include <sys/stat.h>
//Include error reporting functions
#include <errno.h>

char saveState(char* saveLoc);
char saveStateLogic(FILE* fp);
char loadState(char* loadLoc);
char loadStateLogic(FILE* fp);
char removeTempSave(void);
char saveCustomElement(struct Element* createdCustomElement);
char loadCustomElement(char* loadLoc);

#endif //!SAVELOAD_H_INCLUDED
