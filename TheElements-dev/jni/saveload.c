/*
 * saveload.c
 * --------------------------
 * Defines functions relating to saving
 * and loading from files.
 */

#include "saveload.h"
#include <android/log.h>

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot;
}
char saveState(char* saveLoc)
{
    // Lock the mutex so that we don't continue to update
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "Starting save");
    pthread_mutex_lock(&update_mutex);
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "Got mutex");

    FILE* fp = fopen(saveLoc, "w");
    char retVal = saveStateLogic(fp);
    if (fp)
    {
        fclose(fp);
    }

    // Unlock the mutex before quitting
    pthread_mutex_unlock(&update_mutex);
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "Finished save");

    return retVal;
}
char saveStateLogic(FILE* fp)
{
    if (fp != NULL)
    {
        int i, counterX, counterY;
        struct Particle* tempParticle;

        //First write the version code for this saveload version
        fprintf(fp, "%s\n\n", SAVELOAD_VERSION_CODE);

        //Save the max specials (for backward compatibility if needed)
        fprintf(fp, "%d\n\n", MAX_SPECIALS);

        //Save the dimensions
        fprintf(fp, "%d %d\n\n", workWidth, workHeight);

        //Save the particles
        /* Save format:
         * (x y xVel yVel heat element->index) ...
         *   .
         *   .
         *   .
         */
        for (counterY = 0; counterY < workHeight; counterY++)
        {
            for (counterX = 0; counterX < workWidth; counterX++)
            {
                tempParticle = allCoords[getIndex(counterX, counterY)];
                if(tempParticle)
                {
                	// Normal element
                	if(tempParticle->element->index < NUM_BASE_ELEMENTS)
                	{
						fprintf(fp, "B(%f %f %d %d %d %d)",
								tempParticle->x,
								tempParticle->y,
								tempParticle->xVel,
								tempParticle->yVel,
								tempParticle->heat,
								tempParticle->element->index);
                	}
                	// Custom element
                	else
                	{
						fprintf(fp, "C(%f %f %d %d %d %lu)",
								tempParticle->x,
								tempParticle->y,
								tempParticle->xVel,
								tempParticle->yVel,
								tempParticle->heat,
								hashElement(tempParticle->element));
                	}

					fprintf(fp, "[");
					for (i = 0; i < MAX_SPECIALS; i++)
					{
						fprintf(fp, "%d ", tempParticle->specialVals[i]);
					}
					fprintf(fp, "]");
                }
                else
                {
                    // E -- empty location
                    fprintf(fp, "E");
                }
            }
            fprintf(fp, "\n");
        }

        fclose(fp);
        return TRUE;
    }
    if(fp)
    {
        fclose(fp);
    }
    return FALSE;
}

