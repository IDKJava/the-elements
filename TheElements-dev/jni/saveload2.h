// TODO(gkanwar): Rename this file
#ifndef SAVELOAD_2_H_INCLUDED
#define SAVELOAD_2_H_INCLUDED

#include <fstream>

//File extensions
#define SAVE2_EXTENSION ".sav2"
#define ELEMENT2_EXTENSION ".ele2"

using namespace std;

// Public API
char saveState2(char* saveLoc);
char loadState2(char* loadLoc);
char loadCustomElements2(void);

// Private helper functions
bool saveStateLogic2(ofstream& out);
bool loadStateLogic2(ifstream& in);
bool loadCustomElement2(ifstream& in);
int findElementFromFilename(string filename);

#endif  // SAVELOAD_2_H_INCLUDED
