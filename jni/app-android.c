/* San Angeles Observation OpenGL ES version example
 * Copyright 2009 The Android Open Source Project
 * All rights reserved.
 *
 * This source is free software; you can redistribute it and/or
 * modify it under the terms of EITHER:
 *   (1) The GNU Lesser General Public License as published by the Free
 *       Software Foundation; either version 2.1 of the License, or (at
 *       your option) any later version. The text of the GNU Lesser
 *       General Public License is included with this source in the
 *       file LICENSE-LGPL.txt.
 *   (2) The BSD-style license that is included with this source in
 *       the file LICENSE-BSD.txt.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
 */

#include <jni.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>

//Socket includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Include the element characteristics file
#include "elementproperties.h"
//Include the collisions data file
#include "collisions.h"

#define TPoints 500000
#define TElements 25
#define TPixels 512*1024 //144826 for g1, 390385 for droid
#define BLACK -16777216
#define TCollision 28

#define PORTNUM 2000

int cpoint;
int play = 1;
int size = 4;
int lmx;
int lmy;
int lmx2;
int lmy2;
int lmx3;
int lmy3;
int jchanged;
int accelcon = 0;

char username[8];
char password[8];
char userlength;
char passlength;

int bufferlength;

char* error;

//Array for bitmap drawing
unsigned char colors[TPixels * 3]; //3 bytes/pixel

//coordinates of all the pieces 
float x[TPoints];
float y[TPoints];

char frozen[TPoints];

int flipped = 0;

int maxx;
int maxy;

//old coordinates of the sand, used for collision resolving
int oldx[TPoints];
int oldy[TPoints];
int delete[TPoints];
char buffer[3 + 1 + (2 * TPoints * 4) + 200]; //3 bytes for size, 1 for command, the next part for data, plus extra just in case

float gravx = 0; //xgravity
float gravy = 0; //ygravity (9.8 m/s^2 usually)

// what type of element each piece of sand is
int element[TPoints];

//Array to keep track of which points are used
int set[TPoints];
//Array of which points are available to keep CreatePoint fast
int avail[TPoints];
//Value to keep track of location in avail array
int loq = TPoints;
//Array to keep track of spawn elements for spawns
int spawn[TPoints];
int tracker; //keeps track for the array of changed stuff
int changed[TPixels * 10]; //array of changed stuff
int newvalue[TPixels * 10]; //values of the changed stuff
int jcounter = 0; //counter for when transfering to Java

int screensize = 0; //0 = zoomed in, 1 = zoomed out

//velocities of each piece of sand
int xvel[TPoints];
int yvel[TPoints];

// A map of all the coordinates on the screen
int allcoords[900][900];

//will be the mouse positions
int xm = -1;
int ym = 50;

//finger down?
int fd;

//Current element selected
int celement = 0;

int gAppAlive = 1;

//Socket variables
int sockfd; //The file descriptor for the socket
int n; //Used in sending and recieving data
struct sockaddr_in serv_addr; //The server address struct
struct hostent *server; //Pointer to a hostent struct that is used to set up serv_addr

void Java_sand_falling_opengl_DemoRenderer_nativeInit(JNIEnv* env) //Initialize graphics
{
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "8");
	importGLInit();
	appInit();
	gAppAlive = 1;
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "4");
}

void Java_sand_falling_opengl_DemoRenderer_nativeResize(JNIEnv* env,
		jobject thiz, jint w, jint h)
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
Java_sand_falling_opengl_DemoActivity_sendyg(JNIEnv* env, jobject thiz,
		jfloat ygrav)
{
	gravy = ygrav;
}
Java_sand_falling_opengl_DemoActivity_sendxg(JNIEnv* env, jobject thiz,
		jfloat xgrav)
{
	gravx = -xgrav;
}
void Java_sand_falling_opengl_DemoActivity_quickload(JNIEnv* env)
{
	loader(1);
}
void Java_sand_falling_opengl_DemoActivity_quicksave(JNIEnv* env)
{
	saver(1);
}

/* Call to render the next GL frame */
void Java_sand_falling_opengl_DemoRenderer_nativeRender(JNIEnv* env)
{
	appRender(colors);
}

