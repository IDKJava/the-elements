/*
 * saveload2.cpp
 * --------------------------
 * Defines functions relating to saving and loading custom elements
 * and states in files.
 */

#include "saveload2.h"

#include "app.h"
#include "macros.h"
#include "messages.pb.h"
#include "saveload.h"

#include <android/log.h>
#include <pthread.h>

// Per-file logging
#ifndef NDEBUG
//Debug
#define LOGGING 1
#else
//Release
#define LOGGING 1
#endif

char saveState2(char* saveLoc)
{
    // Lock the mutex so that we don't continue to update
    pthread_mutex_lock(&update_mutex);

    bool retVal = false;
    ofstream out(saveLoc, ios::out | ios::binary | ios::trunc);
    if (!out.fail())
    {
        retVal = saveStateLogic2(out);
        out.close();
    }
    else
    {
        LOGE("Save loc did not open properly");
    }

    // Unlock the mutex before quitting
    pthread_mutex_unlock(&update_mutex);

    return retVal;
}

char loadState2(char* loadLoc)
{
    LOGI("loadState2");
    // Hold the update mutex to prevent mid-load updates
    pthread_mutex_lock(&update_mutex);

    bool retVal = false;
    ifstream in(loadLoc, ios::in | ios::binary);
    if (!in.fail())
    {
        retVal = loadStateLogic2(in);
        in.close();
    }
    else
    {
        LOGE("Load loc did not open properly.");
    }

    pthread_mutex_unlock(&update_mutex);

    return retVal;
}

char loadCustomElements2(void)
{
    string loadLoc = string(ROOT_FOLDER) + string(ELEMENTS_FOLDER);
    LOGI("Loading elements from %s", loadLoc.c_str());
    DIR* elementsDir = opendir(loadLoc.c_str());
    if (!elementsDir)
    {
        LOGW("No elements directory found.");
        return FALSE;
    }

    struct dirent* entry;
    while ((entry = readdir(elementsDir)))
    {
        string filename (entry->d_name);
        string ext = filename.substr(filename.find_last_of(".")+1);
        if (ext != string(ELEMENT_EXTENSION))
        {
            LOGW("Skipping non-element file: %s", filename.c_str());
            continue;
        }

        ifstream in(filename.c_str(), ios::in | ios::binary);
        if (in.fail())
        {
            LOGE("Reading element file %s failed.", filename.c_str());
            continue;
        }

        if (!loadCustomElement2(in))
        {
            LOGE("Loading custom element from file %s failed.", filename.c_str());
        }
        else
        {
            LOGI("Loading custom element from file %s succeeded.", filename.c_str());
        }
    }

    LOGI("Done reading element directory");

    closedir(elementsDir);

    return TRUE;
}

bool saveStateLogic2(ofstream& out)
{
    LOGE("Start save state logic");
    SaveFile save;
    save.set_size_x(workWidth);
    save.set_size_y(workHeight);
    LOGE("Save dims: %d %d", workWidth, workHeight);
    for (int i = 0; i < MAX_POINTS; ++i)
    {
        if (!a_set[i]) continue;

        Particle* particle = save.add_particle();
        particle->set_x(a_x[i]);
        particle->set_y(a_y[i]);
        particle->set_x_vel(a_xVel[i]);
        particle->set_y_vel(a_yVel[i]);
        particle->set_heat(a_heat[i]);
        // Special vals
        for (int j = 0; j < MAX_SPECIALS; ++j) {
            ParticleSpecial* special = particle->add_special();
            special->set_val(a_specialVals[i][j]);
        }
        // Base element
        if (a_element[i]->index < NUM_BASE_ELEMENTS)
        {
            particle->set_element_type(Particle::BASIC);
            particle->set_element_index(a_element[i]->index);
        }
        // Custom element
        else
        {
            particle->set_element_type(Particle::CUSTOM);
            particle->set_element_hash(hashElement2(a_element[i]));
        }
    }

    // Write the file itself
    save.SerializeToOstream(&out);

    LOGE("End save state logic");
}

