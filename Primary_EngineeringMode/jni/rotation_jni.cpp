#include "jni.h"
#include "JNIHelp.h"
#include <stdio.h>
#include <utils/misc.h>
#include <utils/Log.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <linux/rotorsrv_io.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "android_runtime/AndroidRuntime.h"
#include <hardware/hardware.h>
#include <sys/stat.h>


#include <utils/Vector.h>
#include <cutils/atomic.h>
#include <cutils/properties.h>
#include <utils/RefBase.h>
#include <rotorsrv/rotorsrv.h>
#include <binder/BinderService.h>



#define MOD "rotation_jni"

#define MOTOR_NAME       		"/dev/motor"
#define HALL_NAME               "/dev/m1120"
#define PSWITCH_NAME       	    "/dev/pswitch"

#define PSWITCH_IOCTL_BASE                   0x8A
#define PSWITCH_IOCTL_GET_DATA				_IOR(PSWITCH_IOCTL_BASE, 0x00, int*)

#define MOTOR_ENABLE_PATH       "/sys/class/motor/cameramotor/pwm_enable"
#define MOTOR_SPEED_PATH       	"/sys/class/motor/cameramotor/mdspeed"
#define MOTOR_DIRECTION_PATH   	"/sys/class/motor/cameramotor/mddir"
#define MOTOR_ANGLE_PATH        "/sys/class/motor/cameramotor/mdangel"
#define MOTOR_MODE_PATH         "/sys/class/motor/cameramotor/mdmode"
#define MOTOR_DISABLE_VAL       0
#define MOTOR_ENABLE_VAL        1
#define MOTOR_LOW_SPEED_VAL     0
#define MOTOR_NORMAL_SPEED_VAL  1
#define MOTOR_FAST_SPEED_VAL    2
#define MOTOR_DEFAULT_MODE    6

#define LOG_TAG "rotation_jni"

static int mFileD_motor = -1;
static int mFileD_hall =-1;

