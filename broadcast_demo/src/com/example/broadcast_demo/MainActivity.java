
/* android发送广播的demo*/

/* 步骤:
 * 1.在manifest中注册广播
 * 2.新建一个接收器类继承与BroadcastReceiver
 * 3.在接受器类处理事务
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

package com.example.broadcast_demo;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;


public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //也可以在adb中发送广播 am broadcast com.example.broadcast_demo.testbroadcast
    //    Intent mIntent=new Intent("com.example.broadcast_demo.testbroadcast");
    //    sendBroadcast(mIntent);
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
}
