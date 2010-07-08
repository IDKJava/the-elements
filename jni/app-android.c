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

#define TPoints 500000
#define TElements 25
#define TPixels 512*1024 //144826 for g1, 390385 for droid
#define BLACK -16777216
#define TCollision 28

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
//Array for bitmap drawing
unsigned char colors[TPixels * 3]; //3 bytes/pixel

//coordinates of all the pieces 
float x[TPoints];
float y[TPoints];

int flipped = 0;

int maxx;
int maxy;

//old coordinates of the sand, used for collision resolving
int oldx[TPoints];
int oldy[TPoints];
int delete[TPoints];

float gravx = 0; //xgravity
float gravy = 0; //ygravity (9.8 m/s^2 usually)

// what type of element each piece of sand is
int element[TPoints];

//what color each element corresponds to
//           sand,water,wall,eraser,plant,fire,ice,generator,spawn,Oil,Magma,Stone,C4,C4++,fuse,destructable_wall,wind,acid,steam,salt,salt-water,custom 1,mud,3
unsigned char red[] =
{ 255, 0, 157, 0, 0, 255, 200, 255, 255, 143, 204, 100, 255, 255, 135, 176, 0,
		150, 220, 255, 50, 200, 0, 49, 255 };
unsigned char green[] =
{ 213, 0, 157, 0, 255, 0, 200, 255, 255, 80, 51, 100, 255, 211, 25, 176, 0,
		255, 220, 255, 100, 255, 213, 35, 213 };
unsigned char blue[] =
{ 85, 255, 157, 0, 0, 0, 255, 255, 255, 80, 0, 100, 143, 115, 30, 164, 0, 50,
		255, 230, 200, 255, 85, 7, 85 };

//fall velocity of each type
int
		fallvel[] =
		{ 1, 1, 0, 0, 0, -1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, -1, 1, -1, 1, 1, 0,
				1, 1, 1 };

//collision data (LOAD FROM A FILE MAYBE?)
//-1 -eraser stuff, shouldn't happen
// 0 -solid solid
// 1 -solid liquid
// 2 -liquid solid -- DNE ANYMORE
// 3 -liquid liquid
// 4 -water plant
// 5 -nonflammable fire
// 6 -flammable fire
// 7 -fire fire
// 8 -ice water
// 9 -fire ice
//10 -hot ice
//11 -oil hot
//12 -magma water
//13 -stone hot
//14 -stone stone
//15 -hot C4
//16 -hot C4++
//17 -magma destructable wall
//18 -acid meltable
//19 -acid water
//20 -steam ice/water
//21 -steam steam
//22 -steam other
//23 -water fire
//24 -salt water
//25 -salt ice
//26 -salt-water plant
//27 -sand-hot


//array  of what element the custom element 1 should be in a collision with an element
int colliseelement1[TElements] =
{

//Sand 0
		0,
		//Water 1
		0,
		//Wall 2
		0,
		//Eraser 3
		0,
		//Plant 4
		0,
		//Fire 5
		0,
		//Ice 6
		0,
		//Generator 7
		0,
		//Spawn 8
		0,
		//Oil 9
		0,
		//Magma 10
		0,
		//Stone 11
		0,
		//C4 12
		0,
		//C4++ 13
		0,
		//Fuse 14
		0,
		//Destructable Wall 15
		0,
		//Wind 16
		0,
		//Acid 17
		0,
		//Steam 18
		0,
		//Salt 19
		0,
		//Salt-water 20
		0,
		//Glass 21
		0,
		//Custom 1 22
		0,
		//Custom 2 23
		0,
		//Custom 3 24
		0 };
