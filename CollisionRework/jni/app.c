/*
 * app.c
 * -------------------------
 * Contains definitions for all of the global
 * variables used in the JNI part of the code.
 */

#include "app.h"

/*
 * FUNCTIONS
 */

//Used to get the index for allcoords (since it's actually a two dimensional array, but we allocated it using malloc
int getIndex(int x, int y)
{
	return y*workWidth + x;
}
//Used specifically for colors
int getColorIndex( int x, int y )
{
	return y*stupidTegra + x;
}

/*
 * VARIABLES
 */

struct Element** elements;
unsigned char numElements;
struct Particle* particles[MAX_POINTS];
struct Particle* avail[MAX_POINTS];
int loq;
struct Element* cElement;
struct Atmosphere* cAtmosphere;

char play = TRUE;
char zoom = ZOOMED_IN;
char flipped = FALSE;
char fingerDown = FALSE;
char accelOn = FALSE;
char dimensionsChanged = TRUE;
char zoomChanged = TRUE;

char borderTop = TRUE;
char borderBottom = TRUE;
char borderLeft = TRUE;
char borderRight = TRUE;

unsigned char brushSize = DEFAULT_BRUSH_SIZE;

struct Particle** allCoords;

short mouseX;
short mouseY;
short lastMouseX;
short lastMouseY;

unsigned char* colors;

int screenWidth;
int screenHeight;
int workWidth;
int workHeight;

int stupidTegra;

char collision[NUM_BASE_ELEMENTS][NUM_BASE_ELEMENTS];

char shouldClear = FALSE;
char shouldZoom = FALSE;
char shouldUpdateMouse = FALSE;


//Accelerometer stuff being taken out for now
//Gravity values
float xGravity;
float yGravity;

//Accelerometer control state
//int accelcon;


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
