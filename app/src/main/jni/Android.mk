LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Threads
LOCAL_SRC_FILES := threads.cpp
LOCAL_C_INCLUDE := $(LOCAL_PATH)/include

LOCAL_LDLIBS = -llog
include $(BUILD_SHARED_LIBRARY)