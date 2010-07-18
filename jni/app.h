/* San Angeles Observation OpenGL ES version example
 * Copyright 2004-2005 Jetro Lauha
 * All rights reserved.
 * Web: http://iki.fi/jetro/ 
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
 *
 * $Id: app.h,v 1.5 2009/12/25 18:54:25 cvs Exp $
 * $Revision: 1.5 $
 */

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif


#define WINDOW_DEFAULT_WIDTH    640
#define WINDOW_DEFAULT_HEIGHT   480

#define WINDOW_BPP              16


#define TPoints 500000
#define TElements 25
#define TPixels 512*1024 //144826 for g1, 390385 for droid
#define BLACK -16777216
#define TCollision 28
// The simple framework expects the application code to define these functions.
extern void appInit();
extern void appDeinit();
extern void appRender(int width, int height, unsigned char colors);

/* Value is non-zero when application is alive, and 0 when it is closing.
 * Defined by the application framework.
 */
extern int gAppAlive;

extern int cpoint;
extern int play;
extern int size;

//Array for bitmap drawing
extern unsigned char colors[TPixels*3];   // 3 bytes per pixel

//coordinates of all the pieces 
extern float x[TPoints];
extern float y[TPoints];

//old coordinates of the sand, used for collision resolving
extern int oldx[TPoints];
extern int oldy[TPoints];

// what type of element each piece of sand is
extern int element[TPoints];
//whether we calculate collisions each round for a prticle
extern char frozen[TPoints];

//The red, green, and blue components of each element color
extern unsigned char red[TElements];    
extern unsigned char green[TElements];  
extern unsigned char blue[TElements];  
//fall velocity of each type
extern int fallvel[TElements];
extern int solid[TElements];
extern int growing[TElements];
extern int condensing[TElements];
extern int fireburn[TElements];
extern int exploness[TElements];

extern int colliseelement1[TCollision];

extern int collision[TElements][TElements];
extern int set[TPoints];
//Array of which points are available to keep CreatePoint fast
extern int avail[TPoints];
//Value to keep track of location in avail array
extern int loq;
extern int spawn[TPoints]; //Array of spawn elements for all spawns
extern int screensize; //zoomed in or not
extern int lmx; 
extern int lmy;
extern int lmx2;
extern int lmy2;
extern int lmy3;
extern int lmx3;
extern float gravx;
extern float gravy;
extern int accelcon; //0 for off, 1 for on

//densities of elements
extern int density[];


//velocities of each piece of sand
extern int xvel[TPoints]; 
extern int yvel[TPoints];
extern int maxx;
extern int maxy;
// A map of all the coordinates on the screen
extern int allcoords[900][900];    

//will be the mouse positions
extern int xm;  
extern int ym;
extern int flipped;

//finger down?
extern int fd;

//Current element selected
extern int celement;

#ifdef __cplusplus
}
#endif


#endif // !APP_H_INCLUDED


