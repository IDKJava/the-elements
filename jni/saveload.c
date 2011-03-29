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
		int i, counterX, counterY;
		struct Particle* tempParticle;
		char* elementSaveFilter;
		elementSaveFilter = (char*)malloc(numElements * sizeof(char));


		for(i = 0; i < numElements; i++)
		{
			elementSaveFilter[i] = 0;
		}

		//Preprocessing loop
		for (counterX = 0; counterX < workWidth; counterX++)
		{
			for (counterY = 0; counterY < workHeight; counterY++)
			{
				tempParticle = allCoords[getIndex(counterX, counterY)];
				if(tempParticle && tempParticle->element->index >= NUM_BASE_ELEMENTS)
				{
					elementSaveFilter[tempParticle->element->index] = 1;
				}
			}
		}
		
		//Save the custom elements that need to be saved
		for (i = 0; i < numElements; i++)
		{
			if(elementSaveFilter[i] == 1)
			{
				fprintf(fp, "%d\n", i);
				fprintf(fp, "%s\n", elements[i]->name);
				fprintf(fp, "%d %d %d %d\n",
						elements[i]->state,
						elements[i]->startingTemp,
						elements[i]->lowestTemp,
						elements[i]->highestTemp);
				fprintf(fp, "%d %d\n",
						elements[i]->lowerElement->index,
						elements[i]->higherElement->index);
				fprintf(fp, "%d %d %d\n",
						elements[i]->red,
						elements[i]->green,
						elements[i]->blue);
				//TODO: Had to eat dinner :)
			}
		}

		//Save the particles
		for (counterX = 0; counterX < workWidth; counterX++)
		{
			for (counterY = 0; counterY < workHeight; counterY++)
			{
				tempParticle = allCoords[getIndex(counterX, counterY)];
				if(tempParticle)
				{
					fprintf(fp, "(%d %d %d %d %d %d) ",
							tempParticle->x,
							tempParticle->y,
							tempParticle->xVel,
							tempParticle->yVel,
							tempParticle->heat,
							tempParticle->element->index);
				}
			}
			fprintf(fp, "\n");
		}
		
		int counter;
		//Save any custom elements that need to be saved, so that this is portable
		for(counter = 0; counter < numElements - NUM_BASE_ELEMENTS; counter++)
		{
			if(1 )//needsToBeSaved[counter])
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
	gameSetup();
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
			createPoint(xCoord, yCoord, elements[element]);
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
