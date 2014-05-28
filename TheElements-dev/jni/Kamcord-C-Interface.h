
#ifndef __KAMCORD_C_INTERFACE_H__
#define __KAMCORD_C_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#if __ANDROID__

#include <stdbool.h>
#include <jni.h>

typedef enum KC_VIDEO_QUALITY
{
    KC_STANDARD_VIDEO_QUALITY = 0,
    KC_TRAILER_VIDEO_QUALITY = 1,  // Trailers only. Do not release your game with this setting.
} KC_VIDEO_QUALITY;


void KamcordAndroid_InitJVMWithEnv(JNIEnv * env);
void KamcordAndroid_InitJVM(JavaVM * vm);

void KamcordAndroid_InitActivity(
    JNIEnv* env,
    jobject activity);

void KamcordAndroid_Init(
    JNIEnv* env,
    const char* developerKey,
    const char* developerSecret,
    const char* appName,
    KC_VIDEO_QUALITY videoQuality);

void KamcordAndroid_SetLoggingEnabled(JNIEnv* env, bool value);

void KamcordAndroid_WhitelistAllBoards(JNIEnv* env);

void KamcordAndroid_BlacklistAllBoards(JNIEnv* env);

void KamcordAndroid_WhitelistBoard(JNIEnv* env, const char* boardName);

void KamcordAndroid_BlacklistBoard(JNIEnv* env, const char* boardName);

const char* KamcordAndroid_GetBoard(JNIEnv* env);

const char* KamcordAndroid_Version(JNIEnv* env);

bool KamcordAndroid_IsEnabled(JNIEnv* env);

void KamcordAndroid_StartRecording(JNIEnv* env);

void KamcordAndroid_StopRecording(JNIEnv* env);

void KamcordAndroid_Pause(JNIEnv* env);

void KamcordAndroid_Resume(JNIEnv* env);

bool KamcordAndroid_IsRecording(JNIEnv* env);

bool KamcordAndroid_IsPaused(JNIEnv* env);

void KamcordAndroid_CreateFramebuffers(JNIEnv* env, int width, int height);

void KamcordAndroid_DeleteFramebuffers(JNIEnv* env);

void KamcordAndroid_BeginDraw(JNIEnv* env);

void KamcordAndroid_EndDraw(JNIEnv* env);

void KamcordAndroid_SetVideoTitle(JNIEnv* env, const char* title);

void KamcordAndroid_SetLevelAndScore(
    JNIEnv* env,
    const char* level,
    double score);

void KamcordAndroid_SetVideoQuality(
    JNIEnv* env,
    KC_VIDEO_QUALITY videoQuality);

void KamcordAndroid_ShowView(JNIEnv* env);

void KamcordAndroid_ShowWatchView(JNIEnv* env);

void KamcordAndroid_SetFacebookDescription(
    JNIEnv* env,
    const char* description);

void KamcordAndroid_SetYouTubeSettings(
    JNIEnv* env,
    const char* description,
    const char * tags);

void KamcordAndroid_SetDefaultTweet(
    JNIEnv* env,
    const char* tweet);

void KamcordAndroid_SetTwitterDescription(
    JNIEnv* env,
    const char* twitterDescription);

void KamcordAndroid_SetDefaultEmailSubject(
    JNIEnv* env,
    const char* subject);

void KamcordAndroid_SetDefaultEmailBody(
    JNIEnv* env,
    const char* body);



void Kamcord_Init(
    const char* developerKey,
    const char* developerSecret,
    const char* appName,
    KC_VIDEO_QUALITY videoQuality);

void Kamcord_WhitelistAllBoards();

void Kamcord_BlacklistAllBoards();

void Kamcord_WhitelistBoard(const char* boardName);

void Kamcord_BlacklistBoard(const char* boardName);

const char* Kamcord_GetBoard();

const char* Kamcord_Version();

bool Kamcord_IsEnabled();

void Kamcord_StartRecording();

void Kamcord_StopRecording();

void Kamcord_Pause();

void Kamcord_Resume();

bool Kamcord_IsRecording();

bool Kamcord_IsPaused();

void Kamcord_CreateFramebuffers(int width, int height);

void Kamcord_DeleteFramebuffers();

void Kamcord_BeginDraw();

void Kamcord_EndDraw();

void Kamcord_SetVideoTitle(const char* title);

void Kamcord_SetLevelAndScore(
    const char* level,
    double score);

void Kamcord_SetVideoQuality(
    KC_VIDEO_QUALITY videoQuality);

void Kamcord_ShowView();

void Kamcord_ShowWatchView();

void Kamcord_SetFacebookDescription(
    const char* description);

void Kamcord_SetYouTubeSettings(
    const char* description,
    const char * tags);

void Kamcord_SetDefaultTweet(
    const char* tweet);

void Kamcord_SetTwitterDescription(
    const char* twitterDescription);

void Kamcord_SetDefaultEmailSubject(
    const char* subject);

void Kamcord_SetDefaultEmailBody(
    const char* body);

#endif // __ANDROID__

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ifndef

