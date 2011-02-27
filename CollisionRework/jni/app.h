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
//Include the server functions
#include "server.h"

/*
 * FUNCTIONS
 */

int getIndex(int x, int y);
int getColorIndex(int x, int y);

/*
 * STRUCTS
 */
 
struct Element
{
	//Index
	unsigned char index;
	//Name
	char* name;

	//Dealing with states
	char state;
	char startingTemp, lowestTemp, highestTemp;
	struct Element* lowerElement;
	struct Element* higherElement;
	
	//Dealing with drawing
	char red, green, blue;
	
	//Properties
	char* specials;
	char* specialVals;
	char density;
	signed char fallVel;
	char inertia;
};

struct Particle
{
	char set;
	float x, y, oldX, oldY;
	short xVel, yVel;
	char heat;
	char* specialVals;
	struct Element* element;
	char frozen;
	char hasMoved;
};

struct Atmosphere
{
	char heat;
	char gravity;

	unsigned char backgroundRed, backgroundGreen, backgroundBlue;

	char borderLeft, borderTop, borderRight, borderBottom;
};

/*
 * VARIABLES
 */

//An array of all the elements
extern struct Element** elements;
//The number of elements available
extern unsigned char numElements;
//An array of all the particles
extern struct Particle* particles[];
//A stack of available particles
extern struct Particle* avail[];
//Points to the index AFTER the top of the stack
extern int loq;
//Current element selected
extern struct Element* cElement;
//Atmosphere in use
extern struct Atmosphere* cAtmosphere;

//State variables
extern char play;
extern char zoom;
extern char flipped;
extern char fingerDown;
extern char accelOn;
extern char dimensionsChanged;
extern char zoomChanged;

extern unsigned char brushSize;

//A map of all the points (a two-dimensional variable-size array)
extern struct Particle** allCoords;

//Mouse positions
extern short mouseX;
extern short mouseY;
//Old mouse positions
extern short lastMouseX;
extern short lastMouseY;

//Array for bitmap drawing
extern unsigned char* colors;

//Screen dimensions
extern int screenWidth;
extern int screenHeight;
//Workspace dimensions
extern int workWidth;
extern int workHeight;

//Nearest power of 2 to workWidth - needed due to stupid Tegra 2.
extern int stupidTegra;

//Collision matrix
extern char collision[NUM_BASE_ELEMENTS][NUM_BASE_ELEMENTS];

//Set when a clear is requested, unset when cleared
extern char shouldClear;
//Set when a mouse update is requested, unset when udpated
extern char shouldUpdateMouse;

/* Accelerometer stuff being taken out for now
//Gravity values
extern float gravX;
extern float gravY;

//Accelerometer control state
//extern int accelcon;
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

#ifdef __cplusplus
}
#endif


#endif // !APP_H_INCLUDED