bool loadStateLogic2(ifstream& in)
{
    LOGI("Load state logic 2!");
    SaveFile save;
    if(!save.ParseFromIstream(&in))
    {
        LOGE("Parsing input file failed: corrupted!");
        return false;
    }

    LOGI("Found input file with dimensions: %d %d", save.size_x(), save.size_y());

    for (int i = 0; i < save.particle_size(); ++i)
    {
        const Particle& particle = save.particle(i);
        // Check the buf
        if (!particle.has_x() || !particle.has_y())
        {
            LOGW("Corrupt particle found. Doesn't have both x and y coords. Skipping.");
            continue;
        }

        // Check for out of particles
        // End the input immediately
        if (loq <= 0) return true;
        int tempParticle = avail[loq-1];

        // Unload the protobuf

        a_x[tempParticle] = particle.x();
        a_y[tempParticle] = particle.y();
        // Integer coords for allcoords and bitmap color
        int i = (int)a_x[tempParticle];
        int j = (int)a_y[tempParticle];
        // Check bounds
        if (i < 0 || i >= workWidth || j < 0 || j >= workHeight)
        {
            LOGE("Out of bound particle found. Skipping");
            continue;
        }
        // Check that coords are not already taken
        if (allCoords[getIndex(i, j)] != -1)
        {
            LOGE("Particle space already occupied. Skipping");
            continue;
        }

        // Velocities have reasonable defaults (0), so no need to check
        a_xVel[tempParticle] = particle.x_vel();
        a_yVel[tempParticle] = particle.y_vel();
        // Make sure heat has a reasonable default
        if (particle.has_heat())
        {
            a_heat[tempParticle] = particle.heat();
        }
        else if (cAtmosphere != NULL)
        {
            a_heat[tempParticle] = cAtmosphere->heat;
        }
        else
        {
            a_heat[tempParticle] = 0;
        }
        // Element
        if (particle.element_type() == Particle::BASIC)
        {
            a_element[tempParticle] = particle.has_element_index() ?
                    elements[particle.element_index()] : elements[SAND_ELEMENT];
        }
        else if (particle.element_type() == Particle::CUSTOM)
        {
            int elementIndex = findElementFromHash2(particle.element_hash());
            a_element[tempParticle] = elementIndex >= 0 ?
                    elements[elementIndex] : elements[SAND_ELEMENT];
        }
        else
        {
            LOGE("Unrecognized element type on particle. Assuming sand.");
            a_element[tempParticle] = elements[SAND_ELEMENT];
        }

        // Specials
        for (int j = 0; j < particle.special_size(); ++j)
        {
            const ParticleSpecial& special = particle.special(j);
            if (j >= MAX_SPECIALS) break;

            a_specialVals[tempParticle][j] =
                    special.has_val() ? special.val() : SPECIAL_VAL_UNSET;
        }

        // Set the particle as enabled
        a_set[tempParticle] = TRUE;

        // Claim it's id
        --loq;

        // Set the allCoords and bitmap color
        allCoords[getIndex(i, j)] = tempParticle;
        setBitmapColor(i, j, a_element[tempParticle]);
    }

    return true;
}

