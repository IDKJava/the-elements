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
4. Add ANDROID_HOME and ANDROID_NDK_HOME to env variables.
