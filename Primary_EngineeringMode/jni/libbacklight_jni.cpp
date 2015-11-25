#define LOG_TAG "libbacklight_jni"
#include "jni.h"
#include "JNIHelp.h"
#include <stdio.h>
#include <stdlib.h>
#include <utils/misc.h>
#include <utils/Log.h>
// terry hack ++
//#include <cutils/log.h>
#include "common_log.h"
// terry hack --
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <hardware/lights.h>
#include <vibrator.h>
#include "cutils/properties.h"

#include <sys/stat.h>

namespace android {
 int register_lightsensor(JNIEnv* env);
 int register_nv(JNIEnv* env);
};
using namespace android;


#define SYNA_CDEV_NAME  "/dev/syna_touch"
#define SYNA_IOCTL_MAGIC 's'
#define SYNA_GET_VERSION  				_IOR(SYNA_IOCTL_MAGIC, 0, unsigned)
#define SYNA_GET_FW_VERSION   			_IOR(SYNA_IOCTL_MAGIC, 1, unsigned)
#define SYNA_PROGRAM_START        		_IOW(SYNA_IOCTL_MAGIC, 2, unsigned)
#define SYNA_GET_PROGRAM_STATE  		_IOR(SYNA_IOCTL_MAGIC, 3, unsigned)
#define BASE_RET_VALUE  100000

char const *  KEYBOARD_BACKLIGHT
        = "/sys/class/leds/keyboard-backlight/brightness";

char const*  BUTTON_BACKLIGHT
        = "/sys/class/leds/button-backlight/brightness";

char const*  CHARGE_RED_LIGHT
		= "/sys/class/leds/red/brightness";

char const*  CHARGE_GREEN_LIGHT
        = "/sys/class/leds/green/brightness";

//Baoti.Li@Prd.WCN, 2014/01/23, Add for 13095 breath lamp
char const * BREATH_LAMP_FRONT
        = "/sys/class/leds/front/brightness";
char const * BREATH_LAMP_BACK
        = "/sys/class/leds/back/brightness";

char const * LCD_GAMA_PATH
        = "/proc/oppoReserve3/lcd_gamaflag";
        
char const * LCD_BACKLIGHT_PATH
        = "/sys/class/leds/lcd-backlight/brightness";

char const* mLight[6] = {
    KEYBOARD_BACKLIGHT ,
    BUTTON_BACKLIGHT ,
    CHARGE_RED_LIGHT,
    CHARGE_GREEN_LIGHT,
    BREATH_LAMP_FRONT, //Baoti.Li Add for 13095 breath lamp
    BREATH_LAMP_BACK   //Baoti.Li Add for 13095 breath lamp
};

static char* SystemProperties_getSS(char* key, char* defaultProperty)
{
    int len;
    char buf[PROPERTY_VALUE_MAX];
    char* result = NULL;

    if (key == NULL) {
		LOGD("SystemProperties_getSS key must not be null.");
        goto error;
    }

    len = property_get(key, buf, "");
    if ((len <= 0) && (defaultProperty != NULL)) {
        result = defaultProperty;
    } else if (len >= 0) {
        result = buf;
    } else {
        result = "";
    }

error:
    return result;
}


static int
write_int(char const* path, int value)
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

static int
write_str(char const* path, char *str)
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

static void  native_openLight(JNIEnv* env, jclass clazz,jint type)
{
    char const* path;
    int flag;
	if( type == 0  )
	{
		path = mLight[0];
		flag = 8;
	}else if(type == 1)
	{
		path = mLight[1];
		flag = 8;
	}
	else if(type == 2)
	{
		path = mLight[2];
		flag = 2;
	}
	else if(type == 3)
	{
		path = mLight[3];
		flag = 2;
	}
	else if(type == 4) //Baoti.Li Add for 13095 breath lamp
	{
	    path = mLight[4];
	    flag = 255;
	    LOGD(" native_openLight path:[%s], flag:[%d]", path, flag);
	}
	else if(type == 5) //Baoti.Li Add for 13095 breath lamp
	{
		path = mLight[5];
		flag = 255;
		LOGD(" native_openLight path:[%s], flag:[%d]", path, flag);
	}
	else
	{
		return;
	}
	write_int(path,flag);
}

static jint native_readLightValue(JNIEnv* env , jclass clazz , jint type)
{
	char const* path = mLight[type];
	int fd = open(path, O_RDONLY);
	char value[10];
	if(fd >= 0)
	{
		int size = read(fd,value,sizeof(value));
		close(fd);
//		LOGE("read file size,%d",size);
		LOGE("read file value,%s",value);
//		LOGE("read file value[0],%d",value[0]);
		jint res;
		if(value[1] >= '0' && value[1] <= '9')
		{
            res = (value[0]-'0')*10 + (value[1]-'0');
		}else
		{
			res = value[0]-'0';
		}
		return  res;
	}else
	{
		LOGE("read file error,%s",strerror(errno));
		return -1;
	}
}

static void  native_colseLight(JNIEnv* env, jclass clazz,jint type)
{
	    char const* path;
	    int flag;
		if( type == 0  )
		{
			path = mLight[0];
			flag = 0;
		}else if(type == 1)
		{
			path = mLight[1];
			flag = 0;
		}
		else if(type == 2)
		{
			path = mLight[2];
			flag = 0;
		}
		else if(type == 3)
		{
			path = mLight[3];
			flag = 0;
		}
		else if(type == 4) //Baoti.Li Add for 13095 breath lamp
		{
		    path = mLight[4];
		    flag = 0;
		    LOGD(" native_colseLight path:[%s], flag:[%d]", path, flag);
		}
		else if(type == 5) //Baoti.Li Add for 13095 breath lamp
		{
		    path = mLight[5];
		    flag = 0;
		    LOGD(" native_colseLight path:[%s], flag:[%d]", path, flag);
		}
		else
		{
			return;
		}
		write_int(path,flag);
}

static void native_flashLamp(JNIEnv* env, jclass clazz, jint type)
{
	char const * pathex = "/sys/class/leds/led:flash_torch/flash_blink"; //Oneplus,Find7S
	char const * path = "/proc/qcom_flash";	 //14005
	//char const * pathex = "/sys/class/leds/led:flash_torch/brightness";
	if (access(path, F_OK) == 0) {
		write_int(path, type);                    
    } else if (access(pathex, F_OK) == 0) {
		write_int(pathex, type);
	}
	
	LOGE("native_openFlashLamp type:%d", type);
}

static void native_openFlashLamp(JNIEnv* env, jclass clazz)
{
	///sys/bus/i2c/drivers/adp1655/1-0030/test_flash
	char const * path = "/sys/devices/i2c-4/4-0030/test_flash";
	write_int(path, 300);
	LOGE("native_openFlashLamp 300");
}

static void native_openFlashlight(JNIEnv* env, jclass clazz) {
	char const * path = "/sys/devices/i2c-4/4-0030/torch_control";
	write_int(path, 160);
	LOGE("native_openFlashlight 160");
}

static void native_closeFlashlight(JNIEnv* env, jclass clazz) {
	char const * path = "/sys/devices/i2c-4/4-0030/torch_control";
	write_int(path, 0);
	LOGE("native_closeFlashlight 0");
}

static void native_setVibrate(JNIEnv* env, jclass clazz, jstring value) {
	/*
	const char * vibrateValue = env->GetStringUTFChars(value, NULL);
	vibrator_test(vibrateValue);
	LOGE("native_setVibrate, %s", vibrateValue);
	env->ReleaseStringUTFChars(value, vibrateValue);
	*/
}

static void native_setElectric(JNIEnv* env, jclass clazz,jint type,jint value)
{
   if(type == 0) // main key
   {
	   write_int(KEYBOARD_BACKLIGHT ,value);
   }else if(type == 1)//tp key
   {
	   write_int(BUTTON_BACKLIGHT ,value);
   }

	LOGE("native_setElectric type:%d,value:%d",type,value);
}

static void native_killProgress(JNIEnv* env, jclass clazz)
{
	//int ans = system(" ps | awk '/\/system\/bin\/diag_mdlog/{print $2}' | xargs kill  ");
	int ans = system(" ps | awk '/diag_mdlog/{print $2}' | xargs kill  ");
	LOGE("native_killProgress_mdlog:%d",ans);

	ans = system(" ps | awk '/diag_klog/{print $2}' | xargs kill  ");
	LOGE("native_killProgress_klog:%d",ans);

	ans = system(" ps | awk '/logcat/{print $2}' | xargs kill  ");
    LOGE("native_killProgress_logcat:%d",ans);
}


static jint native_touch_get_version(JNIEnv* env, jobject clazz)
{
	int g_file = -1;
	 int version;

	if ((g_file = open(SYNA_CDEV_NAME, O_RDWR)) < 0) {
		LOGE("open SYNA_1_NAME error");
	}
	else
	{		
			if (ioctl(g_file, SYNA_GET_VERSION, &version) < 0) {
				LOGE("syna ioctl SYNA_GET_VERSION error\n");
			}
			close(g_file);
	}
	return version;
}

