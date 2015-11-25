#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

// terry hack ++
//#include <utils/Log.h>
#include <utils/Log.h>
#include "common_log.h"
// terry hack --

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
//#include <nv_eng_api.h>//===================================================================by cbt

namespace android{
  #define LOG_TAG "libnv_jni"
/*//==================================================================================by cbt
static int write_str(char const* path, char *str)
{
    int fd;
    fd = open(path, O_WRONLY);
    if (fd >= 0) {
        char buffer[20];
        int bytes = sprintf(buffer, "%s", str);
        int amt = write(fd, buffer, bytes);
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
        return -errno;
    }
}

static int write_int(char const* path, int value)
{
    int fd;

    fd = open(path, O_RDWR);
    if (fd >= 0) {
        char buffer[20];
        int bytes = sprintf(buffer, "%d\n", value);
        int amt = write(fd, buffer, bytes);
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
    	LOGE("write_int,openfile %s error,%s",path , strerror(errno));
        return -errno;
    }
}

char const* NV_FORCE_SYNC = "/sys/devices/platform/rs300000a7.65536/force_sync";

static void native_nvForceSync(JNIEnv* env, jobject clazz)
{
	write_int(NV_FORCE_SYNC, 1);
	LOGE("force sync nv");
}

 static jint native_adjustNvSelf(JNIEnv* env, jobject clazz)
 {
     jint ans = eng_rf_nv_self_check();
	 return ans;
 }

 static jint native_CRCcheckDynamicNV(JNIEnv* env, jobject clazz)
 {
     jint ans = eng_CRCcheckDynamicNV();
	 return ans;
 }

 static jint native_CRCcheckStaticNV(JNIEnv* env, jobject clazz)
 {
     jint ans = eng_CRCcheckStaticNV();
	 return ans;
 }

static jint native_CmpDynamicNV(JNIEnv* env, jobject clazz)
{
    jint ans = eng_CmpDynamicNV();
    return ans;    
}

static jint native_CmpStaticNV(JNIEnv* env, jobject clazz)
{
    jint ans = eng_CmpStaticNV();
    return ans;   
}

static jint native_NVDump(JNIEnv* env, jobject clazz)
{
    jint ans = eng_NVDump();
    return ans;  
}

 static jint native_restoreDynamicNv(JNIEnv* env, jobject clazz)
 {
     jint ans = eng_restoreDynamicNv();
     return ans;
 }

static jint native_restoreStaticNv(JNIEnv* env, jobject clazz)
{
    jint ans = eng_restoreStaticNv();
    return ans;
}


static jint  native_backupDynamicNv(JNIEnv* env, jobject clazz,jint force)
{
    jint ans;
    ans = eng_backupDynamicNv(force);
    return ans;
}

static jint  native_backupStaticNv(JNIEnv* env, jobject clazz,jint force)
{
    jint ans;
    ans = eng_backupStaticNv(force);
    return ans;
}

/***
typedef  struct{
int id;
int status;
}nv_check_result_type;
 **/
 
