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
#define MAX_SPECIALS 6
#define MAX_ELEMENTS 256

#define DEFAULT_BRUSH_SIZE 4

#define ZOOMED_IN 1
#define ZOOMED_OUT 0

#define TRUE 1
#define FALSE 0

#define FINGER_DOWN 1
#define FINGER_UP 0

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

//Sample log call
//__android_log_write(ANDROID_LOG_INFO, "TheElements", "Hi!");

#endif //!MACROS_H_INCLUDED
