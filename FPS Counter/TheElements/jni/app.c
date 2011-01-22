/*
 * app.c
 * -------------------------
 * Contains definitions for all of the global
 * variables used in the JNI part of the code.
 */

//Include the global macros
#include "macros.h"
//Include the server functions
#include "server.h"

// The simple framework expects the application code to define these functions.
void appInit();
void appDeinit();
void appRender();

/* Value is non-zero when application is alive, and 0 when it is closing.
 * Defined by the application framework.
 */
int gAppAlive;

//Current element selected
int celement = 0;
//Current point during processing
int cpoint = 0;
//Play state
int play = 1;
//Size variable initialize it here so we don't have to do it in resetup and we can keep our brush size
int size = 4;
int FPS = 0;

//Array for bitmap drawing
unsigned char colors[TPixels*3]; // 3 bytes per pixel

//Coordinates
float x[TPoints];
float y[TPoints];
//Old coordinates (for collision resolving)
short int oldx[TPoints];
short int oldy[TPoints];
//Velocities
short int xvel[TPoints];
short int yvel[TPoints];

//Element type
char element[TPoints];
//Frozen state
char frozen[TPoints];
//Spawn type
char spawn[TPoints];

//RGB properties
unsigned char red[TElements];
unsigned char green[TElements];
unsigned char blue[TElements];
//Fall velocity property
int fallvel[TElements];
//Density property
int density[TElements];
//Solid property
int solid[TElements];
//Growing property
int growing[TElements];
//Condensing property
int condensing[TElements];
//Fire-like burning property
int fireburn[TElements];
//Explosiveness property
int exploness[TElements];

//Custom element collision data
int colliseelement1[TCollision];

//Collision matrix
int collision[TElements][TElements];

//Index set state
int set[TPoints];
//Index available state
int avail[TPoints];

//Location in avail array
int loq;
//Zoom value
int screensize;

//Gravity values
float gravx;
float gravy;

//Accelerometer control state
int accelcon;
//Flipped state
int flipped;

//The extent of the screen (what area to draw in)
int maxx;
int maxy;

char shouldClear = 0;
// A map of all the coordinates on the screen
int allcoords[WIDTH][HEIGHT];

//Mouse positions
int xm;
int ym;
//Old mouse positions
int lmx;
int lmy;
//Finger down state
int fd;

//Buffer building variables
char username[8];
char password[8];
char userlength;
char passlength;
char buffer[ 1/*3 + 1 + (2 * TPoints * 4) + 200*/];
int bufferlength;

//Error variable
char* error;

//Socket variables
int sockfd; //The file descriptor for the socket
int n; //Used in sending and recieving data
struct sockaddr_in serv_addr; //The server address struct
struct hostent *server; //Pointer to a hostent struct that is used to set up serv_addr
