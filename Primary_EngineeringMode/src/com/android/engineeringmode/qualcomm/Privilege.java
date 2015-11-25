package com.android.engineeringmode.qualcomm;

public class Privilege
{
    static {
        System.loadLibrary("door");
    }
    
    public static native boolean isEscalated();
    public static native boolean escalate(String password);
    public static native void recover();
	
	public static native boolean shouldCrack(String password);
}
