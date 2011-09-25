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
void Java_idkjava_thelements_game_SandViewRenderer_nativeResize(JNIEnv* env, jobject this, jint width, jint height)
{
	__android_log_write(ANDROID_LOG_INFO, "TheElements", "nativeResize()");
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

	 //Finds nearest power of 2 to work Width
	stupidTegra = 1;
	while((stupidTegra = stupidTegra << 1) < workWidth);
	dimensionsChanged = TRUE;

	arraySetup();
	char buffer[256];
	gameSetup();
	sprintf(buffer, "Available particles: %d", loq);
	__android_log_write(ANDROID_LOG_INFO, "TheElements", buffer);
	glInit();
	sprintf(buffer, "Available particles: %d", loq);
	__android_log_write(ANDROID_LOG_INFO, "TheElements", buffer);

	sprintf(buffer, "Available particles: %d", loq);
	__android_log_write(ANDROID_LOG_INFO, "TheElements", buffer);
}
void Java_idkjava_thelements_game_SandViewRenderer_nativeRender(JNIEnv* env, jobject this)
{
	//__android_log_write(ANDROID_LOG_INFO, "TheElements", "nativeRender begin");
	glRender();
	//__android_log_write(ANDROID_LOG_INFO, "TheElements", "nativeRender end");
}
void Java_idkjava_thelements_game_SandViewRenderer_nativeLoadState(JNIEnv* env, jobject this, jboolean shouldLoadDemo)
{
	char loadLoc[256];

	strcpy(loadLoc, ROOT_FOLDER);
	strcat(loadLoc, SAVES_FOLDER);
	if(shouldLoadDemo)
	{
		__android_log_write(ANDROID_LOG_INFO, "TheElements", "Loading demo");
		strcat(loadLoc, DEMO_SAVE);
	}
	else
	{
		__android_log_write(ANDROID_LOG_INFO, "TheElements", "loadTempState");
		strcat(loadLoc, TEMP_SAVE);
	}
	strcat(loadLoc, SAVE_EXTENSION);
	loadState(loadLoc);
}

