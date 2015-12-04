LOCAL_PATH := $(call my-dir)
#FILE_LIST := $(wildcard $(LOCAL_PATH)/[DIRECTORY]/*.cpp)
#LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
include $(CLEAR_VARS)
LOCAL_MODULE    := GLRenderCode
LOCAL_SRC_FILES := com_example_textureviewwithopengles_RenderCode.cpp math3d.cpp
LOCAL_LDLIBS +=-lGLESv2 -lEGL -llog
include $(BUILD_SHARED_LIBRARY)
