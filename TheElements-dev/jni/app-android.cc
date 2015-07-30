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

#include <android/log.h>

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

//Called from SandViewRenderer
JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandViewRenderer_nativeResize(JNIEnv* env, jobject thiz, jint width, jint height)
{
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "nativeResize()");

    //These variable change from pinch to zoom
    viewWidth = width;
    viewHeight = height;
    centerX = width/2;
    centerY = height/2;

    //These variables remain constant unless the screen size changes
    screenWidth = width;
    screenHeight = height;
    workWidth = screenWidth / zoomFactor;
    workHeight = screenHeight / zoomFactor;

    //Finds nearest power of 2 to work Width
    stupidTegra = 1;
    while((stupidTegra = stupidTegra << 1) < workWidth);
    dimensionsChanged = TRUE;

    arraySetup();
    glInit();
    gameSetup();
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

    __android_log_write(ANDROID_LOG_INFO, "TheElements", saveLoc3);

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

    __android_log_write(ANDROID_LOG_INFO, "TheElements", loadLoc3);

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
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "saveTempState");
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
    __android_log_write(ANDROID_LOG_INFO, "TheElements", "nativeInit()");
    atmosphereSetup();
    elementSetup();
    particleSetup();

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

    char buffer[100];
    sprintf(buffer, "blue: %d", blueValue);
    __android_log_write(ANDROID_LOG_INFO, "TheElements", buffer);

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
Java_com_idkjava_thelements_preferences_Preferences_setAtmosphereGravity(JNIEnv* env, jobject thiz, jfloat gravity)
{
    cAtmosphere->gravity = gravity;
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
Java_com_idkjava_thelements_game_SandView_setMouseLocation(JNIEnv* env, jobject thiz, jchar state, jint x, jint y)
{
    pthread_mutex_lock(&mouse_mutex);
    float modViewWidth = viewWidth * zoomScale;
    float modViewHeight = viewHeight * zoomScale;
    if ( !isPanMode ) {
        mouseX = ((float)x/(float)screenWidth)*modViewWidth + ((float)centerX - (modViewWidth/2.0));
        mouseY = ((float)y/(float)screenHeight)*modViewHeight + ((float)centerY - (float)(modViewHeight/2.0));
    }
    else {
        mouseX = x;
        mouseY = y;
    }

    // Translate x and y coords to zoomed coords
    mouseX /= zoomFactor;
    mouseY /= zoomFactor;

    if (state != 2 && fingerDown != state)
    {
        fingerDown = state;

        // If we're got ACTION_DOWN, then we don't want to continue drawing
        // from previous mouse location.
        if (fingerDown)
        {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
    }

    pthread_mutex_unlock(&mouse_mutex);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_MainActivity_setFilterMode(JNIEnv* env, jobject thiz, jchar mode)
{
  filterType = mode;
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_setPinchScale(JNIEnv* env, jobject thiz, jfloat scale)
{
  zoomScale = scale;
}


JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_setPinchActive(JNIEnv* env, jobject thiz, jchar active)
{
    isPinch = active;
    if (!active) 
    {
        viewWidth = viewWidth * zoomScale;
        viewHeight = viewHeight * zoomScale;
        zoomScale = 1.0;
    }
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_game_SandView_setIsPanMode(JNIEnv* env, jobject thiz, jchar isPan)
{
  isPanMode = isPan;
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
Java_com_idkjava_thelements_video_VideoLib_init(JNIEnv *env, jclass clazz,
    jstring key, jstring secret, jstring gameName) {
    const char* nativeKey = env->GetStringUTFChars(key, NULL);
    const char* nativeSecret = env->GetStringUTFChars(secret, NULL);
    const char* nativeName = env->GetStringUTFChars(gameName, NULL);

    Kamcord_Init(nativeKey, nativeSecret, nativeName, KC_STANDARD_VIDEO_QUALITY);

    env->ReleaseStringUTFChars(key, nativeKey);
    env->ReleaseStringUTFChars(secret, nativeSecret);
    env->ReleaseStringUTFChars(gameName, nativeName);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_video_VideoLib_initActivity(JNIEnv *env, jclass clazz, jobject act) {
    Kamcord_InitActivity(act);
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_video_VideoLib_startRecording(JNIEnv *env, jclass clazz) {
    Kamcord_StartRecording();
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_video_VideoLib_stopRecording(JNIEnv *env, jclass clazz) {
    Kamcord_StopRecording();
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_video_VideoLib_pauseRecording(JNIEnv *env, jclass clazz) {
    Kamcord_Pause();
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_video_VideoLib_resumeRecording(JNIEnv *env, jclass clazz) {
    Kamcord_Resume();
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_video_VideoLib_showView(JNIEnv *env, jclass clazz) {
    Kamcord_ShowView();
}

JNIEXPORT void JNICALL
Java_com_idkjava_thelements_video_VideoLib_showWatchView(JNIEnv *env, jclass clazz) {
    Kamcord_ShowWatchView();
}

JNIEXPORT jboolean JNICALL
Java_com_idkjava_thelements_video_VideoLib_isWhitelisted(JNIEnv *env, jclass clazz) {
    return (jboolean) Kamcord_IsWhitelisted();
}

// extern "C"
#if __cplusplus
}
#endif
