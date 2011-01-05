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
//#include <android/log.h>

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
#include "server.h"
//Include the rendering functions
#include "gl.h"

void Java_sand_falling_opengl_DemoRenderer_nativeInit(JNIEnv* env) //Initialize graphics
{
	//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "8");
	importGLInit();
	appInit();
	gAppAlive = 1;
	//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "4");
}

void Java_sand_falling_opengl_DemoRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h)
{
	if (screensize == 0) //this didn't work before becuase tej used = instead of == (green girdle)
	{
		maxx = w / 2;
		maxy = h / 2;
	}
	else
	{
		maxx = w;
		maxy = h;
	}
	appInit();
}

void Java_sand_falling_opengl_DemoRenderer_nativeDone(JNIEnv* env)
{
	appDeinit();
	importGLDeinit();
}

//these two get the gravity from the java code
void Java_sand_falling_opengl_MainActivity_sendYGrav(JNIEnv* env, jobject thiz, jfloat ygrav)
{
	gravy = ygrav;
}
void Java_sand_falling_opengl_MainActivity_sendXGrav(JNIEnv* env, jobject thiz, jfloat xgrav)
{
	gravx = -xgrav;
}
void Java_sand_falling_opengl_MainActivity_quickLoad(JNIEnv* env)
{
	loader(1);
}
void Java_sand_falling_opengl_MainActivity_quickSave(JNIEnv* env)
{
	saver(1);
}

/* Call to render the next GL frame */
void Java_sand_falling_opengl_DemoRenderer_nativeRender(JNIEnv* env)
{
	appRender();
}

void Java_sand_falling_opengl_MainActivity_setup(JNIEnv* env, jobject thiz)
{
	shouldClear = 1;
}

void Java_sand_falling_opengl_MainActivity_pause(JNIEnv* env, jobject thiz)
{
	play = 0;
}
void Java_sand_falling_opengl_MainActivity_play(JNIEnv* env, jobject thiz)
{
	play = 1;
}
int Java_sand_falling_opengl_MainActivity_getPlayState(JNIEnv* env, jobject thiz)
{
	return play;
}

void Java_sand_falling_opengl_MainActivity_setDimensions(JNIEnv* env, jobject thiz, jint width, jint height)
{
	//Decrease by 1 pixel if not divisible by two
	screenWidth = width - width%2;
	screenHeight = height - height%2;
}

void Java_sand_falling_opengl_MainActivity_setBackgroundColor(JNIEnv* env, jobject thiz, jint colorcode)
{
	if (colorcode == 0)
	{
		red[3] = 0; //3 is eraser
		blue[3] = 0;
		green[3] = 0;
		//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "color");
		rsetup();
	}
	else if (colorcode == 1)
	{
		red[3] = 255;
		blue[3] = 255;
		green[3] = 255;
		//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "color");
		rsetup();
	}
	loader(1);
}
void Java_sand_falling_opengl_MainActivity_setExplosiveness(JNIEnv* env, jobject thiz, jint explosiveness)
{
	exploness[22] = explosiveness;
}
void Java_sand_falling_opengl_MainActivity_setRed(JNIEnv* env, jobject thiz, jint redness)
{
	red[22] = redness;
}
void Java_sand_falling_opengl_MainActivity_setGreen(JNIEnv* env, jobject thiz, jint greenness)
{
	green[22] = greenness;
}
void Java_sand_falling_opengl_MainActivity_setBlue(JNIEnv* env, jobject thiz, jint blueness)
{
	blue[22] = blueness;
}

void Java_sand_falling_opengl_MainActivity_setDensity(JNIEnv* env, jobject thiz, jint jdensity)
{
	density[22] = jdensity;
}
void Java_sand_falling_opengl_MainActivity_setFlip(JNIEnv* env, jobject thiz, jint jflipped)
{
	flipped = jflipped;
}
void Java_sand_falling_opengl_MainActivity_setCollision(JNIEnv* env, jobject thiz, jint custnum, jint elementnumb, jint colspot, jint colnum)
{
	if (custnum == 1)
	{
		collision[22][colspot] = colnum;
		colliseelement1[colspot] = elementnumb;
		collision[colspot][22] = colnum;
	}
	else
	{
		collision[22][colspot] = colnum;
		colliseelement1[colspot] = elementnumb;
		collision[colspot][22] = colnum;
	}
	int counter124;
	char foundfire = 0;
	for (counter124 = 0; counter124 < TElements; counter124++)
	{
		if (collision[22][counter124] == 6)
		{
			foundfire = 1;
		}
	}
	if (foundfire == 0)
	{
		fireburn[22] = 0;
	}
	else
	{
		fireburn[22] = 1;
	}
}

