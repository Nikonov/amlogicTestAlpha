LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/armeabi/amcodec/include

LOCAL_SHARED_LIBRARIES += libz libc
LOCAL_LDFLAGS += -L$(LOCAL_PATH)/../jniLibs/armeabi
LOCAL_LDLIBS += -lamcodec -llog

LOCAL_SRC_FILES:= \
	test.cpp

LOCAL_MODULE := alphatest

include $(BUILD_SHARED_LIBRARY)