Java_sand_falling_opengl_DemoActivity_setup(JNIEnv* env, jobject thiz)
{
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "5");
	rsetup();
	return;
}
rsetup()
{
	int j, o, k;
	loq = TPoints;
	cpoint = 0;
	unsigned char blankblue = blue[3];
	unsigned char blankred = red[3];
	unsigned char blankgreen = green[3];

	for (j = 0; j < TPoints; j++)
	{
		x[j] = 0;
		y[j] = 0;
		xvel[j] = 0;
		yvel[j] = 0;
		element[j] = 0;
		oldx[j] = 0;
		oldy[j] = 0;
		set[j] = 0;
		avail[j] = j;
		spawn[j] = -1;
		frozen[j] = 0;
	}
	for (j = 0; j < 8; j++)
	{
		username[j] = 0;
		password[j] = 0;
	}
	userlength = 0;
	passlength = 0;
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "6");
	for (o = 0; o < 1024; o++)
	{
		for (k = 0; k < 512; k++)
		{
			allcoords[k][o] = -1; // -1 is empty
			colors[3 * (k + 512 * o)] = blankred; //0
			colors[3 * (k + 512 * o) + 1] = blankblue; //0
			colors[3 * (k + 512 * o) + 2] = blankgreen; //0
		}
	}
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "7");
	return;
}
Java_sand_falling_opengl_DemoActivity_jPause(JNIEnv* env, jobject thiz)
{
	play = 0;
}
Java_sand_falling_opengl_DemoActivity_Play(JNIEnv* env, jobject thiz)
{
	play = 1;
}

Java_sand_falling_opengl_DemoActivity_setBackgroundColor(JNIEnv* env,
		jobject thiz, jint colorcode)
{
	if (colorcode == 0)
	{
		red[3] = 0; //3 is eraser
		blue[3] = 0;
		green[3] = 0;
		//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "color");
		rsetup();
	}
	else if (colorcode == 1)
	{
		red[3] = 255;
		blue[3] = 255;
		green[3] = 255;
		//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "color");
		rsetup();
	}
	loader(1);
}
Java_sand_falling_opengl_DemoActivity_setexplosiveness(JNIEnv* env,
		jobject thiz, jint explosiveness)
{
	exploness[22] = explosiveness;
}
Java_sand_falling_opengl_DemoActivity_setred(JNIEnv* env, jobject thiz,
		jint redness)
{
	red[22] = redness;
}
Java_sand_falling_opengl_DemoActivity_setgreen(JNIEnv* env, jobject thiz,
		jint greenness)
{
	green[22] = greenness;
}
Java_sand_falling_opengl_DemoActivity_setblue(JNIEnv* env, jobject thiz,
		jint blueness)
{
	blue[22] = blueness;
}

Java_sand_falling_opengl_DemoActivity_setdensity(JNIEnv* env, jobject thiz,
		jint jdensity)
{
	density[22] = jdensity;
}
Java_sand_falling_opengl_DemoActivity_setFlip(JNIEnv* env, jobject thiz,
		jint jflipped)
{
	flipped = jflipped;
}
Java_sand_falling_opengl_DemoActivity_setcollision(JNIEnv* env, jobject thiz,
		jint custnum, jint elementnumb, jint colspot, jint colnum)
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

Java_sand_falling_opengl_DemoActivity_fd(JNIEnv* env, jobject thiz, jint fstate)
{
	//setting finger up or down from onTouch

	fd = fstate;
	if (fd == 1)
	{
		xm = -1;
	}
	return;
}
Java_sand_falling_opengl_DemoActivity_mp(JNIEnv* env, jobject thiz, jint jxm,
		jint jym)
{
	//setting the mouse position when given stuff from jdk
	if (xm != -1)
	{
		lmx3 = lmx2; //this way we can find difference from 3 frames ago
		lmy3 = lmy2;
		lmy2 = lmy;
		lmx2 = lmx;
		lmx = xm;
		lmy = ym;

		int xc = jxm - lmx; //change in x (delta x)
		int yc = jym - lmy; //change in y (delta y)


		int dist = sqrt(xc * xc + yc * yc); //distance between two points


		if (dist > 0 && celement != 16) //if it's not the same place and 
		{
			int xd = xc / dist; // change divided by distance
			int yd = yc / dist;
			int counter;
			int oldplay = play;
			play = 0;
			for (counter = 0; counter <= dist; counter++)
			{
				ym = yd * counter + lmy;
				xm = xd * counter + lmx;
				UpdateView();
			}
			play = oldplay;
		}
	}
	xm = jxm;
	ym = jym;
	return;
}
Java_sand_falling_opengl_DemoActivity_clearquicksave(JNIEnv* env, jobject thiz)
{
	remove("/sdcard/elementworks/quicksave.txt");
	return;
}
Java_sand_falling_opengl_DemoActivity_setelement(JNIEnv* env, jobject thiz,
		jint jelement)
{
	celement = jelement;
	return;
}
int Java_sand_falling_opengl_DemoActivity_getelement(JNIEnv* env, jobject thiz)
{
	return celement;
}
Java_sand_falling_opengl_DemoActivity_setBrushSize(JNIEnv* env, jobject thiz,
		jint jsize)
{
	size = jsize;
	return;
}
Java_sand_falling_opengl_DemoActivity_setAccelOnOff(JNIEnv* env, jobject thiz,
		jint state)
{
	accelcon = state;
	return;
}
Java_sand_falling_opengl_DemoActivity_togglesize(JNIEnv* env, jobject thiz)
{
	if (screensize == 0) //not zoomed in, *2 to zoom out
	{
		screensize = 1;
		maxx = maxx * 2;
		maxy = maxy * 2;
	}
	else
	{
		screensize = 0; //zoomed in
		maxx = maxx / 2;
		maxy = maxy / 2;
	}
}
int Java_sand_falling_opengl_DemoActivity_save(JNIEnv* env, jobject thiz)
{
	return saver(0); //Do a normal save
}

