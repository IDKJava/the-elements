/*
 * saveload.c
 * --------------------------
 * Defines functions relating to saving
 * and loading from files.
 */

#include "saveload.h"

char saveState(int type)
{
	//Strings for file location
	char timestamp[64], saveLoc[256];

	FILE *fp;
	if (type == NORMAL_SAVE)
	{
		//Generate the filename based on date and time
		strftime(timestamp, 255, "%Y-%m-%d-%H:%M", localtime(time(NULL)));
		strcpy(saveLoc, SAVES_FOLDER);
		strcat(saveLoc, timestamp);
		strcat(saveLoc, SAVE_EXTENSION);
	}
	else if (type == TEMP_SAVE)
	{
		//Generate the temp save filename
		strcpy(saveLoc, SAVES_FOLDER);
		strcat(saveLoc, TEMP_SAVE_FILE);
		strcat(saveLoc, SAVE_EXTENSION);
	}
	fp = fopen(saveLoc, "w");

	if (fp != NULL)
	{
		int counter;
		Particle* tempParticle;
		Element* needsToBeSaved[numElements - NUM_BASE_ELEMENTS];
		
		//Save the particles
		for (counter = 0; counter < MAX_POINTS; counter++)
		{
			tempParticle = partices[counter];
			if (tempParticle->set)
			{
				//Save x, y, and element of each set particle
				fprintf(fp, "%d %d %d\n",
					tempParticle->x,
					tempParticle->y,
					tempParticle->element->index);
				
				if(tempParticle->element->index >= NUM_BASE_ELEMENTS)
				{
					needsToBeSaved[tempParticle->element->index - NUM_BASE_ELEMENTS] = TRUE;
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

char loadState(int type, char* filename)
{
	FILE *fp;
	char loadLoc[256];
	//Set up the filename
	if (type == NORMAL_LOAD)
	{
		if(sizeof(filename)/sizeof(char) > 256)
		{
			return FALSE;
		}
		
		strcpy(loadLoc, filename);
	}
	else if (type == QUICK_LOAD)
	{
		strcpy(loadLoc, SAVES_FOLDER);
		strcat(loadLoc, TEMP_SAVE_FILE);
		strcat(loadLoc, SAVE_EXTENSION);
	}
	else if (type == DEMO_LOAD)
	{
		strcpy(loadLoc, SAVES_FOLDER);
		strcat(loadLoc, DEMO_SAVE_FILE);
		strcat(loadLoc, SAVE_EXTENSION);
	}
	//Load the file for reading
	fp = fopen(loadLoc, "r");

	//Clear the screen and set up some temp variables
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
			CreatePoint(xCoord, yCoord, element);
		}

		fclose(fp);
		return TRUE;
	}

	return FALSE;
}

void removeTempSave(void)
{
	char saveLoc[256];
	//Generate the temp save filename
	strcpy(saveLoc, SAVES_FOLDER);
	strcat(saveLoc, TEMP_SAVE_FILE);
	strcat(saveLoc, SAVE_EXTENSION);
	//Remove the file at that location
	remove(saveLoc);
}

//TODO:
void saveCustomElement(struct Element* createdCustomElement) {}
void loadCustomElements(void) {}
void saveAtmosphere(struct Atmosphere* createAtmosphere) {}
void loadAtmosphere(void) {}
int getCustomElementsNumber(void) {}
