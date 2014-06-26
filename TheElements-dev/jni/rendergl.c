/*
 * gl.c
 * --------------------------
 * Defines the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#include "rendergl.h"
#include <android/log.h>

#ifndef NDEBUG
#define LOGGING 0 // Debug
#else
#define LOGGING 0 // Release
#endif

#if LOGGING
// Timing of frames
#include <sys/time.h>

static struct timeval time1;
#endif
unsigned int fb;
unsigned int depthRb;
unsigned int textureID;

int mTextureCoordinateHandle;
int mTextureUniformHandle;


//float vertices[] =
//{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

float vertices[] = {0.0f,0.0f,
                    1.0f,0.0f,
                    1.0f,1.0f,
                    0.0f,0.0f,
                    1.0f,1.0f,
                    0.0f,1.0f};
float texture[] =
{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
unsigned char indices[] =
{0, 1, 3, 0, 3, 2};
int texWidth, texHeight, stupidTegra;
/*

void glInit()
{
	//Set some properties
//glShadeModel(GL_COLOR_BUFFER_BIT);
//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
 
    glGenFramebuffers(1, &fb);
    glGenRenderbuffers(1, &depthRb); // the depth buffer
	//Generate the new texture
	glGenTextures(1, &textureID);
	//Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Enable 2D texturing
	//glEnable(GL_TEXTURE_2D);
	//Disable depth testing
	//glDisable(GL_DEPTH_TEST);

	//Enable the vertex and coord arrays
    //glEnableVertexAttribArrays();
    //glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	//Set tex params
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//Set up texWidth and texHeight
	texWidth = 1;
	texHeight = 1;
	while((texWidth = texWidth << 1) < (screenWidth-screenWidth%zoomFactor));
	while((texHeight = texHeight << 1) < (screenHeight-screenHeight%zoomFactor));


	//Allocate the dummy array
	char* emptyPixels = malloc(3 * texWidth*texHeight * sizeof(char));
	//Generate the tex image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyPixels);
	//Free the dummy array
	free(emptyPixels);

	//Set the pointers
	//glVertexPointer(2, GL_FLOAT, 0, vertices);
	//glTexCoordPointer(2, GL_FLOAT, 0, texture);
}

void glRender()
{
    //Kamcord_BeginDraw();

#if LOGGING
    struct timeval time2;
    struct timeval time3;
    char buffer[20];
    double useconds;

    // TODO: First frame is wrong. We probably don't care...
    gettimeofday(&time2, NULL);
    useconds = (time2.tv_sec - time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec;
    snprintf(buffer, 20, "aFPS: %f", 1000000/useconds);
    __android_log_write(ANDROID_LOG_INFO, "TheElements", buffer);
    time1.tv_sec = time2.tv_sec;
    time1.tv_usec = time2.tv_usec;
#endif



	if(dimensionsChanged)
	{
		__android_log_write( ANDROID_LOG_INFO, "TheElements", "dimensions changed" );
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

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    float modViewWidth = viewWidth/2 * zoomScale;
    float modViewHeight = viewHeight/2 * zoomScale;
    float left = centerX - modViewWidth;
    float right = centerX + modViewWidth;
    float top = centerY - modViewHeight;
    float bottom = centerY + modViewHeight;
    if (!flipped)
    {
        glViewport(left,right, bottom, top);
        //glOrthof(left, right, bottom, top, -1.0f, 1.0f); //--Device
    }
    else
    {
        //glOrthof(left, right, bottom, -1*top, -1.0f, 1.0f); //--Emulator
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, &textureID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, &depthRb);

    //int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //if (status != GL_FRAMEBUFFER_COMPLETE)
    //{
        //This is an error
        //return;
//}


    glClearColor(1,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT );

	//Sub the work portion of the tex(~.025s -- Droid)
#if LOGGING
    gettimeofday(&time2, NULL);
#endif
glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, stupidTegra, workHeight, GL_RGB, GL_UNSIGNED_BYTE, colorsFrameBuffer);
#if LOGGING
    gettimeofday(&time3, NULL);
    useconds = (time3.tv_sec - time2.tv_sec)*1000000 + time3.tv_usec - time2.tv_usec;
    snprintf(buffer, 20, "tFPS: %f", 1000000/useconds);
    __android_log_write(ANDROID_LOG_INFO, "TheElements", buffer);
#endif

    //Actually draw the rectangle with the text on it (~.015s -- Droid)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    //Kamcord_EndDraw();
}

*/

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    GLint error = glGetError();
    for (; error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] = 
    "attribute vec4 vPosition;\n"
    "attribute vec2 a_TexCoordinate;\n"
    "varying vec2 v_TexCoordinate;\n"
    "void main() {\n"
    "  v_TexCoordinate = a_TexCoordinate;\n"
    "  gl_Position = vPosition;\n"
    "}\n";

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "varying vec2 v_TexCoordinate;\n"
    "uniform sampler2D u_Texture;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(v_TexCoordinate.x, v_TexCoordinate.y, 0.0, 1.0);"
    "}\n";

