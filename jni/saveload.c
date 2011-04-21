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
		int i, counterX, counterY, numElementsToBeSaved;
		struct Particle* tempParticle;
		char* elementSaveFilter;
		elementSaveFilter = (char*)malloc(numElements * sizeof(char));

		//Initialize elementSaveFilter
		for(i = 0; i < numElements; i++)
		{
			elementSaveFilter[i] = 0;
		}

		//Preprocessing loop
		numElementsToBeSaved = 0;
		for (counterX = 0; counterX < workWidth; counterX++)
		{
			for (counterY = 0; counterY < workHeight; counterY++)
			{
				tempParticle = allCoords[getIndex(counterX, counterY)];
				//If the particle is a custom element, indicate that we need to save this element
				if(tempParticle && tempParticle->element->index >= NUM_BASE_ELEMENTS && elementSaveFilter[tempParticle->element->index] != 1)
				{
					elementSaveFilter[tempParticle->element->index] = 1;
					numElementsToBeSaved++;
				}
			}
		}
		
		//First write how many custom elements will be saved
		fprintf(fp, "%d\n\n", numElementsToBeSaved);

		//Save the custom elements that need to be saved
		/* Save format:
		 * index
		 * name
		 * state startingTemp lowestTemp highestTemp
		 * lowerElement->index higherElement->index
		 * red green blue
		 * TODO: specials
		 * density fallVel inertia
		 */
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
				//TODO: Specials
				fprintf(fp, "%d %d %d\n\n",
						elements[i]->density,
						elements[i]->fallVel,
						elements[i]->inertia);
			}
		}

		//Save the dimensions
		fprintf(fp, "%d %d\n\n", workWidth, workHeight);

		//Save the particles
		/* Save format:
		 * (x y xVel yVel heat element->index) ...
		 *   .
		 *   .
		 *   .
		 */
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
				else
				{
					fprintf(fp, "()");
				}
			}
			fprintf(fp, "\n");
		}

		fclose(fp);

		return TRUE;
	}

	fclose(fp);

	return FALSE;
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
	elementSetup();
	gameSetup();

	int numElementsSaved, i, j, elementIndex, lowerElementIndex, higherElementIndex, sizeX, sizeY;
	struct Element* tempElement;
	struct Particle* tempParticle;

	if(fp != NULL)
	{
		if(fscanf(fp, "%d", &numElementsSaved) == EOF)
		{
			return FALSE;
		}

		for(i = 0; i < numElementsSaved; i++)
		{
			if(fscanf(fp, "%d", &elementIndex) == EOF) {return FALSE;}

			tempElement = (struct Element*) malloc(sizeof(struct Element));
			if(fscanf(fp, "%s", &tempElement->name) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->state) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->startingTemp) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->lowestTemp) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->highestTemp) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &lowerElementIndex) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &higherElementIndex) == EOF) {return FALSE;}
			tempElement->lowerElement = elements[lowerElementIndex];
			tempElement->higherElement = elements[higherElementIndex];
			if(fscanf(fp, "%d", &tempElement->red) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->green) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->blue) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->density) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->fallVel) == EOF) {return FALSE;}
			if(fscanf(fp, "%d", &tempElement->inertia) == EOF) {return FALSE;}
		}

		if(fscanf(fp, "%d %d", &sizeX, &sizeY) == EOF) {return FALSE;}

		//Make sure saves are portable from different dimensions
		if(sizeX > workWidth)
		{
			sizeX = workWidth;
		}
		if(sizeY > workHeight)
		{
			sizeY = workHeight;
		}

		for(i = 0; i < sizeX; i++)
		{
			for(j = 0; j < sizeY; j++)
			{
				if(loq <= 0) {return TRUE;}
				tempParticle = avail[loq];

				if(fscanf(fp, "(%d %d %d %d %d %d)", &tempParticle->x,
													&tempParticle->y,
													&tempParticle->xVel,
													&tempParticle->yVel,
													&tempParticle->heat,
													&elementIndex) == EOF) {return FALSE;}

				tempParticle->element = elements[elementIndex];
				tempParticle->set = TRUE;
				int k;
				for(k = 0; k < MAX_SPECIALS; k++)
				{
					tempParticle->specialVals[k] = tempParticle->element->specialVals[k];
				}

				allCoords[getIndex(i, j)] = tempParticle;
				setBitmapColor(tempParticle->x, tempParticle->y, tempParticle->element);
			}
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
