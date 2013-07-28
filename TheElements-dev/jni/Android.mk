LOCAL_PATH := $(call my-dir)
$(call import-add-path, $(LOCAL_PATH)/../../)

include $(CLEAR_VARS)

LOCAL_MODULE := thelements

# Use profiling or not?
USE_PROFILING = yes
ifneq ($(APP_OPTIM),debug)
    USE_PROFILING = no
endif
TARGET_ARCH_PROFILING_OK = no
ifeq ($(TARGET_ARCH_ABI),armeabi)
    TARGET_ARCH_PROFILING_OK = yes
endif
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    TARGET_ARCH_PROFILING_OK = yes
endif
ifneq ($(TARGET_ARCH_PROFILING_OK),yes)
    USE_PROFILING = no
endif

# Print some build info
$(warning $(APP_OPTIM) build for $(TARGET_ARCH_ABI), profiling: $(USE_PROFILING))

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL
ifeq ($(USE_PROFILING),yes)
    LOCAL_CFLAGS += -DUSE_PROFILING
endif

# optimization level = 3
LOCAL_CFLAGS += -O3

# compile with profiling
ifeq ($(USE_PROFILING),yes)
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

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)

ifeq ($(USE_PROFILING),yes)
    $(call import-module,android-ndk-profiler)
endif