//    "  gl_FragColor = texture2D(u_Texture, v_TexCoordinate);\n"

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }

        mTextureCoordinateHandle = glGetAttribLocation(program, "a_TexCoordinate");
        mTextureUniformHandle = glGetUniformLocation(program, "u_Texture");
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;

void glInit() {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", screenWidth, screenHeight);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);

    glViewport(0, 0, screenWidth, screenHeight);
    checkGlError("glViewport");

    //Generate the new texture
	glGenTextures(1, &textureID);
    checkGlError("glGenTextures");
	//Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);
    checkGlError("glBindTexture");

    //Set tex params
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    checkGlError("glTexParam");
	
	//Set up texWidth and texHeight
	texWidth = 1;
	texHeight = 1;
	while((texWidth = texWidth << 1) < (screenWidth-screenWidth%zoomFactor));
	while((texHeight = texHeight << 1) < (screenHeight-screenHeight%zoomFactor));


	//Allocate the dummy array
	char* emptyPixels = malloc(3 * texWidth*texHeight * sizeof(char));
	//Generate the tex image
    //LOGI("Running the teximage2d");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyPixels);
    checkGlError("glTexImage2D");
	//Free the dummy array
	free(emptyPixels);
    return;
}

//const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
//0.5f, -0.5f };

void glRender() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");        

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    //glBindTexture(GL_TEXTURE_2D, textureID);
    checkGlError("gl binding");
    
    char* emptyPixels = malloc(3 * stupidTegra*workHeight * sizeof(char));
	//Generate the tex image

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, stupidTegra, workHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyPixels);

	//Free the dummy array
    free(emptyPixels);
    LOGI("Running the teximage2d");
    //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, stupidTegra, workHeight, GL_RGB, GL_UNSIGNED_BYTE, colorsFrameBuffer);
    checkGlError("glTexImage2D");
    //LOGI("Running the tex sub image");
    checkGlError("texsubimage2d");

    // Set the active texture unit to texture unit 0.
    glActiveTexture(GL_TEXTURE0);
 
    // Bind the texture to this unit.

 
    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glUniform1i(mTextureUniformHandle, 0);

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    LOGI("Running enable");
    glEnableVertexAttribArray(mTextureCoordinateHandle);
    LOGI("Running enable2");
    glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, false, 0, texture);
    
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 6);

    checkGlError("glDrawArrays");
}



void glRenderThreaded()
{
    // Synchronization
    pthread_mutex_lock(&frame_ready_mutex);
    while (!frameReady)
    {
        pthread_cond_wait(&frame_ready_cond, &frame_ready_mutex);
    }
    frameReady = FALSE;
    pthread_mutex_unlock(&frame_ready_mutex);

    glRender();
    pthread_mutex_lock(&buffer_free_mutex);
    if (!bufferFree)
    {
        bufferFree = TRUE;
        pthread_cond_signal(&buffer_free_cond);
    }
    pthread_mutex_unlock(&buffer_free_mutex);
}
