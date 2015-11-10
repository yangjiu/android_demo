package com.example.preference_demo;

import java.util.List;

import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.preference.PreferenceFragment;
import android.view.Menu;
import android.view.MenuItem;


/*
 * prefence中hander的使用
 * 
 * 1.调用onBuildHeaders函数，加载hander
 * 2.设置fragment类继承PreferenceFragment
 * 3.fragment类中使用addPreferencesFromResource函数加载相应的preference布局
 * 
 * 
 * 
 * 
 */


public class HanderActivity extends PreferenceActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	  //  addPreferencesFromResource(R.xml.hander);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.hander, menu);
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
	
	
	//com.example.preference_demo.HanderActivity$SettingsFragment
	
	public static class SettingsFragment extends PreferenceFragment {
	    @Override
	    public void onCreate(Bundle savedInstanceState) {
	        super.onCreate(savedInstanceState);

	        String settings = getArguments().getString("settings");
	        if ("one".equals(settings)) {
	            addPreferencesFromResource(R.xml.onefra);
	        } else if ("two".equals(settings)) {
	            addPreferencesFromResource(R.xml.twofra);
	        }
	    }
	}
	
	
	  @Override
	    public void onBuildHeaders(List<Header> target) {
	        loadHeadersFromResource(R.xml.hander, target);
	    }
	
	
}
