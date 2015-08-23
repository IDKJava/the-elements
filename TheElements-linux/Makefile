JNI_DIR = ../TheElements-dev/jni

SRCS = main.cpp
EXT_SRCS = $(JNI_DIR)/rendergl.cc \
           $(JNI_DIR)/app.cc
SRCS += $(EXT_SRCS)

OUT = thelements
BIN_DIR = bin
CC = g++
MKDIR = mkdir
LINKER_FLAGS = -lSDL2 -lX11 -lEGL -lGLESv2
COMPILE_FLAGS = -I../TheElements-dev/jni/

all: $(SRCS)
	$(MKDIR) -p $(BIN_DIR)
	$(CC) $(SRCS) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $(BIN_DIR)/$(OUT)
