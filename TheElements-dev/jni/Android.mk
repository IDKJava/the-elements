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
KAMCORD = no
ifeq ($(TARGET_ARCH_ABI),armeabi)
    TARGET_ARCH_PROFILING_OK = yes
endif
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    TARGET_ARCH_PROFILING_OK = yes
    KAMCORD = yes
endif
ifneq ($(TARGET_ARCH_PROFILING_OK),yes)
    USE_PROFILING = no
endif

# Print some build info
$(warning $(APP_OPTIM) build for $(TARGET_ARCH_ABI), profiling: $(USE_PROFILING))

LOCAL_CFLAGS := -DANDROID_NDK
ifeq ($(USE_PROFILING),yes)
    LOCAL_CFLAGS += -DUSE_PROFILING
endif
ifeq ($(KAMCORD),yes)
    LOCAL_CFLAGS += -DUSE_KAMCORD
    LOCAL_SHARED_LIBRARIES := libkamcord
    LOCAL_HEADER_FILES := $(LOCAL_PATH)/Kamcord-C-Interface.h
endif

# optimization level = 3
LOCAL_CFLAGS += -O3
LOCAL_CFLAGS += -w
LOCAL_CFLAGS += -Wall -Wextra
LOCAL_LDLIBS :=  -llog -ldl -landroid -lEGL -lGLESv2

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
    points.c \
    rendergl.c \
    saveload.c \
    setup.c \
    specials.c \
    update.c \

include $(BUILD_SHARED_LIBRARY)
include $(CLEAR_VARS)

ifeq ($(KAMCORD),yes)
    LOCAL_MODULE := libkamcord
    LOCAL_SRC_FILES := libkamcord.so
    include $(PREBUILT_SHARED_LIBRARY)
endif

ifeq ($(USE_PROFILING),yes)
    $(call import-module,android-ndk-profiler)
endif
