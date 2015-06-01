LOCAL_PATH := $(call my-dir)
$(call import-add-path, $(LOCAL_PATH)/../../)

### Google Proto buffer support ###
include $(CLEAR_VARS)
CC_LITE_SRC_FILES := \
    google/protobuf/stubs/common.cc                              \
    google/protobuf/stubs/once.cc                                \
    google/protobuf/extension_set.cc                             \
    google/protobuf/generated_message_util.cc                    \
    google/protobuf/message_lite.cc                              \
    google/protobuf/repeated_field.cc                            \
    google/protobuf/wire_format_lite.cc                          \
    google/protobuf/io/coded_stream.cc                           \
    google/protobuf/io/zero_copy_stream.cc                       \
    google/protobuf/io/zero_copy_stream_impl_lite.cc


COMPILER_SRC_FILES :=  \
    google/protobuf/descriptor.cc \
    google/protobuf/descriptor.pb.cc \
    google/protobuf/descriptor_database.cc \
    google/protobuf/dynamic_message.cc \
    google/protobuf/extension_set.cc \
    google/protobuf/extension_set_heavy.cc \
    google/protobuf/generated_message_reflection.cc \
    google/protobuf/generated_message_util.cc \
    google/protobuf/message.cc \
    google/protobuf/message_lite.cc \
    google/protobuf/reflection_ops.cc \
    google/protobuf/repeated_field.cc \
    google/protobuf/service.cc \
    google/protobuf/text_format.cc \
    google/protobuf/unknown_field_set.cc \
    google/protobuf/wire_format.cc \
    google/protobuf/wire_format_lite.cc \
    google/protobuf/compiler/code_generator.cc \
    google/protobuf/compiler/command_line_interface.cc \
    google/protobuf/compiler/importer.cc \
    google/protobuf/compiler/main.cc \
    google/protobuf/compiler/parser.cc \
    google/protobuf/compiler/plugin.cc \
    google/protobuf/compiler/plugin.pb.cc \
    google/protobuf/compiler/subprocess.cc \
    google/protobuf/compiler/zip_writer.cc \
    google/protobuf/compiler/cpp/cpp_enum.cc \
    google/protobuf/compiler/cpp/cpp_enum_field.cc \
    google/protobuf/compiler/cpp/cpp_extension.cc \
    google/protobuf/compiler/cpp/cpp_field.cc \
    google/protobuf/compiler/cpp/cpp_file.cc \
    google/protobuf/compiler/cpp/cpp_generator.cc \
    google/protobuf/compiler/cpp/cpp_helpers.cc \
    google/protobuf/compiler/cpp/cpp_message.cc \
    google/protobuf/compiler/cpp/cpp_message_field.cc \
    google/protobuf/compiler/cpp/cpp_primitive_field.cc \
    google/protobuf/compiler/cpp/cpp_service.cc \
    google/protobuf/compiler/cpp/cpp_string_field.cc \
    google/protobuf/compiler/java/java_enum.cc \
    google/protobuf/compiler/java/java_enum_field.cc \
    google/protobuf/compiler/java/java_extension.cc \
    google/protobuf/compiler/java/java_field.cc \
    google/protobuf/compiler/java/java_file.cc \
    google/protobuf/compiler/java/java_generator.cc \
    google/protobuf/compiler/java/java_helpers.cc \
    google/protobuf/compiler/java/java_message.cc \
    google/protobuf/compiler/java/java_message_field.cc \
    google/protobuf/compiler/java/java_primitive_field.cc \
    google/protobuf/compiler/java/java_service.cc \
    google/protobuf/compiler/javamicro/javamicro_enum.cc \
    google/protobuf/compiler/javamicro/javamicro_enum_field.cc \
    google/protobuf/compiler/javamicro/javamicro_field.cc \
    google/protobuf/compiler/javamicro/javamicro_file.cc \
    google/protobuf/compiler/javamicro/javamicro_generator.cc \
    google/protobuf/compiler/javamicro/javamicro_helpers.cc \
    google/protobuf/compiler/javamicro/javamicro_message.cc \
    google/protobuf/compiler/javamicro/javamicro_message_field.cc \
    google/protobuf/compiler/javamicro/javamicro_primitive_field.cc \
    google/protobuf/compiler/python/python_generator.cc \
    google/protobuf/io/coded_stream.cc \
    google/protobuf/io/gzip_stream.cc \
    google/protobuf/io/printer.cc \
    google/protobuf/io/tokenizer.cc \
    google/protobuf/io/zero_copy_stream.cc \
    google/protobuf/io/zero_copy_stream_impl.cc \
    google/protobuf/io/zero_copy_stream_impl_lite.cc \
    google/protobuf/stubs/common.cc \
    google/protobuf/stubs/hash.cc \
    google/protobuf/stubs/once.cc \
    google/protobuf/stubs/structurally_valid.cc \
    google/protobuf/stubs/strutil.cc \
    google/protobuf/stubs/substitute.cc

