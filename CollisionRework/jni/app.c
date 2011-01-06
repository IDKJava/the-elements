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

/*
 * FUNCTIONS
 */

//Used to get the index for allcoords (since it's actually a two dimensional array, but we allocated it using malloc
int getIndex(int x, int y)
{
	return x*workWidth + y;
}

/*
 * VARIABLES
 */

//int gAppAlive; //I don't think this is needed, commenting...

//Current element selected
int cElement = 0;
//Current point during processing
int cPoint = 0;
//Play state
int play = PLAY;
//Size variable initialize it here so we don't have to do it in resetup and we can keep our brush size
int brushSize = DEFAULT_BRUSH_SIZE;

//The number of elements available
int numElements;

//Array for bitmap drawing (a variable-size array)
unsigned char *colors;

//Screen dimensions
int screenWidth;
int screenHeight;
//Workspace dimensions
int workWidth;
int workHeight;

//Coordinates
float x[MAX_POINTS];
float y[MAX_POINTS];
//Old coordinates (for collision resolving)
short int oldX[MAX_POINTS];
short int oldY[MAX_POINTS];
//Velocities
short int xVel[MAX_POINTS];
short int yVel[MAX_POINTS];

//Element type
char element[MAX_POINTS];
//Frozen state
char frozen[MAX_POINTS];

//RGB properties (variable arrays)
unsigned char *red;
unsigned char *green;
unsigned char *blue;
//Fall velocity property (a variable-size array)
int *fallVel;
//Density property - 1 - 10 (a variable-size array)
int *density;
//State property - solid = 0, liquid = 1, gaseous = 2 (a variable-size array)
int *state;
//Special property - indexed special effects occurring every frame (a variable-size array)
int *special;
//Draw solid property - if set to TRUE, will draw solid, if set to FALSE will draw randomized
int *drawSolid;

//Special value - a number used in special effects (if any)
int specialVal[MAX_POINTS];
//Heat value - 1 - 10
int heat[MAX_POINTS];

//Collision matrix (a two-dimensional variable-size array)
int *collision;

//Index set state
int set[MAX_POINTS];
//Index available state
int avail[MAX_POINTS];

//Points to the index AFTER the top of the stack
int loq;
//Zoom value
int zoom;

/* Accelerometer stuff being taken out for now
//Gravity values
float gravX;
float gravY;

//Accelerometer control state
int accelcon;
//Flipped state
int flipped;
*/

//Set when a clear is requested, unset when cleared
char shouldClear = FALSE;
//Set when a mouse update is requested, unset when udpated
char shouldUpdateMouse = FALSE;

//Finger state
int fingerState = FINGER_UP;

//A map of all the points (a two-dimensional variable-size array)
int *allcoords;

//Mouse positions
int mouseX;
int mouseY;
//Old mouse positions
int lastMouseX;
int lastMouseY;

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
