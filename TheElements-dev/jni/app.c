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
inline int getIndex(int x, int y)
{
    return y*workWidth + x;
}
//Used specifically for colors
inline int getColorIndex( int x, int y )
{
    return y*stupidTegra + x;
}

/*
 * VARIABLES
 */

char udid[MAX_UDID_LENGTH];
int versionCode;

struct Element** elements;
unsigned char numElements;
char a_set[MAX_POINTS];
float a_x[MAX_POINTS];
float a_y[MAX_POINTS];
float a_oldX[MAX_POINTS];
float a_oldY[MAX_POINTS];
short a_xVel[MAX_POINTS];
short a_yVel[MAX_POINTS];
char a_heat[MAX_POINTS];
int* a_specialVals[MAX_POINTS];
struct Element* a_element[MAX_POINTS];
char a_frozen[MAX_POINTS];
char a_hasMoved[MAX_POINTS];
int avail[MAX_POINTS];
int loq;
struct Element* cElement;
struct Atmosphere* cAtmosphere;

char play = TRUE;
char flipped = FALSE;
char fingerDown = FALSE;
char accelOn = FALSE;
char dimensionsChanged = TRUE;
char zoomChanged = TRUE;
int shouldClear = FALSE;

char borderTop = TRUE;
char borderBottom = TRUE;
char borderLeft = TRUE;
char borderRight = TRUE;

unsigned char brushSize = DEFAULT_BRUSH_SIZE;
unsigned char zoomFactor = DEFAULT_ZOOM_FACTOR;

int* allCoords = NULL;

short mouseX;
short mouseY;
short lastMouseX = -1;
short lastMouseY = -1;

int randOffset = 0;

unsigned char* colors = NULL;
unsigned char* colorsFrameBuffer = NULL;

int screenWidth;
int screenHeight;
int workWidth;
int workHeight;

int stupidTegra;

char collision[NUM_BASE_ELEMENTS][NUM_BASE_ELEMENTS];
char reciprocals[NUM_COLLISIONS];

char shouldZoom = FALSE;
char shouldUpdateMouse = FALSE;



//Gravity values
float xGravity;
float yGravity;

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

/*
 * THREADS
 */

int threadsInitialized = FALSE;
int bufferFree = TRUE;
int frameReady = FALSE;

pthread_mutex_t update_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t frame_ready_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t frame_ready_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_free_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_free_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mouse_mutex = PTHREAD_MUTEX_INITIALIZER;