bool loadCustomElement2(ifstream& in)
{
    CustomElement customProto;
    if (!customProto.ParseFromIstream(&in))
    {
        LOGE("Parsing custom element failed: input file corrupted");
        return false;
    }

    struct Element* custom = (struct Element*) malloc(sizeof(struct Element));
    custom->index = numElements;
    custom->name = (char*) malloc(customProto.name().length() * sizeof(char));
    custom->allowMovingTransition = customProto.allow_moving_transition();
    custom->state = customProto.state();
    custom->startingTemp = customProto.starting_temp();
    custom->lowestTemp = customProto.lowest_temp();
    custom->highestTemp = customProto.highest_temp();
    // Lower element and higher element get validated
    // using sand as the default if not read properly
    if (customProto.has_lower_element_index())
    {
        int lowerElementIndex = customProto.lower_element_index();
        if (lowerElementIndex >= 0 && lowerElementIndex < numElements)
        {
            custom->lowerElement = elements[lowerElementIndex];
        }
        else
        {
            custom->lowerElement = elements[SAND_ELEMENT];
        }
    }
    else
    {
        custom->lowerElement = elements[SAND_ELEMENT];
    }
    if (customProto.has_higher_element_index())
    {
        int higherElementIndex = customProto.higher_element_index();
        if (higherElementIndex >= 0 && higherElementIndex < numElements)
        {
            custom->higherElement = elements[higherElementIndex];
        }
        else
        {
            custom->higherElement = elements[SAND_ELEMENT];
        }
    }
    else
    {
        custom->higherElement = elements[SAND_ELEMENT];
    }

    custom->red = customProto.red();
    custom->green = customProto.green();
    custom->blue = customProto.blue();
    custom->density = customProto.density();
    custom->fallVel = customProto.fallvel();
    custom->inertia = customProto.inertia();

    // Specials
    for (int i = 0; i < MAX_SPECIALS; ++i)
    {
        if (i < customProto.special_size())
        {
            custom->specials[i] = customProto.special(i).type();
            custom->specialVals[i] = customProto.special(i).val();
        }
        else
        {
            custom->specials[i] = SPECIAL_NONE;
            custom->specialVals[i] = SPECIAL_VAL_UNSET;
        }
    }

    // Collisions, default is solid-solid
    for (int i = 0; i < NUM_BASE_ELEMENTS; ++i)
    {
        if (i < customProto.collision_size())
        {
            int collision = customProto.collision(i).type();
            if (collision >= 0 && collision < NUM_COLLISIONS)
            {
                custom->collisions[i] = collision;
            }
            else
            {
                custom->collisions[i] = 0;
            }
        }
        else
        {
            // Default is solid-solid
            custom->collisions[i] = 0;
        }
    }

    elements = (struct Element**) realloc(elements, numElements+1);
    elements[numElements] = custom;
    numElements++;

    LOGI("Loaded custom element %s, index: %d", custom->name, custom->index);

    return true;
}

// Hash an element (usually a custom element), for the purposes of identifying later
unsigned long hashElement2(struct Element* element)
{
    //Stringify the element
    char* stringified = stringifyElement2(element);
    unsigned long hash = hashStr2(stringified);
    free(stringified);
    return hash;
}

char* stringifyElement2(struct Element* element)
{
    char* buffer = (char*)malloc(sizeof(char)*1024);
    int offset = 0;
    offset += sprintf(&buffer[offset], "%d%s", element->index, element->name);
    offset += sprintf(&buffer[offset], "%c%c%c%c%d%d", element->state,
                      element->startingTemp,
                      element->lowestTemp,
                      element->highestTemp,
                      element->lowerElement->index,
                      element->higherElement->index);
    offset += sprintf(&buffer[offset], "%c%c%c", element->red,
                      element->blue,
                      element->green);
    int i;
    for (i = 0; i < MAX_SPECIALS; ++i)
    {
        offset += sprintf(&buffer[offset], "%d%d", element->specials[i], element->specialVals[i]);
    }
    for (i = 0; i < NUM_BASE_ELEMENTS; ++i)
    {
        offset += sprintf(&buffer[offset], "%d", element->collisions[i]);
    }
    offset += sprintf(&buffer[offset], "%c%c%c%c", element->base,
                      element->density,
                      element->fallVel,
                      element->inertia);
    buffer[offset] = 0;
    return buffer;
}

// djb2 hash for strings
// See: http://www.cse.yorku.ca/~oz/hash.html
unsigned long hashStr2(char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash;
}

// Find an element based on the hash
// For now, do it stupidly: iterate through customs and hash them,
// looking for a match. If not found, return -1.
int findElementFromHash2(unsigned long hash)
{
    int i;
    unsigned long tempHash;
    for (i = NUM_BASE_ELEMENTS; i < numElements; ++i)
    {
        tempHash = hashElement2(elements[i]);
        if (tempHash == hash)
        {
            return i;
        }
    }
    return -1;
}
