/*
 * gl.c
 * --------------------------
 * Defines the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#include "gl.h"
#include <android/log.h>

unsigned int textureID;

float vertices[] =
{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
float texture[] =
{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
unsigned char indices[] =
{0, 1, 3, 0, 3, 2};
int texWidth, texHeight, stupidTegra;


void glInit()
{
	//Set some properties
	glShadeModel(GL_FLAT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	//Generate the new texture
	glGenTextures(1, &textureID);
	//Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	//Disable depth testing
	glDisable(GL_DEPTH_TEST);

	//Enable the vertex and coord arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	//Set tex params
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//Set up texWidth and texHeight
	texWidth = 1;
	texHeight = 1;
	while((texWidth = texWidth << 1) < screenWidth);
	while((texHeight = texHeight << 1) < screenHeight);


	//Allocate the dummy array
	char* emptyPixels = malloc(3 * texWidth*texHeight * sizeof(char));
	//Generate the tex image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyPixels);
	//Free the dummy array
	free(emptyPixels);

	//Set the pointers
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texture);
}

void glRender()
{
	//Check for changes in screen dimensions or work dimensions and handle them
	//char buffer[100];
	//sprintf( buffer,"sw: %d, sh: %d, tw: %d, th: %d, wW: %d, wH: %d ",screenWidth, screenHeight,texWidth,texHeight, workWidth, workHeight );
	//__android_log_write( ANDROID_LOG_INFO, "ThElements", buffer );

	if(dimensionsChanged)
	{
		__android_log_write( ANDROID_LOG_INFO, "ThElements", "dimensions changed" );
		vertices[2] = (float) screenWidth;
		vertices[5] = (float) screenHeight;
		vertices[6] = (float) screenWidth;
		vertices[7] = (float) screenHeight;

		texture[2] = (float) workWidth/texWidth;
		texture[5] = (float) workHeight/texHeight;
		texture[6] = (float) workWidth/texWidth;
		texture[7] = (float) workHeight/texHeight;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (!flipped)
		{
			glOrthof(0, screenWidth, screenHeight, 0, -1, 1); //--Device
		}
		else
		{
			glOrthof(0, screenWidth, 0, -screenHeight, -1, 1); //--Emulator
		}

		dimensionsChanged = FALSE;
		zoomChanged = FALSE;
		//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "Dimensions changed");

	}
	else if(zoomChanged)
	{
		texture[2] = (float) workWidth/texWidth;
		texture[5] = (float) workHeight/texHeight;
		texture[6] = (float) workWidth/texWidth;
		texture[7] = (float) workHeight/texHeight;

		zoomChanged = FALSE;
		//__android_log_write(ANDROID_LOG_INFO, "MainActivity", "zoom changed");
	}
	//__android_log_write(ANDROID_LOG_INFO, "TheElements", "updateview begin");
	pthread_mutex_lock(&update_mutex);
	UpdateView();
	pthread_mutex_unlock(&update_mutex);
	//__android_log_write(ANDROID_LOG_INFO, "TheElements", "updateview end");

	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	//Sub the work portion of the tex(~.025s -- Droid)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, stupidTegra, workHeight, GL_RGB, GL_UNSIGNED_BYTE, colors);

	//Actually draw the rectangle with the text on it (~.015s -- Droid)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
}
