package com.example.tabhost_demo;

import android.app.Activity;
import android.app.TabActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;



/*
 * tabhost_demo
 * 1.activity继承tabactivity
 * 2。gettabhost获取tabhost
 * 3.新建tab，必须设置indicator
 * 4，设置tab点击后显示的内容tab。setcontent()
 * 5.将tab添加到tabhost
 *
 * 布局文件
 * tabhost,tabwidget，fraglayout三者是必须的，而且id必须是android自带id，即android:id/**
 
 */





public class MainActivity extends TabActivity{

	TabHost mtabhost;	
	TabHost.TabSpec wechatSpec;
	TabHost.TabSpec contactSpec;
	TabHost.TabSpec findSpec;
	TabHost.TabSpec meSpec;
	Mefragment mefragment;
	LayoutInflater lfInflater;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        //从TabActivity上面获取放置Tab的TabHost
        mtabhost = getTabHost();
        mefragment=new Mefragment();
        
        lfInflater=getLayoutInflater();
        View mView=lfInflater.inflate(R.layout.mefragment, null);
        
        wechatSpec=mtabhost.newTabSpec("微信").setIndicator("微信");
        contactSpec=mtabhost.newTabSpec("联系人").setIndicator("联系人");
        findSpec=mtabhost.newTabSpec("发现").setIndicator("发现");
        meSpec=mtabhost.newTabSpec("我").setIndicator("我");
      //  meSpec=mtabhost.newTabSpec("我").setIndicator(mView);
        
        
        
        wechatSpec.setContent(R.id.Li1);                       //设置tab点击后显示的内容
        contactSpec.setContent(R.id.Li2);
        findSpec.setContent(R.id.Li1);
        meSpec.setContent(R.id.Li2);
        
        mtabhost.addTab(wechatSpec);
        mtabhost.addTab(contactSpec);
        mtabhost.addTab(findSpec);
        
        
        

       
		

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
