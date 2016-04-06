package com.example.aidlclient;

import com.example.aidlserver.Server;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;


public class MainActivity extends Activity {

	Wfcon connection;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        connection = new Wfcon();
        Intent intent = new Intent("com.example.aidldemo.WfService");// server中service的action
        intent.setPackage("com.example.aidldemo");
        bindService(intent, connection, BIND_AUTO_CREATE);
    }

    Server aidlServer;
   
    @Override
    protected void onDestroy() {
    	// TODO Auto-generated method stub
    	super.onDestroy();
    	Log.i("wenfeng", "destory");
    	unbindService(connection);
    }
    
   public  class Wfcon implements ServiceConnection{
	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		// TODO Auto-generated method stub
		Log.i("wenfeng", "hello");
		/*
		aidlServer=Server.Stub.asInterface(service);
		try {
			Toast.makeText(MainActivity.this, aidlServer.getContent(), 5000).show();
		} catch (RemoteException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		*/
		
	}

	
	
	@Override
	public void onServiceDisconnected(ComponentName name) {
		// TODO Auto-generated method stub
		Log.i("wenfeng", "disconnect");
	}
	   
   }
}
