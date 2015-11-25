package com.android.engineeringmode.functions;
public class Light {

	public final static int FRONT_KEY_LIGHT = 0;
	public final static int MAIN_KEY_LIGHT = 1;
	public final static int CHARGE_RED_LIGHT = 2;
	public final static int CHARGE_GREEN_LIGHT = 3;
	public final static int BREATH_LAMP_FRONT = 4; //Baoti.Li Add for 13095 breath lamp
	public final static int BREATH_LAMP_BACK = 5; //Baoti.Li Add for 13095 breath lamp

	public final static int MAIN_KEY_MAX = 255;
	public final static int MAIN_KEY_MIN = 0;
	public final static int MAIN_KEY_NORMAL = 6;

	static {
		System.loadLibrary("light_jni");
	}

	private Light() {

	}

	/**
	 * open the light on the phone
	 * 
	 * @param type
	 *            : the *_LIGHT;
	 * 
	 * */
	public static native void openLight(int type);

	/**
	 * @see openLight
	 * 
	 * */
	public static native void closeLight(int type);

	/**
	 * open or close(mA = 0) the light
	 * 
	 * @param type
	 *            : the *_LIGHT;
	 * @param mA
	 *            : electric
	 * */
	// public native void controlLight(int type ,int mA);
	/**
	 * read the electric. make sure that the type >=0 && type <= 3
	 * */
	// public native int readLightValue(int type);

	/**
	 * for the BackCameraTest to let the flash lamp work
	 * **/
	public static native void openFlashLamp();
	
	/**
	 * for open flashlight.
	 */
	public static native void openFlashlight();
	
	/**
	 * for close flashlight.
	 */
	public static native void closeFlashlight();
	
	//wangw@OnLineRD.DeviceService,2013/08/29,Add methods to do flash lamp test,we used new interface for future Compatible
	//type:0:close;1:open;2:blink
	public static native void flashLamp(int type);
	
	/** for the main key and tp key electric setting */
	public static native void setElectric(int type, int value);

	/**
	*
	*param version target version
	*/
	public static native void touchStartProg(int version);
	
	/**
	*version 0, 1, 2
	*/
	public static native int touchGetVersion();
	
	/**
	*
	*SYNA_PROGRAM_STATE_STARTED  1   
	*SYNA_PROGRAM_STATE_WRITING  2   
	*SYNA_PROGRAM_STATE_SUCCEED  3   
	*SYNA_PROGRAM_STATE_FAILED   4	
	*/
	public static native int touchGetProgState();
	/** for qualcommActivity */
	// public native void killProgress();
	
	//set vibrate value
	public static native void setVibrate(String value);
	
	public static native void openContactorTest();
	public static native void closeContactorTest();
	
	public static native void openSerialPort();
	public static native void closeSerialPort();
	
	public static native void setCharging(int value);
	
	//add by cbt for huxideng.2012/11/05
	public static native void openHuxiLight(int value);
	public static native void closeHuxiLight();
	public static native void setHuxiRedBrightness(int val);
	public static native void setHuxiGreenBrightness(int val);
	public static native void setHuxiBlueBrightness(int val);
	public static native void setLightFlash(int val);
	//wangw@OnLineRD.DeviceService, 2013/08/14,Add for used to turn off each light
	public static native void closeRedLight();
	public static native void closeGreenLight();
	public static native void closeBlueLight();
	
	//add for Find7s
    public static native void openBreatheLight(boolean blink);
	public static native void setBreatheLightBrightness(int val);
	public static native void closeBreatheLight();
	
	public static native int getLcdGamma();
	public static native int setLcdGamma(int value);
	public static native int getLcdBrightness();
	public static native void setLcdBrightness(int value);

    public static native int openLaserSensor();

    public static native int closeLaserSensor();	

    public static native int getLaserRangeValue();

    public static native float getLaserRtnSignalRateValue();

    public static native int getLaserRawRangeValue();

    public static native int getLaserErrorCodeValue();
    public static native int getLaserRangeOffsetValue();

    public static native float getLaserCrossTalkValue();

    public static native float getLaserRefSignalRateValue();

    public static native float getLaserRtnAmbRateValue();
    public static native int getLaserConvTimeValue();

    public static native float getLaserRefAmbRateValue();

    public static native int getLaserRtnSignalCountValue();

    public static native int getLaserRefSignalCountValue();
    public static native int getLaserRtnAmbCountValue();

    public static native int getLaserRefAmbCountValue();

    public static native int getLaserRtnConvTimeValue();

    public static native int getLaserRefConvTimeValue();

    public static native int calibrateReset();

    public static native int offsetCalibrate();

    public static native int crossTalkCalibrate();

}
