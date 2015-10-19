package com.example.radiogroud_demo;

import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;


/*
 * 本例子演示了radiogroud的作用，radiogroud作为导航栏，viewpage加载fragment
 * 1.布局文件布局radiobutton，viewpage
 * 2.设置fragmentadapter，加载fragment
 * 
 * 注意：本例没有 radiogroud与viewpage的交互
 * 
 * 
 * 
 */



public class MainActivity extends FragmentActivity {

	ViewPager mPager;
	ArrayList<Fragment> fraArrayList;
	MviewpageAdapter mAdapter;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mPager=(ViewPager)findViewById(R.id.mviewpage);
        
        fraArrayList=new ArrayList<Fragment>();
        fraArrayList.add(new Wechat_fragment());
        fraArrayList.add(new Contact_fragment());
        
        mAdapter=new MviewpageAdapter(getSupportFragmentManager()); //注意与getFragmentManager()方法的区别，一个是v4的包
        mPager.setAdapter(mAdapter);
        
    }

    
    
    class MviewpageAdapter extends FragmentPagerAdapter{

		
		
		
		
    	public MviewpageAdapter(FragmentManager fragmentManager) {
			super(fragmentManager);
			// TODO Auto-generated constructor stub
		}

	

		@Override
		public Fragment getItem(int position) {
			// TODO Auto-generated method stub
			return fraArrayList.get(position);
		}

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return fraArrayList.size();
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
