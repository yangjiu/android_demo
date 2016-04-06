package com.example.aidldemo;

import com.example.aidlserver.Server;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;

public class WfService extends Service {

	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return new Server.Stub() {
			
			@Override
			public String modifyContent(String org) throws RemoteException {
				// TODO Auto-generated method stub
				return null;
			}
			
			@Override
			public String getContent() throws RemoteException {
				// TODO Auto-generated method stub
				return "hello";
			}
		};
	}

}
