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

#define MAX_POINTS 100000
#define NUM_BASE_ELEMENTS 24

#define DEFAULT_BRUSH_SIZE 4

#define ZOOMED_IN 1
#define ZOOMED_OUT 0

#define TRUE 1
#define FALSE 0

#define FINGER_DOWN 1
#define FINGER_UP 0

#define SPAWN_ELEMENT 0
#define DRAG_ELEMENT 1
#define ERASER_ELEMENT 2
#define NORMAL_ELEMENT 3

#define INERTIA_UNMOVABLE 255

#define SPECIAL_NOT_SET -1

//Sample log call
//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Hi!");

#endif //!MACROS_H_INCLUDED
