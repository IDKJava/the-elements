/*
 * saveload.c
 * ----------------------------------------------------------------
 * Defines functions relating to saving and loading from files.
 * This file is around for backwards compatibility, as we now use
 * proto saving and loading instead.
 */

#include "saveload.h"

#include <android/log.h>
#include <stdio.h>

// Per-file logging
#ifndef NDEBUG
//Debug
#define LOGGING 1
#else
//Release
#define LOGGING 1
#endif

#define SAVELOAD_VERSION_CODE "$$VC1$$"

// Map from element hash to new element filename.
// This map is maintained as we upgrade each custom, then referenced
// for each upgraded save.
static map<unsigned long, string> elementUpgradeMap;

// NOTE(gkanwar): Ensure that these stay up to date by setting good
// defaults for any future properties we add, but do NOT change the file
// formats being read in, since the whole point is backwards compatibility.

char upgradeStateV1(FILE* fp, SaveFile* saveProto)
{
    if (fp == NULL)
    {
        LOGW("Could not upgrade save -- fp null");
        return FALSE;
    }

    int elementIndex, elementHash,
        sizeX, sizeY, fileMaxSpecials, tempSpecialVal, charsRead;
    struct Element* tempElement;
    int tempParticle;
    char buffer[64];
    char lookAhead;

    // Get the saveload version code
    if((charsRead = fscanf(fp, "%s\n\n", &buffer)) == EOF || charsRead < 1) { return FALSE; }
    if(strcmp(buffer, SAVELOAD_VERSION_CODE) != 0) { LOGW("SAVELOAD VERSION CODE wrong"); return FALSE; }

    // Get the max specials at the time of this file
    if((charsRead = fscanf(fp, "%d\n\n", &fileMaxSpecials)) == EOF || charsRead < 1) {return FALSE;}

    // Get the dimensions
    if((charsRead = fscanf(fp, "%d %d\n\n", &sizeX, &sizeY)) == EOF || charsRead < 2) {return FALSE;}

    // Mostly for bookkeeping, but set this anyway
    saveProto->set_size_x(sizeX);
    saveProto->set_size_y(sizeY);

    while(!feof(fp))
    {
        // Scan in the initial character
        // If there is nothing, we're done
        if((charsRead = fscanf(fp, "%c", &lookAhead)) == EOF || charsRead < 1) {return TRUE;}

        // Not a particle. Either empty or something we don't know about.
        if (lookAhead != 'B' && lookAhead != 'C')
        {
            continue;
        }

        Particle* part = saveProto->add_particle();

        // Basic particle
        if(lookAhead == 'B')
        {
            float x, y;
            int xVel, yVel, elementIndex;
            char heat;
            if((charsRead = fscanf(fp, "(%f %f %d %d %c %d)",
                                   &x, &y, &xVel, &yVel, &heat, &elementIndex)) == EOF
               || charsRead < 6) {continue;}
            part->set_x(x);
            part->set_y(y);
            part->set_x_vel(xVel);
            part->set_y_vel(yVel);
            part->set_heat(heat);
            part->set_element_type(Particle::BASIC);
            part->set_element_index(elementIndex);
        }
        // Custom particle
        else if(lookAhead == 'C')
        {
            float x, y;
            int xVel, yVel;
            char heat;
            unsigned long elementHash;
            if((charsRead = fscanf(fp, "(%f %f %d %d %c %lu)",
                                   &x, &y, &xVel, &yVel, &heat, &elementHash)) == EOF
               || charsRead < 6) {continue;}
            part->set_x(x);
            part->set_y(y);
            part->set_x_vel(xVel);
            part->set_y_vel(yVel);
            part->set_heat(heat);

            // Find custom with that hash, and set the index
            if (elementUpgradeMap.find(elementHash) != elementUpgradeMap.end())
            {
                part->set_element_type(Particle::CUSTOM);
                part->set_element_filename(elementUpgradeMap[elementHash]);
            }
            else
            {
                LOGW("Custom element not found: %ul", elementHash);
                // Fail to a default of sand
                part->set_element_type(Particle::BASIC);
                part->set_element_index(SAND_ELEMENT);
            }
        }

        // Special vals
        // Check that our lookahead is correct
        if((charsRead = fscanf(fp, "%c", &lookAhead)) != EOF && charsRead == 1 && lookAhead == '[')
        {
            for (int k = 0; k < fileMaxSpecials; k++)
            {
                if((charsRead = fscanf(fp, "%d ", &tempSpecialVal)) == EOF || charsRead < 1)
                {
                    // Failed, skip particle
                    break;
                }

                if (k < MAX_SPECIALS)
                {
                    part->add_special()->set_val(tempSpecialVal);
                }
            }

            // Skip past the closing ']'
            fseek(fp, 1, SEEK_CUR);
        }
        else
        {
            // Put the lookAhead char back on the stream
            fseek(fp, -1, SEEK_CUR);
        }
    }

    return TRUE;
}

