#!/bin/sh

BASEDIR=$(dirname $0)

protoc --cpp_out=$BASEDIR/jni --proto_path=$BASEDIR/proto $BASEDIR/proto/messages.proto