static jint native_touch_get_prog_state(JNIEnv* env, jobject clazz)
{
	int g_file = -1;
	 int state;

	if ((g_file = open(SYNA_CDEV_NAME, O_RDWR)) < 0) {
		LOGE("open SYNA_1_NAME error");
	}
	else
	{		
			if (ioctl(g_file, SYNA_GET_PROGRAM_STATE, &state) < 0) {
				LOGE("syna ioctl SYNA_GET_VERSION error\n");
			}
			close(g_file);
	}
	return state;
}



static void native_touch_start_prog(JNIEnv* env, jobject clazz, jint version)
{
	int g_file = -1;

	if ((g_file = open(SYNA_CDEV_NAME, O_RDWR)) < 0) {
		LOGE("open SYNA_1_NAME error");
	}
	else
	{		
		if (ioctl(g_file, SYNA_PROGRAM_START, &version) < 0) {
			LOGE("syna ioctl SYNA_GET_VERSION error");
		}
		close(g_file);
	}
}

char const*  CONTACTOR_TEST
        = "/sys/devices/platform/accessory-battery/accessory_batt_test";
static void native_openContactorTest(JNIEnv* env, jobject clazz) {
	write_int(CONTACTOR_TEST, 199);
}

static void native_closeContactorTest(JNIEnv* env, jobject clazz) {
	write_int(CONTACTOR_TEST, 0);
}

char const* SERIALPORT_SWITCH = "/sys/devices/platform/smb346-battery/hsuart_switch";
static void native_openSerialPort(JNIEnv* env, jobject clazz) {
	write_int(SERIALPORT_SWITCH, 199);
}

static void native_closeSerialPort(JNIEnv* env, jobject clazz) {
	write_int(SERIALPORT_SWITCH, 0);
}

char const* CHARGING_SETTINGS = "/sys/bus/platform/devices/pm8921-charger/pm8921_chg";
static void native_setCharging(JNIEnv *env, jobject clazz, jint value) {
	LOGE("native_setCharging, value: %d", value);
	write_int(CHARGING_SETTINGS, value);
}
//=====add by cbt 2012/11/05
#define SLED_IOCTL_MAGIC 'l'
#define SLED_SET_RED                 _IOW(SLED_IOCTL_MAGIC, 5, unsigned) //set red
#define SLED_SET_GREEEN            _IOW(SLED_IOCTL_MAGIC, 6, unsigned)//set green
#define SLED_SET_BLUE               _IOW(SLED_IOCTL_MAGIC, 7, unsigned) //set blue
#define SLED_ENABLE                  _IOW(SLED_IOCTL_MAGIC, 0, unsigned) //enable led
#define SLED_DISABLE                 _IOW(SLED_IOCTL_MAGIC, 1, unsigned) //disable led
#define SLED_CONFIG_FEATURE      _IOW(SLED_IOCTL_MAGIC, 2, unsigned)//	stop flash or start flah
#define SLED_SET_WORKMOD       _IOW(SLED_IOCTL_MAGIC, 3, unsigned) //set work mod
#define SLED_SET_BTEATHTIME    _IOW(SLED_IOCTL_MAGIC, 4, unsigned)
#define SET_RED_BREATHE           _IOW(SLED_IOCTL_MAGIC, 8, unsigned)
#define SET_GREEN_BREATHE       _IOW(SLED_IOCTL_MAGIC, 9, unsigned)
#define SET_BLUE_BREATHE         _IOW(SLED_IOCTL_MAGIC, 10, unsigned)

//wangw@OnLineRD.DeviceService,2013/08/15,Add methods to turn off each light
#define SET_RED_OFF              _IOW(SLED_IOCTL_MAGIC, 11, unsigned)   //turn off red
#define SET_GREEN_OFF            _IOW(SLED_IOCTL_MAGIC, 12, unsigned)   //turn off green
#define SET_BLUE_OFF             _IOW(SLED_IOCTL_MAGIC, 13, unsigned)   //turn off blue

static void native_openHuxiLight(JNIEnv* env, jclass clazz, jint value)
{
 	int fd = open("/dev/SN3193", O_RDWR); 
 	int status = ioctl(fd,SLED_ENABLE,0);  
 	LOGE("SLED_ENABLE");  
    if( status < 0)  
    {  
        printf("read fail!\n");   
    }
    int val = 0x20;
    status=ioctl(fd,SLED_CONFIG_FEATURE,&val);
    LOGE("SLED_CONFIG_FEATURE");
    val = 0x00;
    status=ioctl(fd,SLED_SET_WORKMOD,&val);
    LOGE("SLED_SET_WORKMOD");
    val = 150;
    switch(value)
    {
    case 1:
      status=ioctl(fd,SLED_SET_RED,&val);
      LOGE("SLED_SET_RED open");
      break;
    case 2:
      status=ioctl(fd,SLED_SET_GREEEN,&val);
      LOGE("SLED_SET_GREEEN open");
      break;
    case 3:
       status=ioctl(fd,SLED_SET_BLUE,&val);
       LOGE("SLED_SET_BLUE open");
       break;
    }
//    close(fd);
}
static void native_closeHuxiLight(JNIEnv* env, jclass clazz)
{
	int fd = open("/dev/SN3193", O_RDWR); 
 	ioctl(fd,SLED_DISABLE,0);
 	 LOGE("SLED_DISABLE");
    close(fd);
}

static void native_closeRedLight(JNIEnv* env, jclass clazz)
{
	int fd = open("/dev/SN3193", O_RDWR);
 	ioctl(fd,SET_RED_OFF,0);
 	LOGE("SET_RED_OFF off");
    close(fd);
}

static void native_closeGreenLight(JNIEnv* env, jclass clazz)
{
	int fd = open("/dev/SN3193", O_RDWR);
 	ioctl(fd,SET_GREEN_OFF,0);
 	LOGE("SET_GREEN_OFF off");
    close(fd);
}

static void native_closeBlueLight(JNIEnv* env, jclass clazz)
{
	int fd = open("/dev/SN3193", O_RDWR);
 	ioctl(fd,SET_BLUE_OFF,0);
 	LOGE("SET_BLUE_OFF off");
    close(fd);
}


static void native_setHuxiBlueBrightness(JNIEnv* env, jclass clazz, jint val)
{
	int fd = open("/dev/SN3193", O_RDWR); 
 	ioctl(fd,SLED_SET_BLUE,&val);
    close(fd);
}
static void native_setHuxiRedBrightness(JNIEnv* env, jclass clazz, jint val)
{
	int fd = open("/dev/SN3193", O_RDWR); 
 	ioctl(fd,SLED_SET_RED,&val);
    close(fd);
}
static void native_setHuxiGreenBrightness(JNIEnv* env, jclass clazz, jint val)
{
	int fd = open("/dev/SN3193", O_RDWR); 
 	ioctl(fd,SLED_SET_GREEEN,&val);
    close(fd);
}

static void native_lightFlash(JNIEnv* env, jclass clazz,jint value)
{
 	int fd = open("/dev/SN3193", O_RDWR);

   int val = 150;
    switch(value)
       {
       case 1:
         ioctl(fd,SET_RED_BREATHE,&val);
         break;
       case 2:
         ioctl(fd,SET_GREEN_BREATHE,&val);
         break;
       case 3:
          ioctl(fd,SET_BLUE_BREATHE,&val);
          break;
       }
	   
}

static int readInt(char const* path) {
	int fd = open(path, O_RDONLY);
	char value[10];
	if(fd >= 0)
	{
		int size = read(fd,value,sizeof(value));
		close(fd);
		LOGE("read file value,%s",value);
		int res;
		if(value[1] >= '0' && value[1] <= '9')
		{
            res = (value[0]-'0')*10 + (value[1]-'0');
		}else
		{
			res = value[0]-'0';
		}
		return  res;
	}else
	{
		LOGE("read file error,%s",strerror(errno));
		return -1;
	}
}

static int native_getLcdBrightness(JNIEnv* env, jclass clazz)
{
    return readInt(LCD_BACKLIGHT_PATH);
}

static void native_setLcdBrightness(JNIEnv* env, jclass clazz,jint value)
{
    write_int(LCD_BACKLIGHT_PATH, value);
}

static int native_getLcdGamma(JNIEnv* env, jclass clazz)
{
    return readInt(LCD_GAMA_PATH);
}

static int native_setLcdGamma(JNIEnv* env, jclass clazz,jint value)
{
    return write_int(LCD_GAMA_PATH, value);
}

/*
 *          闪烁
 *
	echo 255 > sys/class/leds/rgb_red/brightness
	echo 255 > sys/class/leds/rgb_green/brightness
	echo 255 > sys/class/leds/rgb_blue/brightness
	echo 20 > sys/class/leds/rgb_red/ramp_step_ms
	echo 20 > sys/class/leds/rgb_blue/ramp_step_ms  //这个是调节周期
	echo 20 > sys/class/leds/rgb_green/ramp_step_ms
	echo 1 > sys/class/leds/rgb_blue/blink
	echo 1 > sys/class/leds/rgb_red/blink
	echo 1 > sys/class/leds/rgb_green/blink
*/

