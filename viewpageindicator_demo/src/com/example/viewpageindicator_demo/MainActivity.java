package com.example.viewpageindicator_demo;

import java.util.ArrayList;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;

import com.viewpagerindicator.TabPageIndicator;
import com.viewpagerindicator.TitlePageIndicator;

/*viewpageindicator开源件的使用
 * 
 * 
 * 
 * 
 */

public class MainActivity extends FragmentActivity {

	
	ArrayList<Fragment> fraArrayList; 
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    //    requestWindowFeature(Window.FEATURE_NO_TITLE);  
        setContentView(R.layout.activity_main);  
       
        fraArrayList=new ArrayList<Fragment>();
        fraArrayList.add(new Contact_fragment());
        fraArrayList.add(new Wechat_fragment());
        
        
      ViewPager  pager = (ViewPager)findViewById(R.id.ivHomeTop);  
        pager.setAdapter(new TestAdapter(getSupportFragmentManager()));  
        TitlePageIndicator  indicator = (TitlePageIndicator) findViewById(R.id.tabGuide);  
        indicator.setViewPager(pager);  
        
       
    }

    
    class TestAdapter extends FragmentPagerAdapter{

		public TestAdapter(FragmentManager fm) {
			super(fm);
			// TODO Auto-generated constructor stub
		}

		@Override
		public Fragment getItem(int arg0) {
			// TODO Auto-generated method stub
			return fraArrayList.get(arg0);
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