char upgradeCustomElementV1(FILE* fp, CustomElement* customProto,
                            const string& newFilename)
{
    if (fp == NULL)
    {
        return FALSE;
    }

    // Read in basic properties to the proto
    char name[MAX_CE_NAME_LENGTH];
    if(fgets(name, MAX_CE_NAME_LENGTH, fp) == NULL) {return FALSE;}
    customProto->set_name(name);
    int tempInt;
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_base_element_index(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_state(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_starting_temp(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_lowest_temp(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_highest_temp(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_lower_element_index(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_higher_element_index(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_red(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_green(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_blue(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_density(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_fallvel(tempInt);
    if(fscanf(fp, "%d", &tempInt) == EOF) {return FALSE;}
    customProto->set_inertia(tempInt);

    // Defaults for backwards compatibility
    customProto->set_allow_moving_transition(false);
    customProto->set_filename(newFilename);
    // Upgrade map
    // Hash all possible indices
    for (int i = NUM_BASE_ELEMENTS; i < 256; ++i)
    {
        elementUpgradeMap[hashElement(*customProto, i)] = newFilename;
    }

    // Read in the collisions header, with a reasonable fallback
    int numCollisionsToRead;
    int read;
    fseek(fp, 1, SEEK_CUR); // Skip the newline
    if ((read = fscanf(fp, "C%d", &numCollisionsToRead)) == EOF || read < 1)
    {
        numCollisionsToRead = NUM_BASE_ELEMENTS - NORMAL_ELEMENT;
    }
    // Read those many collisions from the file
    for (int i = 0; i < numCollisionsToRead; i++)
    {
        Collision* coll = customProto->add_collision();
        if(fscanf(fp, "%d", &tempInt) != EOF)
        {
            coll->set_type(tempInt);
        }
        else
        {
            // Default: solid-solid
            coll->set_type(0);
        }
    }
    // Read MAX_SPECIALS specials from the file
    for (int i = 0; i < MAX_SPECIALS; i++)
    {
        Special* special = customProto->add_special();
        if(fscanf(fp, "%d", &tempInt) != EOF)
        {
            special->set_type(tempInt);
        }
        else
        {
            // Default: SPECIAL_NONE
            special->set_type(SPECIAL_NONE);
        }
        if(fscanf(fp, "%d", &tempInt) != EOF)
        {
            special->set_val(tempInt);
        }
        else
        {
            // Default: SPECIAL_VAL_UNSET
            special->set_val(SPECIAL_VAL_UNSET);
        }
    }

    fclose(fp);

    return TRUE;
}

// WARNING: Do NOT change these hash functions to be up-to-date with new element
// properties. At some point we'll phase out backwards support, but for now these
// are needed for upgrading old save files.

// Hash an element (usually a custom element), for the purposes of identifying later
unsigned long hashElement(const CustomElement& customProto, int index)
{
    //Stringify the element
    char* stringified = stringifyElement(customProto, index);
    unsigned long hash = hashStr(stringified);
    LOGI("Hashing %s[%d]: [%ul] %s",
            customProto.name().c_str(), index, hash, stringified);
    free(stringified);
    return hash;
}
char* stringifyElement(const CustomElement& customProto, int index)
{
    LOGI("stringifyElement");
    char* buffer = (char*)malloc(sizeof(char)*1024);
    LOGI("buffer alloc'd");
    int offset = 0;
    offset += sprintf(&buffer[offset], "%d%s",
            index, customProto.name().c_str());
    offset += sprintf(&buffer[offset], "%c%c%c%c%d%d",
                      (char)customProto.state(),
                      (char)customProto.starting_temp(),
                      (char)customProto.lowest_temp(),
                      (char)customProto.highest_temp(),
                      customProto.lower_element_index(),
                      customProto.higher_element_index());
    offset += sprintf(&buffer[offset], "%c%c%c",
                      (char)customProto.red(),
                      (char)customProto.blue(),
                      (char)customProto.green());
    int i;
    for (i = 0; i < MAX_SPECIALS; ++i)
    {
        int special, specialVal;
        if (i < customProto.special_size())
        {
            special = customProto.special(i).type();
            specialVal = customProto.special(i).val();
        }
        else
        {
            special = SPECIAL_NONE;
            specialVal = SPECIAL_VAL_UNSET;
        }
        offset += sprintf(&buffer[offset], "%d%d",
                special, specialVal);
    }
    for (i = 0; i < NUM_BASE_ELEMENTS; ++i)
    {
        int collision;
        if (i < customProto.collision_size())
        {
            collision = customProto.collision(i).type();
        }
        else
        {
            collision = 0;
        }
        offset += sprintf(&buffer[offset], "%d", collision);
    }
    offset += sprintf(&buffer[offset], "%c%c%c%c",
                      (char)customProto.base_element_index(),
                      (char)customProto.density(),
                      (char)customProto.fallvel(),
                      (char)customProto.inertia());
    buffer[offset] = 0;
    return buffer;
}
// djb2 hash for strings
// See: http://www.cse.yorku.ca/~oz/hash.html
unsigned long hashStr(char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
