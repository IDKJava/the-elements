/*
 * app.h
 * --------------------------------
 * Contains all the extern declarations for
 * our variables, so that they are essentially
 * global external variables accessible from
 * all files necessary.
 */

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

//Include the global macros
#include "macros.h"

// The simple framework expects the application code to define these functions.
extern void appInit();
extern void appDeinit();
extern void appRender();

/* Value is non-zero when application is alive, and 0 when it is closing.
 * Defined by the application framework.
 */
extern int gAppAlive;

//Current element selected
extern int celement;
//Current point during processing
extern int cpoint;
//Play state
extern int play;
//Size variable
extern int size;

//Array for bitmap drawing
extern unsigned char colors[TPixels*3]; // 3 bytes per pixel

//Coordinates
extern float x[TPoints];
extern float y[TPoints];
//Old coordinates (for collision resolving)
extern short int oldx[TPoints];
extern short int oldy[TPoints];
//Velocities
extern short int xvel[TPoints];
extern short int yvel[TPoints];

//Element type
extern char element[TPoints];
//Frozen state
extern char frozen[TPoints];
//Spawn type
extern char spawn[TPoints];

//RGB properties
extern unsigned char red[TElements];
extern unsigned char green[TElements];
extern unsigned char blue[TElements];
//Fall velocity property
extern int fallvel[TElements];
//Density property
extern int density[TElements];
//Solid property
extern int solid[TElements];
//Growing property
extern int growing[TElements];
//Condensing property
extern int condensing[TElements];
//Fire-like burning property
extern int fireburn[TElements];
//Explosiveness property
extern int exploness[TElements];

//Custom element collision data
extern int colliseelement1[TCollision];

//Collision matrix
extern int collision[TElements][TElements];

//Index set state
extern char set[TPoints];
//Index available state
extern short int avail[TPoints];

//Location in avail array
extern int loq;
//Zoom value
extern int screensize;

//Gravity values
extern float gravx;
extern float gravy;

//Accelerometer control state
extern int accelcon;
//Flipped state
extern int flipped;

//The extent of the screen (what area to draw in)
extern int maxx;
extern int maxy;

// A map of all the coordinates on the screen
extern int allcoords[WIDTH][HEIGHT];

//Mouse positions
extern int xm;  
extern int ym;
//Old mouse positions
extern int lmx;
extern int lmy;
//Finger down state
extern int fd;

//Buffer building variables
extern char username[8];
extern char password[8];
extern char userlength;
extern char passlength;
extern char buffer[3 + 1 + (2 * TPoints * 4) + 200];
extern int bufferlength;

//Error variable
extern char* error;

//Socket variables
extern int sockfd; //The file descriptor for the socket
extern int n; //Used in sending and recieving data
extern struct sockaddr_in serv_addr; //The server address struct
extern struct hostent *server; //Pointer to a hostent struct that is used to set up serv_addr

#ifdef __cplusplus
}
#endif


#endif // !APP_H_INCLUDED
