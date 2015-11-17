package com.example.broadcast_demo;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

public class MyBroadcastReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		Log.v("debug", "accept the broadcast");
		//Toast mToast=new Toast(context);
		//mToast.setText("hahah");
		//mToast.show();
	}

}