void adduserpass (void)
{
	/*
	 * Add the username and password to the save buffer. This happens every time, thus the
	 * need for a separate function.
	 */

	int i; //Counter variable

	//Fill in username
	for (i = 4; i < 4 + userlength; i++)
	{
		buffer[i] = username[i];
	}
	//Add the null byte as the delimiter
	buffer[4 + userlength] = (char) 0;
	//Fill in password
	for (i = 4 + userlength + 1; i < 4 + userlength + 1 + passlength; i++)
	{
		buffer[i] = password[i];
	}
	//Add the null byte as the delimiter
	buffer[4 + userlength + 1 + passlength] = 0;
}

void buildbuffer(int type)
{
	int i = 0;
	//Save data
	if(type == 0)
	{
		//The length of the packet, 3 bytes for length, 1 byte for command, rest for data
		bufferlength = 3 + 1 + (userlength+1) + (passlength+1) + (2*TPoints*4);

		//Fill in the 3 length bytes
		buffer[0] = (char)(bufferlength >> 16);
		buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
		buffer[2] = (char)(bufferlength % (256 * 256 * 256));

		//The command byte is 0
		buffer[3] = (char)0;

		//Put in username and password
		adduserpass();

		//Fill in the save data
		for (i = 4 + (userlength+1) + (passlength+1); i < (2*TPoints*4) + (userlength+1) + (passlength+1); i++)
		{
			buffer[i] = (char) (spawn[i / 8] >> 8);
			buffer[++i] = (char) (spawn[(i - 1) / 8] % 256);
			buffer[++i] = (char) ((int)(x[(i - 2) / 8]) >> 8);
			buffer[++i] = (char) ((int)(x[(i - 3) / 8]) % 256);
			buffer[++i] = (char) (((int)y[(i - 4) / 8]) >> 8);
			buffer[++i] = (char) ((int)(y[(i - 5) / 8]) % 256);
			buffer[++i] = (char) (element[(i - 6) / 8] >> 8);
			buffer[++i] = (char) (element[(i - 7) / 8] % 256);
		}

		//SEND DATA HERE
	}
	//Load a save
	else if (type == 1)
	{
		//Length of the packets: 3 bytes for length, 1 byte for command, rest of user and pass data
		bufferlength = 3 + 1 + (userlength + 1) + (passlength + 1);
		//Fill in length data
		buffer[0] = (char)(bufferlength >> 16);
		buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
		buffer[2] = (char)(bufferlength % (256 * 256 * 256));
		//Command byte is 1
		buffer[3] = (char)1;

		//Put in username and password
		adduserpass();

		//Add the data for what file
		//SEND DATA HERE
	}
	//Validate username and password
	else if (type == 2)
	{
		//Length of the packets: 3 bytes for length, 1 byte for command, rest of user and pass data
		bufferlength = 3 + 1 + (userlength + 1) + (passlength + 1);
		//Fill in length data
		buffer[0] = (char)(bufferlength >> 16);
		buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
		buffer[2] = (char)(bufferlength % (256 * 256 * 256));
		//Command byte is 2
		buffer[3] = (char)2;

		//Put in username and password
		adduserpass();

		//SEND DATA HERE
	}
	//Register a username with a password
	else if (type == 3)
	{
			//Length of packet: 3 bytes for length, 1 byte for command, rest for user and pass data
			bufferlength = 3 + 1 + (userlength + 1) + ( passlength + 1 );
			//Fill in length bytes
			buffer[0] = (char)(bufferlength >> 16);
			buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
			buffer[2] = (char)(bufferlength % (256 * 256 * 256));
			//Command is 3
			buffer[3] = (char)3;

			//Put in username and password
			adduserpass();

			//SEND DATA HERE
	}
	//Send custom element data (still not used)
	else if (type = 4)
	{
		/*
		int length = 3 + 1 + (TElements * 2);
		buffer[0] = (char)(length >> 16);
		buffer[1] = (char)(length % (256 * 256) >> 8);
		buffer[2] = (char)(length % (256 * 256 * 256));
		buffer[3] = (char)1;
		for (i = 4; i < 2 * TElements; i++)
		{
			buffer[i] = (char) (colliseelement1[i / 2]);
			buffer[++i] = (char) (collision[22][(i - 1) / 2]);
		}
		*/
	}
}