int collision[TElements][TElements] =
{
//Sand 0
		{ 0, 28, 0, -1, 0, 27, 0, 10, 0, 1, 27, 0, 0, 0, 0, 0, -1, 18, 22, 0, 1,
				0, 0, 0, 0 },
		//Water 1
		{ 28, 3, 1, -1, 4, 23, 8, 10, 1, 3, 12, 1, 1, 1, 1, 1, -1, 19, 20, 24,
				3, 1, 1, 1, 1 },
		//Wall 2
		{ 0, 1, 0, -1, 0, 5, 0, 10, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, 22, 0, 1, 0,
				0, 0, 0 },
		//Eraser 3
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1 },
		//Plant 4
		{ 0, 1, 0, -1, 0, 6, 0, 10, 0, 1, 6, 0, 0, 0, 0, 0, -1, 18, 22, 0, 26,
				0, 0, 0, 0 },
		//Fire 5
		{ 5, 23, 5, -1, 6, 7, 9, 10, 5, 11, 5, 13, 15, 16, 6, 5, -1, 5, 22, 5,
				23, 5, 5, 29, 5 },
		//Ice 6
		{ 0, 8, 0, -1, 0, 9, 0, 10, 0, 1, 9, 0, 0, 0, 0, 0, -1, 18, 20, 25, 25,
				0, 0, 0, 0 },
		//Generator 7
		{ 10, 10, 10, -1, 10, 10, 10, 0, 0, 10, 10, 10, 10, 10, 10, 10, -1, 10,
				10, 10, 10, 10, 10, 10, 10 },
		//Spawn 8
		{ 0, 1, 0, -1, 0, 5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, 22, 0, 1, 0,
				0, 0, 0 },
		//Oil 9
		{ 1, 3, 1, -1, 1, 11, 1, 10, 1, 3, 11, 1, 1, 1, 1, 1, -1, 18, 22, 1, 3,
				1, 1, 1, 1 },
		//Magma 10
		{ 27, 12, 1, -1, 6, 5, 9, 10, 1, 11, 3, 13, 15, 16, 6, 17, -1, 18, 22,
				1, 12, 1, 1, 30, 1 },
		//Stone 11
		{ 0, 1, 0, -1, 0, 13, 0, 10, 0, 1, 13, 14, 0, 0, 0, 0, -1, 18, 22, 0,
				1, 0, 0, 0, 0 },
		//C4 12
		{ 0, 1, 0, -1, 0, 15, 0, 10, 0, 1, 15, 0, 0, 0, 0, -1, 18, 22, 0, 1, 0,
				0, 0, 0 },
		//C4++ 13
		{ 0, 1, 0, -1, 0, 16, 0, 10, 0, 1, 16, 0, 0, 0, 0, -1, 18, 22, 0, 1, 0,
				0, 0, 0 },
		//Fuse 14
		{ 0, 1, 0, -1, 0, 6, 0, 10, 0, 1, 6, 0, 0, 0, 0, 0, -1, 18, 22, 0, 1,
				0, 0, 0, 0 },
		//Destructable Wall 15
		{ 0, 1, 0, -1, 0, 5, 0, 10, 0, 1, 17, 0, 0, 0, 0, 0, -1, 18, 22, 0, 1,
				0, 0, 0, 0 },
		//Wind 16
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1 },
		//Acid 17
		{ 18, 19, 1, -1, 18, 5, 18, 10, 1, 18, 18, 18, 18, 18, 18, 18, -1, 3,
				22, 18, 18, 1, 18, 18, 18 },
		//Steam 18
		{ 22, 20, 22, -1, 22, 5, 20, 10, 22, 22, 22, 22, 22, 22, 22, 22, -1,
				22, 21, 22, 20, 22, 22, 22, 22 },
		//Salt 19
		{ 0, 24, 0, -1, 0, 5, 25, 10, 0, 1, 1, 0, 0, 0, 0, 0, -1, 18, 22, 0, 1,
				0, 0, 0, 0 },
		//Salt-water 20
		{ 1, 3, 1, -1, 26, 23, 25, 10, 1, 3, 12, 1, 1, 1, 1, 1, -1, 18, 20, 1,
				3, 0, 1, 1, 1 },
		//Glass 21
		{ 0, 1, 0, -1, 0, 5, 0, 10, 0, 1, 1, 0, 0, 0, 0, 0, -1, 1, 22, 0, 1, 0,
				0, 0, 0 },
		//Custom 1 22
		{ 0, 1, 0, -1, 0, 27, 0, 10, 0, 1, 27, 0, 0, 0, 0, 0, -1, 18, 22, 0, 1,
				0, 0, 0, 0 },
		//Mud 2 23
		{ 0, 1, 0, -1, 0, 29, 0, 10, 0, 1, 30, 0, 0, 0, 0, 0, -1, 18, 22, 0, 1,
				0, 0, 0, 0 },
		//Custom 3 24
		{ 0, 1, 0, -1, 0, 27, 0, 10, 0, 1, 27, 0, 0, 0, 0, 0, -1, 18, 22, 0, 1,
				0, 0, 0, 0 } };

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


//densities of elements
int density[] =
{
//Sand:
		16,
		//Water:
		10,
		//Wall:
		30,
		//Eraser:
		-1,
		//Plant:
		15,
		//Fire:
		-1,
		//Ice:
		9,
		//Generator:
		30,
		//Spawn:
		30,
		//Oil:
		5,
		//Magma:
		15,
		//Stone:
		30,
		//C4:
		30,
		//C4++:
		30,
		//Fuse:
		15,
		//Wall, Destructible:
		30,
		//Wind:
		-1,
		//Acid:
		8,
		//Steam:
		1,
		//Salt:
		30,
		//Salt-Water:
		11,
		//Glass:
		30,
		//Custom 1:
		16,
		//Custom 2:
		16,
		//Custom 3:
		16 };
//is solid?
int solid[] =
{
//Sand:
		0,
		//Water:
		0,
		//Wall:
		1,
		//Eraser:
		0,
		//Plant:
		1,
		//Fire:
		0,
		//Ice:
		0,
		//Generator:
		0,
		//Spawn:
		0,
		//Oil:
		0,
		//Magma:
		0,
		//Stone:
		0,
		//C4:
		0,
		//C4++:
		0,
		//Fuse:
		1,
		//Wall, Destructible:
		1,
		//Wind:
		0,
		//Acid:
		0,
		//Steam:
		0,
		//Salt:
		0,
		//Salt-Water:
		0,
		//Glass:
		0,
		//Custom 1:
		0,
		//Custom 2:
		0,
		//Custom 3:
		0 };
