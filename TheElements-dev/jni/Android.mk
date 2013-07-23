LOCAL_PATH := $(call my-dir)
$(call import-add-path, $(LOCAL_PATH)/../../)

include $(CLEAR_VARS)

LOCAL_MODULE := thelements

# Print whether this is a debug or release build
ifeq ($(APP_OPTIM),debug)
    $(warning DEBUG build)
else
    $(warning RELEASE build)
endif

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL

# optimization level = 3
LOCAL_CFLAGS += -O3

# compile with profiling
ifeq ($(APP_OPTIM),debug)
    LOCAL_CFLAGS += -pg -fno-omit-frame-pointer -fno-function-sections
    LOCAL_STATIC_LIBRARIES := android-ndk-profiler
endif

LOCAL_SRC_FILES := \
    app-android.c \
	app.c \
	collide.c \
	elementproperties.c \
    importgl.c \
    points.c \
    rendergl.c \
    saveload.c \
    setup.c \
    specials.c \
    update.c \

LOCAL_LDLIBS := -lGLESv1_CM -lEGL -ldl -llog

include $(BUILD_SHARED_LIBRARY)

ifeq ($(APP_OPTIM),debug)
    $(call import-module,android-ndk-profiler)
endif