/**************************************************************
* Copyright (c)  2013  Oppo Mobile communication Corp.ltd.
* VENDOR_EDIT
*File		£ºtelcelnetlock.cpp
*REF File   : \EngineeringMode\jni\door.cpp
* Description: This file contains the procudre handle the process about the telcelnetlock
* Version   : 1.0
* Date      : 2013-12-31
* Author    : ShaoMing@EXP
* ---------------- Revision History: --------------------------
****************************************************************/


#define LOG_TAG "Telcelnetlock"
#undef NDEBUG
#define LOG_NDDEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/des.h>
#include <openssl/sha.h>

#include <unistd.h>
#include <cutils/properties.h>

#include <jni.h>

#include <utils/Log.h>
#include "common_log.h"
#include <errno.h>

#define MISC_PARTITION_DEV_NAME       "/dev/block/mmcblk0p28" //misc partion
#define TELCELNETLOCK_OFFSET	(1000 * 1024)	//  offset where to store telcelnetlock flag 1000*1024byte
#define TELCELNETLOCKONFLAG "1"
#define TELCELNETLOCKOFFFLAG "0"

#define TELCELNETLOCK_UNLOCK_TIMES_OFFSET	(1000 * 1024 + 1) //offset where to store unlock times		
#define TELCELNETLOCK_TIMES_ZERO	"0"
#define TELCELNETLOCK_TIMES_MAX	"5"

#define PROPERTY_TELCELNETLOCK "persist.sys.oppo.telcelnetlock"

#define TELCELNETLOCK_KEY_LEN 16

/*
*Read values from misc according to specific offset
*/
static int read_bytes_by_offset(char *path, int offset, char *buf, int len)
{
    int fd = -1;
    int ret = -1;
    int size = -1;

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        LOGI("read_bytes_by_offset open:%s failed! errno:%d", path,errno);
        return ret;
    }
    size = lseek(fd, offset, SEEK_SET);
    if (size == -1){
        LOGI("read_bytes_by_offset lseek:%s failed! errno:%d", path,errno);
        goto end;
    }
    
    size = read(fd, buf, len);
    if (size == -1){
        LOGI("read failed! errno:%d", errno);
        goto end;
    }
    LOGI("read %d bytes:%s", size, buf);
    ret = size;
    
end:
    if (fd != -1)
        close(fd);
    
    return ret;
}

/*
*Write values to misc according to specific offset
*/
static int write_bytes_by_offset(char *path, int offset, char *buf, int len)
{
    int fd = -1;
    int ret = -1;
    int size = -1;
    
    fd = open(path, O_RDWR);
    if (fd == -1) {
        LOGI("write_bytes_by_offset open:%s failed! errno:%d", path,errno);
        return ret;
    }
    size = lseek(fd, offset, SEEK_SET);
    if (size == -1){
        LOGI("write_bytes_by_offset lseek:%s failed! errno:%d", path,errno);
        goto end;
    }
    
    size = write(fd, buf, len);
    if (size == -1){
        LOGI("write failed! errno:%d", errno);
        goto end;
    }
    LOGI("write %d bytes:%s", size, buf);
    
    ret = size;
    
end:
    if (fd != -1)
        close(fd);
    
    return ret;
}

/*
*Read telcelnetlock value from misc
*/
static int read_telcelnetlock_flag(char *flag, int flaglen)
{
    int ret = -1;
    ret = read_bytes_by_offset(MISC_PARTITION_DEV_NAME, TELCELNETLOCK_OFFSET, flag, flaglen);
    return ret;
}

/*
*Write unlocktimes value to misc
*/
static int write_telcelnetlock_flag(char *flag, int flaglen)
{
    int ret = -1;
    ret = write_bytes_by_offset(MISC_PARTITION_DEV_NAME, TELCELNETLOCK_OFFSET, flag, flaglen);
    return ret;
}

