#!/bin/sh

BASEDIR=$(dirname $0)
PROTOC=${BASEDIR}/../protobuf/src/protoc

${PROTOC} --cpp_out=$BASEDIR/jni --proto_path=$BASEDIR/proto $BASEDIR/proto/messages.proto
cd ${BASEDIR}/jni
${ANDROID_NDK_HOME}/ndk-build
