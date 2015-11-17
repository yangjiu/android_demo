package com.example.sim_demo;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.telephony.PhoneStateListener;
import android.telephony.ServiceState;
import android.telephony.TelephonyManager;
import android.telephony.gsm.GsmCellLocation;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

/*
 * 该例子描述了sim卡的一些基本数据读取，以及实现不同国家语言的方法（建立特定的value文件夹即可实现）
 * 
 * 
 * 
 */


public class MainActivity extends Activity {

	TextView mTextView;
	TelephonyManager telephonyManager;
	ServiceState mServiceState;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        telephonyManager=(TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
       
     
        
       GsmCellLocation cellLocation=(GsmCellLocation) telephonyManager.getCellLocation();
        
     
       telephonyManager.listen(getPhoneStateListener(), PhoneStateListener.LISTEN_SERVICE_STATE
               | PhoneStateListener.LISTEN_SIGNAL_STRENGTHS
               | PhoneStateListener.LISTEN_DATA_CONNECTION_STATE);
       
        mTextView=(TextView)findViewById(R.id.mtextview);
        mTextView.setText("getSubscriberId:"+telephonyManager.getSubscriberId()+"\n"             // IMSI
        		          +"getNetworkOperator:"+telephonyManager.getNetworkOperator()+"\n"                //与运营商相关
        		          +"getSimOperatorName:"+telephonyManager.getSimOperatorName()+"\n"
        		          +"getSimCountryIso:"+telephonyManager.getSimCountryIso()+"\n"
        		          +"getSimSerialNumber:"+telephonyManager.getSimSerialNumber()+"\n"                 //sim卡串号
        		          +"getLine1Number:"+(TextUtils.isEmpty(telephonyManager.getLine1Number()))+"\n"                  //电话号码
        		          +"GsmCellLocation:"+"cid:"+cellLocation.getCid()+" Lac:"+ cellLocation.getLac()+"\n"           //基站信息
        		          +"isNetworkRoaming:"+telephonyManager.isNetworkRoaming()+"\n"                            //漫游
        		
        		
        		);
        
        
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
   public PhoneStateListener getPhoneStateListener()
   {
	   PhoneStateListener psl=new PhoneStateListener()
	   {
		   @Override
		public void onServiceStateChanged(ServiceState serviceState) {
			// TODO Auto-generated method stub
			super.onServiceStateChanged(serviceState);
			Log.i("wenfeng", serviceState.getState()+" ");
		}
	   };
	   
	   return psl;
	   
   }
    
}