/*
*Read unlocktimes value from misc
*/
static int read_unlocktimes_flag(char *flag, int flaglen)
{
    int ret = -1;
    ret = read_bytes_by_offset(MISC_PARTITION_DEV_NAME, TELCELNETLOCK_UNLOCK_TIMES_OFFSET, flag, flaglen);
    return ret;
}

/*
*Write telcelnetlock value to misc
*/
static int write_unlocktimes_flag(char *flag, int flaglen)
{
    int ret = -1;
    ret = write_bytes_by_offset(MISC_PARTITION_DEV_NAME, TELCELNETLOCK_UNLOCK_TIMES_OFFSET, flag, flaglen);
    return ret;
}

/*
*Set the telcelnetlock property
*/
static void set_telcelnetlock(char *telcelnetlock)
{
    int ret = -1;
	ret = property_set(PROPERTY_TELCELNETLOCK, telcelnetlock);
    if (ret < 0) {
        LOGI("Set property telcelnetlock failed!");
	}
}

/*
*Read values from misc
*/
static int read_bytes(char *path, char *buf, int len)
{
    int fd = -1;
    int ret = -1;
    int size = -1;
    unsigned char temp[1];
    
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        return ret;
    }
    
    size = read(fd, temp, 1);
    if (size <= 0)
        goto end;
    
    size = read(fd, buf, len);
    if (size != temp[0])
        goto end;
    
    ret = size;
    
end:
    if (fd != -1)
        close(fd);
    
    return ret;
}

/*
*Write values to misc
*/
static int write_bytes(const char *path, unsigned char *buf, int len)
{
    int fd = -1;
    int ret = -1;
    int size = -1;
    unsigned char temp[1];
    
    fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if (fd == -1) {
        goto end;
    }
    
    temp[0] = (char)len;
    size = write(fd, temp, 1);
    if (size <= 0)
        goto end;
    
    size = write(fd, buf, len);
    if (size != len)
        goto end;
    
    ret = size;
    
end:
    if (fd != -1)
        close(fd);
    
    return ret;
}

/*
*Get key from IMEI and salt
*/
/* "1*oPnfqk" */   
static int generate_key_with_imei(char *imei, unsigned char *key)
{
    int ret = -1;
    if( (imei == NULL) || (key == NULL)){
        LOGI("imei or key is UULL!");
        return ret;
    }
    
    for(int i=0; i<TELCELNETLOCK_KEY_LEN; ++i){
		switch(i)
		{
			case 1:
				key[i] = 0x31;
				break;
			case 3:
				key[i] = 0x2a;
				break;
			case 5:
				key[i] = 0x6f;
				break;
			case 7:
				key[i] = 0x50;
				break;
			case 9:
				key[i] = 0x6e;
				break;
			case 11:
				key[i] = 0x66;
				break;
			case 13:
				key[i] = 0x71;
				break;
			case 15:
				key[i] = 0x6b;
				break;
			default:
				key[i] = imei[i];
		}
	}
    return ret;
}

/*
*Use SHA256 to encypt the key to get initial password
*/
static void crypto_sha(unsigned char *key, int keylen, unsigned char *out)
{
    SHA256_CTX c;

	SHA256_Init(&c);
	SHA256_Update(&c, key, keylen);
	SHA256_Final(out, &c);
	//OPENSSL_cleanse(&c, sizeof(c));
}

/*
*Check if password user inputed matched with UE's IMEI, return true on match, false on mismatch or failure
*/
static bool check_in(char *pwd, int pwdlen, char *imei, int imeilen)
{
    unsigned char key[TELCELNETLOCK_KEY_LEN+1];
    unsigned char m[SHA256_DIGEST_LENGTH];
    char pwd_buf[SHA256_DIGEST_LENGTH*2+1];
    char pwd_16[TELCELNETLOCK_KEY_LEN+1];
    
    memset(key, 0, sizeof(key));
    memset(m, 0, sizeof(m));
    memset(pwd_buf, 0, sizeof(pwd_buf));
    memset(pwd_16, 0, sizeof(pwd_16));
    
    if ( (pwd == NULL)|| (strlen(pwd) <= 0) || (imei == NULL) || (strlen(imei)<=0)) {
        LOGI("password or imei is invalid!");
        return false;
    }

    generate_key_with_imei(imei, key);

    crypto_sha(key, TELCELNETLOCK_KEY_LEN, m);
	//return memcmp(m, pwd, SHA256_DIGEST_LENGTH);
    //LOGI("The key is:%s", key);
    for(int i=0; i<SHA256_DIGEST_LENGTH; i++){
        sprintf(pwd_buf + (i * 2), "%02x", m[i]);
        if( (i%2 == 0) && (i/2 < TELCELNETLOCK_KEY_LEN) ){
            pwd_16[i/2] = pwd_buf[i*2];
        }
    }
    return (strcmp(pwd_16, pwd)==0);
}

