BIN_DIR = bin
OBJ_DIR = obj
CC = g++
MKDIR = mkdir
JNI_DIR = ../TheElements-dev/jni

LOCAL_SRCS = main.cc \
             palette.cc
EXT_BARE_SRCS = app.cc \
           app-android.cc \
           camera.cc \
           collide.cc \
           collisions.cc \
           elementproperties.cc \
           gravity.cc \
           messages.pb.cc \
           points.cc \
           rendergl.cc \
           saveload.cc \
           saveload2.cc \
           setup.cc \
           specials.cc \
           update.cc

# Google Protobuf
CC_LITE_SRC_FILES = \
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


COMPILER_SRC_FILES =  \
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
PROTOBUF_BARE_SRCS = \
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

PROTOBUF_SRCS = $(addprefix $(JNI_DIR)/, $(PROTOBUF_BARE_SRCS))
PROTOBUF_OUT = $(BIN_DIR)/libprotobuf.so
PROTOBUF_LINKER_FLAGS = -lz


EXT_SRCS = $(addprefix $(JNI_DIR)/, $(EXT_BARE_SRCS))
COMPILE_FLAGS = -DGOOGLE_PROTOBUF_NO_RTTI


SRCS = $(EXT_SRCS) $(LOCAL_SRCS)

OUT = thelements
LINKER_FLAGS = -lSDL2 -lX11 -lEGL -lGLESv2 -pthread
LINKER_FLAGS += -L$(BIN_DIR) -lprotobuf
COMPILE_FLAGS += -I$(JNI_DIR) \
                 -I$(JNI_DIR)/google \
                 -I$(JAVA_HOME)/include/ \
                 -I$(JAVA_HOME)/include/linux

.PHONY: all
$(PROTOBUF_OUT): $(PROTOBUF_SRCS)
	$(MKDIR) -p $(BIN_DIR)
	$(CC) -fPIC -shared $(PROTOBUF_SRCS) $(COMPILE_FLAGS) \
              $(PROTOBUF_LINKER_FLAGS) -o $(PROTOBUF_OUT)

all: $(SRCS) $(PROTOBUF_OUT)
	$(MKDIR) -p $(BIN_DIR)
	$(CC) $(SRCS) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $(BIN_DIR)/$(OUT)
