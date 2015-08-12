/*
 * gl.c
 * --------------------------
 * Defines the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#include "rendergl.h"
#include <android/log.h>
#include <vector>

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
unsigned int bhTex;

int mTextureCoordinateHandle;
int mTextureUniformHandle;
int mScreenSizeHandle;
int mProjMatrixUniformHandle;

int mGravProjMatrixUniformHandle;

float vertices[] = {0.0f,1.0f,
                    1.0f,1.0f,
                    1.0f,0.0f,
                    0.0f,1.0f,
                    1.0f,0.0f,
                    0.0f,0.0f};

float texture[] = {0.0f, 0.0f,
                   1.0f, 0.0f,
                   1.0f, 1.0f,
                   0.0f, 0.0f,
                   1.0f, 1.0f,
                   0.0f, 1.0f};

int texWidth, texHeight;

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

void checkGlError(const char* op) {
    GLint error = glGetError();
    for (; error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] =
    "attribute vec4 vPosition;\n"
    "attribute vec2 a_TexCoordinate;\n"
    "varying vec2 v_TexCoordinate;\n"
    "uniform mat4 u_MVPMatrix;\n"
    "void main() {\n"
    "  v_TexCoordinate = a_TexCoordinate;\n"
    "  gl_Position = vPosition;\n"
    "  gl_Position *= u_MVPMatrix;\n"
    "}\n";

static const char gFragmentShader[] =
    "precision mediump float;\n"
    "varying vec2 v_TexCoordinate;\n"
    "uniform vec2 u_screenSize;\n"
    "uniform sampler2D u_Texture;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(u_Texture, v_TexCoordinate);\n"
//    "  float myI, myJ;\n"
//    "  float diff = 0.0;\n"
//    "  for (myI = -3.0; myI <= 3.0; myI++) { \n"
//    "      for ( myJ = -3.0; myJ <= 3.0; myJ++) { \n"
//    "      float modX = min(max(v_TexCoordinate.x + (myI/u_screenSize.x),0.0),1.0);\n"
//    "      float modY = min(max(v_TexCoordinate.y + (myJ/u_screenSize.y),0.0),1.0);\n"
//    "      vec4 foundColor = texture2D(u_Texture, vec2(modX, modY));\n"
//    "      if ( foundColor.x != gl_FragColor.x ||"
//    "           foundColor.y != gl_FragColor.y || foundColor.z != gl_FragColor.z) {\n"
//    "         diff += (3.0-abs(myI)) + (3.0-abs(myJ));\n"
//    "      }\n"
//    "}\n"
//    "}\n"
//    " float boundDiff = min(diff*0.02, 0.5);"
//    " gl_FragColor *= (1.0 - boundDiff);\n"
    "}\n";

/*
    "  vec2 bottom = vec2(v_TexCoordinate.x, "
    "  min(v_TexCoordinate.y + 1.0f/u_screenSize.y,1.0));"
    "  vec2 top = vec2(v_TexCoordinate.x, "
    "  max(v_TexCoordinate.y - 1.0f/u_screenSize.y,0.0));"
    "  vec2 left = vec2(max(v_TexCoordinate.x - 1.0f/u_screenSize.x,0.0), "
    "  v_TexCoordinate.y);"
    "  vec2 right = vec2(min(v_TexCoordinate.x + 1.0f/u_screenSize.x,1.0), "
    "  v_TexCoordinate.y);"
*/

static const char gFieldVertexShader[] =
    "attribute vec4 vPosition;\n"
    "attribute vec4 aMag;\n"
    "varying vec4 vMag;\n"
    "uniform mat4 u_MVPMatrix;\n"
    "void main() {\n"
    "  vMag = aMag;\n"
    "  gl_Position = vPosition;\n"
    "  gl_Position *= u_MVPMatrix;\n"
    "}\n";

static const char gMagFragShader[] =
    "precision mediump float;\n"
    "varying vec4 vMag;\n"
    "void main() {\n"
    "  gl_FragColor = vMag[0]*vec4(10.0, 5.0, 0.0, 1.0) + vec4(0.2, 0.2, 0.2, 0.0);\n"
    "}\n";

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
        glAttachShader(program, pixelShader);
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
        mScreenSizeHandle = glGetUniformLocation(program, "u_screenSize");
        mTextureUniformHandle = glGetUniformLocation(program, "u_Texture");
        mProjMatrixUniformHandle = glGetUniformLocation(program, "u_MVPMatrix");
    }
    return program;
}

GLuint createGravityProgram(const char* pVertexSource, const char* pFragmentSource) {

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
        glAttachShader(program, pixelShader);
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

        mGravProjMatrixUniformHandle = glGetUniformLocation(program, "u_MVPMatrix");
    }
    return program;
}


//Makes an orthographic projection matrix
void setOthographicMat(float l, float r, float t, float b, float n, float f,  float mat[])
{
    // Matrix is derived from the glOrtho matrix:
    // https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
    mat[0] = 2.0 / (r - l);
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = -(r + l) / (r - l);

    mat[4] = 0;
    mat[5] = 2.0 / (t - b);
    mat[6] = 0;
    mat[7] = -(t + b) / (t - b);

    mat[8] = 0;
    mat[9] = 0;
    mat[10] = - 2.0 / (f - n);
    mat[11] = - (f+n) / (f - n);

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
}


GLuint gProgram;
GLuint gvPositionHandle;

