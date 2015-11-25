LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES += liblog libnativehelper libcutils
LOCAL_SHARED_LIBRARIES += libhardware_legacy #libril-qc-1
#LOCAL_SHARED_LIBRARIES += libnv_eng_api #========================================cbt
LOCAL_SHARED_LIBRARIES += libhardware_legacy libcutils 
LOCAL_PRELINK_MODULE := false

include $(QC_PROP_ROOT)/common/build/remote_api_makefiles/target_api_enables.mk
include $(QC_PROP_ROOT)/common/build/remote_api_makefiles/remote_api_defines.mk

commonIncludes := $(QC_PROP_ROOT)/common/inc
commonIncludes += $(QC_PROP_ROOT)/diag/include
commonIncludes += $(TARGET_OUT_HEADERS)/nv/inc
commonIncludes += $(TARGET_OUT_HEADERS)/oem_rapi/inc
commonIncludes += $(QC_PROP_ROOT)/oncrpc/inc

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(commonIncludes) \
	hardware/libhardware_legacy/include/hardware_legacy \
	vendor/qcom/proprietary/nvitem_backup \
	vendor/qcom/proprietary/qcril/qcril_fusion/oppo \
    
LOCAL_MODULE := liblight_jni
LOCAL_SRC_FILES := \
    libbacklight_jni.cpp  \
    liblightsensor_jni.cpp \
    libnv_jni.cpp \
                 
   
#LOCAL_SRC_FILES := libbacklight_jni.cpp \
#	libnv_jni.cpp                         

include $(BUILD_SHARED_LIBRARY)

#######################################################
# ok libdoor live here
#
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libdoor

LOCAL_SRC_FILES:= door.cpp

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(commonIncludes) \
    external/openssl/include
    
LOCAL_CFLAGS += -DJNI_LIBRARY

LOCAL_SHARED_LIBRARIES := libcrypto libcutils libc liblog libnativehelper libcutils

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

#jiaoyifei delete begin,add in the nfc directory  /2014/06/09 
#######################################################
#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional

#LOCAL_MODULE:= libengnfc

#LOCAL_SRC_FILES:= libengnfc_jni.c

#LOCAL_CFLAGS := -std=c99 
#LOCAL_SHARED_LIBRARIES := liblog

#LOCAL_PRELINK_MODULE := false

#include $(BUILD_SHARED_LIBRARY)

#jiaoyifei delete end,add in the nfc directory  /2014/06/09 

#######################################################
#ifdef VENDOR_EDIT
#TongJing.Shi@EXP.DataComm.Phone, 2014.02.27, add  for LTE TEST
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= libdiagdci

LOCAL_SRC_FILES:= diag_dci.c

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(commonIncludes) \
    external/openssl/include

LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/common/inc
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/data/inc
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/diag/include

LOCAL_CFLAGS += -DJNI_LIBRARY
LOCAL_SHARED_LIBRARIES := libdiag libcrypto libcutils libc liblog libnativehelper

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
#endif /* VENDOR_EDIT */
#######################################################
#ifdef VENDOR_EDIT
#ShaoMing@EXP.DataComm.Phone, 2014/07/07, Add for Telcel netlock
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libtelcelnetlock

LOCAL_SRC_FILES:= telcelnetlock.cpp

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(commonIncludes) \
    external/openssl/include
    
LOCAL_CFLAGS += -DJNI_LIBRARY

LOCAL_SHARED_LIBRARIES := libcrypto libcutils libc liblog libnativehelper libcutils

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
#endif /* VENDOR_EDIT */
#######################################################  
ifeq ($(filter-out MSM_14017 MSM_14021,$(OPPO_TARGET_DEVICE)),)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= librotation_jni

LOCAL_SHARED_LIBRARIES += liblog libnativehelper
LOCAL_SHARED_LIBRARIES += libhardware_legacy
LOCAL_SHARED_LIBRARIES += librotorsrv \
					libutils \
					libbinder\
					libcutils\

LOCAL_C_INCLUDES += $(JNI_H_INCLUDE) \
                    $(commonIncludes) \
                    hardware/libhardware_legacy/include/hardware_legacy \
                    vendor/qcom/proprietary/nvitem_backup \
                    vendor/qcom/proprietary/qcril/qcril_fusion/oppo \
                    frameworks/base/include \
		       
LOCAL_SRC_FILES:= rotation_jni.cpp

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
endif
#######################################################
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= libbluetoothtestmode

LOCAL_SRC_FILES:= libbluetoothtestmode.c

LOCAL_SHARED_LIBRARIES := liblog  libcutils  

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
#######################################################

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= libdiag_modem

LOCAL_SRC_FILES:= diag_modem.c

LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE) \
	$(commonIncludes) \
    external/openssl/include

LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/common/inc
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/data/inc
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/diag/include

LOCAL_CFLAGS += -DJNI_LIBRARY
LOCAL_SHARED_LIBRARIES := libdiag libcrypto libcutils libc liblog libnativehelper

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
#######################################################

include $(call all-makefiles-under,$(LOCAL_PATH))