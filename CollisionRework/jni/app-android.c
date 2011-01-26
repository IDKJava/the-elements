/*
 * app-android.c
 * -----------------------------
 * Contains the entire API for our JNI code. Any
 * access to the native code from Java will be made
 * using these accessible functions.
 */

#include <jni.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <android/log.h>

//Include the global variables
#include "app.h"
//Include the global macros
#include "macros.h"
//Include the initializing function
#include "setup.h"
//Include the element characteristics file
#include "elementproperties.h"
//Include the collisions data file
#include "collisions.h"
//Include the saving and loading functions
#include "saveload.h"
//Include the server access functions
//#include "server.h"
//Include the rendering functions
#include "gl.h"



//Called from SandViewRenderer
void Java_idkjava_thelements_game_SandViewRenderer_nativeInit(JNIEnv* env)
{
	importGLInit();
	glInit();
}
void Java_idkjava_thelements_game_SandViewRenderer_nativeResize(JNIEnv* env, jobject this, jint width, jint height)
{
	screenWidth = width - width%2;
	screenHeight = height - height%2;
	if (zoom == ZOOMED_IN)
	{
		workWidth = screenWidth / 2;
		workHeight = screenHeight / 2;
	}
	else
	{
		workWidth = screenWidth;
		workHeight = screenHeight;
	}
	__android_log_write(ANDROID_LOG_INFO, "TheElements", "nativeresize bro");
	arraySetup();
	gameSetup();
}
void Java_idkjava_thelements_game_SandViewRenderer_nativeRender(JNIEnv* env)
{
	glRender();
}

//Save/load functions
char Java_idkjava_thelements_game_SaveManager_saveState(JNIEnv* env, jobject this, char* saveLoc)
{
	return saveState(saveLoc);
}
char Java_idkjava_thelements_game_SaveManager_loadState(JNIEnv* env, jobject this, char* loadLoc)
{
	return loadState(loadLoc);
}
char Java_idkjava_thelements_MainActivity_tempSaveState(JNIEnv* env)
{
	char saveLoc[256];
	strcpy(saveLoc, ROOT_FOLDER);
	strcat(saveLoc, SAVES_FOLDER);
	strcat(saveLoc, TEMP_SAVE);
	strcat(saveLoc, SAVE_EXTENSION);
	return saveState(saveLoc);
}
char Java_idkjava_thelements_MainActivity_tempLoadState(JNIEnv* env)
{
	char loadLoc[256];
	strcpy(loadLoc, ROOT_FOLDER);
	strcat(loadLoc, SAVES_FOLDER);
	strcat(loadLoc, TEMP_SAVE);
	strcat(loadLoc, SAVE_EXTENSION);
	return loadState(loadLoc);
}
char Java_idkjava_thelements_MainActivity_removeTempSave(JNIEnv* env, jobject this)
{
	return removeTempSave();
}
char Java_idkjava_thelements_MainActivity_loadDemoState(JNIEnv* env, jobject this)
{
	char loadLoc[256];
	strcpy(loadLoc, ROOT_FOLDER);
	strcat(loadLoc, SAVES_FOLDER);
	strcat(loadLoc, DEMO_SAVE);
	strcat(loadLoc, SAVE_EXTENSION);
	return loadState(loadLoc);
}

//General utility functions
void Java_idkjava_thelements_MainActivity_clearScreen(JNIEnv* env, jobject this)
{
	shouldClear = TRUE;
}
void Java_idkjava_thelements_MainActivity_setBackgroundColor(JNIEnv* env, jobject this, jint redValue, jint greenValue, jint blueValue)
{
	//Set the eraser color to the background color, used as the reference whenever background color is needed
	elements[ERASER_ELEMENT]->red = redValue;
	elements[ERASER_ELEMENT]->green = greenValue;
	elements[ERASER_ELEMENT]->blue = blueValue;

	//Call setup again
	gameSetup();

	//Reload
	char loadLoc[256];
	strcpy(loadLoc, ROOT_FOLDER);
	strcat(loadLoc, SAVES_FOLDER);
	strcat(loadLoc, TEMP_SAVE);
	strcat(loadLoc, SAVE_EXTENSION);
	loadState(loadLoc);
}
void Java_idkjava_thelements_MainActivity_setDimensions(JNIEnv* env, jobject this, jint width, jint height)
{
	//Set the screen dimensions and decrease by 1 pixel if not divisible by two
	screenWidth = width - width%2;
	screenHeight = height - height%2;
}

