/*
 * macros.h
 * -------------------------------
 * Some overall macros needed by the JNI code.
 * These are not all of the macros defined, but
 * are ones that are global and may need to be
 * changed often, especially TElements (when
 * adding new elements)
 */

#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define MAX_POINTS 500000
#define NUM_BASE_ELEMENTS 25

#define PLAY 1
#define PAUSE 0

#define DEFAULT_BRUSH_SIZE 4

#define ZOOMED_IN 1
#define ZOOMED_OUT 2

#define TRUE 1
#define FALSE 0

#define NORMAL_SAVE 0
#define QUICK_SAVE 1
#define NORMAL_LOAD 0
#define QUICK_LOAD 1

#define EMPTY -1

#define FINGER_DOWN 1
#define FINGER_UP 0

//Sample log call
//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Hi!");

#endif
