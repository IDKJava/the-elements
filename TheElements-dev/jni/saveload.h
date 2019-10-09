/*
 * saveload.h
 * --------------------------
 * Declares functions relating to saving and loading from files.
 * This file is around for backwards compatibility, as we now use
 * proto saving and loading instead.
 */

#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

#include <string>

#include "app.h"
#include "messages.pb.h"

using namespace std;

// NOTE(gkanwar): Ensure that these stay up to date by setting good
// defaults for any future properties we add, but do NOT change the file
// formats being read in, since the whole point is backwards compatibility.
char upgradeStateV1(FILE* fp, SaveFile* saveProto);
char upgradeCustomElementV1(FILE* fp, CustomElement* customProto,
                            const string& newFilename);

// WARNING: Do NOT change these hash functions to be up-to-date with new element
// properties. At some point we'll phase out backwards support, but for now these
// are needed for upgrading old save files.
unsigned long hashElement(const CustomElement& customProto, int index);
char* stringifyElement(const CustomElement& customProto, int index);
unsigned long hashStr(char* str);
unsigned long hashBytes(char* bytes, int num);

#endif //!SAVELOAD_H_INCLUDED
