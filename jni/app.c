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
	return x*workWidth + y;
}

/*
 * VARIABLES
 */

struct Element** elements;
unsigned char numElements;
struct Particle** particles;
struct Particle** avail;
short loq;
struct Element* cElement;

char play;
char zoom;
unsigned char brushSize;
char flipped;
char fingerState;

struct Particle** allCoords;

short mouseX;
short mouseY;
short lastMouseX;
short lastMouseY;

unsigned char* colors;

short screenWidth;
short screenHeight;
short workWidth;
short workHeight;

char collision[NUM_BASE_ELEMENTS][NUM_BASE_ELEMENTS];

char shouldClear;
char shouldUpdateMouse;


/* Accelerometer stuff being taken out for now
//Gravity values
float gravX;
float gravY;

//Accelerometer control state
//int accelcon;
 */

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
