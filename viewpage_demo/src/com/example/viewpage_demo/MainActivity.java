package com.example.viewpage_demo;

import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;


public class MainActivity extends Activity {

	private ViewPager mViewPager;
	private LayoutInflater lf;
	private MpageAdapter mpageAdapter;
	ArrayList<View> mpageArrayList;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mViewPager=(ViewPager)findViewById(R.id.settings_viewpager);
        mpageArrayList=new ArrayList<View>();
        mpageAdapter=new MpageAdapter();
        lf=getLayoutInflater();
        
        
        View general_page=lf.inflate(R.layout.general_page, null);
        View sound_page=lf.inflate(R.layout.sound_page, null);
        View display_page=lf.inflate(R.layout.display_page, null);
        
       
        
        
        mpageArrayList.add(general_page);
        mpageArrayList.add(sound_page);
        mpageArrayList.add(display_page);
        
        
        mViewPager.setAdapter(mpageAdapter);
        
        
        
        
        
    }

    
    class MpageAdapter extends PagerAdapter
    {

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return mpageArrayList.size();
		//	return 0;
		}

		@Override
		public boolean isViewFromObject(View arg0, Object arg1) {
			// TODO Auto-generated method stub
			return arg0==arg1;
		}
		
	
		@Override
		public Object instantiateItem(ViewGroup container, int position) {
			// TODO Auto-generated method stub
			container.addView(mpageArrayList.get(position));
			Log.v("debug", "初始化： "+position+" "+"个数： "+container.getChildCount());
			return mpageArrayList.get(position);
			//return super.instantiateItem(container, position);
		}
		
		@Override
		public void destroyItem(ViewGroup container, int position, Object object) {
			// TODO Auto-generated method stub
	//		super.destroyItem(container, position, object);
			container.removeView((View) object);
			object=null;
			Log.v("debug", "毁灭： "+position);
//			container.removeViewAt(position);
//			object=null;
		}
		
		
    	
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
