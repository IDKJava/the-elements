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

//The simple framework expects the application code to define these functions.
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
int play = PLAY;
//Size variable initialize it here so we don't have to do it in resetup and we can keep our brush size
int size = DEFAULT_BRUSH_SIZE;

//Array for bitmap drawing
unsigned char colors[];

//Screen dimensions
int screenWidth;
int screenHeight;
//Workspace dimensions
int workWidth;
int workHeight;

//Coordinates
float x[];
float y[];
//Old coordinates (for collision resolving)
short int oldx[];
short int oldy[];
//Velocities
short int xvel[];
short int yvel[];

//Element type
char element[];
//Frozen state
char frozen[];
//Spawn type
char spawn[];

//RGB properties
unsigned char red[];
unsigned char green[];
unsigned char blue[];
//Fall velocity property
int fallVel[];
//Density property - 1 - 10
int density[];
//State property - solid = 0, liquid = 1, gaseous = 2
int state[];
//Special property - indexed special effects occurring every fram
int special[];
//Special value - a number used in special effects (if any)
int specialVal[];
//Heat value - 1 - 10
int heat[];

//Collision matrix
int collision[][];

//Index set state
int set[];
//Index available state
int avail[];

//Location in avail array
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

//A map of all the pixelsmaxx
int allcoords[][];

//Mouse positions
int mouseX;
int mouseY;
//Old mouse positions
int lastMouseX;
int lastMouseY;
//Finger down state
int fingerState;

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
