LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := mruby-jni
LOCAL_SRC_FILES := mruby-jni.c
LOCAL_C_INCLUDES := $(ANDROID_MRUBY)/include
LOCAL_LDFLAGS := -L../jniLibs/$(TARGET_ARCH_ABI) \

#-Wl,-dn,-lopencv_info,\
-lopencv_calib3d,-lopencv_core,-lopencv_flann,-lopencv_legacy,\
-lopencv_ml,-lopencv_photo,-lopencv_highgui,\
-lopencv_objdetect,-lopencv_java3,-lmruby,,-llibjpeg,-lIlmImf,-llibjasper,-llibpng,-llibtiff

LOCAL_LDLIBS := -llog -lz -lgnustl_shared -lopencv_java3 \
../jniLibs/$(TARGET_ARCH_ABI)/libmruby.a \
../jniLibs/$(TARGET_ARCH_ABI)/libopencv_androidcamera.a \
../jniLibs/$(TARGET_ARCH_ABI)/libopencv_highgui.a \
../jniLibs/$(TARGET_ARCH_ABI)/libopencv_objdetect.a \
../jniLibs/$(TARGET_ARCH_ABI)/libopencv_imgproc.a \
../jniLibs/$(TARGET_ARCH_ABI)/libopencv_core.a \
../jniLibs/$(TARGET_ARCH_ABI)/libIlmImf.a \
../jniLibs/$(TARGET_ARCH_ABI)/liblibjpeg.a \
../jniLibs/$(TARGET_ARCH_ABI)/liblibtiff.a \
../jniLibs/$(TARGET_ARCH_ABI)/liblibpng.a \
../jniLibs/$(TARGET_ARCH_ABI)/liblibjasper.a

#LOCAL_STATIC_LIBRARIES := mruby opencv_info opencv_java3 opencv_objdetect \
	  opencv_calib3d opencv_core opencv_flann opencv_legacy \
	  opencv_imgproc opencv_ml opencv_contrib opencv_photo opencv_highgui \
	  gnustl_shared log z libjpeg IlmImf libjasper libpng libtiff

include $(BUILD_SHARED_LIBRARY)
#include $(PREBUILT_SHARED_LIBRARY)