//Setter functions
void Java_idkjava_thelements_MainActivity_setPlayState(JNIEnv* env, jobject this, jboolean playState)
{
	play = (char) playState;
}
void Java_idkjava_thelements_MainActivity_setAccelState(JNIEnv* env, jobject this, jboolean accelState)
{
	accelOn = (char) accelState;
}
void Java_idkjava_thelements_MainActivity_setZoomState(JNIEnv* env, jobject this, jboolean zoomState)
{
	zoom = (char) zoomState;
	if (zoom == ZOOMED_IN)
	{
		//Divide by two to zoom in
		workWidth = workWidth / 2;
		workHeight = workHeight / 2;
	}
	else
	{
		//Multiply by two to zoom out
		workWidth = workWidth * 2;
		workHeight = workWidth * 2;
	}
}
void Java_idkjava_thelements_MainActivity_setFlippedState(JNIEnv* env, jobject this, jboolean flippedState)
{
	flipped = (char) flippedState;
}
void Java_idkjava_thelements_MainActivity_setElement(JNIEnv* env, jobject this, jchar element)
{
	cElement = elements[element];
}
void Java_idkjava_thelements_MainActivity_setBrushSize(JNIEnv* env, jobject this, jchar brushSizeValue)
{
	brushSize = brushSizeValue;
}
void Java_idkjava_thelements_SandView_setFingerState(JNIEnv* env, jobject this, jchar state)
{
	fingerDown = state;
	//To prevent drawing from the previous point, invalidate the mouse pointer
	mouseX = -1;
}
void Java_idkjava_thelements_SandView_setMouseLocation(JNIEnv* env, jobject this, jint x, jint y)
{
	//Set the mouse position and draw lines if needed
	if (mouseX != -1)
	{
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		int changeX = x - lastMouseX; //change in x (delta x)
		int changeY = y - lastMouseY; //change in y (delta y)


		int distance = sqrt(changeX * changeX + changeY * changeY); //distance between two points


		if (distance > 0 && cElement->index != DRAG_ELEMENT) //if it's not the same place and not wind
		{
			float dx = (float)changeX / (float)distance; // change divided by distance
			float dy = (float)changeY / (float)distance;
			int counter;
			int oldplay = play;
			play = FALSE;
			for (counter = 0; counter <= distance; counter++)
			{
				mouseY = dy * counter + lastMouseY;
				mouseX = dx * counter + lastMouseX;
				UpdateView();
			}
			play = oldplay;
		}
	}
	mouseX = x;
	mouseY = y;
}

//Getter functions
char Java_idkjava_thelements_MainActivity_getElement(JNIEnv* env, jobject this)
{
	return cElement->index;
}

//TODO: Accelerometer related
void Java_idkjava_thelements_MainActivity_setXGravity(JNIEnv* env, jobject this, float xGravity)
{
	//Stuff
}
void Java_idkjava_thelements_MainActivity_setYGravity(JNIEnv* env, jobject this, float yGravity)
{
	//Stuff
}

/* TODO: Network related
void Java_idkjava_thelements_MainActivity_setPassword(JNIEnv *env, jobject this, jbyteArray minut)
{
	int i; //Counter variable

	//Get the array length of the password
	jsize len  = (*env)->GetArrayLength(env,minut);
	//Create a byte array with the size of the array in the byteArray object
	jbyte* minut1 = (jbyte *)malloc(len * sizeof(jbyte));

	//Extract the byteArray object to a byte array
	(*env)->GetByteArrayRegion(env,minut,0,len,minut1);

	//Copy the byte array over into the password
	for(i = 0; i < len; i++)
	{
		password[i] = minut1[i];
	}
	//Add the null byte
	password[len] = 0;
	//Set the length variable
	passlength = len;

	//Free the created byte array
	free(minut1);
}
void Java_idkjava_thelements_MainActivity_setUsername(JNIEnv *env, jobject this, jbyteArray minut)
{
	int i; //Counter variable

	//Get the array length of the username
	jsize len  = (*env)->GetArrayLength(env,minut);
	//Create a byte array with the size of the array in the byteArray object
	jbyte* minut1 = (jbyte *)malloc(len * sizeof(jbyte));

	//Extract the byteArray object to a byte array
	(*env)->GetByteArrayRegion(env,minut,0,len,minut1);

	//Copy the byte array over into the username
	for(i = 0; i < len; i++)
	{
		username[i] = minut1[i];
	}
	//Add the null byte
	username[len] = 0;
	//Set the length variable
	userlength = len;

	//Free the created byte array
	free(minut1);
}
int Java_idkjava_thelements_MainActivity_login(JNIEnv *env, jobject this)
{
	buildbuffer(3);
	if(!sendbuffer())
	{
		return -1;
	}

	return 0;
}
int Java_idkjava_thelements_MainActivity_register(JNIEnv *env, jobject this)
{
	buildbuffer(2);
	if(!sendbuffer())
	{
		return -1;
	}


	return 0;
}
char* Java_idkjava_thelements_MainActivity_viewErr (JNIEnv *env, jobject this)
{
	return error;
}
*/

//Atmosphere related
char Java_idkjava_thelements_game_AtmosphereManager_loadAtmosphere(char* loadLoc)
{
	return loadAtmosphere(loadLoc);
}

//Custom elements related
char Java_idkjava_thelements_game_CustomElementManager_loadCustomElement(JNIEnv* env, jobject this, char* loadLoc)
{
	return loadCustomElement(loadLoc);
}



