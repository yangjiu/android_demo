package com.android.engineeringmode.manualtest;

import android.content.Context;

public class LightSensorUtils
{
	
	private Context mContext;
	public LightSensorUtils(Context context)
	{
		mContext = context;
	}
	
	public native int  openLightSensor();
	public native void  closeLightSensor();
	/**return the ADC values
	 * a int array has four values
	 * <p>1: ClearData</p>
	 * <p>2: IrData</p>
	 * <p>3: prox</p>
	 * <p>4: lux</p> 
	 * 
	 * **/
	public native int[]   getADC(int fd);
	
	public native int[]   startAdjust();
	//public native int   getPilt();
	//public native int   getPiht();
	
	//for adjust gsensor
	//public native int startGsensorAdjust();
	
	
	//for msensor autotest
	public native String startMsensorAutoTest();
	
	static{
		System.loadLibrary("light_jni");
	}
}