//is solid?
int growing[] =
{
//Sand:
		0,
		//Water:
		0,
		//Wall:
		0,
		//Eraser:
		0,
		//Plant:
		0, // plant has it's own stuff
		//Fire:
		0,
		//Ice:
		1,
		//Generator:
		0,
		//Spawn:
		0,
		//Oil:
		0,
		//Magma:
		0,
		//Stone:
		0,
		//C4:
		0,
		//C4++:
		0,
		//Fuse:
		0,
		//Wall, Destructible:
		0,
		//Wind:
		0,
		//Acid:
		0,
		//Steam:
		0,
		//Salt:
		0,
		//Salt-Water:
		0,
		//Glass:
		0,
		//Custom 1:
		0,
		//Custom 2:
		0,
		//Custom 3:
		0 };
//is condensing?
int condensing[] =
{
//Sand:
		-1,
		//Water:
		-1,
		//Wall:
		-1,
		//Eraser:
		-1,
		//Plant:
		-1, // plant has it's own stuff
		//Fire:
		-1,
		//Ice:
		-1,
		//Generator:
		-1,
		//Spawn:
		-1,
		//Oil:
		-1,
		//Magma:
		-1,
		//Stone:
		-1,
		//C4:
		-1,
		//C4++:
		-1,
		//Fuse:
		-1,
		//Wall, Destructible:
		-1,
		//Wind:
		-1,
		//Acid:
		-1,
		//Steam:
		1,
		//Salt:
		-1,
		//Salt-Water:
		-1,
		//Glass:
		-1,
		//Custom 1
		-1,
		//Custom 2
		-1,
		//Custom 3
		-1 };
//does it burn stuff like fire?
int fireburn[] =
{
//Sand:
		0,
		//Water:
		0,
		//Wall:
		0,
		//Eraser:
		0,
		//Plant:
		0, // plant has it's own stuff
		//Fire:
		1,
		//Ice:
		0,
		//Generator:
		0,
		//Spawn:
		0,
		//Oil:
		0,
		//Magma:
		0,
		//Stone:
		0,
		//C4:
		0,
		//C4++:
		0,
		//Fuse:
		0,
		//Wall, Destructible:
		0,
		//Wind:
		0,
		//Acid:
		0,
		//Steam:
		0,
		//Salt:
		0,
		//Salt-Water:
		0,
		//Glass:
		0,
		//Custom 1
		0,
		//Custom 2
		0,
		//Custom 3
		0 };
//how much explosiveness?
int exploness[] =
{
//Sand:
		0,
		//Water:
		0,
		//Wall:
		0,
		//Eraser:
		0,
		//Plant:
		0, // plant has it's own stuff
		//Fire:
		0,
		//Ice:
		0,
		//Generator:
		0,
		//Spawn:
		0,
		//Oil:
		0,
		//Magma:
		0,
		//Stone:
		0,
		//C4:
		10,
		//C4++:
		20,
		//Fuse:
		0,
		//Wall, Destructible:
		0,
		//Wind:
		0,
		//Acid:
		0,
		//Steam:
		0,
		//Salt:
		0,
		//Salt-Water:
		0,
		//Glass:
		0,
		//Custom 1
		0,
		//Custom 2
		0,
		//Custom 3
		0 };
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

static int sWindowWidth = 320;
static int sWindowHeight = 480;
static int sDemoStopped = 0;
static long sTimeOffset = 0;
static int sTimeOffsetInit = 0;
static long sTimeStopped = 0;

static long _getTime(void)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	return (long) (now.tv_sec * 1000 + now.tv_usec / 1000);
}

void Java_sand_falling_opengl_DemoRenderer_nativeInit(JNIEnv* env) //Initialize graphics
{
	importGLInit();
	appInit();
	gAppAlive = 1;
	sTimeOffsetInit = 0;
}

void Java_sand_falling_opengl_DemoRenderer_nativeResize(JNIEnv* env,
		jobject thiz, jint w, jint h)
{
	sWindowWidth = w;
	sWindowHeight = h;
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
void Java_sand_falling_opengl_DemoActivity_nativePause(JNIEnv* env)
{
	sDemoStopped = !sDemoStopped;
	if (sDemoStopped)
	{
		/* we paused the animation, so store the current
		 * time in sTimeStopped for future nativeRender calls */
		saver(1);
	}
	else
	{
		/* we resumed the animation, so adjust the time offset
		 * to take care of the pause interval. */
		loader(1);
	}
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
	appRender(sWindowWidth, sWindowHeight, colors);
}

Java_sand_falling_opengl_DemoActivity_setup(JNIEnv* env, jobject thiz)
{
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
	}
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
		rsetup();
	}
	else if (colorcode == 1)
	{
		red[3] = 255;
		blue[3] = 255;
		green[3] = 255;
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
Java_sand_falling_opengl_DemoActivity_loaddemo(JNIEnv* env, jobject thiz)
{
	FILE *fp;
	fp = fopen("/sdcard/save2.txt", "r");
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

