/*
 * saveload.h
 * --------------------------
 * Contains the function definitions for saver
 * and loader, the two functions which save and
 * load an element setup.
 */

//Define the file locations so that they can be easily changed
#define SAVE_FILE "/sdcard/elementworks/save.txt"
#define QUICK_SAVE_FILE "/sdcard/elementworks/quicksave.txt"
#define DEMO_SAVE_FILE "/sdcard/elementworks/save2.txt"
#define CUSTOM_ELEMENT_FILE "/sdcard/elementworks/customele.txt"

#define TElements 25

//Declare the rsetup() function here
void rsetup();

int saver(int type)
{
	FILE *fp;
	if (type == 0) //If it's a normal save
	{
		fp = fopen(SAVE_FILE, "w");
	}
	else if (type == 1) //If it's a quicksave (it's being paused)
	{
		fp = fopen(QUICK_SAVE_FILE, "w");
	}
	if (fp != NULL)
	{
		int counter, added_to_file = 0;
		for (counter = 0; counter < TPoints; counter++)
		{
			if (set[counter] == 1)
			{
				fprintf(fp, "%d %d %d %d ", spawn[counter], (int) x[counter],
						(int) y[counter], element[counter]); //Save the spawn, x y, and element of each current point
				added_to_file = 1;
			}
		}
		fclose(fp);
		if (added_to_file == 0)
		{
			if (type == 0)
			{
				remove(SAVE_FILE);
			}
			else if (type == 1)
			{
				remove(QUICK_SAVE_FILE);
			}
		}
		return 1; //success
	}
	else
	{
		return 0; //error: didn't open file, prolly sdcard not there
	}
}

int loader(int type)
{
	FILE *fp;
	if (type == 0) //normal load
	{
		fp = fopen(SAVE_FILE, "r");
	}
	else if (type == 1) //quickload
	{
		fp = fopen(QUICK_SAVE_FILE, "r");
	}
	rsetup();
	int i;
	int xcoordinate;
	int ycoordinate;
	int loadelement;
	int spawnv;

	if (fp != NULL)
	{
		while (!feof(fp))
		{
			fscanf(fp, "%d%d%d%d", &spawnv, &xcoordinate, &ycoordinate,
					&loadelement);
			spawn[avail[loq - 1]] = spawnv;
			CreatePoint(xcoordinate, ycoordinate, loadelement);
		}

		fclose(fp);
		return 1;
	}
}

void removeQuicksave(void)
{
	remove(QUICK_SAVE_FILE);
}

int loadDemoFile()
{
	FILE *fp;
	fp = fopen(DEMO_SAVE_FILE, "r");
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "demo");
	rsetup();
	int i;
	int xcoordinate;
	int ycoordinate;
	int loadelement;
	int spawnv;

	if (fp != NULL)
	{
		while (!feof(fp))
		{
			fscanf(fp, "%d%d%d%d", &spawnv, &xcoordinate, &ycoordinate,
					&loadelement);
			spawn[avail[loq - 1]] = spawnv;
			CreatePoint(xcoordinate, ycoordinate, loadelement);
		}

		fclose(fp);
		return 1;
	}
	else
	{
		return 0;
	}
}

int loadCustomFile(void)
{

	FILE *fp;
	fp = fopen(CUSTOM_ELEMENT_FILE, "r");
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "custom");
	rsetup();
	int i;
	int collisiondata;

	if (fp != NULL)
	{
		for (i = 0; i < TElements; i++)
		{
			fscanf(fp, "%d", &collisiondata);
			colliseelement1[i] = collisiondata;
		}
		for (i = 0; i < TElements; i++)
		{
			fscanf(fp, "%d", &collisiondata);
			collision[22][i] = collisiondata;
		}

		fclose(fp);
		return 1;
	}
	else
	{
		return 0;
	}
}

int saveCustomFile(void)
{
	FILE *fp;
	int i;
	fp = fopen(CUSTOM_ELEMENT_FILE, "w");

	if (fp != NULL)
	{
		for (i = 0; i < TElements; i++)
		{
			fprintf(fp, "%d", colliseelement1[i]);
		}
		for (i = 0; i < TElements; i++)
		{
			fprintf(fp, "%d", collision[22][i]);
		}
		fclose(fp);
		return 1;
	}
	else
	{
		return 0;
	}
}
