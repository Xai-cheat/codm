LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)

# ============================================================================ #
include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := libcurl.a
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================ #
include $(CLEAR_VARS)
LOCAL_MODULE := libssl
LOCAL_SRC_FILES := libssl.a
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================ #
include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)

# ============================================================================#
# ============================================================================
include $(CLEAR_VARS)
LOCAL_MODULE            := libdobby
LOCAL_SRC_FILES         := libdobby.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/IL2CppSDKGenerator/Dobby/
include $(PREBUILT_STATIC_LIBRARY)

# ============================================================================ #
include $(CLEAR_VARS)
LOCAL_MODULE := libfoxcheats
LOCAL_SRC_FILES := libfoxcheats.a
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================#

include $(CLEAR_VARS)

# ============================================================================#

# ============================================================================#
# Your existing code remains unchanged above this point

# ============================================================================#
include $(CLEAR_VARS)
LOCAL_MODULE := libxhook
LOCAL_SRC_FILES := \
    SDK/Xhook/xhook.c \
    SDK/Xhook/xh_core.c \
    SDK/Xhook/xh_jni.c \
    SDK/Xhook/xh_elf.c \
    SDK/Xhook/xh_log.c \
    SDK/Xhook/xh_util.c \
    SDK/Xhook/xh_version.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/SDK/Xhook
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/SDK/Xhook
include $(BUILD_STATIC_LIBRARY)

# Define prebuilt libctorHook so modules depending on it can link
include $(CLEAR_VARS)
LOCAL_MODULE := libctorHook
LOCAL_SRC_FILES := libctorHook.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE           := libv+++
LOCAL_CFLAGS           := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS           += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS         := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS         += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive


LOCAL_LDFLAGS          += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE         := arm
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz

LOCAL_C_INCLUDES       += $(LOCAL_PATH)
# Use explicit source extensions to avoid picking up non-source files (like .cnf)
FILE_LIST              := $(wildcard $(LOCAL_PATH)/ImGui/*.c) $(wildcard $(LOCAL_PATH)/ImGui/*.cpp) $(wildcard $(LOCAL_PATH)/ImGui/*.cc)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/IL2CppSDKGenerator/IL2Cpp/*.c) $(wildcard $(LOCAL_PATH)/IL2CppSDKGenerator/IL2Cpp/*.cpp)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/IL2CppSDKGenerator/KittyMemory/*.c) $(wildcard $(LOCAL_PATH)/IL2CppSDKGenerator/KittyMemory/*.cpp)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/System/Texture/*.c) $(wildcard $(LOCAL_PATH)/System/Texture/*.cpp)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/*.c) $(wildcard $(LOCAL_PATH)/*.cpp) $(wildcard $(LOCAL_PATH)/*.cc)

LOCAL_SRC_FILES        := $(FILE_LIST:$(LOCAL_PATH)/%=%) \
                               Substrate/hde64.c \
                               Substrate/SubstrateDebug.cpp \
                               Substrate/SubstrateHook.cpp \
                               Substrate/SubstratePosixMemory.cpp \
                               Substrate/And64InlineHook.cpp \
                               AstralPrtctn/md5.cpp
                               
LOCAL_LDLIBS := -llog -landroid
FILE_LIST := $(wildcard $(LOCAL_PATH)/libzip/*.c)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libzip

LOCAL_LDLIBS           := -llog -landroid -lz -lEGL -lGLESv2 -lGLESv3
LOCAL_STATIC_LIBRARIES := libcurl libssl libcrypto libdobby libfoxcheats libxhook libctorHook

LOCAL_CPP_FEATURES                      := exceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/foxcheats
LOCAL_C_INCLUDES += $(LOCAL_PATH)/foxcheats/includes

                      
include $(BUILD_SHARED_LIBRARY)
# ============================================================================