/*
 *          常量
 *
	echo 255 > sys/class/leds/rgb_red/brightness
	echo 255 > sys/class/leds/rgb_green/brightness
	echo 255 > sys/class/leds/rgb_blue/brightness

	echo 0 > sys/class/leds/rgb_blue/blink
	echo 0 > sys/class/leds/rgb_red/blink
	echo 0 > sys/class/leds/rgb_green/blink
*/
static void native_openBreatheLight(JNIEnv* env, jclass clazz, jboolean blink) {

	if (blink)
		LOGE("native_openBreatheLight ");
	else
		LOGE("native_openBreatheLight blink:false");

	write_int("/sys/class/leds/rgb_red/brightness", 255);
	write_int("/sys/class/leds/rgb_green/brightness", 255);
	write_int("/sys/class/leds/rgb_blue/brightness", 255);

	if (blink) {
		write_int("/sys/class/leds/rgb_red/ramp_step_ms", 20);
		write_int("/sys/class/leds/rgb_green/ramp_step_ms", 20);
		write_int("/sys/class/leds/rgb_blue/ramp_step_ms", 20);
	}

	write_int("/sys/class/leds/rgb_blue/blink", blink ? 1 : 0);
	if (blink) {
		write_int("/sys/class/leds/rgb_red/blink", blink ? 1 : 0 );
		write_int("/sys/class/leds/rgb_green/blink", blink ? 1 : 0);
	}
}

static void native_closeBreatheLight(JNIEnv* env, jclass clazz) {
	LOGE("native_closeBreatheLight");
	write_int("/sys/class/leds/rgb_red/brightness", 0);
	write_int("/sys/class/leds/rgb_green/brightness", 0);
	write_int("/sys/class/leds/rgb_blue/brightness", 0);
	write_int("/sys/class/leds/rgb_blue/blink", 0);
	write_int("/sys/class/leds/rgb_red/blink", 0);
	write_int("/sys/class/leds/rgb_green/blink", 0);
}

static void native_setBreatheLightBrightness(JNIEnv* env, jclass clazz, jint val) {
    LOGE("native_setBreatheLightBrightness val:%d", val);
    write_int("/sys/class/leds/rgb_red/brightness", val);
	write_int("/sys/class/leds/rgb_green/brightness", val);
	write_int("/sys/class/leds/rgb_blue/brightness", val);
}


#if 1
#define VL6180_DEV_NAME  "/dev/stmvl6180_ranging"
#define VL6180x_HAVE_RATE_DATA
#define  VL6180x_HAVE_DMAX_RANGING 1
#define  VL6180x_HAVE_WRAP_AROUND_DATA
#define  ALLOWED_MIN_ERROR_RATIO 85/100
#define  ALLOWED_MAX_ERROR_RATIO 115/100

/**
 * Filtered result data structure  range data is to be used
 */
typedef struct RangeFilterResult_tag {
    uint16_t range_mm;      /*!< Filtered ranging value */
    uint16_t rawRange_mm;   /*!< raw range value (scaled) */
} RangeFilterResult_t;

/**
 * @struct VL6180x_RangeData_t
 * @brief Range and any optional measurement data.
 */
 typedef struct {
    int32_t range_mm;          /*!< range distance in mm. */
    int32_t signalRate_mcps;   /*!< signal rate (MCPS)\n these is a 9.7 fix point value, which is effectively a measure of target reflectance.*/
    uint32_t errorStatus;      /*!< Error status of the current measurement. \n
                                  see @a ::RangeError_u @a VL6180x_GetRangeStatusErrString() */


#ifdef VL6180x_HAVE_RATE_DATA
    uint32_t rtnAmbRate;    /*!< Return Ambient rate in KCount per sec related to \a RESULT_RANGE_RETURN_AMB_COUNT */
    uint32_t rtnRate;       /*!< Return rate in KCount per sec  related to \a RESULT_RANGE_RETURN_SIGNAL_COUNT  */
    uint32_t rtnConvTime;   /*!< Return Convergence time \a RESULT_RANGE_RETURN_CONV_TIME */
    uint32_t refConvTime;   /*!< Reference convergence time \a RESULT_RANGE_REFERENCE_CONV_TIME */
#endif


#if  VL6180x_HAVE_DMAX_RANGING
    uint32_t DMax;              /*!< DMax  when applicable */
#endif

#ifdef  VL6180x_HAVE_WRAP_AROUND_DATA
    RangeFilterResult_t FilteredData; /*!< Filter result main range_mm is updated */
#endif

//add for debug
    uint32_t m_refAmbRate;
    uint32_t m_convTime;
    uint32_t m_rtnSignalCount;
    uint32_t m_refSignalCount;
    uint32_t m_rtnAmbientCount;
    uint32_t m_refAmbientCount;
    uint16_t m_refRate;	
    uint16_t m_crossTalk;
    uint8_t m_rangeOffset;	
    uint8_t m_rawRange_mm;	
//add for debug
}VL6180x_RangeData_t;

VL6180x_RangeData_t datas;

//******************************** IOCTL definitions
#define VL6180_IOCTL_INIT 			_IO('p', 0x01)
#define VL6180_IOCTL_XTALKCALB		_IO('p', 0x02)
#define VL6180_IOCTL_OFFCALB		_IO('p', 0x03)
#define VL6180_IOCTL_STOP			_IO('p', 0x05)
#define VL6180_IOCTL_SETXTALK		_IOW('p', 0x06, unsigned int)
#define VL6180_IOCTL_SETOFFSET		_IOW('p', 0x07, int8_t)
#define VL6180_IOCTL_GETDATA 		_IOR('p', 0x0a, unsigned long)
//#define VL6180_IOCTL_GETDATAS 		_IOR('p', 0x0b, RangeData)
#define VL6180_IOCTL_GETDATAS 		_IOR('p', 0x0b, VL6180x_RangeData_t)
/* for ver< 1.0 */

int calibrate_fd = 0;

/*
return value:
-1 means open /dev/stmvl6180_ranging failed
>=0 means OK
*/
static jint native_open_laser_sensor(JNIEnv* env, jobject clazz)
{
	calibrate_fd = open(VL6180_DEV_NAME, O_RDWR | O_SYNC);
	if (calibrate_fd < 0) {
		LOGE("open /dev/stmvl6180_ranging failed: %s", strerror(errno));
		return (jint)-1;// -1 means open /dev/stmvl6180_ranging failed
	} else {
        	LOGE("open /dev/stmvl6180_ranging succeed");
		return 0;
	}

	return 0;
}

static jint native_close_laser_sensor(JNIEnv* env, jobject clazz)
{
	if(calibrate_fd > 0)
	{
		int ret;
		ret = ioctl(calibrate_fd, VL6180_IOCTL_STOP, NULL);
		if (ret < 0) {
			LOGE("ioctl VL6180_IOCTL_STOP:%d failed: %s", VL6180_IOCTL_STOP, strerror(errno));		
		}
		
		close(calibrate_fd);
		calibrate_fd = 0;
	}

	return 0;
}


/*
return value:
-1 means open /dev/stmvl6180_ranging failed
-2 means VL6180_IOCTL_STOP failed
-3 means VL6180_IOCTL_INIT failed
-4 means VL6180_IOCTL_GETDATAS failed
>=0 means OK
*/
static jint native_get_laser_range_value(JNIEnv* env, jobject clazz)
{
	int ret;

	if(calibrate_fd <= 0){
		LOGE("%s:%d invalid calibrate_fd", __func__, __LINE__);		
		return (jint)-1;//-1 means open /dev/stmvl6180_ranging failed
	}
	
	ret = ioctl(calibrate_fd, VL6180_IOCTL_STOP, &datas);
	if (ret < 0) {
		LOGE("ioctl VL6180_IOCTL_STOP failed: %s", strerror(errno));
		return (jint)-2;// -2 means VL6180_IOCTL_STOP failed
	} else {
		LOGE("ioctl VL6180_IOCTL_STOP succeed");
    	}

	ret = ioctl(calibrate_fd, VL6180_IOCTL_INIT, &datas);
	if (ret < 0) {
		LOGE("ioctl VL6180_IOCTL_INIT failed: %s", strerror(errno));
		return (jint)-3;// -3 means VL6180_IOCTL_INIT failed
	} else {
		LOGE("ioctl VL6180_IOCTL_INIT succeed");
	}
	 
	usleep(500*1000);
	 
	ret = ioctl(calibrate_fd, VL6180_IOCTL_GETDATAS, &datas);
	if (ret < 0) {
		LOGE("ioctl VL6180_IOCTL_GETDATAS failed: %s", strerror(errno));
		ioctl(calibrate_fd, VL6180_IOCTL_STOP, NULL);			
		return (jint)-4;// -4 means VL6180_IOCTL_GETDATAS failed
	} else {
		LOGE("ioctl VL6180_IOCTL_GETDATAS succeed");
    	}

	LOGE("VL6180_IOCTL_INIT:%ld",datas.m_rawRange_mm*3);
	ret = ioctl(calibrate_fd, VL6180_IOCTL_STOP, NULL);
	if (ret < 0) {
		LOGE("ioctl VL6180_IOCTL_STOP:%d failed: %s", VL6180_IOCTL_STOP, strerror(errno));		
	}	

	return (jint) (datas.range_mm);//(datas.m_rawRange_mm*3);
}

