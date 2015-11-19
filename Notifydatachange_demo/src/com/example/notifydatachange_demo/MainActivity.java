package com.example.notifydatachange_demo;


import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;

/*
 * 该例子讲述了listview的更新操作。（不在阐述listview的创建方法了）
 * 1.当数据arr发生改变时，调用Adapter.notifyDataSetChanged()方法就可
 * 
 * 
 * 
 */


public class MainActivity extends Activity {

	ListView lv;
	ArrayAdapter<String> Adapter;
	ArrayList<String> arr=new ArrayList<String>();
	Handler handler;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        lv=(ListView)findViewById(R.id.listView1);
    	arr.add("123");
    	arr.add("234");
    	arr.add("345");
    //	Adapter = new ArrayAdapter<String>(this,R.layout.listitem, arr);
    	Adapter=new ArrayAdapter<String>(MainActivity.this, R.layout.listitem, R.id.textView1, arr);
    	lv.setAdapter(Adapter);
        handler=new Handler();
    	handler.postDelayed(add,1000);//延迟3秒执行
    }
    Runnable add=new Runnable(){
 
		@Override
		public void run() {
			// TODO Auto-generated method stub
			arr.add("增加一项");//增加一项
			Adapter.notifyDataSetChanged();	
			//handler.postDelayed(this,1000);
		}   	
    };

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
