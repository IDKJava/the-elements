/*
 * macros.h
 * -------------------------------
 * Some overall macros needed by the JNI code.
 * These are not all of the macros defined, but
 * are ones that are global and may need to be
 * changed often, especially NUM_BASE_ELEMENTS
 * (when adding new elements)
 */

#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

/*
 * CONSTANTS
 */
#define MAX_POINTS 300000
#define NUM_BASE_ELEMENTS 33
#define MAX_SPECIALS 6
#define MAX_ELEMENTS 256

#define MAX_CE_NAME_LENGTH 256
#define MAX_UDID_LENGTH 256

#define NUM_COLLISIONS 13

#define DEFAULT_BRUSH_SIZE 4

#define TRUE 1
#define FALSE 0

#define FINGER_DOWN 1
#define FINGER_UP 0

#define DEFAULT_ZOOM_FACTOR 4

#define NORMAL_ELEMENT 3

#define SPAWN_ELEMENT 0
#define DRAG_ELEMENT 1
#define ERASER_ELEMENT 2
#define SAND_ELEMENT 3
#define WATER_ELEMENT 4
#define STEAM_ELEMENT 5
#define ICE_ELEMENT 6
#define WALL_ELEMENT 7
#define DESTRUCTIBLE_WALL_ELEMENT 8
#define PLANT_ELEMENT 9
#define FIRE_ELEMENT 10
#define MAGMA_ELEMENT 11
#define STONE_ELEMENT 12
#define OIL_ELEMENT 13
#define C4_ELEMENT 14
#define C4PP_ELEMENT 15
#define FUSE_ELEMENT 16
#define ACID_ELEMENT 17
#define SALT_ELEMENT 18
#define SALT_WATER_ELEMENT 19
#define GLASS_ELEMENT 20
#define MUD_ELEMENT 21
#define GENERATOR_ELEMENT 22
#define COAL_ELEMENT 23
#define ANT_ELEMENT 24
#define GUNPOWDER_ELEMENT 25
#define MOSQUITO_ELEMENT 26
#define WOOD_ELEMENT 27
#define TERMITE_ELEMENT 28
#define INSECT_KILLER_ELEMENT 29
#define ELECTRICITY_ELEMENT 30
#define METAL_ELEMENT 31
#define PORTAL_ELEMENT 32

#define GENERATOR_SPAWN_PROB 200

#define INERTIA_UNMOVABLE 255

#define DEFAULT_ATMOSPHERE_TEMP 100
#define DEFAULT_ATMOSPHERE_GRAVITY 1

#define DEFAULT_RED 0
#define DEFAULT_GREEN 0
#define DEFAULT_BLUE 0

#define DEFAULT_BORDER_LEFT 1
#define DEFAULT_BORDER_TOP 1
#define DEFAULT_BORDER_RIGHT 1
#define DEFAULT_BORDER_BOTTOM 1

#define SPECIAL_NONE -1
#define SPECIAL_SPAWN 1
#define SPECIAL_BREAK 2
#define SPECIAL_GROW 3
#define SPECIAL_HEAT 4
#define SPECIAL_EXPLODE 5
#define SPECIAL_LIFE 6
#define SPECIAL_WANDER 7
#define SPECIAL_JUMP 8
#define SPECIAL_TUNNEL 9
#define SPECIAL_BURN 10
#define SPECIAL_CONDUCTIVE 11
#define SPECIAL_CONDUCTABLE 12
#define SPECIAL_TRAIL 13
#define SPECIAL_PORTAL 14

#define ELECTRIC_NO_DIR 0 // 0000
#define ELECTRIC_X1     1 // 0001
#define ELECTRIC_Y1     2 // 0010
#define ELECTRIC_XN     4 // 0100
#define ELECTRIC_YN     8 // 1000
#define ELECTRIC_WAIT  16 //10000
#define SPECIAL_VAL_UNSET -1

#define FILTER_NONE 0
#define FILTER_MOTION 1

#define WORLD_EARTH 0
#define WORLD_SPACE 1

// Size of the gravity field blocks
#define GF_BLOCK_SIZE 3

#define BLACK_HOLE 0
#define WHITE_HOLE 1
#define CURL_HOLE 2
#define NULL_GRAVITY 3
#define MAX_SPACE_OBJS 100


// Portal properties
#define MAX_PORTALS 10
#define NOT_PAIRED -1

// Brush props
#define BRUSH_NORMAL 0
#define BRUSH_SPRAY 1
#define BRUSH_SLINGSHOT 2

// Render overlay type
#define RT_RECT 0
#define RT_LINE 1
#define RT_CIRCLE 2
#define RT_TRI 3

/*
 * LOGGING
 */
#if __ANDROID__
// ANDROID logging
#include <android/log.h>
#define LOG_STR "TheElements"
#define LOGI(...) if (LOGGING) __android_log_print(ANDROID_LOG_INFO, LOG_STR, __VA_ARGS__)
#define LOGD(...) if (LOGGING) __android_log_print(ANDROID_LOG_DEBUG, LOG_STR, __VA_ARGS__)
#define LOGW(...) if (LOGGING) __android_log_print(ANDROID_LOG_WARN, LOG_STR, __VA_ARGS__)
#define LOGE(...) if (LOGGING) __android_log_print(ANDROID_LOG_ERROR, LOG_STR, __VA_ARGS__)

#else
// PC logging
#include <stdio.h>
#define LOGI(...) if (LOGGING) {printf("INFO: "); printf(__VA_ARGS__); printf("\n");}
#define LOGD(...) if (LOGGING) {printf("DEBUG: "); printf(__VA_ARGS__); printf("\n");}
#define LOGW(...) if (LOGGING) {printf("WARN: "); printf(__VA_ARGS__); printf("\n");}
#define LOGE(...) if (LOGGING) {printf("ERROR: "); printf(__VA_ARGS__); printf("\n");}

#endif



//Sample log call
//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Hi!");

#endif //!MACROS_H_INCLUDED
