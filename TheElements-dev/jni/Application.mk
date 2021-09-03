APP_PLATFORM := android-29
APP_ABI := armeabi-v7a arm64-v8a x86 x86_64
APP_STL := c++_shared
# For ASan
ifeq ($(APP_OPTIM),debug)
    APP_CFLAGS := -fsanitize=address -fno-omit-frame-pointer
    APP_LDFLAGS := -fsanitize=address
endif