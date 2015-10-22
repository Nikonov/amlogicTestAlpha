NDK_TOOLCHAIN_VERSION := 4.8
APP_ABI := armeabi
APP_CPPFLAGS += -std=c++11 -D__STDC_CONSTANT_MACROS
APP_STL := gnustl_static
LOCAL_C_INCLUDES += ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/include

