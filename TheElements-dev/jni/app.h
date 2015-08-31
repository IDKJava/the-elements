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

//Include the global macros
#include "macros.h"
//Include the server functions
#include "server.h"
//Include the pthread functions
#include <pthread.h>

/*
 * STRUCTS
 */
 
    struct Element
    {
        //Index
        unsigned char index;
        //Name
        char* name;
        //Filename, only for customs
        char* filename;

        //Dealing with phases
        unsigned char state;
        unsigned char startingTemp, lowestTemp, highestTemp;
        struct Element* lowerElement;
        struct Element* higherElement;

        //Dealing with drawing
        unsigned char red, green, blue;

        //Properties
        int specials[MAX_SPECIALS];
        int specialVals[MAX_SPECIALS];
        unsigned char collisions[NUM_BASE_ELEMENTS];  // Only for customs
        unsigned char base; //Only for customs
        unsigned char density;
        int fallVel;
        unsigned char inertia;
    };

    struct Atmosphere
    {
        char heat;
        char gravity;

        unsigned char backgroundRed, backgroundGreen, backgroundBlue;

        char borderLeft, borderTop, borderRight, borderBottom;
    };

    struct SpaceObj
    {
        int type;
        int x, y;
        int ex, ey; // Only used for null gravity
    };

/*
 * VARIABLES
 */

//Variables to track the user/app version
    extern char udid[];
    extern int versionCode;

//An array of all the elements
    extern struct Element** elements;
//The number of elements available
    extern unsigned char numElements;

    extern char a_set[];
    extern float a_x[];
    extern float a_y[];
    extern float a_oldX[];
    extern float a_oldY[];
    extern float a_xVel[];
    extern float a_yVel[];
    extern unsigned char a_heat[];
    extern int* a_specialVals[];
    extern struct Element* a_element[];
    extern unsigned long a_frozen[];
    extern char a_hasMoved[];




    

//A stack of available particles
    extern int avail[];
//Points to the index AFTER the top of the stack
    extern int loq;
//Current element selected
    extern struct Element* cElement;
//Atmosphere in use
    extern struct Atmosphere* cAtmosphere;

//State variables
    extern char play;
    extern char flipped;
    extern char fingerDown;
    extern char accelOn;
    extern char dimensionsChanged;
    extern char zoomChanged;
    extern int shouldClear;

    extern unsigned char zoomFactor;

    // Brush tool
    extern unsigned char brushSize;
    extern short brushLocX, brushLocY, brushNextLocX, brushNextLocY;
    extern bool brushOn;
    extern int brushProps;
    extern float slingshotX, slingshotY;

    extern unsigned char filterType;

//A map of all the points (a two-dimensional variable-size array)
    extern int* allCoords;

//Gravity field (space world)
    extern float* gravityFieldX;
    extern float* gravityFieldY;
    extern int* gravityMask;
    extern float* gravCoords; // Rendering only
    extern float* gravMag; // Rendering only

//Array of all space holes
    extern int numSpaceObjs;
    extern SpaceObj spaceObjs[];

    extern int randOffset;

//Array for bitmap drawing
    extern unsigned char* colors;
    extern unsigned char* colorsFrameBuffer;

//Currently drawing rect
    extern int rectValid;
    extern float rectSX, rectSY, rectEX, rectEY;

//Screen dimensions
    extern int screenWidth;
    extern int screenHeight;
//Workspace dimensions
    extern int workWidth;
    extern int workHeight;
//Gravity field dimensions
    extern int gfWidth;
    extern int gfHeight;

//Nearest power of 2 to workWidth - needed due to stupid Tegra 2.
    extern int stupidTegra;

//Collision matrix
    extern char collision[NUM_BASE_ELEMENTS][NUM_BASE_ELEMENTS];
    extern char reciprocals[NUM_COLLISIONS];

//Set when a mouse update is requested, unset when udpated
    extern char shouldUpdateMouse;

    //For pinch to zoom/ panning
    extern float zoomScale;
    extern float centerX;
    extern float centerY;
    extern float viewWidth;
    extern float viewHeight;

//Gravity values, for Earth world
    extern float xGravity;
    extern float yGravity;

//World
    extern int world;

//OpenGL variables
    extern int bhTexWidth, bhTexHeight;
    extern char *bhTexPixels;
    extern int whTexWidth, whTexHeight;
    extern char *whTexPixels;
    extern int chTexWidth, chTexHeight;
    extern char *chTexPixels;
    extern int ngTexWidth, ngTexHeight;
    extern char *ngTexPixels;

    extern int renderOverlayType;

    extern bool paidCameraOn;


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
//Index into gravity field array (space world)
//Params are GAME COORDS, not coordinates of the 2D array. I.e. don't divide
//the coordinates by GF_BLOCK_SIZE before passing in.
inline int getGravityIndex(int x, int y) {
    return (y/GF_BLOCK_SIZE)*gfWidth + (x/GF_BLOCK_SIZE);
}

/*
 * THREADS
 */

    extern int threadsInitialized;
    extern int bufferFree;
    extern int frameReady;

    extern pthread_mutex_t update_mutex;
    extern pthread_mutex_t frame_ready_mutex;
    extern pthread_cond_t frame_ready_cond;
    extern pthread_mutex_t buffer_free_mutex;
    extern pthread_cond_t buffer_free_cond;

    extern pthread_mutex_t brush_mutex;

#endif // !APP_H_INCLUDED