char loadState(char* loadLoc)
{
    //Lock the mutex before loading so that we don't update frames while loading
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "Starting loadstate");
    pthread_mutex_lock(&update_mutex);
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "Got mutex lock");

    FILE* fp = fopen(loadLoc, "r");
    char retVal = loadStateLogic(fp);
    play = FALSE;
    if (fp)
    {
        fclose(fp);
    }

    //Unlock the mutex before quitting
    pthread_mutex_unlock(&update_mutex);
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "Finishing loadstate");

    return retVal;
}
char loadStateLogic(FILE* fp)
{
    //Clear the screen and set up some temp variables
    arraySetup();
    elementSetup();
    gameSetup();

    int i, j, elementIndex, elementHash,
        sizeX, sizeY, fileMaxSpecials, tempSpecialVal, charsRead, failed;
    struct Element* tempElement;
    struct Particle* tempParticle;
    char buffer[64];
    char lookAhead;

    if(fp != NULL)
    {
        // Get the saveload version code
        if((charsRead = fscanf(fp, "%s\n\n", &buffer)) == EOF || charsRead < 1) { return FALSE; }
        if(strcmp(buffer, SAVELOAD_VERSION_CODE) != 0) { return FALSE; }

        // Get the max specials at the time of this file
        if((charsRead = fscanf(fp, "%d\n\n", &fileMaxSpecials)) == EOF || charsRead < 1) {return FALSE;}

        // Get the dimensions
        if((charsRead = fscanf(fp, "%d %d\n\n", &sizeX, &sizeY)) == EOF || charsRead < 2) {return FALSE;}


        //Make sure saves are portable from different dimensions
        if(sizeX > workWidth)
        {
            sizeX = workWidth;
        }
        if(sizeY > workHeight)
        {
            sizeY = workHeight;
        }


        while(!feof(fp))
        {
        	failed = FALSE;

            // Check for all particles used
            if(loq <= 0) {return TRUE;}

            // Scan in the initial character
            if((charsRead = fscanf(fp, "%c", &lookAhead)) == EOF || charsRead < 1) {return FALSE;}


            // Basic particle
            if(lookAhead == 'B')
            {
				// Try to read in a particle
				tempParticle = avail[loq-1];
				if((charsRead = fscanf(fp, "(%f %f %d %d %d %d)", &tempParticle->x,
									   &tempParticle->y,
									   &tempParticle->xVel,
									   &tempParticle->yVel,
									   &tempParticle->heat,
									   &elementIndex)) == EOF
				   || charsRead < 6) {continue;}
				tempParticle->element = elements[elementIndex];
            }
            // Custom particle
            else if(lookAhead == 'C')
            {
				// Try to read in a particle
				tempParticle = avail[loq-1];
				if((charsRead = fscanf(fp, "(%f %f %d %d %d %lu)", &tempParticle->x,
									   &tempParticle->y,
									   &tempParticle->xVel,
									   &tempParticle->yVel,
									   &tempParticle->heat,
									   &elementHash)) == EOF
				   || charsRead < 6) {continue;}

				// Find custom with that hash, and set the index
				elementIndex = findElementFromHash(elementHash);
				if (elementIndex)
				{
					tempParticle->element = elements[elementIndex];
				}
				else
				{
					// Don't fail now, we need to finish reading in all specials, etc. so that
					// we don't corrupt the stream. Instead, finish reading in, then don't actually
					// allocate the element.
					failed = TRUE;
				}
            }
            // Empty location
            else if(lookAhead == 'E')
            {
            	continue;
            }
            else
            {
            	// Ignore whitespace and other characters
            	continue;
            }

            // Save the integral coordinates for writing to allCoords
            i = (int)tempParticle->x;
            j = (int)tempParticle->y;
            if (i >= sizeX || j >= sizeY) { return FALSE; }

            // Check that our lookahead is correct
            if((charsRead = fscanf(fp, "%c", &lookAhead)) != EOF && charsRead == 1 && lookAhead == '[')
            {
                int k;
                for (k = 0; k < fileMaxSpecials; k++)
                {
                    if((charsRead = fscanf(fp, "%d ", &tempSpecialVal)) == EOF || charsRead < 1)
                    {
                        failed = TRUE;
                        break;
                    }

                    if (k < MAX_SPECIALS)
                    {
                        tempParticle->specialVals[k] = tempSpecialVal;
                    }
                }

                // Skip past the closing ']'
                fseek(fp, 1, SEEK_CUR);
            }
            else
            {
                // Put the lookAhead char back on the stream
                fseek(fp, -1, SEEK_CUR);
                failed = TRUE;
            }
            // If reading in failed, initialize to cleared
            if (failed)
            {
                clearSpecialVals(tempParticle);
            }

            // Only finish creating the particle if we succeed
            if (!failed)
            {
            	// Remove the particle from avail
            	loq--;

				// Make the particle set
				tempParticle->set = TRUE;

				// Set the allCoords and bitmap color
				allCoords[getIndex(i, j)] = tempParticle;
				setBitmapColor(tempParticle->x, tempParticle->y, tempParticle->element);
            }
        }

        fclose(fp);
        return TRUE;
    }
    return FALSE;
}

