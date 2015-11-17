package com.example.preference_demo;


import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.preference.Preference;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


/*
 *主界面
 *有四个button，可以跳转到不同的demo
 *
 * 
 *
 *
 */

public class MainActivity extends Activity {

	private Button preeditButton;
	private Button prebaseButton;
	private Button wcButton;
	private Button hdButton;
	private Intent tzIntent;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tzIntent=new Intent();
        
       
        //preference与preference editor的demo
        preeditButton=(Button)findViewById(R.id.preditor_button);      
        preeditButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				tzIntent.setClass(MainActivity.this,PEReditor_Activity.class);
				startActivity(tzIntent);
				
			}
		});
      
      //基础preference
        prebaseButton=(Button)findViewById(R.id.prebase_button);
        prebaseButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				tzIntent.setClass(MainActivity.this,Basepre.class);
				startActivity(tzIntent);
				
			}
		});
        
        
        //仿微信的preference
        wcButton=(Button)findViewById(R.id.wcsetting_button);
        wcButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				tzIntent.setClass(MainActivity.this,WcsettingActivity.class);
				startActivity(tzIntent);
				
			}
		});
        
        //hander的preference
        hdButton=(Button)findViewById(R.id.hander_button);
        hdButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				tzIntent.setClass(MainActivity.this,HanderActivity.class);
				startActivity(tzIntent);
				
			}
		});
        
        
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