static jfloat native_get_laser_rtn_signal_rate_value(JNIEnv* env, jobject clazz)
{
	jfloat res;
	res = (jfloat)(datas.signalRate_mcps/128.0);
	return res;
}

static jint native_get_laser_raw_range_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.m_rawRange_mm*3);
       return res;
}

static jint native_get_laser_error_code_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.errorStatus);
       return res;
}

static jint native_get_laser_range_offset_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.m_rangeOffset);
       return res;
}

static jfloat native_get_laser_cross_talk_value(JNIEnv* env, jobject clazz)
{
	jfloat res;
	res = (jfloat)(datas.m_crossTalk/128.0);
       return res;
}

static jfloat native_get_laser_ref_signal_rate_value(JNIEnv* env, jobject clazz)
{
	jfloat res;
	res = (jfloat)(datas.m_refRate/128.0);
       return res;
}

static jfloat native_get_laser_rtn_amb_rate_value(JNIEnv* env, jobject clazz)
{
	jfloat res;
	res = (jfloat)(datas.rtnAmbRate/128.0);
       return res;
}

static jfloat native_get_laser_ref_amb_rate_value(JNIEnv* env, jobject clazz)
{
	jfloat res;
	res = (jfloat)(datas.m_refAmbRate/128.0);
       return res;
}

static jint native_get_laser_conv_time_value(JNIEnv* env, jobject clazz)
{
 	jint res;
	res = (jint)(datas.m_convTime);
       return res;
}

static jint native_get_laser_rtn_signal_count_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.m_rtnSignalCount);
       return res;
}

static jint native_get_laser_ref_signal_count_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.m_refSignalCount);
       return res;
}

static jint native_get_laser_rtn_amb_count_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.m_rtnAmbientCount);
       return res;
}

static jint native_get_laser_ref_amb_count_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.m_refAmbientCount);
       return res;
}

static jint native_get_laser_rtn_conv_time_value(JNIEnv* env, jobject clazz)
{
 	jint res;
	res = (jint)(datas.rtnConvTime);
       return res;
}

static jint native_get_laser_ref_conv_time_value(JNIEnv* env, jobject clazz)
{
	jint res;
	res = (jint)(datas.refConvTime);
       return res;
}

/*
return value:
-1 means open open stmvl6180_ranging failed
-2 means VL6180_IOCTL_SETOFFSET failed
-3 means VL6180_IOCTL_SETXTALK failed
0 means no error
*/
static jint native_calibrate_reset(JNIEnv* env, jobject clazz)
{
	int fd = 0;
	int offset = 0;
	int cross_talk = 0;

	system("mkdir /persist/camera");//kelong
	system("echo 0 > /persist/camera/LaserFocusOffset.txt");
	system("echo 0 > /persist/camera/LaserFocusCrossTalk.txt");
	fd = open("/dev/stmvl6180_ranging",O_RDWR | O_SYNC);
	if (fd <= 0){
		LOGE("Error open stmvl6180_ranging device: %s\n", strerror(errno));
		return (jint)-1;//-1 means open open stmvl6180_ranging failed
	}
	LOGE("open vl6180_ranging device successfully!\n");

	if (ioctl(fd, VL6180_IOCTL_SETOFFSET , &offset) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_SETOFFSET : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)-2;//-2 means VL6180_IOCTL_SETOFFSET failed
	}	
	if (ioctl(fd, VL6180_IOCTL_SETXTALK , &cross_talk) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_SETXTALK : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)-3;//-3 means VL6180_IOCTL_SETXTALK failed
	}		

	if(fd > 0)
		close(fd);
	return (jint)0;
}

/*
return value:
256 means open vl6180_ranging failed
257 means VL6180_IOCTL_STOP failed
258 means VL6180_IOCTL_INIT failed
259 means VL6180_IOCTL_GETDATAS failed
260 means VL6180_IOCTL_OFFCALB failed
261 means the offset is out of boundary
262 means VL6180_IOCTL_SETOFFSET failed
*/
static jint native_offset_calibrate(JNIEnv* env, jobject clazz)
{
	int fd = 0;
	int num_samples = 40;
	int sum_count = 0;
	int i=0;
	int offset = 0;
	int cross_talk  = 0;
	int RangeSum =0,RangeAvg=0;
	int OffsetInt =0;
	VL6180x_RangeData_t range_datas;
	
	system("mkdir /persist/camera");//kelong
	system("echo 0 > /persist/camera/LaserFocusOffset.txt");
	fd = open("/dev/stmvl6180_ranging",O_RDWR | O_SYNC);
	if (fd <= 0){
		LOGE("Error open stmvl6180_ranging device: %s\n", strerror(errno));
		return (jint)256;//256 means open vl6180_ranging failed
	}
	LOGE("open vl6180_ranging device successfully!\n");

	if (ioctl(fd, VL6180_IOCTL_SETOFFSET , &offset) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_SETOFFSET : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)-2;//-2 means VL6180_IOCTL_SETOFFSET failed
	}	

	if (ioctl(fd, VL6180_IOCTL_SETXTALK , &cross_talk) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_SETXTALK : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)-3;//-3 means VL6180_IOCTL_SETXTALK failed
	}		

	//make sure it's not started
	if (ioctl(fd, VL6180_IOCTL_STOP , NULL) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_STOP : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)257;//257 means VL6180_IOCTL_STOP failed
	}	

	LOGE("offset Calibrate place white target at 100mm from glass===\n");
// to offset calibration 
	if (ioctl(fd, VL6180_IOCTL_OFFCALB , NULL) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_INIT : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)258;//258 means VL6180_IOCTL_INIT failed
	}
	
	for (i=0; i< num_samples; i++)
	{	
		usleep(50*1000); /*50ms*/
		if(ioctl(fd, VL6180_IOCTL_GETDATAS,&range_datas) < 0){
			LOGE("Error: Could not perform VL6180_IOCTL_GETDATAS : %s\n", strerror(errno));
			if(fd > 0)
				close(fd);
			return (jint)259;//259 means VL6180_IOCTL_GETDATAS failed
		}
				
		LOGE(" VL6180 Range Data:%d, error status:0x%x, Rtn Signal Rate:%d, signalRate_mcps:%d\n",
					range_datas.range_mm, range_datas.errorStatus, range_datas.rtnRate, range_datas.signalRate_mcps);	
		RangeSum += range_datas.range_mm;
	
	}
	RangeAvg = RangeSum/num_samples;
	LOGE("VL6180 Offset Calibration get the average Range as %d\n", RangeAvg);
	if ((RangeAvg >= (100-3)) && (RangeAvg <= (100+3)))
		LOGE("VL6180 Offset Calibration: original offset is OK, no need to do offset calibration\n");
	else{
		if (ioctl(fd, VL6180_IOCTL_STOP , NULL) < 0) {
			LOGE("Error: Could not perform VL6180_IOCTL_STOP : %s\n", strerror(errno));
			if(fd > 0)
				close(fd);
			return (jint)257;//257 means VL6180_IOCTL_STOP failed
		}
		if (ioctl(fd, VL6180_IOCTL_OFFCALB , NULL) < 0) {//will set Offset, CrossTalk to 0 and do init in kernel driver
			LOGE("Error: Could not perform VL6180_IOCTL_OFFCALB : %s\n", strerror(errno));
			if(fd > 0)
				close(fd);
			return (jint)260;//260 means VL6180_IOCTL_OFFCALB failed
		}
		RangeSum = 0;
		sum_count = 0;
		for (i=0; i< num_samples; i++)
		{
			usleep(50*1000); /*50ms*/
			if(ioctl(fd, VL6180_IOCTL_GETDATAS,&range_datas) < 0){
				LOGE("Error: Could not perform VL6180_IOCTL_GETDATAS : %s\n", strerror(errno));
				if(fd > 0)
					close(fd);
				return (jint)259;//259 means VL6180_IOCTL_GETDATAS failed
			}
			LOGE(" VL6180 Range Data:%d, error status:0x%x, Rtn Signal Rate:%d, signalRate_mcps:%d\n",
						range_datas.range_mm, range_datas.errorStatus, range_datas.rtnRate, range_datas.signalRate_mcps);	
			if (range_datas.range_mm >= (RangeAvg * ALLOWED_MIN_ERROR_RATIO)
				&& range_datas.range_mm <= (RangeAvg * ALLOWED_MAX_ERROR_RATIO))
			{
				RangeSum += range_datas.range_mm;
				sum_count++;
			}
		}
		// RangeAvg = RangeSum/num_samples;
		if (sum_count > 0)
		{
			RangeAvg = RangeSum/sum_count;
		}
		else
		{
			if(fd > 0)
				close(fd);
			return (jint)261;//261 means the offset is out of boundary
		}
		LOGE("VL6180 Offset Calibration get the average Range as %d\n", RangeAvg);
	}
	
	offset = 100 - RangeAvg;
	if((offset > 127) ||(offset < -128))
	{
		LOGE("%s:%d out of boundary, the offset is %d", __func__, __LINE__, offset);
		if(fd > 0)
			close(fd);
		return (jint)261;//261 means the offset is out of boundary
	}