int saver(int type)
{
	FILE *fp;
	if (type == 0) //If it's a normal save
	{
		fp = fopen("/sdcard/elementworks/save.txt", "w");
	}
	else if (type == 1) //If it's a quicksave (it's being paused)
	{
		fp = fopen("/sdcard/elementworks/quicksave.txt", "w");
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
				remove("/sdcard/elementworks/save.txt");
			}
			else if (type == 1)
			{
				remove("/sdcard/elementworks/quicksave.txt");
			}
		}
		return 1; //success
	}
	else
	{
		return 0; //error: didn't open file, prolly sdcard not there
	}
}
int Java_sand_falling_opengl_DemoActivity_load(JNIEnv* env, jobject thiz)
{
	return loader(0); // call the load function, normal load
}
int loader(int type)
{
	FILE *fp;
	if (type == 0) //normal load
	{
		fp = fopen("/sdcard/elementworks/save.txt", "r");
	}
	else if (type == 1) //quickload
	{
		fp = fopen("/sdcard/elementworks/quicksave.txt", "r");
	}
	__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "load");
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
	//__android_log_write(ANDROID_LOG_INFO, "DemoActivity", "load done");
}
Java_sand_falling_opengl_DemoActivity_loaddemo(JNIEnv* env, jobject thiz)
{
	FILE *fp;
	fp = fopen("/sdcard/elementworks/save2.txt", "r");
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
Java_sand_falling_opengl_DemoActivity_loadcustom(JNIEnv* env, jobject thiz)
{
	FILE *fp;
	fp = fopen("/sdcard/customele.txt", "r");
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
Java_sand_falling_opengl_DemoActivity_savecustom(JNIEnv* env, jobject thiz)
{

	FILE *fp;
	int i;
	fp = fopen("/sdcard/elementworks/customele.txt", "w");

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

Java_sand_falling_opengl_DemoActivity_setPassword(JNIEnv *env, jobject thiz, jbyteArray minut)
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

Java_sand_falling_opengl_DemoActivity_setUserName(JNIEnv *env, jobject thiz, jbyteArray minut)
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

//TODO: Implement these
int Java_sand_falling_opengl_DemoActivity_login(JNIEnv *env, jobject thiz)
{
	buildbuffer(3);
	if(!sendbuffer())
	{
		return -1;
	}

	return 0;
}
int Java_sand_falling_opengl_DemoActivity_register(JNIEnv *env, jobject thiz)
{
	buildbuffer(2);
	if(!sendbuffer())
	{
		return -1;
	}


	return 0;
}

int sendbuffer(void)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //Create a socket using IPv4 and TCP
	if(sockfd < 0)
	{
		error = "Could not create socket";
		return 0;
	}
	server = gethostbyname("71.244.112.67"); //Create the hostent using server IP
	bzero((char*) &serv_addr, sizeof(serv_addr)); //Clear the serv_addr struct
	serv_addr.sin_family = AF_INET; //Use IPv4
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length); //Use the hostent to fill the serv_addr struct
	serv_addr.sin_port = htons(PORTNUM); //Set up the port number using network order
	if(connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
	{
		error = "Could not connect";
		return 0;
	}

	while(bufferlength > 0)
	{
		n = write(sockfd, buffer, bufferlength);
		if(n < 0)
		{
			error = "Could not write to socket";
			return 0;
		}
		bufferlength -= n;
	}

	return 1;
}

char* Java_sand_falling_opengl_DemoActivity_viewerr (JNIEnv *env, jobject thiz)
{
	return error;
}
