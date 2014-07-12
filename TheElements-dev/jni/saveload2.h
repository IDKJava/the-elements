// TODO(gkanwar): Rename this file
#ifndef SAVELOAD_2_H_INCLUDED
#define SAVELOAD_2_H_INCLUDED

#include <fstream>

//File/folder locations
#define ROOT_FOLDER "/sdcard/thelements/"
#define SAVES_FOLDER "saves/"
#define ELEMENTS_FOLDER "elements/"
#define ATMOSPHERES_FOLDER "atmospheres/"
#define TEMP_SAVE "temp"
#define DEMO_SAVE "demo"
#define LIST_SAVE "eleList"
//File extensions
#define ATMOSPHERE_EXTENSION ".atm"
#define SAVE2_EXTENSION ".sav2"
#define ELEMENT2_EXTENSION ".ele2"
//Deprecated
#define SAVE_EXTENSION ".sav"
#define ELEMENT_EXTENSION ".ele"

using namespace std;

// Public API
char saveState2(char* saveLoc);
char loadState2(char* loadLoc);
char loadCustomElements2(void);
void copyFile(char* srcLoc, char* dstLoc);

// Private helper functions
bool saveStateLogic2(ofstream& out);
bool loadStateLogic2(ifstream& in);
bool loadCustomElement2(ifstream& in);
int findElementFromFilename(const string& filename);

#endif  // SAVELOAD_2_H_INCLUDED
