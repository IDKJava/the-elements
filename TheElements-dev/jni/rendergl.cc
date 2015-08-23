/*
 * gl.c
 * --------------------------
 * Defines the gl rendering and initialization
 * functions appInit, appDeinit, and appRender.
 */

#include "rendergl.h"

//Include the GLES 2.0 functions
#include <GLES2/gl2.h>
//Include pthread functions
#include <pthread.h>
#include <vector>

//Include the global variables
#include "app.h"
//Include the update function
#include "update.h"
#include "macros.h"


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
unsigned int whTex;
unsigned int chTex;
unsigned int ngTex;

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
    "  gl_FragColor = vMag[0]*vec4(10.0, 5.0, 0.0, 0.0) + vec4(0.2, 0.2, 0.2, 1.0);\n"
    "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            LOGE("Shader compile failed.");
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
    else {
      checkGlError("Create shader");
      LOGE("Create shader failed.");
    }
    return shader;
}

GLuint buildProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        LOGE("Create vertex shader failed.");
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        LOGE("Create fragment shader failed.");
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
            LOGE("Link program failed.");
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
    }
    return program;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint program = buildProgram(pVertexSource, pFragmentSource);
    mTextureCoordinateHandle = glGetAttribLocation(program, "a_TexCoordinate");
    mScreenSizeHandle = glGetUniformLocation(program, "u_screenSize");
    mTextureUniformHandle = glGetUniformLocation(program, "u_Texture");
    mProjMatrixUniformHandle = glGetUniformLocation(program, "u_MVPMatrix");
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0, 0.0, 0.0, 1.0);

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
    glGenTextures(1, &whTex);
    glGenTextures(1, &chTex);
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

    //Bind the WH tex image
    glBindTexture(GL_TEXTURE_2D, whTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // NOTE: BH tex image MUST be power-of-two dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, whTexWidth, whTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, whTexPixels);

    //Bind the CH tex image
    glBindTexture(GL_TEXTURE_2D, chTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // NOTE: BH tex image MUST be power-of-two dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, chTexWidth, chTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, chTexPixels);

    //Bind the NG tex image
    glBindTexture(GL_TEXTURE_2D, ngTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // NOTE: BH tex image MUST be power-of-two dimensions
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ngTexWidth, ngTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ngTexPixels);
    return;
}

void makeSpriteSquare(float cx, float cy, float radx, float rady,
    vector<float> &verts, vector<float> &texCoords) {
    verts.push_back(cx - radx); // Bottom-left
    verts.push_back(cy - rady);
    verts.push_back(cx + radx); // Bottom-right
    verts.push_back(cy - rady);
    verts.push_back(cx + radx); // Top-right
    verts.push_back(cy + rady);
    verts.push_back(cx - radx); // Bottom-left
    verts.push_back(cy - rady);
    verts.push_back(cx + radx); // Top-right
    verts.push_back(cy + rady);
    verts.push_back(cx - radx); // Top-left
    verts.push_back(cy + rady);

    // Tex coords go in the opposite order, so texture is drawn with the right parity
    texCoords.push_back(0.0);
    texCoords.push_back(0.0);
    texCoords.push_back(0.0);
    texCoords.push_back(1.0);
    texCoords.push_back(1.0);
    texCoords.push_back(1.0);
    texCoords.push_back(0.0);
    texCoords.push_back(0.0);
    texCoords.push_back(1.0);
    texCoords.push_back(1.0);
    texCoords.push_back(1.0);
    texCoords.push_back(0.0);
}

void makeLineRect(float sx, float sy, float ex, float ey,
    vector<float> &verts) {
    verts.push_back(sx);
    verts.push_back(sy);
    verts.push_back(sx);
    verts.push_back(ey);


    verts.push_back(sx);
    verts.push_back(ey);
    verts.push_back(ex);
    verts.push_back(ey);


    verts.push_back(ex);
    verts.push_back(ey);
    verts.push_back(ex);
    verts.push_back(sy);


    verts.push_back(ex);
    verts.push_back(sy);
    verts.push_back(sx);
    verts.push_back(sy);
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

    setOthographicMat(left/(screenWidth), right/(screenWidth), top/(screenHeight), bottom/(screenHeight), 1, -1, proj);

    glUniformMatrix4fv(mProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);

    float screenSizeV[2];
    screenSizeV[0] = texWidth;
    screenSizeV[1] = texHeight;

    glUniform2fv(mScreenSizeHandle, 1, &screenSizeV[0]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (world == WORLD_SPACE) {
        // Build OpenGL arrays for sprites
        // TODO: Build arrays at time of sprite addition to scene
        vector<float> bhVerts;
        vector<float> bhTexCoords;
        vector<float> whVerts;
        vector<float> whTexCoords;
        vector<float> chVerts;
        vector<float> chTexCoords;
        vector<float> ngTermVerts;
        vector<float> ngTexCoords;
        vector<float> ngSideVerts;
        const float radX = 10.0/workWidth;
        const float radY = 10.0/workHeight;
        for (int i = 0; i < numSpaceObjs; ++i) {
            const SpaceObj &h = spaceObjs[i];
            float fx = h.x/(float)workWidth, fy = 1.0 - h.y/(float)workHeight;
            if (h.type == BLACK_HOLE) {
                makeSpriteSquare(fx, fy, radX, radY, bhVerts, bhTexCoords);
            }
            else if (h.type == WHITE_HOLE) {
                makeSpriteSquare(fx, fy, radX, radY, whVerts, whTexCoords);
            }
            else if (h.type == CURL_HOLE) {
                makeSpriteSquare(fx, fy, radX, radY, chVerts, chTexCoords);
            }
            else if (h.type == NULL_GRAVITY) {
                makeSpriteSquare(fx, fy, radX, radY, ngTermVerts, ngTexCoords);
                float ex = h.ex/(float)workWidth;
                float ey = 1.0 - h.ey/(float)workHeight;
                makeSpriteSquare(ex, ey, radX, radY, ngTermVerts, ngTexCoords);
                makeLineRect(fx, fy, ex, ey, ngSideVerts);
            }
        }

        // Draw gravity field
        // TODO: Do this only conditionally
        glUseProgram(gGravProgram);
        glUniformMatrix4fv(mGravProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);
        glEnableVertexAttribArray(gGravVPositionHandle);
        glVertexAttribPointer(gGravVPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gravCoords);
        glEnableVertexAttribArray(gGravVMagHandle);
        glVertexAttribPointer(gGravVMagHandle, 1, GL_FLOAT, GL_FALSE, 0, gravMag);

        glDrawArrays(GL_LINES, 0, 2*gfWidth*gfHeight);

        // Draw the null gravity rect lines using the grav program
        int ngLineSize = ngSideVerts.size();
        if (ngLineSize > 0) {
            float *ngLineVertsArr = (float*) malloc(ngLineSize*sizeof(float));
            float *ngLineMagArr = (float*) malloc((ngLineSize/2)*sizeof(float));
            for (int i = 0; i < ngLineSize; ++i) {
                ngLineVertsArr[i] = ngSideVerts[i];
                if (i % 2 == 0) {
                    ngLineMagArr[i/2] = 1.0;
                }
            }

            glEnableVertexAttribArray(gGravVPositionHandle);
            glVertexAttribPointer(gGravVPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, ngLineVertsArr);
            glEnableVertexAttribArray(gGravVMagHandle);
            glVertexAttribPointer(gGravVMagHandle, 1, GL_FLOAT, GL_FALSE, 0, ngLineMagArr);

            glDrawArrays(GL_LINES, 0, ngLineSize/2);

            free(ngLineVertsArr);
            free(ngLineMagArr);
        }
        ngSideVerts.clear();

        // Switch to texture rect program
        glUseProgram(gProgram);
        glUniformMatrix4fv(mProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);

        // Draw black hole sprites
        int bhSize = bhVerts.size();
        if (bhSize > 0) {
            float *bhVertsArr = (float*) malloc(bhSize*sizeof(float));
            for (int i = 0; i < bhSize; ++i) {
                bhVertsArr[i] = bhVerts[i];
            }
            float *bhTexArr = (float*) malloc(bhSize*sizeof(float));
            for (int i = 0; i < bhSize; ++i) {
                bhTexArr[i] = bhTexCoords[i];
            }

            // Swap to bh texture
            glBindTexture(GL_TEXTURE_2D, bhTex);
            glEnableVertexAttribArray(gvPositionHandle);
            glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, bhVertsArr);
            glEnableVertexAttribArray(mTextureCoordinateHandle);
            glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_FALSE, 0, bhTexArr);

            glDrawArrays(GL_TRIANGLES, 0, bhSize/2);

            free(bhVertsArr);
            free(bhTexArr);
        }
        bhVerts.clear();
        bhTexCoords.clear();

        // Draw white hole sprites
        int whSize = whVerts.size();
        if (whSize > 0) {
            float *whVertsArr = (float*) malloc(whSize*sizeof(float));
            for (int i = 0; i < whSize; ++i) {
                whVertsArr[i] = whVerts[i];
            }
            float *whTexArr = (float*) malloc(whSize*sizeof(float));
            for (int i = 0; i < whSize; ++i) {
                whTexArr[i] = whTexCoords[i];
            }

            // Swap to wh texture
            glBindTexture(GL_TEXTURE_2D, whTex);
            glEnableVertexAttribArray(gvPositionHandle);
            glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, whVertsArr);
            glEnableVertexAttribArray(mTextureCoordinateHandle);
            glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_FALSE, 0, whTexArr);

            glDrawArrays(GL_TRIANGLES, 0, whSize/2);

            free(whVertsArr);
            free(whTexArr);
        }
        whVerts.clear();
        whTexCoords.clear();

        // Draw curl hole sprites
        int chSize = chVerts.size();
        if (chSize > 0) {
            float *chVertsArr = (float*) malloc(chSize*sizeof(float));
            for (int i = 0; i < chSize; ++i) {
                chVertsArr[i] = chVerts[i];
            }
            float *chTexArr = (float*) malloc(chSize*sizeof(float));
            for (int i = 0; i < chSize; ++i) {
                chTexArr[i] = chTexCoords[i];
            }

            // Swap to ch texture
            glBindTexture(GL_TEXTURE_2D, chTex);
            glEnableVertexAttribArray(gvPositionHandle);
            glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, chVertsArr);
            glEnableVertexAttribArray(mTextureCoordinateHandle);
            glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_FALSE, 0, chTexArr);

            glDrawArrays(GL_TRIANGLES, 0, chSize/2);

            free(chVertsArr);
            free(chTexArr);
        }
        chVerts.clear();
        chTexCoords.clear();

        // Draw null gravity terminal sprites
        int ngSize = ngTermVerts.size();
        if (ngSize > 0) {
            float *ngTermVertsArr = (float*) malloc(ngSize*sizeof(float));
            for (int i = 0; i < ngSize; ++i) {
                ngTermVertsArr[i] = ngTermVerts[i];
            }
            float *ngTexArr = (float*) malloc(ngSize*sizeof(float));
            for (int i = 0; i < ngSize; ++i) {
                ngTexArr[i] = ngTexCoords[i];
            }

            // Swap to ng texture
            glBindTexture(GL_TEXTURE_2D, ngTex);
            glEnableVertexAttribArray(gvPositionHandle);
            glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, ngTermVertsArr);
            glEnableVertexAttribArray(mTextureCoordinateHandle);
            glVertexAttribPointer(mTextureCoordinateHandle, 2, GL_FLOAT, GL_FALSE, 0, ngTexArr);

            glDrawArrays(GL_TRIANGLES, 0, ngSize/2);

            free(ngTermVertsArr);
            free(ngTexArr);
        }
        ngTermVerts.clear();
        ngTexCoords.clear();
    }

    // Currently drawing overlay
    if (rectValid) {
        float sx = rectSX/workWidth, sy = 1.0 - rectSY/workHeight,
            ex = rectEX/workWidth, ey = 1.0 - rectEY/workHeight;
        if (renderOverlayType == RT_RECT) {
            float *rectCoords = new float[8];
            rectCoords[0] = sx;
            rectCoords[1] = sy;
            rectCoords[2] = sx;
            rectCoords[3] = ey;
            rectCoords[4] = ex;
            rectCoords[5] = ey;
            rectCoords[6] = ex;
            rectCoords[7] = sy;
            // Mag chosen for a slightly different color than the null gravity rect
            float *magVals = new float[4];
            magVals[0] = magVals[1] = magVals[2] = magVals[3] = 0.1;
            glUseProgram(gGravProgram);
            glUniformMatrix4fv(mGravProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);
            glEnableVertexAttribArray(gGravVPositionHandle);
            glVertexAttribPointer(gGravVPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, rectCoords);
            glEnableVertexAttribArray(gGravVMagHandle);
            glVertexAttribPointer(gGravVMagHandle, 1, GL_FLOAT, GL_FALSE, 0, magVals);

            glDrawArrays(GL_LINE_LOOP, 0, 4);

            free(rectCoords);
            free(magVals);
        }
        else if (renderOverlayType == RT_LINE) {
            float *lineCoords = new float[4];
            lineCoords[0] = sx;
            lineCoords[1] = sy;
            lineCoords[2] = ex;
            lineCoords[3] = ey;
            float *magVals = new float[2];
            magVals[0] = magVals[1] = 0.1;

            glUseProgram(gGravProgram);
            glUniformMatrix4fv(mGravProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);
            glEnableVertexAttribArray(gGravVPositionHandle);
            glVertexAttribPointer(gGravVPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, lineCoords);
            glEnableVertexAttribArray(gGravVMagHandle);
            glVertexAttribPointer(gGravVMagHandle, 1, GL_FLOAT, GL_FALSE, 0, magVals);

            glDrawArrays(GL_LINES, 0, 2);

            free(lineCoords);
            free(magVals);
        }
        else if (renderOverlayType == RT_CIRCLE) {
            float cx = (sx+ex)/2.0;
            float cy = (sy+ey)/2.0;
            float dx = fabs(rectSX-rectEX), dy = fabs(rectSY-rectEY);
            float r = sqrt(dx*dx+dy*dy)*0.5;
            float rX = r/workWidth, rY = r/workHeight;
            // Circle is actually a 20-sided poly
            float *circleCoords = new float[20*2];
            float *magVals = new float[20];
            for (int i = 0; i < 20; ++i) {
                magVals[i] = 0.1;
                circleCoords[2*i] = rX*cos(i*2*M_PI/20)+cx;
                circleCoords[2*i+1] = rY*sin(i*2*M_PI/20)+cy;
            }

            glUseProgram(gGravProgram);
            glUniformMatrix4fv(mGravProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);
            glEnableVertexAttribArray(gGravVPositionHandle);
            glVertexAttribPointer(gGravVPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, circleCoords);
            glEnableVertexAttribArray(gGravVMagHandle);
            glVertexAttribPointer(gGravVMagHandle, 1, GL_FLOAT, GL_FALSE, 0, magVals);

            glDrawArrays(GL_LINE_LOOP, 0, 20);

            free(circleCoords);
            free(magVals);
        }
        else if (renderOverlayType == RT_TRI) {
            float *triCoords = new float[6];
            triCoords[0] = sx;
            triCoords[1] = sy;
            triCoords[2] = sx;
            triCoords[3] = ey;
            triCoords[4] = ex;
            triCoords[5] = ey;
            float *magVals = new float[3];
            magVals[0] = magVals[1] = magVals[2] = 0.1;

            glUseProgram(gGravProgram);
            glUniformMatrix4fv(mGravProjMatrixUniformHandle, 1, GL_FALSE, &proj[0]);
            glEnableVertexAttribArray(gGravVPositionHandle);
            glVertexAttribPointer(gGravVPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, triCoords);
            glEnableVertexAttribArray(gGravVMagHandle);
            glVertexAttribPointer(gGravVMagHandle, 1, GL_FLOAT, GL_FALSE, 0, magVals);

            glDrawArrays(GL_LINE_LOOP, 0, 3);

            free(triCoords);
            free(magVals);
        }
    }
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
