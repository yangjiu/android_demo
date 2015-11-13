package com.example.contentobserver_demo;

import android.app.Activity;
import android.database.ContentObserver;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

/*
 * contentobserver的使用
 * 用途:观察数据库的改变
 * 
 * 用法:
 * 1.创建一个观察者类继承与ContentObserver
 * 2.在观察者类的onChange方法编写观察到数据库改变要进行的操作
 * 
 * 至此，观察者类编写完毕，接下来要注册这个观察者
 * getContentResolver().registerContentObserver(Uri, false, contentObserver);
 * (1)Uri为数据库的uri
 * (2)contentObserver为上面编写的观察者类
 * 
 * 
 */

public class MainActivity extends Activity {

	AirmodeObserver mAirmodeObserver;
	Uri airmodeUri;
	SMSContentObserver mSmsContentObserver;
	Uri smsUri;
	Handler handler=new Handler(new Handler.Callback() {
		
		@Override
		public boolean handleMessage(Message msg) {
			// TODO Auto-generated method stub
			return false;
		}
	});
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
       
     
        
        mAirmodeObserver=new AirmodeObserver(handler);
        airmodeUri =Settings.System.getUriFor(Settings.System.AIRPLANE_MODE_ON);         
       regist_demoobserver(mAirmodeObserver, airmodeUri);
       
       
       //信息的uri应该已经不是这个，这个看不出效果
       mSmsContentObserver=new SMSContentObserver(MainActivity.this, handler);
       smsUri=Uri.parse("content://sms");  
       regist_demoobserver(mSmsContentObserver, smsUri);
        
        
        
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
    
    public void regist_demoobserver(ContentObserver contentObserver,Uri airmodeUri)
    {
    	
    	
    	getContentResolver().registerContentObserver(airmodeUri, false, contentObserver);
    	
    }
    
    
    
    
    
    
    class AirmodeObserver extends ContentObserver
    {

    	Handler mHandler;
    	
		public AirmodeObserver(Handler handler) {
			super(handler);
			// TODO Auto-generated constructor stub
			mHandler=handler;
		}
		
		@Override
		public void onChange(boolean selfChange) {
			// TODO Auto-generated method stub
			super.onChange(selfChange);
			Log.i("wenfeng", "airmode changed");
		}
		
		
    	
    }
    
    
    
}
