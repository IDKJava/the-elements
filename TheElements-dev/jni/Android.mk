LOCAL_PATH := $(call my-dir)
$(call import-add-path, $(LOCAL_PATH)/../../)

include $(CLEAR_VARS)

LOCAL_MODULE := thelements

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
				-pg
# compile with profiling
LOCAL_STATIC_LIBRARIES := android-ndk-profiler

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

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android-ndk-profiler)