#if 0//kernel driver already did this
	if(offset < 0)
		offset = 256+offset;
#endif	
	LOGE("VL6180 Offset Calibration to set the offset value(pre-scaling) as %d\n",offset);
/**now need to resset the driver state to scaling mode that is being turn off by IOCTL_OFFCALB**/ 
	if (ioctl(fd, VL6180_IOCTL_STOP , NULL) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_STOP : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)257;//257 means VL6180_IOCTL_STOP failed
	}	
	
	if (ioctl(fd, VL6180_IOCTL_SETOFFSET, &offset) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_SETOFFSET : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)262;//262 means VL6180_IOCTL_SETOFFSET failed
	}
	
	if(fd > 0)
		close(fd);

	return offset;
}

/*
return value:
-1 means open /dev/stmvl6180_ranging failed
-2 means VL6180_IOCTL_STOP failed
-3 means ioctl VL6180_IOCTL_XTALKCALB  failed
-4 means ioctl VL6180_IOCTL_GETDATAS failed
-5 means VL6180_IOCTL_SETXTALK failed
*/
static jint native_cross_talk_calibrate(JNIEnv* env, jobject clazz)
{
	int fd = 0;
	int num_samples = 40;
	int valid_samples = 0;
	int i=0;
	int RangeSum =0;
	int RateSum = 0;
	int XtalkInt =0;
	VL6180x_RangeData_t range_datas;	

	system("mkdir /persist/camera");
	system("echo 0 > /persist/camera/LaserFocusCrossTalk.txt");
	fd = open("/dev/stmvl6180_ranging",O_RDWR | O_SYNC);
	if (fd <= 0){
		LOGE("Error open stmvl6180_ranging device: %d, %s\n", fd, strerror(errno));
		return -1;//-1 means open /dev/stmvl6180_ranging failed
	}
	LOGE("open vl6180_ranging device successfully!\n");

	if (ioctl(fd, VL6180_IOCTL_SETXTALK , &XtalkInt) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_SETXTALK : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return (jint)-3;//-3 means VL6180_IOCTL_SETXTALK failed
	}		

	//make sure it's not started
	if (ioctl(fd, VL6180_IOCTL_STOP , NULL) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_STOP : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return -2;//-2 means VL6180_IOCTL_STOP failed
	}	

	LOGE("xtalk Calibrate place black target at 400mm from glass===\n");
		// to xtalk calibration 
	if (ioctl(fd, VL6180_IOCTL_XTALKCALB , NULL) < 0) {//will set CrossTalk to 0 and do init
		LOGE("Error: Could not perform VL6180_IOCTL_XTALKCALB : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return -3;//-3 means ioctl VL6180_IOCTL_XTALKCALB  failed
	}
	for (i=0; i< num_samples; i++)
	{
		usleep(50*1000); /*50ms*/
		if(ioctl(fd, VL6180_IOCTL_GETDATAS,&range_datas) < 0){
			LOGE("Error: Could not perform VL6180_IOCTL_GETDATAS : %s\n", strerror(errno));
			if(fd > 0)
				close(fd);
			return -4;//-4 means ioctl VL6180_IOCTL_GETDATAS failed
		}
				
		LOGE(" VL6180 Range Data:%d, error status:0x%x, Rtn Signal Rate:%d, signalRate_mcps:%d\n",
				range_datas.range_mm, range_datas.errorStatus, range_datas.rtnRate, range_datas.signalRate_mcps);
        //lanhe@oppo.com NOTE:while calibrate crosstalk we need filter invalid ranges
		if(range_datas.range_mm < 765){
			valid_samples++;
		    RangeSum += range_datas.range_mm;
		    RateSum += 	range_datas.signalRate_mcps;// signalRate_mcps in 9.7 format
		}
	}
//	XtalkInt = (int)((float)RateSum/num_samples *(1-(float)RangeSum/num_samples/400));
	XtalkInt = (RateSum *(400*valid_samples-RangeSum))/(valid_samples*400*valid_samples);

	LOGE("VL6180 Xtalk Calibration get Xtalk Compensation rate in 9.7 format as %d\n",XtalkInt);
	if (ioctl(fd, VL6180_IOCTL_SETXTALK,&XtalkInt) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_SETXTALK : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return -5;//-5 means VL6180_IOCTL_SETXTALK failed
	}
	if (ioctl(fd, VL6180_IOCTL_STOP , NULL) < 0) {
		LOGE("Error: Could not perform VL6180_IOCTL_STOP : %s\n", strerror(errno));
		if(fd > 0)
			close(fd);
		return -2;
	}			

	if(fd > 0)
		close(fd);
	return (XtalkInt + BASE_RET_VALUE);
}

#else

#define VL6180_DEV_NAME  "/dev/stmvl6180_ranging"
#define VL6180_IOCTL_INIT         _IO('p', 0x01)
#define VL6180_IOCTL_GETDATA         _IO('p', 0x02)
#define VL6180_IOCTL_GETDATAS         _IO('p', 0x03)
#define VL6180_IOCTL_CONFIG         _IO('p', 0x04)

typedef struct
{
    unsigned int m_range;
    unsigned int m_trueRange_mm;
    unsigned int m_rawRange_mm;
    unsigned int m_rtnRate;
    unsigned int m_refRate;
    unsigned int m_rtnAmbRate;
    unsigned int m_refAmbRate;
    unsigned int m_convTime;
    unsigned int m_rtnSignalCount;
    unsigned int m_refSignalCount;
    unsigned int m_rtnAmbientCount;
    unsigned int m_refAmbientCount;
    unsigned int m_rtnConvTime;
    unsigned int m_refConvTime;
    int m_strayLightFactor;
       unsigned int m_errorCode;
       unsigned int m_rangeOffset;
       unsigned int m_crossTalk;
}RangeData;

typedef struct
{
    uint32_t addr;
    unsigned int size;
    uint16_t data;
}RegisterInfo;

RangeData datas;


/*
return value:
-1 means open /persist/camera/LaserFocusOffset.txt failed
-2 means read /persist/camera/LaserFocusOffset.txt failed
-3 means offset is incorrect
-4 means open /persist/camera/LaserFocusCrossTalk.txt failed
-5 means read /persist/camera/LaserFocusCrossTalk.txt failed
-6 means cross talk is incorrect
-7 means open /dev/stmvl6180_ranging failed
-8 means VL6180_IOCTL_CONFIG for offset failed
-9 means VL6180_IOCTL_CONFIG for cross talk failed
-10 means VL6180_IOCTL_GETDATAS failed
>=0 means OK
*/
static jint native_get_laser_range_value(JNIEnv* env, jobject clazz)
{
    int calibrate_fd ,ret = 0;
    int offset_fd = 0;
    int cross_talk_fd = 0;
    int offset = 0;
    int cross_talk = 0;
    char offset_string[20] = {0};
    char cross_talk_string[20] = {0};
        unsigned long data;
        jint res;
    int data_num = 10;
    RegisterInfo register_data;

    offset_fd = open("/persist/camera/LaserFocusOffset.txt", O_RDWR | O_SYNC);
    if (offset_fd < 0) {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt failed: %s", __func__, __LINE__, strerror(errno));
        return (jint)-1;// -1 means open /persist/camera/LaserFocusOffset.txt failed
    } else {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt succeed", __func__, __LINE__);
    }

    ret =  read(offset_fd, offset_string, 20);
    if(ret < 0){
        LOGE("%s:%d read offset calibrate data failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        return (jint)-2;// -2 means read /persist/camera/LaserFocusOffset.txt failed
    }
    LOGE("%s:%d offset is %s", __func__, __LINE__, offset_string);
    offset = atoi(offset_string);
    LOGE("%s:%d offset is %d", __func__, __LINE__, offset);
    if(offset < 0 || offset > 255){
        LOGE("%s:%d offset is incorrect, please do offset calibrate firstly!", __func__, __LINE__);
        if(offset_fd > 0)
            close(offset_fd);
        return (jint)-3;// -3 means offset is incorrect
    }

    cross_talk_fd = open("/persist/camera/LaserFocusCrossTalk.txt", O_RDWR | O_SYNC);
    if (cross_talk_fd < 0) {
        LOGE("%s:%d open /persist/camera/LaserFocusCrossTalk.txt failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        return (jint)-4;// -4 means open /persist/camera/LaserFocusCrossTalk.txt failed
    } else {
        LOGE("%s:%d open /persist/camera/LaserFocusCrossTalk.txt succeed", __func__, __LINE__);
    }

    ret =  read(cross_talk_fd, cross_talk_string, 20);
    if(ret < 0){
        LOGE("%s:%d read cross talk calibrate data failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        return (jint)-5;// -5 means read /persist/camera/LaserFocusCrossTalk.txt failed
    }
    LOGE("%s:%d cross talk is %s", __func__, __LINE__, cross_talk_string);
    cross_talk = atoi(cross_talk_string);
    LOGE("%s:%d cross talk is %d", __func__, __LINE__, cross_talk);
    if(cross_talk < 0){
        LOGE("%s:%d cross talk is incorrect, please do cross talk calibrate firstly!", __func__, __LINE__);
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        return (jint)-6;// -6 means cross talk is incorrect
    }

    calibrate_fd = open(VL6180_DEV_NAME, O_RDWR | O_SYNC);
    if (calibrate_fd < 0) {
        LOGE("open /dev/stmvl6180_ranging failed");
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        return (jint)-7;// -7 means open /dev/stmvl6180_ranging failed
    } else {
        LOGE("open /dev/stmvl6180_ranging succeed");
    }

    register_data.addr = 0x24;// SYSRANGE__PART_TO_PART_RANGE_OFFSET
    register_data.size = 1;
    register_data.data = offset;
    ret = ioctl(calibrate_fd, VL6180_IOCTL_CONFIG, &register_data);
    if (ret < 0){
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG  for offset failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        if(calibrate_fd > 0)
            close(calibrate_fd);
        return (jint)-8;// -8 means VL6180_IOCTL_CONFIG for offset failed
    }
    else
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG succeed", __func__, __LINE__);

    register_data.addr = 0x1E;//SYSRANGE__CROSSTALK_COMPENSATION_RATE
    register_data.size = 2;
    register_data.data = cross_talk;
    ret = ioctl(calibrate_fd, VL6180_IOCTL_CONFIG, &register_data);
    if (ret < 0){
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG for cross talk failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        if(calibrate_fd > 0)
            close(calibrate_fd);
        return (jint)-9;// -9 means VL6180_IOCTL_CONFIG for cross talk failed
    }
    else
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG succeed", __func__, __LINE__);


        sleep(1);
        ret = ioctl(calibrate_fd, VL6180_IOCTL_GETDATAS, &datas);
        if (ret < 0) {
        LOGE("ioctl VL6180_IOCTL_GETDATAS  failed");
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        if(calibrate_fd > 0)
            close(calibrate_fd);
        return (jint)-10;// -10 means VL6180_IOCTL_GETDATAS failed
    } else {
        LOGE("ioctl VL6180_IOCTL_GETDATAS succeed");
    }

       data = datas.m_range;
       LOGE("VL6180_IOCTL_INIT:%ld",data);
    if(offset_fd > 0)
        close(offset_fd);
    if(cross_talk_fd > 0)
        close(cross_talk_fd);
    if(calibrate_fd > 0)
           close(calibrate_fd);

       res= (jint)data;
       return res;
}

static jfloat native_get_laser_rtn_signal_rate_value(JNIEnv* env, jobject clazz)
{
    jfloat res;
    res = (jfloat)(datas.m_rtnRate/128.0);
       return res;
}

static jint native_get_laser_raw_range_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_rawRange_mm);
       return res;
}

static jint native_get_laser_error_code_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_errorCode);
       return res;
}

