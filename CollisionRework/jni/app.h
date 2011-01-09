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

/*
 * FUNCTIONS
 */

int getIndex(int x, int y);

/*
 * STRUCTS
 */
 
struct Element;
struct Particle;

/*
 * VARIABLES
 */

//An array of all the elements
extern Element* elements;
//The number of elements available
extern unsigned char numElements;

//extern int gAppAlive; //I don't think this is needed, commenting...

//Current element selected
extern Element cElement;
//Current point during processing
extern Particle cPoint;
//Play state
extern char play;
//Size variable initialize it here so we don't have to do it in resetup and we can keep our brush size
extern char brushSize;

//Array for bitmap drawing
extern unsigned char* colors[];

//Screen dimensions
extern short screenWidth;
extern short screenHeight;
//Workspace dimensions
extern short workWidth;
extern short workHeight;

//Coordinates
//extern float x[];
//extern float y[];
//Old coordinates (for collision resolving)
//extern short int oldX[];
//extern short int oldY[];
//Velocities
//extern short int xVel[];
//extern short int yVel[];

//Element type
//extern char element[];
//Frozen state
//extern char frozen[];

//RGB properties (variable arrays)
//extern unsigned char *red;
//extern unsigned char *green;
//extern unsigned char *blue;
//Fall velocity property (a variable-size array)
//extern int *fallVel;
//Density property - 1-10 (a variable-size array)
//extern int *density;
//State property - solid = 0, liquid = 1, gaseous = 2 (a variable-size array)
//extern int *state;
//Special property - indexed special effects occurring every frame (a variable-size array)
//extern int *special;
//Draw solid property - if set to TRUE, will draw solid, if set to FALSE will draw randomized
//extern int *drawSolid;
//Inertia property - 0-10 - use this for giving velocities in explosions,
//also we will use for drag because I feel like it; -1 means unmovable (a variable-size array)
extern char *inertia;

//Special value - a number used in special effects (if any)
//extern int specialVal[];
//Heat value - 1-10
//extern int heat[];

//Collision matrix
extern char* collision;

//Index set state
//extern int set[];
//Index available state
extern Particle avail[];

//Points to the index AFTER the top of the stack
extern short loq;
//Zoom value
extern char zoom;

/* Accelerometer stuff being taken out for now
//Gravity values
extern float gravX;
extern float gravY;

//Accelerometer control state
//extern int accelcon;
//Flipped state
extern char flipped;
*/

//Screen dimensions
extern char screenWidth;
extern char screenHeight;
//Workspace dimensions
extern char workWidth;
extern char workHeight;
//Set when a clear is requested, unset when cleared
extern char shouldClear;
//Set when a mouse update is requested, unset when udpated
extern char shouldUpdateMouse;

//Finger state
extern char fingerState;

//A map of all the points (a two-dimensional variable-size array)
extern Particle* allCoords;

//Mouse positions
extern short mouseX;
extern short mouseY;
//Old mouse positions
extern short lastMouseX;
extern short lastMouseY;

/*Network stuff taken out for now
//Buffer building variables
char username[8];
char password[8];
char userlength;
char passlength;
char buffer[3 + 1 + (2 * TPoints * 4) + 200];
int bufferlength;

//Error variable
char* error;

//Socket variables
int sockfd; //The file descriptor for the socket
int n; //Used in sending and recieving data
struct sockaddr_in serv_addr; //The server address struct
struct hostent *server; //Pointer to a hostent struct that is used to set up serv_addr
*/

#ifdef __cplusplus
}
#endif


#endif // !APP_H_INCLUDED
