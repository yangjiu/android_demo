package com.example.service_demo;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.widget.Toast;

public class Mservice extends Service{

	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		Toast.makeText(Mservice.this, "service create!", 500).show();
	}
	
    @Override
    @Deprecated
    public void onStart(Intent intent, int startId) {
    	// TODO Auto-generated method stub
    	Toast.makeText(Mservice.this, "service start!", 500).show();
    }
	
	
}
