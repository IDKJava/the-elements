/*
 * gl.c
 * --------------------------
 * Defines the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#include "gl.h"

static unsigned int textureID;

// Called from the app framework. is onSurfaceCreated
void appInit()
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

	glEnable(GL_TEXTURE_2D);

	//Set tex params
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Different possible texture parameters, e.g
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate the tex image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, colors);

	//Disable tex (clean up)
	glDisable(GL_TEXTURE_2D);
}

// Called from the app framework.
void appDeinit()
{

}

// Called from the app framework.
void appRender()
{
	float vertices[] =
	{0.0f, 0.0f, (float)screenWidth, 0.0f, 0.0f, (float)screenHeight, (float)screenWidth, (float)screenHeight};

	float texture[] =
	{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

	unsigned char indices[] =
	{0, 1, 3, 0, 3, 2};

	UpdateView();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (flipped == 0)
	{
		glOrthof(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f, -1.0f); //--Device
	}
	else
	{
		glOrthof(0.0f, (float)screenWidth, 0.0f, (float)-screenHeight, 1.0f, -1.0f); //--Emulator
	}

	//Draw the texture stuff --------------------------------------------------------------
	glEnable(GL_TEXTURE_2D);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, colors);

	//Enable the vertex and coord arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Set the pointers
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texture);

	//Actually draw the rectangle with the text on it
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

	//Disable the vertex and coord arrays (clean up)
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//Disable tex (clean up)
	glDisable(GL_TEXTURE_2D);
}
