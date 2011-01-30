/*
 * gl.c
 * --------------------------
 * Defines the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#include "gl.h"
#include <android/log.h>

static unsigned int textureID;

float vertices[] =
{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
float texture[] =
{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
unsigned char indices[] =
{0, 1, 3, 0, 3, 2};
int texWidth = 1, texHeight = 1;

// Called from the app framework. is onSurfaceCreated
void glInit()
{
	glShadeModel(GL_FLAT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	//glClearDepthf(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	//Generate the new texture
	glGenTextures(1, &textureID);
	//Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Enable 2D texturing
	glEnable(GL_TEXTURE_2D);

	//Enable the vertex and coord arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	//Set tex params
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Different possible texture parameters, e.g
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//Create a dummy array
	while((texWidth << 1) < screenWidth) {}
	while((texHeight << 1) < screenHeight) {}
	char* emptyPixels = malloc(3 * texWidth*texHeight * sizeof(char));

	//Generate the tex image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyPixels);

	//Free the dummy array
	free(emptyPixels);

	//Set the pointers
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texture);
}

// Called from the app framework.
void glRender()
{
	if(dimensionsChanged)
	{
		vertices[2] = (float) screenWidth;
		vertices[5] = (float) screenHeight;
		vertices[6] = (float) screenWidth;
		vertices[7] = (float) screenHeight;

		texture[2] = (float) workWidth/texWidth;
		texture[5] = (float) workHeight/texHeight;
		texture[6] = (float) workWidth/texWidth;
		texture[7] = (float) workHeight/texHeight;

		dimensionsChanged = FALSE;
		zoomChanged = FALSE;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (!flipped)
		{
			glOrthof(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f, -1.0f); //--Device
		}
		else
		{
			glOrthof(0.0f, (float)screenWidth, 0.0f, (float)-screenHeight, 1.0f, -1.0f); //--Emulator
		}
	}
	else if(zoomChanged)
	{
		texture[2] = (float) workWidth/texWidth;
		texture[5] = (float) workHeight/texHeight;
		texture[6] = (float) workWidth/texWidth;
		texture[7] = (float) workHeight/texHeight;

		zoomChanged = FALSE;
	}
	__android_log_write(ANDROID_LOG_INFO, "TheElements", "Right before updateview");
	UpdateView();

	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Sub the work portion of the tex
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, workWidth, workHeight, GL_RGB, GL_UNSIGNED_BYTE, colors);

	//Actually draw the rectangle with the text on it
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

	//Disable the vertex and coord arrays (clean up)
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//Disable tex (clean up)
	//glDisable(GL_TEXTURE_2D);
}
