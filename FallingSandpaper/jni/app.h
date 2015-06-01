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
//Include KamCord stuff
#include <Kamcord-C-Interface.h>

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
        char allowMovingTransition;
        char state;
        char startingTemp, lowestTemp, highestTemp;
        struct Element* lowerElement;
        struct Element* higherElement;

        //Dealing with drawing
        char red, green, blue;

        //Properties
        int specials[MAX_SPECIALS];
        int specialVals[MAX_SPECIALS];
        char collisions[NUM_BASE_ELEMENTS];  // Only for customs
        char base; //Only for customs
        char density;
        int fallVel;
        char inertia;
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
    extern short a_xVel[];
    extern short a_yVel[];
    extern char a_heat[];
    extern int* a_specialVals[];
    extern struct Element* a_element[];
    extern char a_frozen[];
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

    extern unsigned char brushSize;
    extern unsigned char zoomFactor;

    extern unsigned char filterType;

//A map of all the points (a two-dimensional variable-size array)
    extern int* allCoords;

//Mouse positions
    extern short mouseX;
    extern short mouseY;
//Old mouse positions
    extern short lastMouseX;
    extern short lastMouseY;

    extern int randOffset;

//Array for bitmap drawing
    extern unsigned char* colors;
    extern unsigned char* colorsFrameBuffer;

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
    extern char reciprocals[NUM_COLLISIONS];

//Set when a mouse update is requested, unset when udpated
    extern char shouldUpdateMouse;

    //For pinch to zoom/ panning
    extern float zoomScale;
    extern float centerX;
    extern float centerY;
    extern float viewWidth;
    extern float viewHeight;
    extern char isPinch;
    extern char isPanMode;

//Gravity values
    extern float xGravity;
    extern float yGravity;


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

    extern pthread_mutex_t mouse_mutex;

#endif // !APP_H_INCLUDED