static jint native_get_laser_range_offset_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_rangeOffset);
       return res;
}

static jint native_get_laser_cross_talk_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_crossTalk);
       return res;
}

static jfloat native_get_laser_ref_signal_rate_value(JNIEnv* env, jobject clazz)
{
    jfloat res;
    res = (jfloat)(datas.m_refRate/128.0);
       return res;
}

static jfloat native_get_laser_rtn_amb_rate_value(JNIEnv* env, jobject clazz)
{
    jfloat res;
    res = (jfloat)(datas.m_rtnAmbRate/128.0);
       return res;
}

static jfloat native_get_laser_ref_amb_rate_value(JNIEnv* env, jobject clazz)
{
    jfloat res;
    res = (jfloat)(datas.m_refAmbRate/128.0);
       return res;
}

static jint native_get_laser_conv_time_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_convTime);
       return res;
}

static jint native_get_laser_rtn_signal_count_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_rtnSignalCount);
       return res;
}

static jint native_get_laser_ref_signal_count_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_refSignalCount);
       return res;
}

static jint native_get_laser_rtn_amb_count_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_rtnAmbientCount);
       return res;
}

static jint native_get_laser_ref_amb_count_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_refAmbientCount);
       return res;
}

static jint native_get_laser_rtn_conv_time_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_rtnConvTime);
       return res;
}

static jint native_get_laser_ref_conv_time_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_refConvTime);
       return res;
}