 /*//==================================================================================by cbt
 static  jintArray native_adjustNvBackup(JNIEnv* env, jobject clazz)
 {


     nv_check_result_type* nvType = (nv_check_result_type *)malloc(sizeof(nv_check_result_type) * 400);

     int* psize = (int *) malloc(sizeof(int));
     eng_rf_nv_check(nvType , psize);
     int size = *psize;

     LOGI("%d",size);

     if(size <= 0 )
     {
    	 return NULL;
     }

     jintArray ans = env->NewIntArray(1 + 2 * size);
     int tmp[1 + 2 * size];

     tmp[0] = size;

     for(int i = 1 ;i < 1 + 2 * size; i = i + 2)
     {
    	 tmp[i]  =  nvType->id;
    	 tmp[i + 1] = nvType->status;
    	 nvType++;
    	 LOGE("%d,%d",tmp[i] , tmp[i+1]);
     }
     env->SetIntArrayRegion(ans,0,size,tmp);
	 return ans;

 }
 static jint native_restoreNv(JNIEnv* env, jobject clazz)
 {

     jint ans = eng_rf_nv_restore();
	 return ans;
 }

 static jint native_backupNv(JNIEnv* env, jobject clazz)
 {
	 int ans = eng_check_backup();
	 return ans;
 }

 static jint native_adjustStaticNv(JNIEnv* env, jobject clazz)
 {

	 int ans = eng_static_nv_check();
	 return ans;
 }
 /*
 static jint native_restoreStaticNv(JNIEnv* env, jobject clazz)
 {
     int ans = eng_static_nv_restore();
	 return ans;
 }
 */ 
/*//==================================================================================by cbt
 static jint native_adjustForceBackup(JNIEnv* env, jobject clazz)
 {
     int ans = eng_force_backup();
	 return ans;
 }
 // 0 to colse , 1 to open
 static void native_setAdjustRestoreEnable(JNIEnv* env, jobject clazz ,jint value)
 {
	 eng_set_rfnv_restore_switch(value);
 }

 // 0 to colse , 1 to open
 static void native_setStaicRestoreEnable(JNIEnv* env, jobject clazz ,jint value)
 {
	 eng_set_staticnv_restore_switch(value);
 }
*///==================================================================================by cbt
/**     int arr_status: array of nv status result:
						arr_status[0]:RfRestoreSwitch,
  						arr_status[1]:StaRestoreSwitch,
  						arr_status[2]:RfRestoreTimes,
  						arr_status[3]:StaRestoreTimes,
 * */
 /*//==================================================================================by cbt
 static jintArray native_getNvStatus(JNIEnv* env, jobject clazz )
 {
	 int status[4];
	 eng_get_status(status);
	 jintArray ans = env->NewIntArray(4);
	 env->SetIntArrayRegion(ans,0,4,status);
	 return ans;
 }

 static JNINativeMethod gNotify[] = {
	{ "nvForceSync", "()V",  (void*) native_nvForceSync },
	{ "adjustNvSelf", "()I",  (void*) native_adjustNvSelf },
    { "adjustNvBackup", "()[I",  (void*) native_adjustNvBackup },
    { "restoreNv", "()I",           (void*) native_restoreNv },
    { "backupNv", "()I",         (void*) native_backupNv },   
    { "adjustStaticNv" , "()I" , (void*)native_adjustStaticNv},
    //{ "restoreStaticNv" , "()I" , (void*)native_restoreStaticNv},
    { "adjustForceBackup" , "()I" , (void*)native_adjustForceBackup},
    { "setAdjustRestoreEnable" , "(I)V" , (void*)native_setAdjustRestoreEnable},
    { "setStaicRestoreEnable" , "(I)V" , (void*)native_setStaicRestoreEnable},
    { "getNvStatus" , "()[I" , (void*)native_getNvStatus},
    { "CRCcheckDynamicNV", "()I",  (void*) native_CRCcheckDynamicNV },
    { "CRCcheckStaticNV", "()I",  (void*) native_CRCcheckStaticNV },
    { "restoreDynamicNv", "()I",  (void*) native_restoreDynamicNv },
    { "restoreStaticNv", "()I",  (void*) native_restoreStaticNv },
    { "backupDynamicNv", "(I)I",  (void*) native_backupDynamicNv },
    { "backupStaticNv", "(I)I",  (void*) native_backupStaticNv },
    { "CmpDynamicNV", "()I",  (void*) native_CmpDynamicNV },
    { "CmpStaticNV", "()I",  (void*) native_CmpStaticNV },
    { "NVDump",  "()I", (void*) native_NVDump },
};

int register_nv(JNIEnv* env) {
    int res = jniRegisterNativeMethods(env, "com/android/engineeringmode/qualcomm/QualCommNvUtils", gNotify, NELEM(gNotify));
    return res;
}
*///==================================================================================by cbt
}
