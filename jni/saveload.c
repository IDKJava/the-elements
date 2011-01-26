/*
 * saveload.c
 * --------------------------
 * Defines functions relating to saving
 * and loading from files.
 */

#include "saveload.h"
#include <android/log.h>

char saveState(char* saveLoc)
{
	FILE *fp;
	/*
	//Strings for file location
	char timestamp[64], saveLoc[256];

	time_t rawtime;
	if (type == NORMAL_SAVE)
	{
		//Generate the filename based on date and time
		time(&rawtime);
		strftime(timestamp, 255, "%Y-%m-%d-%H:%M", localtime(&rawtime));
		strcpy(saveLoc, ROOT_FOLDER);
		strcat(saveLoc, SAVES_FOLDER);
		strcat(saveLoc, timestamp);
		strcat(saveLoc, SAVE_EXTENSION);
	}
	else if (type == TEMP_SAVE)
	{
		//Generate the temp save filename
		strcpy(saveLoc, ROOT_FOLDER);
		strcat(saveLoc, SAVES_FOLDER);
		strcat(saveLoc, TEMP_SAVE_FILE);
		strcat(saveLoc, SAVE_EXTENSION);
	}
	*/
	fp = fopen(saveLoc, "w");

	if (fp != NULL)
	{
		int counter;
		struct Particle* tempParticle;
		struct Element* needsToBeSaved[numElements - NUM_BASE_ELEMENTS];
		
		//Save the particles
		for (counter = 0; counter < MAX_POINTS; counter++)
		{
			tempParticle = particles[counter];
			if (tempParticle->set)
			{
				//Save x, y, and element of each set particle
				fprintf(fp, "%d %d %d\n",
					tempParticle->x,
					tempParticle->y,
					tempParticle->element->index);
				
				if(tempParticle->element->index >= NUM_BASE_ELEMENTS)
				{
					//I'm commenting this out to get it to build
					//need to decide we want needsToBeSaved to be a bool(char) or a Element*
					//needsToBeSaved[tempParticle->element->index - NUM_BASE_ELEMENTS] = TRUE;
				}
			}
		}
		
		//Save any custom elements that need to be saved, so that this is portable
		for(counter = 0; counter < numElements - NUM_BASE_ELEMENTS; counter++)
		{
			if(needsToBeSaved[counter])
			{
				//TODO: Save the custom element at the bottom section with an index
			}
		}
		fclose(fp);

		return TRUE;
	}
	else
	{
		//An error occurred, most likely lack of an SDCard
		return FALSE;
	}
}

char loadState(char* loadLoc)
{
	FILE *fp;
	/*
	char loadLoc[256];
	//Set up the filename
	if (type == NORMAL_SAVE)
	{
		if(sizeof(filename)/sizeof(char) > 256)
		{
			return FALSE;
		}
		
		strcpy(loadLoc, filename);
	}
	else if (type == TEMP_SAVE)
	{
		strcpy(loadLoc, ROOT_FOLDER);
		strcat(loadLoc, SAVES_FOLDER);
		strcat(loadLoc, TEMP_SAVE_FILE);
		strcat(loadLoc, SAVE_EXTENSION);
	}
	else if (type == DEMO_SAVE)
	{
		strcpy(loadLoc, ROOT_FOLDER);
		strcat(loadLoc, SAVES_FOLDER);
		strcat(loadLoc, DEMO_SAVE_FILE);
		strcat(loadLoc, SAVE_EXTENSION);
	}
	*/
	//Load the file for reading
	fp = fopen(loadLoc, "r");

	//Clear the screen and set up some temp variables
	arraySetup();
	setup();
	int xCoord;
	int yCoord;
	int element;

	if(fp != NULL)
	{
		while (!feof(fp))
		{
			fscanf(fp, "%d%d%d\n",
				&xCoord,
				&yCoord,
				&element);
			CreatePoint(xCoord, yCoord, elements[element]);
			//Cool thing I found on my way to doing this,
			//you can do something like (struct Element){.index = element}
		}

		fclose(fp);
		return TRUE;
	}

	return FALSE;
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

//TODO:
char saveCustomElement(struct Element* createdCustomElement)
{
	char saveLoc[256];
	strcpy(saveLoc, ROOT_FOLDER);
	strcat(saveLoc, ELEMENTS_FOLDER);
	strcat(saveLoc, createdCustomElement->name);
	strcat(saveLoc, ELEMENT_EXTENSION);

	//TODO: Save to saveLoc

	return FALSE;
}
char loadCustomElement(char* loadLoc)
{
	//TODO: Load from loadLoc (figure out saving of an element first)

	return FALSE;
}
char saveAtmosphere(struct Atmosphere* createdAtmosphere)
{
	char saveLoc[256];
	strcpy(saveLoc, ROOT_FOLDER);
	strcat(saveLoc, ATMOSPHERES_FOLDER);
	strcat(saveLoc, createdAtmosphere->name);

	//TODO: Save to saveLoc

	return FALSE;
}
char loadAtmosphere(char* loadLoc)
{
	//TODO: Load from loadLoc (figure out saving of an atmosphere first)

	return FALSE;
}