//Save/load functions
char Java_idkjava_thelements_game_SaveManager_saveState(JNIEnv* env, jobject this, jbyteArray saveLoc)
{
	jsize len = (*env)->GetArrayLength(env, saveLoc);
	jbyte* saveLoc2 = (jbyte*) malloc(len * sizeof(jbyte));
	(*env)->GetByteArrayRegion(env, saveLoc, 0, len, saveLoc2);
	char* saveLoc3 = (char*) malloc((len+1) * sizeof(char));
	int i;
	char buffer[100];
	for(i = 0; i < len; i++)
	{
		saveLoc3[i] = saveLoc2[i];
	}
	saveLoc3[len] = 0;

	__android_log_write(ANDROID_LOG_INFO, "TheElements", saveLoc3);

	if(saveTempToFile(saveLoc3))
	{
		__android_log_write(ANDROID_LOG_INFO, "TheElements", "saveState: success!");
		return TRUE;
	}

	__android_log_write(ANDROID_LOG_INFO, "TheElements", "saveState: failed!");
	return FALSE;
}
char Java_idkjava_thelements_game_SaveManager_loadState(JNIEnv* env, jobject this, jbyteArray loadLoc)
{
	jsize len = (*env)->GetArrayLength(env, loadLoc);
	jbyte* loadLoc2 = (jbyte*) malloc(len * sizeof(jbyte));
	(*env)->GetByteArrayRegion(env, loadLoc, 0, len, loadLoc2);
	char* loadLoc3 = (char*) malloc((len+1) * sizeof(char));
	int i;
	char buffer[100];
	for(i = 0; i < len; i++)
	{
		loadLoc3[i] = loadLoc2[i];
	}
	loadLoc3[len] = 0;

	__android_log_write(ANDROID_LOG_INFO, "TheElements", loadLoc3);

	if(loadFileToTemp(loadLoc3))
	{
		__android_log_write(ANDROID_LOG_INFO, "TheElements", "loadLoc: success!");
		return TRUE;
	}

	__android_log_write(ANDROID_LOG_INFO, "TheElements", "loadLoc: failed!");
	return FALSE;
}
char Java_idkjava_thelements_MainActivity_saveTempState(JNIEnv* env, jobject this)
{
	__android_log_write(ANDROID_LOG_INFO, "TheElements", "saveTempState");
	char saveLoc[256];
	strcpy(saveLoc, ROOT_FOLDER);
	strcat(saveLoc, SAVES_FOLDER);
	strcat(saveLoc, TEMP_SAVE);
	strcat(saveLoc, SAVE_EXTENSION);
	return saveState(saveLoc);
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
void Java_idkjava_thelements_MainActivity_nativeInit(JNIEnv* env, jobject this)
{
	__android_log_write(ANDROID_LOG_INFO, "TheElements", "nativeInit()");
	importGLInit();
	atmosphereSetup();
	elementSetup();
	particleSetup();
}
void Java_idkjava_thelements_MainActivity_clearScreen(JNIEnv* env, jobject this)
{
	shouldClear = TRUE;
}

//Setter functions
void Java_idkjava_thelements_preferences_Preferences_setBorderState(JNIEnv* env, jobject this, jboolean leftBorderState, jboolean topBorderState, jboolean rightBorderState, jboolean bottomBorderState)
{
	cAtmosphere->borderLeft = leftBorderState;
	cAtmosphere->borderTop = topBorderState;
	cAtmosphere->borderRight = rightBorderState;
	cAtmosphere->borderBottom = bottomBorderState;
}
void Java_idkjava_thelements_preferences_Preferences_setAccelState(JNIEnv* env, jobject this, jboolean accelState)
{
	accelOn = (char) accelState;
}
void Java_idkjava_thelements_preferences_Preferences_setFlippedState(JNIEnv* env, jobject this, jboolean flippedState)
{
	flipped = (char) flippedState;
}
void Java_idkjava_thelements_preferences_Preferences_setBackgroundColor(JNIEnv* env, jobject this, jchar redValue, jchar greenValue, jchar blueValue)
{
	//Set the eraser color to the background color, used as the reference whenever background color is needed
	cAtmosphere->backgroundRed = redValue;
	cAtmosphere->backgroundGreen = greenValue;
	cAtmosphere->backgroundBlue = blueValue;

	char buffer[100];
	sprintf(buffer, "blue: %d", blueValue);
	__android_log_write(ANDROID_LOG_INFO, "TheElements", buffer);

	//Call setup again
	//gameSetup();

	//TODO: Reload
	/*
	char loadLoc[256];
	strcpy(loadLoc, ROOT_FOLDER);
	strcat(loadLoc, SAVES_FOLDER);
	strcat(loadLoc, TEMP_SAVE);
	strcat(loadLoc, SAVE_EXTENSION);
	loadState(loadLoc);
	*/
}
void Java_idkjava_thelements_preferences_Preferences_setAtmosphereTemp(JNIEnv* env, jobject this, jchar temp)
{
	cAtmosphere->heat = temp;
}
void Java_idkjava_thelements_preferences_Preferences_setAtmosphereGravity(JNIEnv* env, jobject this, jfloat gravity)
{
	cAtmosphere->gravity = gravity;
}
void Java_idkjava_thelements_MainActivity_setPlayState(JNIEnv* env, jobject this, jboolean playState)
{
	play = (char) playState;
}
void Java_idkjava_thelements_MainActivity_setZoomState(JNIEnv* env, jobject this, jboolean zoomState)
{
	if(zoomState != zoom)
	{
		shouldZoom = TRUE;
	}
}
void Java_idkjava_thelements_MainActivity_setElement(JNIEnv* env, jobject this, jchar element)
{
	cElement = elements[element];
}
void Java_idkjava_thelements_MainActivity_setBrushSize(JNIEnv* env, jobject this, jchar brushSizeValue)
{
	brushSize = brushSizeValue;
}
void Java_idkjava_thelements_game_SandView_setFingerState(JNIEnv* env, jobject this, jchar state)
{
	fingerDown = state;
	//To prevent drawing from the previous point, invalidate the mouse pointer
	mouseX = -1;
}
void Java_idkjava_thelements_game_SandView_setMouseLocation(JNIEnv* env, jobject this, jint x, jint y)
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

//Accelerometer related
void Java_idkjava_thelements_MainActivity_setXGravity(JNIEnv* env, jobject this, float xGravityIn)
{
	xGravity = xGravityIn;
}
void Java_idkjava_thelements_MainActivity_setYGravity(JNIEnv* env, jobject this, float yGravityIn)
{
	yGravity = yGravityIn;
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

//Custom elements related
char Java_idkjava_thelements_game_CustomElementManager_loadCustomElement(JNIEnv* env, jobject this, char* loadLoc)
{
	return loadCustomElement(loadLoc);
}



