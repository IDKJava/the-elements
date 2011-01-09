/*
 * saveload.c
 * --------------------------
 * Defines the function definitions for saver
 * and loader, the two functions which save and
 * load an element setup.
 */

#include "saveload.h"

char save(int type)
{
	FILE *fp;
	if (type == NORMAL_SAVE)
	{
		fp = fopen(SAVE_FILE, "w");
	}
	else if (type == QUICK_SAVE)
	{
		fp = fopen(QUICK_SAVE_FILE, "w");
	}

	if (fp != NULL)
	{
		int counter, addedToFile = FALSE;
		for (counter = 0; counter < maxPoints; counter++)
		{
			if (set[counter] == TRUE)
			{
				fprintf(fp, "%d %d %d %d ", spawn[counter], (int) x[counter],
						(int) y[counter], element[counter]); //Save the spawn, x y, and element of each current point
				addedToFile = TRUE;
			}
		}
		fclose(fp);
		if (addedToFile == FALSE)
		{
			if (type == NORMAL_SAVE)
			{
				remove(SAVE_FILE);
			}
			else if (type == QUICK_SAVE)
			{
				remove(QUICK_SAVE_FILE);
			}
		}
		return TRUE; //success
	}
	else
	{
		return FALSE; //error: didn't open file, prolly sdcard not there
	}
}

char load(int type)
{
	FILE *fp;
	if (type == NORMAL_LOAD)
	{
		fp = fopen(SAVE_FILE, "r");
	}
	else if (type == QUICK_LOAD)
	{
		fp = fopen(QUICK_SAVE_FILE, "r");
	}
	else if (type == DEMO_LOAD)
	{
		fp = fopen(DEMO_SAVE_FILE, "r");
	}

	setup();
	int xCoordinate;
	int yCoordinate;
	int loadElement;
	int spawnV;

	if(fp != NULL)
	{
		while (!feof(fp))
		{
			fscanf(fp, "%d%d%d%d", &spawnV, &xCoordinate, &yCoordinate,
					&loadElement);
			spawn[avail[loq - 1]] = spawnV;
			CreatePoint(xCoordinate, yCoordinate, loadElement);
		}

		fclose(fp);
		return TRUE;
	}

	return FALSE;
}

void removeQuicksave(void)
{
	remove(QUICK_SAVE_FILE);
}

char loadDemoFile()
{
	return load(DEMO_LOAD);
}

//TODO:
void saveCustomElement(Element createdCustomElement) {}
void loadCustomElements(void) {}
void saveAtmosphere(Atmosphere createAtmosphere) {}
void loadAtmosphere(void) {}
int getCustomElementsNumber(void) {}
