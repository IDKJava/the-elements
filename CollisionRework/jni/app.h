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

//The simple framework expects the application code to define these functions.
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
//Size variable initialize it here so we don't have to do it in resetup and we can keep our brush size
extern int size;

//Array for bitmap drawing
extern unsigned char colors[];

//Screen dimensions
extern int screenWidth;
extern int screenHeight;
//Workspace dimensions
extern int workWidth;
extern int workHeight;

//Coordinates
extern float x[];
extern float y[];
//Old coordinates (for collision resolving)
extern short int oldx[];
extern short int oldy[];
//Velocities
extern short int xvel[];
extern short int yvel[];

//Element type
extern char element[];
//Frozen state
extern char frozen[];
//Spawn type
extern char spawn[];

//RGB properties
extern unsigned char red[];
extern unsigned char green[];
extern unsigned char blue[];
//Fall velocity property
extern int fallVel[];
//Density property - 1 - 10
extern int density[];
//State property - solid = 0, liquid = 1, gaseous = 2
extern int state[];
//Special property - indexed special effects occurring every fram
extern int special[];
//Special value - a number used in special effects (if any)
extern int specialVal[];
//Heat value - 1 - 10
extern int heat[];

//Collision matrix
extern int collision[][];

//Index set state
extern int set[];
//Index available state
extern int avail[];

//Location in avail array
extern int loq;
//Zoom value
extern int zoom;

/* Accelerometer stuff being taken out for now
//Gravity values
float gravX;
float gravY;

//Accelerometer control state
int accelcon;
//Flipped state
int flipped;
*/
//Screen dimensions
int screenWidth;
int screenHeight;
//Workspace dimensions
int workWidth;
int workHeight;
//Set when a clear is requested, unset when cleared
extern char shouldClear;
//Set when a mouse update is requested, unset when udpated
extern char shouldUpdateMouse;

//A map of all the pixels
extern int allcoords[][];

//Mouse positions
extern int mouseX;
extern int mouseY;
//Old mouse positions
extern int lastMouseX;
extern int lastMouseY;
//Finger down state
extern int fingerState;

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