# C++ full library
# =======================================================
#include $(CLEAR_VARS)

LOCAL_MODULE := libprotobuf
LOCAL_MODULE_TAGS := optional

LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES := \
    $(CC_LITE_SRC_FILES)                                             \
    google/protobuf/stubs/strutil.cc                             \
    google/protobuf/stubs/substitute.cc                          \
    google/protobuf/stubs/structurally_valid.cc                  \
    google/protobuf/descriptor.cc                                \
    google/protobuf/descriptor.pb.cc                             \
    google/protobuf/descriptor_database.cc                       \
    google/protobuf/dynamic_message.cc                           \
    google/protobuf/extension_set_heavy.cc                       \
    google/protobuf/generated_message_reflection.cc              \
    google/protobuf/message.cc                                   \
    google/protobuf/reflection_ops.cc                            \
    google/protobuf/service.cc                                   \
    google/protobuf/text_format.cc                               \
    google/protobuf/unknown_field_set.cc                         \
    google/protobuf/wire_format.cc                               \
    google/protobuf/io/gzip_stream.cc                            \
    google/protobuf/io/printer.cc                                \
    google/protobuf/io/tokenizer.cc                              \
    google/protobuf/io/zero_copy_stream_impl.cc                  \
    google/protobuf/compiler/importer.cc                         \
    google/protobuf/compiler/parser.cc

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/src

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/android \
    bionic \
    $(LOCAL_PATH)/google \
    $(JNI_H_INCLUDE)

LOCAL_SHARED_LIBRARIES := \
    libz libcutils libutils
LOCAL_LDLIBS := -lz
# stlport conflicts with the host stl library
ifneq ($(TARGET_SIMULATOR),true)
LOCAL_C_INCLUDES += external/stlport/stlport
LOCAL_SHARED_LIBRARIES += libstlport
endif

LOCAL_CFLAGS := -DGOOGLE_PROTOBUF_NO_RTTI

include $(BUILD_SHARED_LIBRARY)
### End Google Proto buffer ###

include $(CLEAR_VARS)

LOCAL_MODULE := thelements

# Pull in libprotobuf
LOCAL_LDLIBS := -L$(LOCAL_PATH)/../libs/$(TARGET_ARCH_ABI)
LOCAL_SHARED_LIBRARIES := protobuf

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

LOCAL_CFLAGS := -DANDROID_NDK
LOCAL_LDLIBS +=  -llog -ldl -landroid -lEGL -lGLESv2 -lprotobuf
ifeq ($(USE_PROFILING),yes)
    LOCAL_CFLAGS += -DUSE_PROFILING
endif

KAMCORD = yes
ifeq ($(KAMCORD),yes)
    LOCAL_CFLAGS += -DUSE_KAMCORD
    LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../kamcord-android-sdk/kamcord/jni/
    LOCAL_SHARED_LIBRARIES := libkamcord
    LOCAL_HEADER_FILES := $(LOCAL_PATH)/../../kamcord-android-sdk/kamcord/jni/Kamcord-C-Interface.h
    LOCAL_LD_LIBS += $(LOCAL_PATH)/../../kamcord-android-sdk/kamcord/libs/$(TARGET_ARCH_ABI)/ -lkamcord
endif

# stlport conflicts with the host stl library
ifneq ($(TARGET_SIMULATOR),true)
LOCAL_C_INCLUDES += external/stlport/stlport
LOCAL_SHARED_LIBRARIES += libstlport
endif

# optimization level = 3
LOCAL_CFLAGS += -O3
LOCAL_CFLAGS += -w
LOCAL_CFLAGS += -Wall -Wextra

LOCAL_CPP_EXTENSION := .cc

# compile with profiling
ifeq ($(USE_PROFILING),yes)
    LOCAL_CFLAGS += -pg -fno-omit-frame-pointer -fno-function-sections
    LOCAL_STATIC_LIBRARIES := android-ndk-profiler
endif

LOCAL_SRC_FILES := \
    app-android.cc \
	app.cc \
	collide.cc \
	collisions.cc \
	elementproperties.cc \
	messages.pb.cc \
    points.cc \
    rendergl.cc \
    saveload.cc \
    saveload2.cc \
    setup.cc \
    specials.cc \
    update.cc	

$(LOCAL_PATH)/messages.pb.h: $(LOCAL_PATH)/../proto/messages.proto
	echo "Build messages.pb.h"
	protoc $(LOCAL_PATH)/../proto/messages.proto \
		--proto_path=$(LOCAL_PATH)/../proto/ --cpp_out=$(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)

ifeq ($(USE_PROFILING),yes)
    $(call import-module,android-ndk-profiler)
endif