/*
*Check AP side's netlock, returns 1 when netlock is on, 0 when netlock is off
*-1 when failure happed
*/
static jint check_netlock(JNIEnv *env, jobject object)
{
    int ret = -1;
    char buf[2];
    memset(buf, 0, sizeof(buf));
    
    ret = read_telcelnetlock_flag(buf, strlen(TELCELNETLOCKONFLAG));
    
    //if buf is "1", it means the telcelnetlock is on, return 1
    //if buf is "0", it means the telcelnetlock is off, return 0
    //if buf is "", it means the telcelnetlock is still initial state. since we must have handled this state in rmt_storage, so double check rmt_storage. 
    if(-1 != ret){
        if(0 == strlen(buf)){
            LOGE("check_netlock:netlock state is still initial state, check rmt_storage!!!");
            return -1;
        }
        else{
            LOGI("check_netlock:state %c.", buf[0]);
            LOGI("check_netlock: return %d.", strncmp(buf, TELCELNETLOCKOFFFLAG, strlen(TELCELNETLOCKOFFFLAG)));
            return (strncmp(buf, TELCELNETLOCKOFFFLAG, strlen(TELCELNETLOCKOFFFLAG)));
        }
    }
    else{
        LOGE("check_netlock:failed!");
        return ret;
    }
}

/*
*Clear AP side's netlock, returns true on success, false on failure
*/
static jboolean match_password(JNIEnv *env, jobject object, jstring imei, jstring pwd)
{
    bool ret = false;
    int pwdlen = -1;
    int imeilen = -1;
    char *npwd = NULL;
    char *nimei = NULL;

    npwd = (char *)env->GetStringUTFChars(pwd, NULL);
    nimei = (char *)env->GetStringUTFChars(imei, NULL);
    pwdlen = env->GetStringUTFLength(pwd);
    imeilen = env->GetStringUTFLength(imei);
    ret = check_in(npwd, pwdlen, nimei, imeilen);
    
    env->ReleaseStringUTFChars(pwd, npwd);
    env->ReleaseStringUTFChars(imei, nimei);
    
    return ret;
}

/*
*Clear AP side's netlock, returns true on success, false on failure
*/
static jboolean clear_netlock(JNIEnv *env, jobject object)
{
    int ret = -1;
    
    ret = write_telcelnetlock_flag(TELCELNETLOCKOFFFLAG,strlen(TELCELNETLOCKOFFFLAG));

    return (ret == strlen(TELCELNETLOCKOFFFLAG));
}

/*
*Recover AP side's netlock and reset unlocktimes to zero, returns true on success, false on failure
*/
static jboolean recover_netlock(JNIEnv *env, jobject object)
{
    int ret = -1;
    
    ret = write_telcelnetlock_flag(TELCELNETLOCKONFLAG,strlen(TELCELNETLOCKONFLAG));
    
    if(-1 != ret)
    {
        ret = write_unlocktimes_flag(TELCELNETLOCK_TIMES_ZERO,strlen(TELCELNETLOCK_TIMES_ZERO));          
        return (ret == strlen(TELCELNETLOCK_TIMES_ZERO)); 
    }
    else 
    {
        return (ret == strlen(TELCELNETLOCKONFLAG));
    }
}

