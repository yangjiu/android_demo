LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := NDK_demo
LOCAL_SRC_FILES := NDK_demo.cpp

include $(BUILD_SHARED_LIBRARY)