char saveTempToFile(char* saveLoc)
{
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "saveTempToFile");
    FILE* saveFile = fopen(saveLoc, "wb");
    if (saveFile == NULL)
    {
        return FALSE;
    }
    char tempLoc[256];
    strcpy(tempLoc, ROOT_FOLDER);
    strcat(tempLoc, SAVES_FOLDER);
    strcat(tempLoc, TEMP_SAVE);
    strcat(tempLoc, SAVE_EXTENSION);
    FILE* tempFile = fopen(tempLoc, "rb");
    if (tempFile == NULL)
    {
        return FALSE;
    }
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "Opened the files");

    char temp;

    while(!feof(tempFile))
    {
        temp = fgetc(tempFile);
        if(ferror(tempFile))
        {
            __android_log_write(ANDROID_LOG_ERROR, "TheElements", "Error reading from temp.sav file");
            clearerr(tempFile);
            return FALSE;
        }
        else if (!feof(tempFile))
        {
            fputc(temp, saveFile);
            if(ferror(saveFile))
            {
                __android_log_write(ANDROID_LOG_ERROR, "TheElements", "Error writing to save file");
                clearerr(saveFile);
                return FALSE;
            }
        }
    }

    fclose(saveFile);
    fclose(tempFile);

    return TRUE;
}

char loadFileToTemp(char* loadLoc)
{
    FILE* loadFile = fopen(loadLoc, "rb");
    if (loadFile == NULL)
    {
        return FALSE;
    }
    char tempLoc[256];
    strcpy(tempLoc, ROOT_FOLDER);
    strcat(tempLoc, SAVES_FOLDER);
    strcat(tempLoc, TEMP_SAVE);
    strcat(tempLoc, SAVE_EXTENSION);
    FILE* tempFile = fopen(tempLoc, "wb");
    if (tempFile == NULL)
    {
        return FALSE;
    }

    char temp;

    while(!feof(loadFile))
    {
        temp = fgetc(loadFile);
        if(ferror(loadFile))
        {
            __android_log_write(ANDROID_LOG_ERROR, "TheElements", "Error reading from save file");
            clearerr(loadFile);
            break;
        }
        else if (!feof(loadFile))
        {
            fputc(temp, tempFile);
            if(ferror(tempFile))
            {
                __android_log_write(ANDROID_LOG_ERROR, "TheElements", "Error writing to temp.sav file");
                clearerr(tempFile);
                break;
            }
        }
    }

    fclose(loadFile);
    fclose(tempFile);

    return TRUE;
}

