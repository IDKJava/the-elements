Installing
==========
0. Set up Android Studio.
1. Clone the repo (with submodules):
   git clone --recursive git@github.com:IDKJava/the-elements.git
2. Build the protobuf compiler:
   -- cd protobuf
   -- ./autogen.sh
   -- ./configure
   -- make -j
   -- Confirm protobuf/src/protoc exists.
3. Download the Android NDK.
4. Set ANDROID_HOME and ANDROID_NDK_HOME env variables.
5. Set IDKJAVA_KEYSTORE to point to the keystore file location, and
   IDKJAVA_KEYALIAS to the key alias within the keystore.
6. If using Android Studio GUI to build the gradle project, then create
   a gradle.properties file under TheElements-dev, with ndkdir=path/to/ndk
   to ensure that the gradle spawned under Android Studio can find the NDK.

Updating
=======
1. git pull
2. DON'T FORGET: `git submodule sync`
   This will receive any changes to submodule HEAD.

Building an APK (gradle)
========================
The root of the repository contains a gradlew shell wrapper.
To build a debug build:
-- ./gradlew assembleDebug
To build a release build:
-- ./gradlew assembleRelease
-- Respond to the password prompts for keystore signing
The built apk will be outputted to TheElements-dev/build/outputs/apk/.