GLuint gGravProgram;
GLuint gGravVPositionHandle;
GLuint gGravVMagHandle;

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
    gGravProgram = createGravityProgram(gFieldVertexShader, gMagFragShader);
    if (!gGravProgram) {
        LOGE("Could not create grav program.");
        return;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    gGravVPositionHandle = glGetAttribLocation(gGravProgram, "vPosition");
    gGravVMagHandle = glGetAttribLocation(gGravProgram, "aMag");

    glViewport(0, 0, screenWidth, screenHeight);
    //Generate the new texture
    glGenTextures(1, &textureID);
    glGenTextures(1, &bhTex);
    checkGlError("glTexParam");
    //Bind the sand texture
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
    char* emptyPixels = (char*)malloc(3 * texWidth*texHeight * sizeof(char));
    //Generate the tex image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, emptyPixels);
    checkGlError("glTexImage2D");
    //Free the dummy array
    free(emptyPixels);

    //Bind the BH tex image
    glBindTexture(GL_TEXTURE_2D, bhTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // NOTE: BH tex image MUST be power-of-two dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bhTexWidth, bhTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bhTexPixels);
    return;
}



void glRender() {
    // Update dimensions
    texture[2] = (float) workWidth/texWidth;
    texture[4] = (float) workWidth/texWidth;
    texture[8] = (float) workWidth/texWidth;
    texture[5] = (float) workHeight/texHeight;
    texture[9] = (float) workHeight/texHeight;
    texture[11] = (float) workHeight/texHeight;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gProgram);

    // Swap to sand texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, stupidTegra, workHeight, GL_RGB, GL_UNSIGNED_BYTE, colorsFrameBuffer);

    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glUniform1i(mTextureUniformHandle, 0);

    glEnableVertexAttribArray(gvPositionHandle);
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_TRUE, 0, vertices);

    glEnableVertexAttribArray(mTextureCoordinateHandle);
    glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_TRUE, 0, texture);


    //Set up the projection matrix for zoom
    float proj[16];
    float yOffset = centerY - (screenHeight/2);
    float modCenterY = (screenHeight/2) - yOffset;

    float modViewWidth = viewWidth/2 * zoomScale;
    float modViewHeight = viewHeight/2 * zoomScale;
    float left = centerX - modViewWidth;
    float right = centerX + modViewWidth;
    float top = modCenterY + modViewHeight;
    float bottom = modCenterY - modViewHeight;


    //TODO: Figure out why everything here is multiplied by 0.5 (probaly should reverse the verticies being 2.0 o something)
    setOthographicMat(left/(screenWidth), right/(screenWidth), top/(screenHeight), bottom/(screenHeight), 1, -1, proj);

    glUniformMatrix4fv(mProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);

    float screenSizeV[2];
    screenSizeV[0] = texWidth;
    screenSizeV[1] = texHeight;

    glUniform2fv(mScreenSizeHandle, 1, &screenSizeV[0]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw black holes
    vector<float> bhVerts;
    vector<float> bhTexCoords;
    const float bhRadX = 10.0/workWidth;
    const float bhRadY = 10.0/workHeight;
    for (int i = 0; i < numSpaceHoles; ++i) {
        const SpaceHole &h = spaceHoles[i];
        if (h.type == BLACK_HOLE) {
            float fx = h.x/(float)workWidth, fy = 1.0 - h.y/(float)workHeight;
            bhVerts.push_back(fx - bhRadX); // Bottom-left
            bhVerts.push_back(fy - bhRadY);
            bhVerts.push_back(fx + bhRadX); // Bottom-right
            bhVerts.push_back(fy - bhRadY);
            bhVerts.push_back(fx + bhRadX); // Top-right
            bhVerts.push_back(fy + bhRadY);
            bhVerts.push_back(fx - bhRadX); // Bottom-left
            bhVerts.push_back(fy - bhRadY);
            bhVerts.push_back(fx + bhRadX); // Top-right
            bhVerts.push_back(fy + bhRadY);
            bhVerts.push_back(fx - bhRadX); // Top-left
            bhVerts.push_back(fy + bhRadY);
            bhTexCoords.push_back(0.0);
            bhTexCoords.push_back(0.0);
            bhTexCoords.push_back(1.0);
            bhTexCoords.push_back(0.0);
            bhTexCoords.push_back(1.0);
            bhTexCoords.push_back(1.0);
            bhTexCoords.push_back(0.0);
            bhTexCoords.push_back(0.0);
            bhTexCoords.push_back(1.0);
            bhTexCoords.push_back(1.0);
            bhTexCoords.push_back(0.0);
            bhTexCoords.push_back(1.0);
        }
    }
    int size = bhVerts.size();

    if (size > 0) {
        float *bhVertsArr = (float*) malloc(size*sizeof(float));
        for (int i = 0; i < size; ++i) {
            bhVertsArr[i] = bhVerts[i];
        }
        float *bhTexArr = (float*) malloc(size*sizeof(float));
        for (int i = 0; i < size; ++i) {
            bhTexArr[i] = bhTexCoords[i];
        }

        // Swap to bh texture
        glBindTexture(GL_TEXTURE_2D, bhTex);
        glEnableVertexAttribArray(gvPositionHandle);
        glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, bhVertsArr);
        glEnableVertexAttribArray(mTextureCoordinateHandle);
        glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_FALSE, 0, bhTexArr);

        glDrawArrays(GL_TRIANGLES, 0, size/2);

        free(bhVertsArr);
        free(bhTexArr);
    }
    bhVerts.clear();
    bhTexCoords.clear();

    // Draw gravity field
    // TODO: Do this only conditionally
    glUseProgram(gGravProgram);
    glUniformMatrix4fv(mGravProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);
    glEnableVertexAttribArray(gGravVPositionHandle);
    glVertexAttribPointer(gGravVPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gravCoords);
    glEnableVertexAttribArray(gGravVMagHandle);
    glVertexAttribPointer(gGravVMagHandle, 1, GL_FLOAT, GL_FALSE, 0, gravMag);

    glDrawArrays(GL_LINES, 0, 2*gfWidth*gfHeight);
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
