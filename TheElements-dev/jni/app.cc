/*
 * app.c
 * -------------------------
 * Contains definitions for all of the global
 * variables used in the JNI part of the code.
 */

#include "app.h"

/*
 * VARIABLES
 */

char udid[MAX_UDID_LENGTH];
int versionCode;

struct Element** elements = NULL;
unsigned char numElements;
char a_set[MAX_POINTS];
float a_x[MAX_POINTS];
float a_y[MAX_POINTS];
float a_oldX[MAX_POINTS];
float a_oldY[MAX_POINTS];
float a_xVel[MAX_POINTS];
float a_yVel[MAX_POINTS];
unsigned char a_heat[MAX_POINTS];
int* a_specialVals[MAX_POINTS];
struct Element* a_element[MAX_POINTS];
unsigned long a_frozen[MAX_POINTS];
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

unsigned char filterType = FILTER_NONE;

int* allCoords = NULL;
float* gravityFieldX = NULL;
float* gravityFieldY = NULL;
int* gravityMask = NULL;
float* gravCoords = NULL;
float* gravMag = NULL;

int numSpaceObjs = 0;
SpaceObj spaceObjs[MAX_SPACE_OBJS];

short brushLocX = -1, brushLocY = -1, brushNextLocX = -1, brushNextLocY = -1;
bool brushOn = false;
int brushProps = BRUSH_NORMAL;
float slingshotX = 0.0, slingshotY = 0.0;

int randOffset = 0;

unsigned char* colors = NULL;
unsigned char* colorsFrameBuffer = NULL;

int rectValid = false;
float rectSX, rectSY, rectEX, rectEY;

int screenWidth;
int screenHeight;
int workWidth;
int workHeight;
int gfWidth;
int gfHeight;

int stupidTegra;

char shouldZoom = FALSE;
char shouldUpdateMouse = FALSE;


//For pinch to zoom/ panning
float zoomScale = 1.0;
float centerX = -1;
float centerY = -1;
float viewWidth = -1;
float viewHeight = -1;

//Gravity values
float xGravity;
float yGravity;

//World state
int world = WORLD_EARTH;

//OpenGL
int bhTexWidth = 0, bhTexHeight = 0;
char *bhTexPixels;
int whTexWidth = 0, whTexHeight = 0;
char *whTexPixels;
int chTexWidth = 0, chTexHeight = 0;
char *chTexPixels;
int ngTexWidth = 0, ngTexHeight = 0;
char *ngTexPixels;

int renderOverlayType = RT_RECT;

bool paidCameraOn = false;

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

pthread_mutex_t brush_mutex = PTHREAD_MUTEX_INITIALIZER;
