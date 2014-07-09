// TODO(gkanwar): Rename this file
#ifndef SAVELOAD_2_H_INCLUDED
#define SAVELOAD_2_H_INCLUDED

#include <fstream>

using namespace std;

// Public C API
#ifdef __cplusplus
extern "C" {
#endif

char saveState2(char* saveLoc);
char loadState2(char* loadLoc);

#ifdef __cplusplus
}
#endif

// Private C++ helper functions
bool saveStateLogic2(ofstream& out);
bool loadStateLogic2(ifstream& in);
unsigned long hashElement2(struct Element* element);
char* stringifyElement2(struct Element* element);
unsigned long hashStr2(char* str);
int findElementFromHash2(unsigned long hash);

#endif  // SAVELOAD_2_H_INCLUDED