namespace android {

class DeathNotifer : public IBinder::DeathRecipient {
	bool mDied =  false;
public:
	DeathNotifer() {}
	bool getIsDied() {
		return mDied;
	}
    void binderDied(const wp<IBinder>& who)
    {
        ALOGE("DeathNotifer notify service died...");
		mDied = true;
    }
};


static sp<IRotorSrv>  mRotorClient = NULL;
static sp<DeathNotifer> mNotifer = NULL;
static const sp<IRotorSrv>& getRotorService();
//native: camerservice   proxy:rotorsrv, ...   
int testRotorServiceBinder();        
int getRotorServicePara(int cmd_type);  // cmd_type ->CAMERASERVICE_BINDER_CMD
int setRotorServicePara(int cmd_type, int val);      


static const sp<IRotorSrv>& getRotorService()
{
    if (mRotorClient.get() == 0) 
    { 
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;

        binder = sm->getService(String16(ROTORSRV_NAME));
        
        if (binder == 0)
        {
            ALOGW("RotorService not published, please retry...");
            return NULL;
        }

        ALOGW("CameraService connect to RotorService success !!!");
        
        mRotorClient = interface_cast<IRotorSrv>(binder);
        mNotifer = new DeathNotifer();
		int status = binder->linkToDeath(mNotifer);
		ALOGW("getRotorService 3333 :%d", status);
    }
        
    return mRotorClient;        
}

int testRotorServiceBinder()
{
    ALOGI("testRotorServiceBinder ok");
    if (mRotorClient.get() != 0) {
        mRotorClient->testRotorSrvBinder();
    }
    return 0;
}
int getRotorServicePara(int cmd_type)
{
	if (mNotifer.get() != 0) {
		if(mNotifer->getIsDied()) {
			mRotorClient = NULL;
			getRotorService();
		}
	}
    if (mRotorClient.get() != 0) {
        mRotorClient->getRotorSrvPara(0);
    }
    return 0;
}

int setRotorServicePara(int mode, int direction, int angle, int speed, int enable)
{
	if (mNotifer.get() != 0) {
		if(mNotifer->getIsDied()) {
			mRotorClient = NULL;
			getRotorService();
		}
	}
    if (mRotorClient.get() != 0) {
        mRotorClient->setRotorSrvPara(mode, direction, angle, speed, enable);
    }
    return 0;
}

void setFillFlashBrightness(int val)
{
    if (mNotifer.get() != 0) {
        if(mNotifer->getIsDied()) {
            mRotorClient = NULL;
            getRotorService();
        }
    }
    if (mRotorClient.get() != 0) {
        mRotorClient->setFlashBrightness(val);
    }
}

/*static int
write_int(char const* path, int value)
{
    int fd;

    fd = open(path, O_RDWR);
    if (fd >= 0) {
        char buffer[20];
        int bytes = sprintf(buffer, "%d\n", value);
        int amt = write(fd, buffer, bytes);
    	ALOGD("write_int,openfile %s ok,amt = %d", path, amt);
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
    	ALOGD("write_int,openfile %s error,%s",path , strerror(errno));
        return -errno;
    }
}*/

static jint native_motor_set_direction(JNIEnv* env, jobject clazz, jint mode, jint direction, jint enable)
    {
/*
        int ret;
        int fd = mFileD_motor;
        XLOGD("MOTOR_IOCTL_SET_ENABLE enable = %d",enable);
        if(enable == 1){
    	    if(ret = ioctl(fd,RMOTOR_IOCTL_SET_PARA,&mode)){
    		    XLOGD("MOTOR_IOCTL_SET_ENABLE failed");
    		    return ret;
	        }
	        
    	    if(ret = ioctl(fd,RMOTOR_IOCTL_SET_DIR,&direction)){
    		    XLOGD("MOTOR_IOCTL_SET_DIR failed");
    		    return ret;
	        }
    	}
    	
        if(ret =  ioctl(fd,RMOTOR_IOCTL_SET_ENABLE,&enable)){
		    XLOGD("MOTOR_IOCTL_SET_ENABLE failed");
		    return ret;
	    }
    	
        XLOGD("[%s] native_motor_set_direction = %d, mode = %d, enable = %d\r\n", MOD, direction, mode, enable);  */

        /*if (enable == MOTOR_DISABLE_VAL) {
            write_int(MOTOR_ENABLE_PATH, enable);
        } else {
            write_int(MOTOR_DIRECTION_PATH, direction);
            write_int(MOTOR_ANGLE_PATH, 206);
            write_int(MOTOR_MODE_PATH, MOTOR_DEFAULT_MODE);
            write_int(MOTOR_SPEED_PATH, mode);
            write_int(MOTOR_ENABLE_PATH, enable);
        }*/

        setRotorServicePara(MOTOR_DEFAULT_MODE, direction, 206, mode/*int speed*/, enable);
        return 0;
    }
    
static jint native_motor_set_angle(JNIEnv* env, jobject clazz, jint mode, jint angle, jint direction,jint enable)
    {
/*
        int ret;
        int fd = mFileD_motor;
        XLOGD("MOTOR_IOCTL_SET_ENABLE enable = %d",enable);
        if(enable == 1){
    	    if(ret = ioctl(fd,RMOTOR_IOCTL_SET_PARA,&mode)){
    		    XLOGD("MOTOR_IOCTL_SET_ENABLE failed");
    		    return ret;
	        }
	        
    	    if(ret = ioctl(fd,RMOTOR_IOCTL_SET_DIR,&direction)){
    		    XLOGD("MOTOR_IOCTL_SET_DIR failed");
    		    return ret;
	        }

	        if (mode == 4 || mode ==5) {
        	    if(ret = ioctl(fd, RMOTOR_IOCTL_SET_ANGLE, &angle)){
        		    XLOGD("MOTOR_IOCTL_SET_ANGLE failed");
        		    return ret;
    	        }
	        }
    	}
    	
        if(ret =  ioctl(fd,RMOTOR_IOCTL_SET_ENABLE,&enable)){
		    XLOGD("MOTOR_IOCTL_SET_ENABLE failed");
		    return ret;
	    }
    	
        XLOGD("[%s] native_motor_set_angle = %d, mode = %d, angle = %d, enable = %d\r\n", MOD, direction, mode, angle, enable); */
/*        write_int(MOTOR_DIRECTION_PATH, direction);
        write_int(MOTOR_ANGLE_PATH, angle);
        write_int(MOTOR_MODE_PATH, MOTOR_DEFAULT_MODE);
        write_int(MOTOR_SPEED_PATH, mode);
        write_int(MOTOR_ENABLE_PATH, enable);*/
        setRotorServicePara(MOTOR_DEFAULT_MODE, direction, angle, mode, enable);
        return 0;
    }

static jint native_motor_set_para(JNIEnv* env, jobject clazz, jint mode, jint speed, jint angle, jint direction,jint enable)
{
    /*write_int(MOTOR_DIRECTION_PATH, direction);
    write_int(MOTOR_ANGLE_PATH, angle);
    write_int(MOTOR_MODE_PATH, mode);
    write_int(MOTOR_SPEED_PATH, speed);
    write_int(MOTOR_ENABLE_PATH, enable);*/
    setRotorServicePara(mode, direction, angle, speed, enable);
    return 0;
}

static jint native_open_motor(JNIEnv* env, jobject clazz){
    	ALOGD("---->native_open_motor");  
/*    	int fd = 0;
    	int ret = 0;
    	int lsStatus = 1;
    	//fd = open(MSENSOR_DEVICE_NAME, O_RDONLY);
    	//fd = open(MOTOR_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    	fd = open(MOTOR_NAME, O_RDONLY);
        if (fd < 0) {
    		ALOGD("open motor failed");
    		return fd;
    	} else {
    		ALOGD("open motor succeed");
    	}


    	mFileD_motor = fd;
    	return fd;*/
        if (mRotorClient.get() == 0) {
            getRotorService();
        }

    	return 0;
	}

static void native_close_motor(JNIEnv* env, jobject clazz){
/*    	ALOGD("---->native_close_motor");
       // ioctl(mFileD_motor,MOTOR_IOCTL_SET_ENABLE,0);
       native_motor_set_direction(env, clazz, 1, 0, 0);
    	close(mFileD_motor);*/
    	//write_int(MOTOR_ENABLE_PATH, MOTOR_DISABLE_VAL);
        setRotorServicePara(0, 0, 0, 0, 0);
    }
   
   
static jint native_open_hall(JNIEnv* env, jobject clazz) {
    int fd = 0;
    
    ALOGD("---->native_open_hall");  
    fd = open(HALL_NAME, O_RDONLY);

    if (fd < 0) {
        ALOGD("open hall failed");
        return fd;
    } else {
        ALOGD("open hall succeed");
    }

    mFileD_hall = fd;
    return fd;
}

static void native_close_hall(JNIEnv* env, jobject clazz) {
    ALOGD("---->native_close_hall");
    close(mFileD_hall);
}


static jintArray native_hall_test(JNIEnv* env, jobject clazz) {
    jintArray test;
    int cd[4];
    test = env->NewIntArray(4);

    ALOGD("---->native_hall_test");
    //get lux
    ioctl(mFileD_hall,RHALL_IOCTL_GET_THRESHOLD_LOW ,&cd[0]);
    //get IRdata
    ioctl(mFileD_hall,RHALL_IOCTL_GET_THRESHOLD_HIGH,&cd[1]);
    //get Prox
    ioctl(mFileD_hall,RHALL_IOCTL_GET_CALIBRATED_DATA,&cd[2]);

    ioctl(mFileD_hall,RHALL_IOCTL_GET_DATA,&cd[3]);

    ALOGD("RHALL_IOCTL_GET_THRESHOLD_LOW:%d",cd[0]);
    ALOGD("RHALL_IOCTL_GET_THRESHOLD_HIGH:%d",cd[1]);
    ALOGD("RHALL_IOCTL_GET_CALIBRATED_DATA:%d",cd[2]);
    ALOGD("RHALL_IOCTL_GET_CALIBRATED_DATA:%d",cd[3]);

    env->SetIntArrayRegion(test,0,4,cd);

    return test;
}

static jint native_get_ps_switch_data(JNIEnv* env, jobject clazz){
    	ALOGD("---->native_get_ps_switch_data");  
    	int fd = 0;
        int ret;

    	fd = open(PSWITCH_NAME, O_RDONLY);
        if (fd < 0) {
    		ALOGD("PSWITCH failed");
    		return -1;
    	} else {
    		ALOGD("open PSWITCH succeed");
    	}
        ioctl(fd,PSWITCH_IOCTL_GET_DATA,&ret);

        ALOGD("open PSWITCH succeed value %d\n", ret);
        
        close(fd);
        return ret;	   

}

static jint native_set_hall_calibrate(JNIEnv* env, jobject clazz, jshort calibrate_flag, jshort low_threshold, jshort high_threshold) {   
    int fd = -1;
    short buf_tmp[3];
    int ret = -1;
    RotorDetectionCaliData mRotorDetectionCaliData;
    
    memset(&mRotorDetectionCaliData,0,sizeof(mRotorDetectionCaliData));
    
    mRotorDetectionCaliData.cali_flag = calibrate_flag;
    mRotorDetectionCaliData.lowthd = low_threshold;
    mRotorDetectionCaliData.highthd = high_threshold;
    
    ALOGD("[%s] SetCalibrate mRotorDetectionCaliData lowthd = %d,highthd= %d ,cali_flag =%d\n", MOD,
            mRotorDetectionCaliData.lowthd,mRotorDetectionCaliData.highthd ,mRotorDetectionCaliData.cali_flag);
    //Get calibrate data here and set date in buf_tmp
    fd = open(HALL_NAME, O_RDWR);

    if(fd < 0) {			
        ALOGD("[%s] ERROR: open HALL file fail \n", MOD);
        goto exit;
    }

    if(ioctl(fd, RHALL_IOCTL_SET_CALIBRATION, &mRotorDetectionCaliData) < 0) {			
        ALOGD("[%s] ERROR: IOCTL HALL file fail or set HALL CaliData data fail \n", MOD);
        goto exit;
    }		

    ret = 1;
exit:		
    close(fd);

    return ret;
}


static jint native_get_calibrate_adc_data(JNIEnv* env, jobject clazz) {   
    int fd = -1;
    int result = 0;
    
    /***************Get calibrate data here and set date in buf_tmp*********************/
    fd = open(HALL_NAME, O_RDWR);

    if(fd < 0) {			
        ALOGD("[%s] ERROR: open HALL file fail \n", MOD);
        goto exit;
    }

    if(ioctl(fd, RHALL_IOCTL_GET_CALIBRATED_DATA, &result) < 0) {			
        ALOGD("[%s] ERROR: IOCTL HALL file fail or get HALL CaliData data fail \n", MOD);
        goto exit;
    }		

exit:		
    close(fd);
    ALOGD("[%s] native_get_calibrate_adc_data result = %d\n", MOD, result);

    return result;
}


static jshortArray native_calibrate_hall(JNIEnv* env, jobject clazz) {   
    int fd = -1;
    short buf_tmp[3];
    RotorDetectionCaliData mRotorDetectionCaliData;
    
    memset(&mRotorDetectionCaliData,0,sizeof(mRotorDetectionCaliData));

    //Get calibrate data here and set date in buf_tmp
    fd = open(HALL_NAME, O_RDWR);

    if(fd < 0) {			
        ALOGD("[%s] ERROR: open HALL file fail \n", MOD);
        goto exit;
    }

    if(ioctl(fd, RHALL_IOCTL_GET_CALIBRATION, &mRotorDetectionCaliData) < 0) {			
        ALOGD("[%s] ERROR: IOCTL HALL file fail or get HALL CaliData data fail \n", MOD);
        goto exit;
    }		

exit:		
    close(fd);
    ALOGD("[%s] Calibrate mRotorDetectionCaliData lowthd = %d,highthd= %d ,result =%d\n", MOD,
            mRotorDetectionCaliData.lowthd,mRotorDetectionCaliData.highthd ,mRotorDetectionCaliData.cali_flag);

    buf_tmp[0] = mRotorDetectionCaliData.cali_flag;
    buf_tmp[1] = mRotorDetectionCaliData.lowthd;
    buf_tmp[2] = mRotorDetectionCaliData.highthd;

    jshortArray buf = env->NewShortArray(3);
    env->SetShortArrayRegion(buf, 0, 3, buf_tmp);

    return buf;
}

static void native_set_fill_flash_brightness(JNIEnv* env, jobject clazz, jint val)
{
    setFillFlashBrightness(val);
}


static JNINativeMethod gNotify[] = {
    { "open_hall", "()I",  (void*)native_open_hall },
    { "close_hall", "()V",  (void*)native_close_hall },
    { "hall_test", "()[I", (void*) native_hall_test },
    { "calibrate_hall", "()[S", (void*) native_calibrate_hall },
    { "set_hall_calibrate_data", "(SSS)I", (void*) native_set_hall_calibrate },
    { "get_calibrate_adc_data", "()I", (void*) native_get_calibrate_adc_data },
    { "get_pswitch_value", "()I",  (void*)native_get_ps_switch_data },
    //{ "motor_rotation_set_direction", "(III)I",  (void*)native_motor_set_direction},
    { "motor_rotation_test", "(IIII)I",  (void*)native_motor_set_angle},
    { "motor_rotation_set_para", "(IIIII)I",  (void*)native_motor_set_para},
    { "open_motor", "()I",  (void*)native_open_motor },
    { "close_motor", "()V",  (void*)native_close_motor },
    { "set_fill_flash_brightness", "(I)V",  (void*)native_set_fill_flash_brightness },
};

int register_Rotation(JNIEnv* env) {
    int res = jniRegisterNativeMethods(env, "com/android/engineeringmode/manualtest/RotationUtils", gNotify, NELEM(gNotify));
    return res;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGD("GetEnv failed!");
        return result;
    }
    register_Rotation(env);

    return JNI_VERSION_1_4;
}
} // namespace android