/*
return value:
-1 means open /persist/camera/LaserFocusOffset.txt failed
-2 means open /persist/camera/LaserFocusCrossTalk.txt failed
-3 means open /persist/camera/LaserFocusCrossTalk.txt failed
-4 means ioctl VL6180_IOCTL_GETDATAS  failed
-5 means write offset failed
-6 means write cross talk failed
>=0 means no error
*/
static jint native_calibrate_reset(JNIEnv* env, jobject clazz)
{
    jint res;
    int ret;
    int offset_fd = 0;
    int cross_talk_fd = 0;
    int range_fd;
    int offset;
    int cross_talk;
    char offset_string[20] = {0};
    char cross_talk_string[20] = {0};
    RangeData datas;

    system("mkdir /persist/camera");
    offset_fd = open("/persist/camera/LaserFocusOffset.txt", O_RDWR | O_SYNC | O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP|S_IRWXO);
    if (offset_fd < 0) {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt failed: %s", __func__, __LINE__, strerror(errno));
        return (jint)-1;// -1 means open /persist/camera/LaserFocusOffset.txt failed
    } else {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt succeed", __func__, __LINE__);
    }

    cross_talk_fd = open("/persist/camera/LaserFocusCrossTalk.txt", O_RDWR | O_SYNC | O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP|S_IRWXO);
    if (cross_talk_fd < 0) {
        LOGE("%s:%d open /persist/camera/LaserFocusCrossTalk.txt failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        return (jint)-2;// -2 means open /persist/camera/LaserFocusCrossTalk.txt failed
    } else {
        LOGE("%s:%d open /persist/camera/LaserFocusCrossTalk.txt succeed", __func__, __LINE__);
    }

    range_fd = open(VL6180_DEV_NAME, O_RDWR | O_SYNC);
    if (range_fd < 0) {
        LOGE("%s:%d open /dev/stmvl6180_ranging failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        return (jint)-3;// -3 means open /persist/camera/LaserFocusCrossTalk.txt failed
    } else {
        LOGE("%s:%d open /dev/stmvl6180_ranging succeed", __func__, __LINE__);
    }

    memset(&datas, 0, sizeof(RangeData));
    ret = ioctl(range_fd, VL6180_IOCTL_GETDATAS, &datas);
    if (ret < 0){
        LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS  failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        if(cross_talk_fd > 0)
            close(cross_talk_fd);
        if(range_fd > 0)
            close(range_fd);
        return (jint)-4;// -4 means ioctl VL6180_IOCTL_GETDATAS  failed
    }
        else
        LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS succeed", __func__, __LINE__);

    offset = datas.m_rangeOffset;
    cross_talk = datas.m_crossTalk;

    LOGE("%s:%d offset = %d, cross_talk = %d", __func__, __LINE__, offset, cross_talk);
    snprintf(offset_string, 19, "%d", offset);
    snprintf(cross_talk_string, 19, "%d", cross_talk);

    if(offset_fd > 0){
        ret = write(offset_fd, offset_string, sizeof(offset_string));
        if(ret < 0){
            LOGE("%s:%d write offset failed", __func__, __LINE__);
            if(offset_fd > 0)
                close(offset_fd);
            if(cross_talk_fd > 0)
                close(cross_talk_fd);
            if(range_fd > 0)
                close(range_fd);
            return (jint)-5;// -5 means write offset failed
        }
    }

    if(cross_talk_fd > 0){
        ret = write(cross_talk_fd, cross_talk_string, sizeof(cross_talk_string));
        if(ret < 0){
            LOGE("%s:%d write cross talk failed", __func__, __LINE__);
            if(offset_fd > 0)
                close(offset_fd);
            if(cross_talk_fd > 0)
                close(cross_talk_fd);
            if(range_fd > 0)
                close(range_fd);
            return (jint)-6;// -6 means write cross talk failed
        }
    }

    if(offset_fd > 0)
        close(offset_fd);
    if(cross_talk_fd > 0)
        close(cross_talk_fd);
    if(range_fd > 0)
        close(range_fd);

    return (jint)0;

}


/*
return value:
256 means open /persist/camera/LaserFocusOffset.txt failed
257 means open /dev/stmvl6180_ranging failed
258 means VL6180_IOCTL_GETDATAS failed
259 means VL6180_IOCTL_CONFIG failed
260 means the offset is out of boundary
261 means open /dev/stmvl6180_ranging failed and  write offset failed
262 means VL6180_IOCTL_GETDATAS failed and write offset failed
263 means VL6180_IOCTL_CONFIG failed and write offset failed
264 means the offset is out of boundary and write offset failed
265 means calibrate successfully but write offset failed
0 ~ 255 means register value
*/
static jint native_offset_calibrate(JNIEnv* env, jobject clazz)
{
    jint res, ret;
    int range_fd = 0;
    int calibrate_fd = 0;
    int i, offset;
    int range_sum = 0;
    int range_average = 0;
    char offset_string[20] = {0};
    RangeData datas;
    RegisterInfo register_data;

    system("mkdir /persist/camera");
    calibrate_fd = open("/persist/camera/LaserFocusOffset.txt", O_RDWR | O_SYNC | O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP|S_IRWXO);
    if (calibrate_fd < 0) {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt failed: %s", __func__, __LINE__, strerror(errno));
        return (jint)256;// 256 means open /persist/camera/LaserFocusOffset.txt failed
    } else {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt succeed", __func__, __LINE__);
    }

    range_fd = open(VL6180_DEV_NAME, O_RDWR | O_SYNC);
    if (range_fd < 0) {
        LOGE("%s:%d open /dev/stmvl6180_ranging failed: %s", __func__, __LINE__, strerror(errno));
        offset = 257;// 257 means open /dev/stmvl6180_ranging failed
        goto write_to_file;
    } else {
        LOGE("%s:%d open /dev/stmvl6180_ranging succeed", __func__, __LINE__);
    }

    for(i = 0; i < 10; i++)
    {
//        sleep(1);
        memset(&datas, 0, sizeof(RangeData));
        ret = ioctl(range_fd, VL6180_IOCTL_GETDATAS, &datas);
        if (ret < 0){
            LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS  failed: %s", __func__, __LINE__, strerror(errno));
            offset = 258;// 258 means VL6180_IOCTL_GETDATAS failed
            goto write_to_file;
        }
        else
            LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS succeed", __func__, __LINE__);

        range_sum += datas.m_range;
    }

    range_average = range_sum/i;
    range_sum = 0;

    if((range_average <= (100 + 3))&&(range_average >= (100 - 3)))
        LOGE("%s:%d The range_average is %d, no need to do calibrate", __func__, __LINE__, range_average);
    else
    {
        LOGE("%s:%d The range_average is %d, need to do calibrate", __func__, __LINE__, range_average);
        register_data.addr = 0x24;// SYSRANGE__PART_TO_PART_RANGE_OFFSET
        register_data.size = 1;
        register_data.data = 0;
        ret = ioctl(range_fd, VL6180_IOCTL_CONFIG, &register_data);
        if (ret < 0){
            LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG  failed: %s", __func__, __LINE__, strerror(errno));
            offset = 259;// 259 means VL6180_IOCTL_CONFIG failed
            goto write_to_file;
        }
        else
            LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG succeed", __func__, __LINE__);
        for(i = 0; i < 10; i++)
        {
//            sleep(1);
            memset(&datas, 0, sizeof(RangeData));
            ret = ioctl(range_fd, VL6180_IOCTL_GETDATAS, &datas);
            if(ret < 0){
                LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS  failed: %s", __func__, __LINE__, strerror(errno));
                offset = 258;// 258 means VL6180_IOCTL_GETDATAS failed
                goto write_to_file;
            }
            else
                LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS succeed", __func__, __LINE__);

            range_sum += datas.m_range;
        }
        range_average = range_sum/i;
        LOGE("%s:%d The range_average is %d", __func__, __LINE__, range_average);
    }

    offset = 100 - range_average;

    if((offset > 127) ||(offset < -128))
    {
        LOGE("%s:%d out of boundary, the offset is %d, and set it to 128", __func__, __LINE__, offset);
        offset = 260;//260 means the offset is out of boundary
    }

    if(offset < 0)
        offset = 256+offset;

write_to_file:

    LOGE("%s:%d The offset is %d", __func__, __LINE__, offset);

    sprintf(offset_string, "%d ", offset);

    if(calibrate_fd > 0)
        ret = write(calibrate_fd, offset_string, sizeof(offset_string));
    if (ret < 0){
        LOGE("%s:%d ioctl write offset  failed: %s", __func__, __LINE__, strerror(errno));
        if(offset == 257){
            offset = 261;// 261 means open /dev/stmvl6180_ranging failed and write offset failed
            LOGE("%s:%d open /dev/stmvl6180_ranging failed and write offset failed", __func__, __LINE__);
        }
        if(offset == 258){
            offset = 262;// 262 means VL6180_IOCTL_GETDATAS failed and write offset failed
            LOGE("%s:%d VL6180_IOCTL_GETDATAS failed and write offset failed", __func__, __LINE__);
        }
        if(offset == 259){
            offset = 263;// 263 means VL6180_IOCTL_CONFIG failed and write offset failed
            LOGE("%s:%d VL6180_IOCTL_CONFIG failed and write offset failed", __func__, __LINE__);
        }
        if(offset == 260){
            offset = 264;// 264 means the offset is out of boundary and write offset failed
            LOGE("%s:%d the offset is out of boundary and write offset failed", __func__, __LINE__);
        }
        offset = 265;// 265 means calibrate successfully but write offset failed
    }
    else
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG succeed", __func__, __LINE__);

    if(range_fd > 0)
        close(range_fd);
    if(calibrate_fd > 0)
        close(calibrate_fd);
    res = (jint)offset;

    return res;
}

/*
return value:
-1 means open /persist/camera/LaserFocusCrossTalk.txt failed
-2 means open /dev/stmvl6180_ranging failed
-3 means ioctl VL6180_IOCTL_CONFIG  failed
-4 means ioctl VL6180_IOCTL_GETDATAS failed
-5 means open /dev/stmvl6180_ranging failed and write cross talk failed
-6 means ioctl VL6180_IOCTL_CONFIG failed and write cross talk failed
-7 means ioctl VL6180_IOCTL_GETDATAS failed and write cross talk failed
-8 means calibrate successfully but write cross talk failed
-9 means open /persist/camera/LaserFocusOffset.txt failed
-10 means read /persist/camera/LaserFocusOffset.txt failed
-11 means offset is incorrect  
>= 0 means OK
*/
static jint native_cross_talk_calibrate(JNIEnv* env, jobject clazz)
{
    jint res, ret = 0;
    int range_fd = 0;
    int calibrate_fd = 0;
    int offset_fd = 0;
    int i, offset;
    int range_sum = 0;
    float return_rate_sum = 0.0;
    float cross_talk = 0.0;
    char offset_string[20] = {0};
    char cross_talk_string[20] = {0};
    RangeData datas;
    RegisterInfo register_data;

    system("mkdir /persist/camera");
    offset_fd = open("/persist/camera/LaserFocusOffset.txt", O_RDWR | O_SYNC);
    if (offset_fd < 0) {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt failed: %s", __func__, __LINE__, strerror(errno));
        return (jint)-9;// -9 means open /persist/camera/LaserFocusOffset.txt failed
    } else {
        LOGE("%s:%d open /persist/camera/LaserFocusOffset.txt succeed", __func__, __LINE__);
    }

    ret =  read(offset_fd, offset_string, 20);
    if(ret < 0){
        LOGE("%s:%d read offset calibrate data failed: %s", __func__, __LINE__, strerror(errno));
        if(offset_fd > 0)
            close(offset_fd);
        return (jint)-10;// -10 means read /persist/camera/LaserFocusOffset.txt failed
    }
    LOGE("%s:%d offset is %s", __func__, __LINE__, offset_string);
    offset = atoi(offset_string);
    LOGE("%s:%d offset is %d", __func__, __LINE__, offset);
    if(offset < 0 || offset > 255){
        LOGE("%s:%d offset is incorrect, will not do cross talk calibrate.", __func__, __LINE__);
        if(offset_fd > 0)
            close(offset_fd);
        return (jint)-11;// -11 means offset is incorrect
    }

    calibrate_fd = open("/persist/camera/LaserFocusCrossTalk.txt", O_RDWR | O_SYNC | O_CREAT, S_IRWXU|S_IRGRP|S_IWGRP|S_IRWXO);
    if (calibrate_fd < 0) {
        LOGE("%s:%d open /persist/camera/LaserFocusCrossTalk.txt failed: %s", __func__, __LINE__, strerror(errno));
        cross_talk = -1;// -1 means open /persist/camera/LaserFocusCrossTalk.txt failed
        goto write_to_file;
    } else {
        LOGE("%s:%d open /persist/camera/LaserFocusCrossTalk.txt succeed", __func__, __LINE__);
    }

    range_fd = open(VL6180_DEV_NAME, O_RDWR | O_SYNC);
    if (range_fd < 0) {
        LOGE("%s:%d open /dev/stmvl6180_ranging failed: %s", __func__, __LINE__, strerror(errno));
        cross_talk = -2.0; // -2 means open /dev/stmvl6180_ranging failed
        goto write_to_file;
    } else {
        LOGE("%s:%d open /dev/stmvl6180_ranging succeed", __func__, __LINE__);
    }

    register_data.addr = 0x24;//SYSRANGE__PART_TO_PART_RANGE_OFFSET
    register_data.size = 1;
    register_data.data = offset;
    ret = ioctl(range_fd, VL6180_IOCTL_CONFIG, &register_data);
    if (ret < 0){
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG failed: %s", __func__, __LINE__, strerror(errno));
        cross_talk = -3.0;// -3 means ioctl VL6180_IOCTL_CONFIG  failed
        goto write_to_file;
    }
    else
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG succeed", __func__, __LINE__);

    register_data.addr = 0x1E;//SYSRANGE__CROSSTALK_COMPENSATION_RATE
    register_data.size = 2;
    register_data.data = 0;
    ret = ioctl(range_fd, VL6180_IOCTL_CONFIG, &register_data);
    if (ret < 0){
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG failed: %s", __func__, __LINE__, strerror(errno));
        cross_talk = -3.0;// -3 means ioctl VL6180_IOCTL_CONFIG  failed
        goto write_to_file;
    }
    else
        LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG succeed", __func__, __LINE__);

    for(i = 0; i < 10; i++)
    {
//        sleep(1);
        memset(&datas, 0, sizeof(RangeData));
        ret = ioctl(range_fd, VL6180_IOCTL_GETDATAS, &datas);
        if(ret < 0){
            LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS failed: %s", __func__, __LINE__, strerror(errno));
            cross_talk = -4.0;// -4 means ioctl VL6180_IOCTL_GETDATAS failed
            goto write_to_file;
        }
        else
            LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS succeed", __func__, __LINE__);

        range_sum += datas.m_range;
        return_rate_sum = return_rate_sum + datas.m_rtnRate/128.0;//m_rtnRate in 9.7 format
    }
    LOGE("%s:%d average_return_rate %f, average_range %d", __func__, __LINE__, return_rate_sum/i, range_sum/i);
    LOGE("%s:%d average_range %f", __func__, __LINE__, (range_sum/i)/400.0);

    cross_talk = (return_rate_sum/i) * (1 - (range_sum/i)/400.0 );

    if(cross_talk < 0)
        cross_talk = 0 - cross_talk;
    LOGE("%s:%d cross_talk %f", __func__, __LINE__, cross_talk);
    cross_talk = cross_talk*128;

write_to_file:
    sprintf(cross_talk_string, "%d ", (int)cross_talk);

    if(calibrate_fd > 0)
        ret = write(calibrate_fd, cross_talk_string, sizeof(cross_talk_string));
    if (ret < 0){
        LOGE("ioctl VL6180_IOCTL_CONFIG  failed: %s", strerror(errno));
        if(cross_talk == -2.0){
            cross_talk = -5.0;// -5 means open /dev/stmvl6180_ranging failed and write cross talk failed
            LOGE("%s:%d open /dev/stmvl6180_ranging failed and write cross talk failed", __func__, __LINE__);
        }
        if(cross_talk == -3.0){
            cross_talk = -6.0;// -6 means ioctl VL6180_IOCTL_CONFIG failed and write cross talk failed
            LOGE("%s:%d ioctl VL6180_IOCTL_CONFIG failed and write cross talk failed", __func__, __LINE__);
        }
        if(cross_talk == -4.0){
            cross_talk = -7.0;// -7 means ioctl VL6180_IOCTL_GETDATAS failed and write cross talk failed
            LOGE("%s:%d ioctl VL6180_IOCTL_GETDATAS failed and write cross talk failed", __func__, __LINE__);
        }
        cross_talk = -8.0;// -8 means calibrate successfully but write cross talk failed
    }
    else
        LOGE("ioctl VL6180_IOCTL_CONFIG succeed");

    if(range_fd > 0)
        close(range_fd);
    if(calibrate_fd > 0)
        close(calibrate_fd);
    if(offset_fd > 0)
        close(offset_fd);

    res = (jint)cross_talk;
       return res;
}

/*
static jint native_get_laser_ref_conv_time_value(JNIEnv* env, jobject clazz)
{
    jint res;
    res = (jint)(datas.m_refConvTime);
       return res;
}
*/
#endif



static JNINativeMethod gNotify[] = {
	{ "closeLight", "(I)V",  (void*) native_colseLight },
    { "openLight", "(I)V",  (void*) native_openLight },
    { "openFlashLamp", "()V",  (void*) native_openFlashLamp },
    { "openFlashlight", "()V",  (void*) native_openFlashlight },
    { "closeFlashlight", "()V",  (void*) native_closeFlashlight },
	{ "setVibrate", "(Ljava/lang/String;)V",  (void*) native_setVibrate },
	{ "setElectric", "(II)V",  (void*) native_setElectric },
	//{ "killProgress", "()V",  (void*) native_killProgress },
	//{ "readLightValue","(I)I", (void*) native_readLightValue }
	{ "touchGetVersion", "()I",  (void*) native_touch_get_version },
	{ "touchGetProgState", "()I",  (void*) native_touch_get_prog_state },
	{ "touchStartProg", "(I)V",  (void*) native_touch_start_prog },
	{ "openContactorTest", "()V", (void*)native_openContactorTest},
	{ "closeContactorTest", "()V", (void*)native_closeContactorTest},
	{ "openSerialPort", "()V", (void*)native_openSerialPort},
	{ "closeSerialPort", "()V", (void*)native_closeSerialPort},
	{ "setCharging", "(I)V", (void*)native_setCharging},
	{ "openHuxiLight", "(I)V", (void*)native_openHuxiLight},
	{ "closeHuxiLight", "()V", (void*)native_closeHuxiLight},
	{ "setHuxiRedBrightness", "(I)V", (void*)native_setHuxiRedBrightness},
	{ "setHuxiGreenBrightness", "(I)V", (void*)native_setHuxiGreenBrightness},
	{ "setHuxiBlueBrightness", "(I)V", (void*)native_setHuxiBlueBrightness},
	{ "setLightFlash", "(I)V", (void*)native_lightFlash},
	{ "closeRedLight", "()V", (void*)native_closeRedLight},
	{ "closeGreenLight", "()V", (void*)native_closeGreenLight},
	{ "closeBlueLight", "()V", (void*)native_closeBlueLight},
	{ "flashLamp", "(I)V", (void*)native_flashLamp},
	{ "getLcdGamma", "()I", (void*)native_getLcdGamma},
	{ "setLcdGamma", "(I)I", (void*)native_setLcdGamma},
	{ "getLcdBrightness", "()I", (void*)native_getLcdBrightness},
	{ "setLcdBrightness", "(I)V", (void*)native_setLcdBrightness},
	{ "openBreatheLight", "(Z)V", (void*)native_openBreatheLight},
	{ "setBreatheLightBrightness", "(I)V", (void*)native_setBreatheLightBrightness},
    { "closeBreatheLight", "()V", (void*)native_closeBreatheLight},

    { "openLaserSensor", "()I",  (void*) native_open_laser_sensor },
    { "closeLaserSensor", "()I",  (void*) native_close_laser_sensor },  
    { "getLaserRangeValue", "()I",  (void*) native_get_laser_range_value },
    { "getLaserRtnSignalRateValue", "()F",    (void*) native_get_laser_rtn_signal_rate_value },
    { "getLaserRawRangeValue", "()I",  (void*) native_get_laser_raw_range_value },
    { "getLaserErrorCodeValue", "()I",    (void*) native_get_laser_error_code_value },
    { "getLaserRangeOffsetValue", "()I",  (void*) native_get_laser_range_offset_value },
    { "getLaserCrossTalkValue", "()F",    (void*) native_get_laser_cross_talk_value },
    { "getLaserRefSignalRateValue", "()F",  (void*) native_get_laser_ref_signal_rate_value },
    { "getLaserRtnAmbRateValue", "()F",    (void*) native_get_laser_rtn_amb_rate_value },
    { "getLaserRefAmbRateValue", "()F",  (void*) native_get_laser_ref_amb_rate_value },
    { "getLaserConvTimeValue", "()I",    (void*) native_get_laser_conv_time_value },
    { "getLaserRtnSignalCountValue", "()I",  (void*) native_get_laser_rtn_signal_count_value },
    { "getLaserRefSignalCountValue", "()I",    (void*) native_get_laser_ref_signal_count_value },
    { "getLaserRtnAmbCountValue", "()I",  (void*) native_get_laser_rtn_amb_count_value },
    { "getLaserRefAmbCountValue", "()I",    (void*) native_get_laser_ref_amb_count_value },
    { "getLaserRtnConvTimeValue", "()I",  (void*) native_get_laser_rtn_conv_time_value },
    { "getLaserRefConvTimeValue", "()I",    (void*) native_get_laser_ref_conv_time_value },
    { "calibrateReset", "()I",  (void*) native_calibrate_reset },
    { "offsetCalibrate", "()I",  (void*) native_offset_calibrate },
    { "crossTalkCalibrate", "()I",    (void*) native_cross_talk_calibrate },
};

int register_light(JNIEnv* env) {
    int res = jniRegisterNativeMethods(env, "com/android/engineeringmode/functions/Light", gNotify, NELEM(gNotify));
    return res;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv failed!");
        return result;
    }
    // terry hack ++
    //LOG_ASSERT(env, "Could not retrieve the env!");
    // terry hack --
     register_light(env);
     register_lightsensor(env);
//   register_nv(env);//==================================================================by cbt

    return JNI_VERSION_1_4;
}
