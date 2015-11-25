/*
**
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
**
**     http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License.
*/

#define LOG_TAG "door"

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

#undef printf
#ifdef JNI_LIBRARY
#define LOG_NDDEBUG 1
// terry hack ++
//#include <cutils/log.h>
#include <utils/Log.h>
#include "common_log.h"
// terry hack --
#define printf(...) LOGD(__VA_ARGS__)
#else
#define printf(...) printf(__VA_ARGS__)
#endif

#define DATA_SIZE 256
#define PROPERTY_PASSWORD_PATH "persist.sys.develop.pwd"
#define PROPERTY_KEY_PATH "persist.sys.develop.key"


static int read_bytes(char *path, char *buf, int len)
{
    int fd = -1;
    int ret = -1;
    int size;
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

static int write_bytes(const char *path, unsigned char *buf, int len)
{
    int fd = -1;
    int ret = -1;
    int size;
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

static int read_data(const char *prop, char *buf, int len)
{
    int ret = -1;
    char value[PROPERTY_VALUE_MAX];
    
    property_get(prop, value, "");
    if (strcmp(value, "") == 0)
        goto error;
    
    ret = read_bytes(value, buf, len);
    
error:
    return ret;
}

static void generate_combo_key(char *buf, int len)
{
    char value[PROPERTY_VALUE_MAX];
    
    memset(buf, 0, len);
    
    property_get("ro.build.type", value, "");
    if (len - strlen(buf) > strlen(value)) {
        strcat(buf, value);
    }
    
    property_get("ro.build.user", value, "");
    if (len - strlen(buf) > strlen(value)) {
        strcat(buf, value);
    }
    
    property_get("ro.build.host", value, "");
    if (len - strlen(buf) > strlen(value)) {
        strcat(buf, value);
    }
    
    property_get("ro.build.tags", value, "");
    if (len - strlen(buf) > strlen(value)) {
        strcat(buf, value);
    }
    
    property_get("ro.product.model", value, "");
    if (len - strlen(buf) > strlen(value)) {
        strcat(buf, value);
    }
    
    property_get("ro.product.brand", value, "");
    if (len - strlen(buf) > strlen(value)) {
        strcat(buf, value);
    }
    
    property_get("ro.board.platform", value, "");
    if (len - strlen(buf) > strlen(value)) {
        strcat(buf, value);
    }
}

static int encrypt( char *key, char *msg, int size, char *res)
{
    int n=0;
    DES_cblock key2;
    DES_key_schedule schedule;

    /* Prepare the key for use with DES_cfb64_encrypt */
    memcpy( key2, key,8);
    DES_set_odd_parity( &key2 );
    DES_set_key_checked( &key2, &schedule );

    /* Encryption occurs here */
    DES_cfb64_encrypt( ( unsigned char * ) msg, ( unsigned char * ) res,
                       size, &schedule, &key2, &n, DES_ENCRYPT );

    return size;
}

/* "angela" */
static char itn_code[SHA256_DIGEST_LENGTH] = {
    0x79, 0xa6, 0xa9, 0x33, 0xdf, 0xc9, 0xb1, 0x97,
    0x5e, 0x44, 0x4d, 0x4e, 0x84, 0x81, 0xc6, 0x4c,
    0x77, 0x1d, 0x8a, 0xb4, 0x0b, 0x7a, 0xc7, 0x2f,
    0x8b, 0xc1, 0xa1, 0xbc, 0xa1, 0x71, 0x8b, 0xef,
    };

static void crypto_sha(unsigned char *pwd, int len, unsigned char *out)
{
    SHA256_CTX c;

	SHA256_Init(&c);
	SHA256_Update(&c, pwd, len);
	SHA256_Final(out, &c);
	//OPENSSL_cleanse(&c, sizeof(c));
}

static int verify_password(unsigned char *pwd, int len)
{
    unsigned char m[SHA256_DIGEST_LENGTH];

    crypto_sha(pwd, len, m);
    
	return memcmp(m, itn_code, SHA256_DIGEST_LENGTH);
}

static int crypto(unsigned char *password, int len, const char *path_passwd, const char *path_key)
{
    int ret = -1;
    char extkey[DATA_SIZE] = {0};
    char clrkey[DATA_SIZE], ecpkey[DATA_SIZE];
    unsigned char m[SHA256_DIGEST_LENGTH];
    
    for(ret = 0; ret < len; ret++) {
        password[ret] |= 0x60;
    }
    printf("crypto path, password: %s, key: %s\n", path_passwd, path_key);
    
    crypto_sha(password, len, m);
    ret = write_bytes(path_passwd, m, SHA256_DIGEST_LENGTH);
    if (ret < 0) {
        printf("write passwd to %s failed\n", path_passwd);
        goto error;
    }
    ret = property_set(PROPERTY_PASSWORD_PATH, path_passwd);
    if (ret < 0) {
        printf("setproperty failed\n");
        goto error;
    }
    
    generate_combo_key(clrkey, DATA_SIZE);
    printf("combo key: %s\n", clrkey);
    encrypt((char *)password, clrkey, strlen(clrkey), ecpkey);
    ret = write_bytes(path_key, (unsigned char*)ecpkey, strlen(clrkey));
    if (ret < 0) {
        printf("write key to %s failed\n", path_key);
        goto error;
    }
    ret = property_set(PROPERTY_KEY_PATH, path_key);
    if (ret < 0) {
        printf("setproperty failed\n");
        goto error;
    }
    
    ret = 0;
error: 
    return ret;
}

static int get_enable()
{
    char value[PROPERTY_VALUE_MAX];
    
    property_get(PROPERTY_PASSWORD_PATH, value, "");
    if (0 != access(value, R_OK)) {
        printf("%s can't read\n", value);
        return -1;
    }
    
    property_get(PROPERTY_KEY_PATH, value, "");
    if (0 != access(value, R_OK)) {
        printf("%s can't read\n", value);
        return -1;
    }
    
    return 0;
}

static int login(char *pwd, int len)
{
    int ret;
    unsigned char internal_pwd[] = {0x0f, 0x10, 0x05, 0x0e, 0x04, 0x0f, 0x0f, 0x12};
    
    if (pwd == NULL || strlen(pwd) <= 0) {
        return -1;
    }
    
    if (verify_password((unsigned char*)pwd, strlen(pwd)) == 0)
        printf("password verify passed\n");
    else {
        printf("password verify failed\n");
        return -1;
    }
    
    ret = crypto((unsigned char*)internal_pwd, sizeof(internal_pwd), "/data/misc/fpwd", "/data/misc/fkey");
    
    return ret;
}

static void logout()
{
    char value[PROPERTY_VALUE_MAX];
    
    printf("logout\n");
    property_get(PROPERTY_PASSWORD_PATH, value, "");
    unlink(value);
    property_set(PROPERTY_PASSWORD_PATH, "");
    
    property_get(PROPERTY_KEY_PATH, value, "");
    unlink(value);
    property_set(PROPERTY_KEY_PATH, "");
}

#ifndef JNI_LIBRARY
/* for test */
static int verify_password2(unsigned char *pwd, int pwd_len, unsigned char *enpwd, int enpwd_len)
{
    unsigned char m[SHA256_DIGEST_LENGTH];

    if (enpwd_len < SHA256_DIGEST_LENGTH)
        return -1;
    
    crypto_sha(pwd, pwd_len, m);
    
	return memcmp(m, enpwd, SHA256_DIGEST_LENGTH);
}

/* for key generate tool */
static int generate_code(unsigned char *pwd, int len, char *out)
{
    int ret;
    unsigned char m[SHA256_DIGEST_LENGTH];
    
	crypto_sha(pwd, len, m);
    
    for (int i=0; i<SHA256_DIGEST_LENGTH; i++) {
        printf("0x%2x, ", m[i]);
    }
    printf("\n");
    /*
    ret = write_bytes(out, m, SHA256_DIGEST_LENGTH);
    if (ret < 0) {
        printf("write sha code to %s failed\n", out);
    }
    */
    return 0;
}

/* for test */
static int decryp(unsigned char *password, int len)
{
    int ret = -1;
    char extkey[DATA_SIZE] = {0};
    char enpassword[DATA_SIZE] = {0};
    char clrkey[DATA_SIZE], ecpkey[DATA_SIZE];
    
    ret = read_data(PROPERTY_PASSWORD_PATH, enpassword, DATA_SIZE);
    if (ret < 0)
        goto failed;
    printf("read password: %s\n", enpassword);
    
    // verify password
    ret = verify_password2((unsigned char*)password, len, (unsigned char*)enpassword, ret);
    if (ret != 0)
        goto failed;
    
    ret = read_data(PROPERTY_KEY_PATH, extkey, DATA_SIZE);
    if (ret < 0)
        goto failed;
    printf("read key size: %d\n", ret);
    
    ret = -1;
    generate_combo_key(clrkey, DATA_SIZE);
    
    encrypt((char *)password, clrkey, strlen(clrkey), ecpkey);
    if (memcmp(ecpkey, extkey, strlen(clrkey)) != 0) {
        printf("key not match!\n");
        goto failed;
    }
    ret = 0;
failed:
    return ret;
}

int main(int argc, char **argv)
{
    int ret;
    
    if (argc < 2)
        return 0;
    
    if (strcmp(argv[1], "--login") == 0) {
        if (argc < 3) {
            printf("useage: %s --login password\n", argv[0]);
            return 1;
        }
        
        ret = login(argv[2], strlen(argv[2]));
        if (ret < 0) {
            printf("door: login failed\n");
        } else {
            printf("door: login success\n");
        }
    } else if (strcmp(argv[1], "--decryp") == 0) {
        ret = decryp((unsigned char*)"opendoor", 8);
        if (ret)
            printf("decryp failed\n");
        else
            printf("decryp success\n");
    } else if (strcmp(argv[1], "--generate") == 0) {
        if (argc < 4) {
            printf("useage: %s --generate password output\n", argv[0]);
            return 1;
        }
        generate_code((unsigned char*)argv[2], strlen(argv[2]), argv[3]);
    } else if (strcmp(argv[1], "--verify") == 0) {
        if (argc < 3) {
            printf("useage: %s --verify password\n", argv[0]);
            return 1;
        }
        if (verify_password((unsigned char*)argv[2], strlen(argv[2])) == 0)
            printf("passed\n");
        else
            printf("failed\n");
    } else if (strcmp(argv[1], "--clear") == 0) {
        logout();
    } else {
        printf("useage: %s -[ldgv]\n", argv[0]);
    }
    
    return 0;
}
#endif

/************************************************************/
#ifdef JNI_LIBRARY

#include <jni.h>

static jboolean door_isOpen(JNIEnv *env, jobject object)
{
    return get_enable() == 0;
}

static jboolean door_open(JNIEnv *env, jobject object, jstring pwd)
{
    int ret = -1;
    int len;
    char *npwd;

    npwd = (char *)env->GetStringUTFChars(pwd, NULL);
    len = env->GetStringUTFLength(pwd);
    ret = login(npwd, len);
    
    env->ReleaseStringUTFChars(pwd, npwd);
    
    return ret == 0;
}

static void door_close(JNIEnv *env, jobject object)
{
    logout();
}



int verifyResult(char * param , int sequence ,char * data)
{
   int ret = -1;
   char machine[92] = {0};
   
   if(param == NULL)
        return -1;
    
   switch(sequence){
    case 0 :
		 property_get("ro.product.name", machine, "");
         if(strstr(param,machine) != NULL)
            ret = 0;
        break;
    case 1 :
        if(strcmp(param,"0")== 0)
            ret = 0;
            break;
    case 2 :
        if(data != NULL && strcmp(param,data)== 0)
            ret = 0;
            break;
   }
   
    printf("==verifyResult=%d\n",ret);
    return ret;
}

int should_crack_roaming_policy(char *file)
{
  char imeibuf[92] = {0};
  char readbuf[92] = {0};
  char* const delim = "#";  
  char *token, *cur = readbuf;  
  int sequence = 0 , ret = 0, readSize = 0, i = 0;
  int len = property_get("persist.sys.oppo.device.imei", imeibuf, "");  
  if(len != 15 || strlen(imeibuf)!=15)
      return -1;   
  printf("should_crack_roaming_policy Enter\n");
  FILE *fp = fopen(file, "rb");
  if (!fp) 
  {
    printf("should_crack_roaming_policy=fp=NULL\n");
    return -1;
  } 
  readSize = fread(readbuf, sizeof(char), 32, fp); 
  for(i = 0; i < readSize; i++) 
  {  
    readbuf[i] ^= 14;
    readbuf[i] = ~readbuf[i];
  }
  readbuf[i]='\0';
  fclose(fp);
  
  remove(file);
  
  while (token = strsep(&cur, delim)) { 
        if(verifyResult(token,sequence,sequence==2?imeibuf:NULL) == 0)
        {
           ret++;
        }
        sequence++;  
  } 
  printf("=====ret===%d\n",ret);
  if(ret >= 3)
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

static jboolean should_crack(JNIEnv *env, jobject object, jstring pwd)
{
    int ret = -1;
    int len;
    char *npwd;

    npwd = (char *)env->GetStringUTFChars(pwd, NULL);
    ret = should_crack_roaming_policy(npwd);
    env->ReleaseStringUTFChars(pwd, npwd);
    
    return ret == 0;
}

static JNINativeMethod gMethods[] = {
    {"isEscalated", "()Z", (void*)door_isOpen},
    {"escalate", "(Ljava/lang/String;)Z", (void*)door_open},
    {"recover", "()V", (void*)door_close},
	{"shouldCrack", "(Ljava/lang/String;)Z", (void*)should_crack},
};

static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        printf("Native registration unable to find class '%s'\n", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        printf("RegisterNatives failed for '%s'\n", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static int registerNatives(JNIEnv *env)
{
    const char* const kClassPathName = "com/android/engineeringmode/qualcomm/Privilege";
    return registerNativeMethods(env,
            kClassPathName, gMethods, sizeof(gMethods) / sizeof(gMethods[0]));
}

/*
 * Returns the JNI version on success, -1 on failure.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        printf("ERROR: GetEnv failed\n");
        goto bail;
    }

    if (!registerNatives(env)) {
        printf("ERROR: BinaryDictionary native registration failed\n");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}
#endif