char removeTempSave(void)
{
    char saveLoc[256];
    //Generate the temp save filename
    strcpy(saveLoc, ROOT_FOLDER);
    strcat(saveLoc, SAVES_FOLDER);
    strcat(saveLoc, TEMP_SAVE);
    strcat(saveLoc, SAVE_EXTENSION);

    //Remove the file at that location
    if(remove(saveLoc) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

char loadCustomElements(void)
{
    char loadLoc[256];
    char saveLoc[256];
    //Load the file names of everything in the loading directory
    strcpy(loadLoc, ROOT_FOLDER);
    strcpy(saveLoc, ROOT_FOLDER);
    strcat(loadLoc, ELEMENTS_FOLDER);
    strcat(saveLoc, ELEMENTS_FOLDER);
    strcat(saveLoc, LIST_SAVE);

    strcat(saveLoc, LIST_EXTENSION);

    FILE* sp = fopen(saveLoc, "wb");
    if (sp == NULL)
    {
        __android_log_write(ANDROID_LOG_ERROR, "LOG", "loadCustomElements: Unable to open file");
        return FALSE;
    }

    struct stat sb;
    if (!stat(loadLoc, &sb) == 0 || !S_ISDIR(sb.st_mode))
    {
        __android_log_write(ANDROID_LOG_ERROR, "LOG", "loadCustomElements: Directory doesn't exist!");
        return FALSE;
    }
    DIR* mydir = opendir(loadLoc);
    if (!mydir)
    {
        __android_log_write(ANDROID_LOG_ERROR, "LOG", "loadCustomElements: Null directory! Quitting.");
        __android_log_write(ANDROID_LOG_ERROR, "LOG", strerror(errno));
        return FALSE;
    }

    struct dirent *entry = NULL;

    while((entry = readdir(mydir))) /* If we get EOF, the expression is 0 and the loop stops. */
    {
        if (!strcmp(get_filename_ext(entry->d_name), ELEMENT_EXTENSION))
        {
            char location[256];
            strcpy(location, loadLoc);
            strcat(location, entry->d_name);
            loadCustomElement(location);
            fprintf(sp, entry->d_name);
            fprintf(sp, "\n");
        }
    }

    closedir(mydir);

    fclose(sp);
    return TRUE;
}
char loadCustomElement(char* loadLoc)
{

    // Try opening the custom element file
    FILE* fp = fopen(loadLoc, "r");
    if (fp == NULL)
    {
        return FALSE;
    }

    int lowerElementIndex, higherElementIndex;

    // Create the struct and read in basic properties
    struct Element* tempCustom = (struct Element*) malloc(sizeof(struct Element));
    tempCustom->name = malloc(MAX_CE_NAME_LENGTH * sizeof(char));
    if(fgets(tempCustom->name, MAX_CE_NAME_LENGTH, fp) == NULL) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->base) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->state) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->startingTemp) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->lowestTemp) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->highestTemp) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &lowerElementIndex) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &higherElementIndex) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->red) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->green) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->blue) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->density) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->fallVel) == EOF) {return FALSE;}
    if(fscanf(fp, "%d", &tempCustom->inertia) == EOF) {return FALSE;}

    // Allocate collisions and specials related arrays
    tempCustom->collisions = malloc(NUM_BASE_ELEMENTS * sizeof(char));
    tempCustom->specials = malloc(MAX_SPECIALS*sizeof(char));
    tempCustom->specialVals = malloc(MAX_SPECIALS*sizeof(char));

    // Read in the collisions header, with a reasonable fallback
    int numCollisionsToRead;
    int read;
    fseek(fp, 1, SEEK_CUR); // Skip the newline
    if ((read = fscanf(fp, "C%d", &numCollisionsToRead)) == EOF || read < 1)
    {
        numCollisionsToRead = NUM_BASE_ELEMENTS - NORMAL_ELEMENT;
    }
    // Read those many collisions from the file
    int i;
    for (i = 0; i < numCollisionsToRead + NORMAL_ELEMENT; i++)
    {
        if (i < NORMAL_ELEMENT)
        {
            tempCustom->collisions[i] = 0;
        }
        else if(fscanf(fp,"%d",&tempCustom->collisions[i]) == EOF)
        {
            tempCustom->collisions[i] = 0;
        }
    }
    // Read MAX_SPECIALS specials from the file
    for (i = 0; i < MAX_SPECIALS; i++)
    {
        if(fscanf(fp,"%d",&tempCustom->specials[i]) == EOF)
        {
            tempCustom->specials[i] = SPECIAL_NONE;
        }
        if(fscanf(fp,"%d",&tempCustom->specialVals[i]) == EOF)
        {
            tempCustom->specialVals[i] = 0;
        }
    }

    // Effectively a realloc
    // TODO: Replace with realloc
    struct Element** tempElementArray;
    tempElementArray = malloc(numElements*sizeof( struct Element*));
    for (i = 0; i < numElements; i++)
    {
        tempElementArray[i] = elements[i];
    }
    free(elements);
    elements = malloc((numElements+1)*sizeof( struct Element*));
    for (i = 0; i < numElements; i++)
    {
        elements[i] = tempElementArray[i];
    }

    // Resolve index-based values
    tempCustom->index = numElements;
    tempCustom->higherElement = elements[higherElementIndex];
    tempCustom->lowerElement = elements[lowerElementIndex];

    // Increment number of Elements and add the newest element in
    numElements++;
    elements[numElements-1] = tempCustom;

    // REMOVE THIS
    char buffer[256];
    sprintf(buffer, "Loaded custom element, index: %d, element->index: %d", numElements-1, tempCustom->index);
    __android_log_write(ANDROID_LOG_INFO, "LOG", buffer);

    fclose(fp);

    return TRUE;
}


// Hash an element (usually a custom element), for the purposes of identifying later
unsigned long hashElement(struct Element* element)
{
	//Stringify the element
	char* stringified = stringifyElement(element);
	unsigned long hash = hashStr(stringified);
	free(stringified);
	return hash;
}
char* stringifyElement(struct Element* element)
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
unsigned long hashStr(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
// Find an element based on the hash
// For now, do it stupidly: iterate through customs and hash them,
// looking for a match.
unsigned char findElementFromHash(unsigned long hash)
{
	int i;
	unsigned long tempHash;
	for (i = NUM_BASE_ELEMENTS; i < numElements; ++i)
	{
		tempHash = hashElement(elements[i]);
		if (tempHash == hash)
		{
			return i;
		}
	}
}
