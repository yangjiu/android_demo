#define LOG_TAG "liblightsensor_jni"
#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
#include <utils/Log.h>
// terry hack ++
#include "common_log.h"
// terry hack --
#include <hardware/hardware.h>
#include <hardware/lights.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
//#include "akmtest.h"//=============================================by cbt
//#include <sensor.h>

#define AKMIO    0xA1

#define ECS_IOCTL_APP_SET_ALSFLAG		_IOW(AKMIO, 0x20, short)	/*ambient sensor*/
#define ECS_IOCTL_APP_GET_ALSFLAG		_IOR(AKMIO, 0x21, short)
#define ECS_IOCTL_APP_SET_PFLAG		_IOW(AKMIO, 0x22, short)	/*proximity sensor*/
#define ECS_IOCTL_APP_GET_PFLAG		_IOR(AKMIO, 0x23, short)
#define ECS_IOCTL_APP_SET_AFLAG_AK		_IOW(AKMIO, 0x24, short)
#define ECS_IOCTL_APP_GET_AFLAG_AK		_IOR(AKMIO, 0x25, short)
#define ECS_IOCTL_APP_CALIBRATE		_IOR(AKMIO, 0x26, short[3])

#define LIG_DEVICE_NAME       "/dev/lsensor_dev"
#define GSENSOR_DEVICE_NAME       "/dev/bma150_dev"

struct light_data
{
    unsigned short  light;
    unsigned short  proximity;
    unsigned short  ADC_PROX;
};
light_data lights;
int mFileD;
namespace android{

int ctrlLightSensor( short flagg )
{
    short flag = 1 , ret;
    int fd1 = -1;

    fd1 = open(LIG_DEVICE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd1 < 0) {
		LOGE("open file failed");
		return fd1;
	} else {
		LOGE("open file succeed");
	}

    if (flagg != 0) {
		flag = flagg;
		ret = ioctl(fd1, ECS_IOCTL_APP_SET_ALSFLAG, &flag);
		if (ret) {
			LOGE("Unknow err\n");
			close(fd1);
			return ret;
		} else {
			LOGE("prox :%d\n", flag);
		}
	}

    flag = flagg;
    ret = ioctl(fd1, ECS_IOCTL_APP_SET_PFLAG, &flag);
    if (ret) {
		printf("open als err\n");
		return ret;
	} else {
		printf("prox :%d\n", flag);
	}
    //close(fd1);
    mFileD = fd1;
    return fd1;
}


static jint native_openLightSensor(JNIEnv* env, jobject clazz)
{
	LOGD("---->native_openLightSensor");
	return ctrlLightSensor(1);
}

static void native_closeLightSensor(JNIEnv* env, jobject clazz)
{
	LOGD("---->native_closeLightSensor");
	close(mFileD);
	ctrlLightSensor(0);
}


static jintArray native_startAdjust(JNIEnv* env, jobject clazz)
{
	int fd ,ret ;
	fd = open(LIG_DEVICE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		LOGE("open file failed");
	} else {
		LOGE("open file succeed");
	}
	short value[3];
    ret = ioctl(fd, ECS_IOCTL_APP_CALIBRATE, value);
    LOGE("Adjust:%d",value[0]);
    LOGE("Adjust:%d",value[1]);
    LOGE("Adjust:%d",value[2]);
    jintArray adc = env->NewIntArray(3);
    int ans[3];
    ans[0] = value[0];
    ans[1] = value[1];
    ans[2] = value[2];
    env->SetIntArrayRegion(adc,0,3,ans);
    close(fd);
    return adc;
}

/*
static jint native_startGsensorAdjust(JNIEnv* env, jobject clazz)
{
	int ret = GSensorCalibrate();
	return ret;
}
*/


static jintArray native_getADC(JNIEnv* env, jobject clazz,jint fd)
{
	jintArray adc;
	int fd1;
	int ret, flag;
	adc = env->NewIntArray(5);
//	fd1 = open(LIG_DEVICE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
//	if (fd1 < 0) {
//		LOGE("open file failed");
//       // return adc;
//	}
	fd1 = fd;

	//get Cleardata
	int cd[5];
	int ans = read(fd1 ,&lights , sizeof(unsigned short )* 3);
    cd[0] = lights.light;
    cd[1] = lights.ADC_PROX;
    cd[2] = lights.proximity;
	
		/*
		LOGE("light:%d",cd[0]);
		LOGE("ADC_PROX:%d",cd[1]);
		LOGE("proximity:%d",cd[2]);
		LOGE("ans:%d",ans);
		*/

    env->SetIntArrayRegion(adc,0,5,cd);

	return adc;
}



static jint native_getPilt()
{
	int fd, ret;
	fd = open(LIG_DEVICE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		LOGE("open file failed");
	} else {
		LOGE("open file succeed");
	}
	close(fd);
	return 0;
}

static jint native_getPiht()
{
	int fd, ret;
	fd = open(LIG_DEVICE_NAME, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		LOGE("open file failed");
	} else {
		LOGE("open file succeed");
	}
	close(fd);

	return 0;
}

static jstring native_startMsensorAutoTest(JNIEnv* env, jobject clazz)
{
	char * testData = (char *)malloc(1024*sizeof(char));
	memset(testData,0,1024*sizeof(char));
	//=================================================================cbt
/*	int res = autotest(testData);
	
	LOGE("startMsensorAutoTest, return:%d, \n return data:%s" , res, testData);
	
	if(res < 0)
	{
		return NULL;
	}
*/ //========================================================================cbt	
	jstring s = env->NewStringUTF(testData);
	free(testData);
	
	return s;
}


static JNINativeMethod gNotify[] = {
	{ "openLightSensor", "()I",  (void*) native_openLightSensor },
    { "closeLightSensor", "()V",  (void*) native_closeLightSensor },
    { "getADC", "(I)[I",           (void*) native_getADC },
    { "startAdjust", "()[I",         (void*) native_startAdjust },
   // { "getPilt", "()I",         (void*) native_getPilt },
   // { "getPiht", "()I",         (void*) native_getPiht },
    //{ "startGsensorAdjust","()I",(void*)native_startGsensorAdjust},
    { "startMsensorAutoTest","()Ljava/lang/String;",(void*)native_startMsensorAutoTest},
};

int register_lightsensor(JNIEnv* env) {
    int res = jniRegisterNativeMethods(env, "com/android/engineeringmode/manualtest/LightSensorUtils", gNotify, NELEM(gNotify));
    return res;
}

}

