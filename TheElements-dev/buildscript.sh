#!/bin/sh

BASEDIR=$(dirname $0)
PROTOC=${BASEDIR}/../protobuf/src/protoc

${PROTOC} --cpp_out=$BASEDIR/jni --proto_path=$BASEDIR/proto $BASEDIR/proto/messages.proto
# cd ${BASEDIR}/jni
# if [ "$ANDROID_NDK_HOME" ]; then
#     # Command-line mode
#     echo "ANDROID_NDK_HOME set to $ANDROID_NDK_HOME"
#     ${ANDROID_NDK_HOME}/ndk-build -j
# else
#     # Android Studio mode
#     echo "Must set ANDROID_NDK_HOME."
#     exit 1
# fi
