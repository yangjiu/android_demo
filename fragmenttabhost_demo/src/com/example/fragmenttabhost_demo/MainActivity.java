package com.example.fragmenttabhost_demo;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTabHost;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TabHost.TabSpec;

/*
 *fragmenttabhost
 *1.activity继承 FragmentActivity
 *2.编写布局文件包含android.support.v4.app.FragmentTabHost
 *3.获取fragmenttabhost，并且需要setup
 *4.将tab添加到fragmenttabhost中，可以适配fragment，这正是tabhost没有的 
 * 
 * 
 * 
 */

public class MainActivity extends FragmentActivity {

	FragmentTabHost mfragmentTabHost;
	TabSpec wechaTabSpec;
	TabSpec contacTabSpec;
	
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        mfragmentTabHost=(FragmentTabHost)findViewById(android.R.id.tabhost);
        mfragmentTabHost.setup(this, getSupportFragmentManager(), R.id.realtabcontent);
        
        wechaTabSpec=mfragmentTabHost.newTabSpec("微信").setIndicator("微信");
        contacTabSpec=mfragmentTabHost.newTabSpec("联系人").setIndicator("联系人");
        
        mfragmentTabHost.addTab(wechaTabSpec, Wechat_fragment.class, null);
        mfragmentTabHost.addTab(contacTabSpec, Contact_fragment.class, null);
        
        
    
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
