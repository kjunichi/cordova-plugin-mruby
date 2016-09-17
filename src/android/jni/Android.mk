LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := mruby-jni
LOCAL_SRC_FILES := mruby-jni.c
include $(BUILD_SHARED_LIBRARY)
