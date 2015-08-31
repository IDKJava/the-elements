#ifndef INTERFACE_H
#define INTERFACE_H

#include <jni.h>

extern "C" {
  void Java_com_idkjava_thelements_MainActivity_nativeInit(JNIEnv* env, jobject thiz);
  void Java_com_idkjava_thelements_MainActivity_setElement(JNIEnv* env, jobject thiz, jchar element);
  void Java_com_idkjava_thelements_game_SandViewRenderer_nativeResize(JNIEnv* env, jobject thiz, jint width, jint height);
  void Java_com_idkjava_thelements_game_SandViewRenderer_nativeRender(JNIEnv* env, jobject thiz);
  void Java_com_idkjava_thelements_game_SandView_brushStartLocation(JNIEnv* env, jobject thiz, jint x, jint y);
  void Java_com_idkjava_thelements_game_SandView_brushMoveLocation(JNIEnv* env, jobject thiz, jint x, jint y);
  void Java_com_idkjava_thelements_game_SandView_brushEndLocation(JNIEnv* env, jobject thiz, jint x, jint y);
  void Java_com_idkjava_thelements_preferences_Preferences_setZoom(JNIEnv* env, jobject thiz, jint zoom);
}

inline void nativeInit() {
  Java_com_idkjava_thelements_MainActivity_nativeInit(NULL, NULL);
}
inline void setElement(int eltInd) {
  Java_com_idkjava_thelements_MainActivity_setElement(NULL, NULL, eltInd);
}
inline void nativeResize(int width, int height) {
  Java_com_idkjava_thelements_game_SandViewRenderer_nativeResize(NULL, NULL, width, height);
}
inline void nativeRender() {
  Java_com_idkjava_thelements_game_SandViewRenderer_nativeRender(NULL, NULL);
}
inline void brushStartLocation(int x, int y) {
  Java_com_idkjava_thelements_game_SandView_brushStartLocation(NULL, NULL, x, y);
}
inline void brushMoveLocation(int x, int y) {
  Java_com_idkjava_thelements_game_SandView_brushMoveLocation(NULL, NULL, x, y);
}
inline void brushEndLocation(int x, int y) {
  Java_com_idkjava_thelements_game_SandView_brushEndLocation(NULL, NULL, x, y);
}
inline void setZoom(int zoom) {
  Java_com_idkjava_thelements_preferences_Preferences_setZoom(NULL, NULL, zoom);
}
                          

#endif // INTERFACE_H
