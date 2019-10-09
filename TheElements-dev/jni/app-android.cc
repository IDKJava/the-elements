/*
 * app-android.c
 * -----------------------------
 * Contains the entire API for our JNI code. Any
 * access to the native code from Java will be made
 * using these accessible functions.
 */

#include <jni.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

//Include the global variables
#include "app.h"
//Include the global macros
#include "macros.h"
//Include the initializing function
#include "setup.h"
//Include the element characteristics file
#include "elementproperties.h"
//Include the collisions data file
#include "collisions.h"
//Include proto messages
#include "messages.pb.h"
//Include the saving and loading functions
#include "saveload.h"
#include "saveload2.h"
//Include the server access functions
//#include "server.h"
//Include the rendering functions
#include "rendergl.h"
#include "gravity.h"
#include "update.h"

//Include camera parsing functions
#include "camera.h"

//Include pthread functions
#include <pthread.h>

// Per-file logging
#ifndef NDEBUG
//Debug
#define LOGGING 1
#else
//Release
#define LOGGING 0
#endif

// JNI expect C names, not mangled C++ names
#if __cplusplus
extern "C" {
#endif

void convertMouseCoords(int x, int y, float* gameX, float* gameY) {
    float modViewWidth = viewWidth * zoomScale;
    float modViewHeight = viewHeight * zoomScale;
    *gameX = ((float)x/(float)screenWidth)*modViewWidth + ((float)centerX - (modViewWidth/2.0));
    *gameY = ((float)y/(float)screenHeight)*modViewHeight + ((float)centerY - (float)(modViewHeight/2.0));
    *gameX /= zoomFactor;
    *gameY /= zoomFactor;
}

//Called from SandViewRenderer
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandViewRenderer_nativeResize(JNIEnv* env, jobject thiz, jint width, jint height)
{
    LOGI("nativeResize()");

    pthread_mutex_lock(&update_mutex);

    //These variable change from pinch to zoom
    viewWidth = width;
    viewHeight = height;
    centerX = width/2;
    centerY = height/2;

    int changed = 0;
    if ( width != screenWidth || height != screenHeight) {
      changed = 1;
    }

    //These variables remain constant unless the screen size changes
    screenWidth = width;
    screenHeight = height;
    workWidth = screenWidth / zoomFactor;
    workHeight = screenHeight / zoomFactor;
    gfWidth = (workWidth+GF_BLOCK_SIZE-1)/GF_BLOCK_SIZE;
    gfHeight = (workHeight+GF_BLOCK_SIZE-1)/GF_BLOCK_SIZE;

    //Finds nearest power of 2 to work Width
    stupidTegra = 1;
    while((stupidTegra = stupidTegra << 1) < workWidth);
    dimensionsChanged = TRUE;


    glInit();
    arraySetup();
    gameSetup();

    pthread_mutex_unlock(&update_mutex);
    startUpdateThread();
}
// TODO: I think this should be removed, but I don't have the time to figure it out right now
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandViewRenderer_nativeLoadState(JNIEnv* env, jobject thiz, jboolean shouldLoadDemo)
{
    char loadLoc[256];

    strcpy(loadLoc, ROOT_FOLDER);
    strcat(loadLoc, SAVES_FOLDER);
    if(shouldLoadDemo)
    {
        LOGI("Loading demo");
        strcat(loadLoc, DEMO_SAVE);
    }
    else
    {
        LOGI("loadTempState");
        strcat(loadLoc, TEMP_SAVE);
    }
    strcat(loadLoc, SAVE2_EXTENSION);
    LOGI("Calling loadstate2");
    loadState2(loadLoc);
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandViewRenderer_nativeRender(JNIEnv* env, jobject thiz)
{
    glRenderThreaded();
}

//Save/load functions
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_game_SaveManager_saveState(JNIEnv* env, jobject thiz, jbyteArray saveLoc)
{
#ifdef USE_PROFILING
    // Stop profiling
    // this saves to /sdcard/gmon.out
    moncleanup();
#endif

    jsize len = env->GetArrayLength(saveLoc);
    jbyte* saveLoc2 = (jbyte*) malloc(len * sizeof(jbyte));
    env->GetByteArrayRegion(saveLoc, 0, len, saveLoc2);
    char* saveLoc3 = (char*) malloc((len+1) * sizeof(char));
    int i;
    char buffer[100];
    for(i = 0; i < len; i++)
    {
        saveLoc3[i] = saveLoc2[i];
    }
    saveLoc3[len] = 0;

    LOGI("Save to: %s", saveLoc3);

    char tempLoc[256];
    strcpy(tempLoc, ROOT_FOLDER);
    strcat(tempLoc, SAVES_FOLDER);
    strcat(tempLoc, TEMP_SAVE);
    strcat(tempLoc, SAVE2_EXTENSION);

    // Copy the temp file into save
    copyFile(tempLoc, saveLoc3);
    return TRUE;
}
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_game_SaveManager_loadState(JNIEnv* env, jobject thiz, jbyteArray loadLoc)
{
    jsize len = env->GetArrayLength(loadLoc);
    jbyte* loadLoc2 = (jbyte*) malloc(len * sizeof(jbyte));
    env->GetByteArrayRegion(loadLoc, 0, len, loadLoc2);
    char* loadLoc3 = (char*) malloc((len+1) * sizeof(char));
    for(int i = 0; i < len; i++)
    {
        loadLoc3[i] = loadLoc2[i];
    }
    loadLoc3[len] = 0;

    LOGI("Load from: %s", loadLoc3);

    char tempLoc[256];
    strcpy(tempLoc, ROOT_FOLDER);
    strcat(tempLoc, SAVES_FOLDER);
    strcat(tempLoc, TEMP_SAVE);
    strcat(tempLoc, SAVE2_EXTENSION);

    // Copy the load file into temp, so that we resume into it
    copyFile(loadLoc3, tempLoc);

    free(loadLoc2);
    free(loadLoc3);
    return TRUE;
}
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_MainActivity_saveTempState(JNIEnv* env, jobject thiz)
{
    LOGI("saveTempState");
    char saveLoc[256];
    strcpy(saveLoc, ROOT_FOLDER);
    strcat(saveLoc, SAVES_FOLDER);
    strcat(saveLoc, TEMP_SAVE);
    strcat(saveLoc, SAVE2_EXTENSION);
    return saveState2(saveLoc);
}
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_MainActivity_loadDemoState(JNIEnv* env, jobject thiz)
{
    char loadLoc[256];
    strcpy(loadLoc, ROOT_FOLDER);
    strcat(loadLoc, SAVES_FOLDER);
    strcat(loadLoc, DEMO_SAVE);
    strcat(loadLoc, SAVE2_EXTENSION);
    return loadState2(loadLoc);
}

//General utility functions
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_nativeInit(JNIEnv* env, jobject thiz)
{
    // Initialization
    atmosphereSetup();
    elementSetup();
    particleSetup();
    preCalculateHSL();
    calculateClosestElement();

    // Profiling
#ifdef USE_PROFILING
    monstartup("libthelements.so");
#endif
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_nativeRefreshElements(JNIEnv* env, jobject thiz)
{
	// Reload all elements, recreate eleList.lst
	elementSetup();
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_clearScreen(JNIEnv* env, jobject thiz)
{
    shouldClear = TRUE;
}

//Setter functions
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_preferences_Preferences_setBorderState(JNIEnv* env, jobject thiz, jboolean leftBorderState, jboolean topBorderState, jboolean rightBorderState, jboolean bottomBorderState)
{
    cAtmosphere->borderLeft = leftBorderState;
    cAtmosphere->borderTop = topBorderState;
    cAtmosphere->borderRight = rightBorderState;
    cAtmosphere->borderBottom = bottomBorderState;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_preferences_Preferences_setAccelState(JNIEnv* env, jobject thiz, jboolean accelState)
{
    accelOn = (char) accelState;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_preferences_Preferences_setFlippedState(JNIEnv* env, jobject thiz, jboolean flippedState)
{
    flipped = (char) flippedState;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_preferences_Preferences_setBackgroundColor(JNIEnv* env, jobject thiz, jchar redValue, jchar greenValue, jchar blueValue)
{
    //Set the eraser color to the background color, used as the reference whenever background color is needed
    cAtmosphere->backgroundRed = redValue;
    cAtmosphere->backgroundGreen = greenValue;
    cAtmosphere->backgroundBlue = blueValue;

    //Call setup again
    //gameSetup();

    //TODO: Reload
    /*
      char loadLoc[256];
      strcpy(loadLoc, ROOT_FOLDER);
      strcat(loadLoc, SAVES_FOLDER);
      strcat(loadLoc, TEMP_SAVE);
      strcat(loadLoc, SAVE2_EXTENSION);
      loadState(loadLoc);
    */
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_preferences_Preferences_setAtmosphereTemp(JNIEnv* env, jobject thiz, jchar temp)
{
    cAtmosphere->heat = temp;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_preferences_Preferences_setZoom(JNIEnv* env, jobject thiz, jint zoom)
{
    if(zoom != zoomFactor)
    {
        zoomFactor = zoom;
        zoomChanged = TRUE;
    }
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setPlayState(JNIEnv* env, jobject thiz, jboolean playState)
{
    play = (char) playState;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setElement(JNIEnv* env, jobject thiz, jchar element)
{
	if (element >= 0 && element < numElements) {
		cElement = elements[element];
	}
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setBrushSize(JNIEnv* env, jobject thiz, jchar brushSizeValue)
{
    brushSize = brushSizeValue;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_ElementsApplication_setBHTex(JNIEnv* env, jobject thiz, jint w, jint h, jbyteArray pixels) {
    int numChannels = 4;
    bhTexWidth = w;
    bhTexHeight = h;
    bhTexPixels = (char*) malloc(numChannels*w*h*sizeof(char));
    jbyte *bhArr = env->GetByteArrayElements(pixels, NULL);
    memcpy(bhTexPixels, bhArr, numChannels*w*h*sizeof(char));
    env->ReleaseByteArrayElements(pixels, bhArr, 0);
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_ElementsApplication_setWHTex(JNIEnv* env, jobject thiz, jint w, jint h, jbyteArray pixels) {
    int numChannels = 4;
    whTexWidth = w;
    whTexHeight = h;
    whTexPixels = (char*) malloc(numChannels*w*h*sizeof(char));
    jbyte *whArr = env->GetByteArrayElements(pixels, NULL);
    memcpy(whTexPixels, whArr, numChannels*w*h*sizeof(char));
    env->ReleaseByteArrayElements(pixels, whArr, 0);
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_ElementsApplication_setCHTex(JNIEnv* env, jobject thiz, jint w, jint h, jbyteArray pixels) {
    int numChannels = 4;
    chTexWidth = w;
    chTexHeight = h;
    chTexPixels = (char*) malloc(numChannels*w*h*sizeof(char));
    jbyte *chArr = env->GetByteArrayElements(pixels, NULL);
    memcpy(chTexPixels, chArr, numChannels*w*h*sizeof(char));
    env->ReleaseByteArrayElements(pixels, chArr, 0);
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_ElementsApplication_setNGTex(JNIEnv* env, jobject thiz, jint w, jint h, jbyteArray pixels) {
    int numChannels = 4;
    ngTexWidth = w;
    ngTexHeight = h;
    ngTexPixels = (char*) malloc(numChannels*w*h*sizeof(char));
    jbyte *ngArr = env->GetByteArrayElements(pixels, NULL);
    memcpy(ngTexPixels, ngArr, numChannels*w*h*sizeof(char));
    env->ReleaseByteArrayElements(pixels, ngArr, 0);
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_setBrushProps(JNIEnv* env, jobject thiz, jint brushType) {
    brushProps = brushType;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_setDragProps(JNIEnv* env, jobject thiz, jint renderType) {
    renderOverlayType = renderType;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_rectStart(JNIEnv* env, jobject thiz, jint x, jint y) {
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    rectValid = true;
    rectSX = rectEX = mX;
    rectSY = rectEY = mY;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_rectMove(JNIEnv* env, jobject thiz, jint x, jint y) {
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    rectEX = mX;
    rectEY = mY;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_rectEnd(JNIEnv* env, jobject thiz) {
    rectValid = false;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_brushStartLocation(JNIEnv* env, jobject thiz, jint x, jint y) {
    pthread_mutex_lock(&brush_mutex);
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    brushLocX = mX;
    brushLocY = mY;
    brushOn = true;
    pthread_mutex_unlock(&brush_mutex);
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_brushMoveLocation(JNIEnv* env, jobject thiz, jint x, jint y) {
    pthread_mutex_lock(&brush_mutex);
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    brushNextLocX = mX;
    brushNextLocY = mY;
    pthread_mutex_unlock(&brush_mutex);
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_brushEndLocation(JNIEnv* env, jobject thiz, jint x, jint y) {
    pthread_mutex_lock(&brush_mutex);
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    brushNextLocX = mX;
    brushNextLocY = mY;
    brushOn = false;
    pthread_mutex_unlock(&brush_mutex);
}

JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_game_SandView_makeBlackHole(JNIEnv* env, jobject thiz, jint x, jint y) {
    if (world != WORLD_SPACE) {
        return false;
    }
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    return makeBlackHole(mX, mY);
}

JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_game_SandView_makeWhiteHole(JNIEnv* env, jobject thiz, jint x, jint y) {
    if (world != WORLD_SPACE) {
        return false;
    }
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    return makeWhiteHole(mX, mY);
}

JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_game_SandView_makeCurlHole(JNIEnv* env, jobject thiz, jint x, jint y) {
    if (world != WORLD_SPACE) {
        return false;
    }
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);
    return makeCurlHole(mX, mY);
}

JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_game_SandView_makeNullGravity(JNIEnv* env, jobject thiz,
    jint sx, jint sy, jint ex, jint ey) {
    if (world != WORLD_SPACE) {
        return false;
    }
    float startX, startY, endX, endY;
    convertMouseCoords(sx, sy, &startX, &startY);
    convertMouseCoords(ex, ey, &endX, &endY);
    return makeNullGravity(startX, startY, endX, endY);
}

JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_game_SandView_removeGravObject(JNIEnv* env, jobject thiz, jint x, jint y) {
    if (world != WORLD_SPACE) {
        return false;
    }
    float mX, mY;
    convertMouseCoords(x, y, &mX, &mY);

    int closest = -1;
    float dist;
    for (int i = 0; i < numSpaceObjs; ++i) {
        SpaceObj *obj = &spaceObjs[i];
        float dx = obj->x - mX;
        float dy = obj->y - mY;
        float objDist = dx*dx+dy*dy;
        if (closest == -1 || dist > objDist) {
            closest = i;
            dist = objDist;
        }
        // Deleting null gravity zone should work from either terminal
        if (obj->type == NULL_GRAVITY) {
            dx = obj->ex - mX;
            dy = obj->ey - mY;
            objDist = dx*dx+dy*dy;
            if (dist > objDist) {
                closest = i;
                dist = objDist;
            }
        }
    }
    SpaceObj *deleteObj = &spaceObjs[closest];
    updateGravityField(deleteObj, true);
    numSpaceObjs--;
    for (int j = closest; j < numSpaceObjs; ++j) {
        spaceObjs[j] = spaceObjs[j+1];
    }
    return false;
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_drawRect(JNIEnv* env, jobject thiz,
    jint startX, jint startY, jint endX, jint endY) {
    float sx, sy, ex, ey;
    convertMouseCoords(startX, startY, &sx, &sy);
    convertMouseCoords(endX, endY, &ex, &ey);
    pthread_mutex_lock(&update_mutex);
    drawCircleyLine(sx, sy, sx, ey);
    drawCircleyLine(sx, ey, ex, ey);
    drawCircleyLine(ex, ey, ex, sy);
    drawCircleyLine(ex, sy, sx, sy);
    pthread_mutex_unlock(&update_mutex);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_drawTri(JNIEnv* env, jobject thiz,
    jint startX, jint startY, jint endX, jint endY) {
    float sx, sy, ex, ey;
    convertMouseCoords(startX, startY, &sx, &sy);
    convertMouseCoords(endX, endY, &ex, &ey);
    pthread_mutex_lock(&update_mutex);
    drawCircleyLine(sx, sy, sx, ey);
    drawCircleyLine(sx, ey, ex, ey);
    drawCircleyLine(ex, ey, sx, sy);
    pthread_mutex_unlock(&update_mutex);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_drawCircle(JNIEnv* env, jobject thiz,
    jint startX, jint startY, jint endX, jint endY) {
    float sx, sy, ex, ey;
    convertMouseCoords(startX, startY, &sx, &sy);
    convertMouseCoords(endX, endY, &ex, &ey);
    float dx = fabs(sx-ex), dy = fabs(sy-ey);
    float r = sqrt(dx*dx+dy*dy)*0.5;
    float cx = (sx+ex)/2.0, cy = (sy+ey)/2.0;
    pthread_mutex_lock(&update_mutex);
    // 20-sided poly loop
    float lastX = cx + r, lastY = cy;
    for (int i = 1; i < 21; ++i) {
        float nextX = r*cos(i*2*M_PI/20) + cx;
        float nextY = r*sin(i*2*M_PI/20) + cy;
        drawCircleyLine(lastX, lastY, nextX, nextY);
        lastX = nextX;
        lastY = nextY;
    }
    pthread_mutex_unlock(&update_mutex);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_drawLine(JNIEnv* env, jobject thiz,
    jint startX, jint startY, jint endX, jint endY) {
    float sx, sy, ex, ey;
    convertMouseCoords(startX, startY, &sx, &sy);
    convertMouseCoords(endX, endY, &ex, &ey);
    pthread_mutex_lock(&update_mutex);
    drawCircleyLine(sx, sy, ex, ey);
    pthread_mutex_unlock(&update_mutex);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_drawLineDashed(JNIEnv* env, jobject thiz,
    jint startX, jint startY, jint endX, jint endY) {
    float sx, sy, ex, ey;
    convertMouseCoords(startX, startY, &sx, &sy);
    convertMouseCoords(endX, endY, &ex, &ey);

    int deltaX = ex-sx;
    int deltaY = ey-sy;
    if (deltaX == 0 && deltaY == 0) {
        return;
    }
    float curX = sx, curY = sy;
    float dx = deltaX/11.0, dy = deltaY/11.0;
    // Multiply the comparison by the delta to fix the signs
    pthread_mutex_lock(&update_mutex);
    while (curX*deltaX < ex*deltaX && curY*deltaY < ey*deltaY) {
        drawCircleyLine(curX, curY, curX+dx, curY+dy);
        curX += dx*2;
        curY += dy*2;
    }
    pthread_mutex_unlock(&update_mutex);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_drawSlingshot(JNIEnv* env, jobject thiz,
    jint startX, jint startY, jint endX, jint endY) {
    float sx, sy, ex, ey;
    convertMouseCoords(startX, startY, &sx, &sy);
    convertMouseCoords(endX, endY, &ex, &ey);
    pthread_mutex_lock(&update_mutex);
    brushProps = BRUSH_SLINGSHOT;
    slingshotX = (ex-sx)/10.0;
    slingshotY = (ey-sy)/10.0;
    drawCircle(sx, sy);
    brushProps = BRUSH_NORMAL;
    pthread_mutex_unlock(&update_mutex);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setFilterMode(JNIEnv* env, jobject thiz, jchar mode)
{
  filterType = mode;
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setWorld(JNIEnv* env, jobject thiz, jint newWorld) {
  world = newWorld;
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_panView(JNIEnv* env, jobject thiz, jint dx, jint dy) {
  // Scale to view coords and pan by the given delta
  centerX += (dx/(float)screenWidth)*viewWidth;
  centerY += (dy/(float)screenHeight)*viewHeight;
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_setPinchScale(JNIEnv* env, jobject thiz, jfloat scale)
{
  zoomScale = scale;
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_commitPinch(JNIEnv* env, jobject thiz)
{
  viewWidth = viewWidth * zoomScale;
  viewHeight = viewHeight * zoomScale;
  zoomScale = 1.0;
}


//Getter functions
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_MainActivity_getElement(JNIEnv* env, jobject thiz)
{
    return cElement->index;
}
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_MainActivity_getElementRed(JNIEnv* env, jobject thiz, int i)
{
	if (i >= 0 && i < numElements) {
		return elements[i]->red;
	}
	else {
		return 0;
	}
}
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_MainActivity_getElementGreen(JNIEnv* env, jobject thiz, int i)
{
	if (i >= 0 && i < numElements) {
		return elements[i]->green;
	}
	else {
		return 0;
	}
}
JNIEXPORT char JNICALL
Java_com_idkjava_thelements_MainActivity_getElementBlue(JNIEnv* env, jobject thiz, int i)
{
	if (i >= 0 && i < numElements) {
		return elements[i]->blue;
	}
	else {
		return 0;
	}
}
JNIEXPORT jbyteArray JNICALL
Java_com_idkjava_thelements_MainActivity_getElementInfo(JNIEnv* env, jobject thiz, int i)
{
    // TODO(gkanwar): Rename CustomElement message to ElementInfo,
    // since this is not actually a custom.
    CustomElement custom;
    custom.set_name(baseName[i]);
    custom.set_state(baseState[i]);
    custom.set_starting_temp(baseStartingTemp[i]);
    custom.set_lowest_temp(baseLowestTemp[i]);
    custom.set_highest_temp(baseHighestTemp[i]);
    custom.set_lower_element_index(baseLowerElement[i]);
    custom.set_higher_element_index(baseHigherElement[i]);
    custom.set_red(baseRed[i]);
    custom.set_green(baseGreen[i]);
    custom.set_blue(baseBlue[i]);
    custom.set_density(baseDensity[i]);
    custom.set_fallvel(baseFallVel[i]);
    custom.set_inertia(baseInertia[i]);

    for (int j = NORMAL_ELEMENT; j < NUM_BASE_ELEMENTS; j++)
    {
        custom.add_collision()->set_type(collision[i][j]);
    }

    for (int j = 0; j < MAX_SPECIALS; j++)
    {
        Special* special = custom.add_special();
        special->set_type(baseSpecial[i][j]);
        special->set_val(baseSpecialValue[i][j]);
    }

    string out;
    if (!custom.SerializeToString(&out))
    {
        LOGE("Could not serialize element info");
        return env->NewByteArray(0);
    }

    jbyteArray arr = env->NewByteArray(out.size());
    env->SetByteArrayRegion(arr, 0, out.size(), (jbyte*)out.c_str());
    return arr;
}
JNIEXPORT int JNICALL
Java_com_idkjava_thelements_MainActivity_getMaxSpecials(JNIEnv* env, jobject thiz)
{
    return MAX_SPECIALS;
}
JNIEXPORT int JNICALL
Java_com_idkjava_thelements_MainActivity_getWorld(JNIEnv* env, jobject thiz) {
    return world;
}
JNIEXPORT int JNICALL
Java_com_idkjava_thelements_MainActivity_getSurfaceWidth(JNIEnv* env, jobject thiz) {
    return screenWidth;
}
JNIEXPORT int JNICALL
Java_com_idkjava_thelements_MainActivity_getSurfaceHeight(JNIEnv* env, jobject thiz) {
    return screenHeight;
}

//Accelerometer related
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setXGravity(JNIEnv* env, jobject thiz, float xGravityIn)
{
    xGravity = xGravityIn;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setYGravity(JNIEnv* env, jobject thiz, float yGravityIn)
{
    yGravity = yGravityIn;
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_loadFromImage(JNIEnv* env,
                                                            jobject thiz,
                                                            jintArray pixels,
                                                            jint offsetx,
                                                            jint offsety,
                                                            jint w,
                                                            jint h)
{
  LOGI("loading from image");
  int* bitPixels = env->GetIntArrayElements(pixels, 0);
  setGameToImage(bitPixels, offsetx, offsety, w, h);
}

//Upgrading save files (for backwards compatibility)
JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_SplashActivity_upgradeCustomElement(JNIEnv* env, jobject thiz, jstring filename)
{
    jboolean success = (jboolean) true;
    const char* nativeFilename = env->GetStringUTFChars(filename, NULL);
    // Get the new save location
    string newFilename = string(nativeFilename);
    newFilename.replace(newFilename.find(ELEMENT_EXTENSION),
            string(ELEMENT2_EXTENSION).length(), ELEMENT2_EXTENSION);

    CustomElement custom;
    FILE* customFile = fopen(nativeFilename, "r");
    upgradeCustomElementV1(customFile, &custom, newFilename);

    ofstream out(newFilename.c_str(), ios::out | ios::binary | ios::trunc);
    if (!out.fail())
    {
        success = (jboolean) custom.SerializeToOstream(&out);
    }
    else
    {
        success = (jboolean) false;
    }

    env->ReleaseStringUTFChars(filename, nativeFilename);
    return success;
}
JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_SplashActivity_upgradeSaveFile(JNIEnv* env, jobject thiz, jstring filename)
{
    jboolean success = (jboolean) true;
    const char* nativeFilename = env->GetStringUTFChars(filename, NULL);

    SaveFile save;
    FILE* saveFile = fopen(nativeFilename, "r");
    upgradeStateV1(saveFile, &save);
    LOGI("Upgraded dims: %d %d", save.size_x(), save.size_y());
    LOGI("Upgraded particles: %d", save.particle_size());

    // Get the new save location
    string newFilename = string(nativeFilename);
    newFilename.replace(newFilename.find(SAVE_EXTENSION),
            string(SAVE2_EXTENSION).length(), SAVE2_EXTENSION);

    ofstream out(newFilename.c_str(), ios::out | ios::binary | ios::trunc);
    if (!out.fail())
    {
        success = (jboolean) save.SerializeToOstream(&out);
    }
    else
    {
        success = (jboolean) false;
    }

    env->ReleaseStringUTFChars(filename, nativeFilename);
    return success;
}
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_SplashActivity_setRootDir(JNIEnv* env, jobject thiz, jstring newRoot) {
    const char* nativeRoot = env->GetStringUTFChars(newRoot, NULL);
    char *nativeRootCpy = new char[256];
    strncpy(nativeRootCpy, nativeRoot, 256);
    ROOT_FOLDER = nativeRootCpy;
    env->ReleaseStringUTFChars(newRoot, nativeRoot);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setPaidCameraOn(JNIEnv* env, jobject thiz, jboolean cameraOn)
{
    paidCameraOn = cameraOn;
}


// extern "C"
#if __cplusplus
}
#endif