void Java_sand_falling_opengl_MainActivity_setFingerState(JNIEnv* env, jobject thiz, jint fstate)
{
	fingerState = fstate;
	if (fingerState == 1)
	{
		mouseX = -1; //To prevent drawing from the previous point, invalidate the mouse pointer
	}
	return;
}
void Java_sand_falling_opengl_MainActivity_setMouseLocation(JNIEnv* env, jobject thiz, jint xpos, jint ypos)
{
	//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "mp start");
	//Set the mouse position and draw lines if needed
	if (mouseX != -1)
	{
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		int changeX = xpos - lastMouseX; //change in x (delta x)
		int changeY = ypos - lastMouseY; //change in y (delta y)


		int distance = sqrt(changeX * changeX + changeY * changeY); //distance between two points


		if (distance > 0 && celement != 16) //if it's not the same place and not wind
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
	mouseX = xpos;
	mouseY = ypos;
	//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "mp end");
	return;
}
void Java_sand_falling_opengl_MainActivity_clearQuickSave(JNIEnv* env, jobject thiz)
{
	removeQuicksave();
	return;
}
void Java_sand_falling_opengl_MainActivity_setElement(JNIEnv* env, jobject thiz, jint jelement)
{
	celement = jelement;
	return;
}
int Java_sand_falling_opengl_MainActivity_getElement(JNIEnv* env, jobject thiz)
{
	return celement;
}
void Java_sand_falling_opengl_MainActivity_setBrushSize(JNIEnv* env, jobject thiz, jint jsize)
{
	size = jsize;
	return;
}
void Java_sand_falling_opengl_MainActivity_setAccelOnOff(JNIEnv* env, jobject thiz, jint state)
{
	accelcon = state;
	return;
}
void Java_sand_falling_opengl_MainActivity_toggleSize(JNIEnv* env, jobject thiz)
{
	if (zoom == ZOOMED_IN)
	{
		//Multiply by two to zoom out
		zoom = ZOOMED_OUT;
		workWidth = workWidth * 2;
		workHeight = workWidth * 2;
	}
	else
	{
		//Divide by two to zoom in
		zoom = ZOOMED_IN;
		workWidth = workWidth / 2;
		workHeight = workHeight / 2;
	}
}
char Java_sand_falling_opengl_MainActivity_save(JNIEnv* env, jobject thiz)
{
	return saver(NORMAL_SAVE); //Do a normal save
}

char Java_sand_falling_opengl_MainActivity_load(JNIEnv* env, jobject thiz)
{
	return loader(NORMAL_LOAD); // call the load function, normal load
}

char Java_sand_falling_opengl_MainActivity_loadDemo(JNIEnv* env, jobject thiz)
{
	return loadDemoFile();
}

void Java_sand_falling_opengl_MainActivity_setPassword(JNIEnv *env, jobject thiz, jbyteArray minut)
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

void Java_sand_falling_opengl_MainActivity_setUsername(JNIEnv *env, jobject thiz, jbyteArray minut)
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

int Java_sand_falling_opengl_MainActivity_login(JNIEnv *env, jobject thiz)
{
	buildbuffer(3);
	if(!sendbuffer())
	{
		return -1;
	}

	return 0;
}
int Java_sand_falling_opengl_MainActivity_register(JNIEnv *env, jobject thiz)
{
	buildbuffer(2);
	if(!sendbuffer())
	{
		return -1;
	}


	return 0;
}

char* Java_sand_falling_opengl_MainActivity_viewErr (JNIEnv *env, jobject thiz)
{
	return error;
}