/*
*Get unlocktimes, return the unlocktimes on success, -1 on failure
*/
static jint get_unlock_times(JNIEnv *env, jobject object)
{
    int ret = -1;
    char buf[2];
    memset(buf, 0, sizeof(buf));
    
    ret = read_unlocktimes_flag(buf, strlen(TELCELNETLOCK_TIMES_ZERO));
    
    //if buf is "", it means the unlocktimes is still initial state. since we must have handled this state in rmt_storage, so double check rmt_storage. 
    if(-1 != ret){
        if(0 == strlen(buf)){
            LOGE("get_unlock_times:unlocktimes state is still initial state, check rmt_storage!!!");
            return -1;
        }
        else{
            LOGI("get_unlock_times:unlocktimes %c.", buf[0]);
            LOGI("get_unlock_times: return %d.", strncmp(buf, TELCELNETLOCK_TIMES_ZERO, strlen(TELCELNETLOCK_TIMES_ZERO)));
            return (strncmp(buf, TELCELNETLOCK_TIMES_ZERO, strlen(TELCELNETLOCK_TIMES_ZERO)));
        }
    }
    else{
        LOGE("get_unlock_times:failed !");
        return ret;
    }
}

/*
*Unlocktimes plus 1, return true on success, false on failure
*/
static jboolean add_unlock_times(JNIEnv *env, jobject object)
{
    int ret = -1;
    char buf[2];
    memset(buf, 0, sizeof(buf));
    
    ret = read_unlocktimes_flag(buf, strlen(TELCELNETLOCK_TIMES_ZERO));
    
    //if buf is "", it means the unlocktimes is still initial state. since we must have handled this state in rmt_storage, so double check rmt_storage. 
    if(-1 != ret){
        if(0 == strlen(buf)){
            LOGE("add_unlock_times:unlocktimes state is still initial state, check rmt_storage!!!");
            return false;
        }
        else{
            LOGI("add_unlock_times:unlocktimes %c.", buf[0]);
            buf[0]++;
            LOGI("add_unlock_times:unlocktimes++ %c.", buf[0]);
        }
    }
    else{
        LOGE("add_unlock_times:failed !");
        return false;
    }
    
    ret = write_unlocktimes_flag(buf,strlen(TELCELNETLOCKONFLAG));

    return (ret == strlen(TELCELNETLOCKONFLAG));
}

/*
*Reset unlocktimes to 0, return ture on success, false on failure

static jboolean reset_unlock_times(JNIEnv *env, jobject object)
{
    int ret = -1;
    
    ret = write_unlocktimes_flag(TELCELNETLOCK_TIMES_ZERO,strlen(TELCELNETLOCK_TIMES_ZERO));

    return (ret == strlen(TELCELNETLOCK_TIMES_ZERO));
}
*/


/*
*Global native methods array
*/
static JNINativeMethod gMethods[] = {
    {"check", "()I", (void*)check_netlock},
    {"match", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)match_password},
    {"clear", "()Z", (void*)clear_netlock},
    {"recover", "()Z", (void*)recover_netlock},
    {"getUnlockTimes", "()I", (void*)get_unlock_times},
    {"addUnlockTimes", "()Z", (void*)add_unlock_times},
};

/*
 *Register native methods, returns the JNI_TRUE on success, JNI_FALSE on failure.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGI("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGI("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 *Register native methods, returns the JNI_TRUE on success, JNI_FALSE on failure.
 */
static int registerNatives(JNIEnv *env)
{
    const char* const kClassPathName = "com/android/engineeringmode/qualcomm/Telcelnetlock";
    return registerNativeMethods(env,
            kClassPathName, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
}

/*
 * Returns the JNI version on success, -1 on failure.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI("JNI_OnLoad");
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        //printf("ERROR: GetEnv failed\n");
        LOGI("ERROR: GetEnv failed");
        goto bail;
    }

    if (!registerNatives(env)) {
        //printf("ERROR: BinaryDictionary native registration failed\n");
        LOGI("ERROR: BinaryDictionary native registration failed");
        goto bail;
    }
    LOGI("Success");
    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}