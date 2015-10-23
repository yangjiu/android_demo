package com.example.fangwechat;

import java.util.ArrayList;

import android.app.ActionBar;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ActionProvider;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.view.WindowManager.LayoutParams;
import android.widget.PopupWindow;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.ShareActionProvider;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends FragmentActivity {

	
	
	Fragment wechat_fragment,contact_fragment;
	ArrayList<Fragment> list_fragment;
	ViewPager mainViewPager;
	WC_pagerAdapter wc_pagerAdapter; 
	
	RadioGroup wcRadioGroup;
	
	ActionBar wc_ActionBar;
	
	PopupWindow popupWindow_home;
	
	TextView tmpTextView;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
   //     requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        
       
       wc_ActionBar=getActionBar();
       wcRadioGroup=(RadioGroup)findViewById(R.id.navi_radioGroup);
       
      // wc_ActionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_LIST);
        
      init_actionbar(wc_ActionBar);
        
      
        
        
        list_fragment=new ArrayList<Fragment>();
        list_fragment.add(new Wechat_fragment(MainActivity.this));
        list_fragment.add(new Contact_fragment(MainActivity.this));
        list_fragment.add(new Find_fragment(MainActivity.this));
        list_fragment.add(new Me_fragment(MainActivity.this));
        
        
        mainViewPager=(ViewPager)findViewById(R.id.main_viewpage);
        wc_pagerAdapter=new WC_pagerAdapter(getSupportFragmentManager());
        mainViewPager.setAdapter(wc_pagerAdapter);
        
        mainViewPager.setOnPageChangeListener(new Mainviewpagelistener());
        wcRadioGroup.setOnCheckedChangeListener(new WCRadiolistener());
     
      //  setOverflowShowingAlways();
        tmpTextView=(TextView)findViewById(R.id.textView1);
        
    }

    
//    private void setOverflowShowingAlways() {  
//        try {  
//            ViewConfiguration config = ViewConfiguration.get(this);  
//            Field menuKeyField = ViewConfiguration.class.getDeclaredField("sHasPermanentMenuKey");  
//            menuKeyField.setAccessible(true);  
//            menuKeyField.setBoolean(config, true);  
//        } catch (Exception e) {  
//            e.printStackTrace();  
//        }  
//    }  
    
    class Mainviewpagelistener implements OnPageChangeListener
    {

	

	@Override
	public void onPageSelected(int position) {
		// TODO Auto-generated method stub
		switch (position) {
		case 0:
		wcRadioGroup.check(R.id.wechat_radio);
		break;
		case 1:
		wcRadioGroup.check(R.id.contact_radio);
		break;
		case 2:
		wcRadioGroup.check(R.id.find_radio);
		break;
		case 3:
		wcRadioGroup.check(R.id.me_radio);
		break;
		}
	}

	@Override
	public void onPageScrollStateChanged(int arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onPageScrolled(int arg0, float arg1, int arg2) {
		// TODO Auto-generated method stub
		
	}
    	
    }
    
    
    
    
    class WCRadiolistener implements RadioGroup.OnCheckedChangeListener
    {

		@Override
		public void onCheckedChanged(RadioGroup group, int checkedId) {
			// TODO Auto-generated method stub
			switch (checkedId) {
			case R.id.wechat_radio:
			mainViewPager.setCurrentItem(0);
			break;
			case R.id.contact_radio:
			mainViewPager.setCurrentItem(1);
			break;
			case R.id.find_radio:
			mainViewPager.setCurrentItem(2);
			break;
			case R.id.me_radio:
			mainViewPager.setCurrentItem(3);
		}
    	
    }
    
    }
    
    class WC_pagerAdapter extends FragmentPagerAdapter
    {

		

		

		public WC_pagerAdapter(FragmentManager fm) {
			super(fm);
			// TODO Auto-generated constructor stub
		}

		@Override
		public Fragment getItem(int position) {
			// TODO Auto-generated method stub
			return list_fragment.get(position);
		}

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return list_fragment.size();
		}
    	
    }
    
    void init_actionbar(ActionBar actionBar){
    	LayoutInflater lf=getLayoutInflater();
    	String actionbar_title=getResources().getString(R.string.actionbar_title);
    	actionBar.setTitle(actionbar_title);
    	//actionBar.setDisplayUseLogoEnabled(false);
    	actionBar.setDisplayShowHomeEnabled(false);
    	
    	//next is popup window
    //	View popwindow_homeView=lf.inflate(R.layout.popwindow_home, null);
    //	popupWindow_home=new PopupWindow(popwindow_homeView,LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT,true);
    	//popupWindow_home.setBackgroundDrawable(getResources().getDrawable(R.drawable.find_gray));
    	
    	
//    	actionBar.setDisplayShowCustomEnabled(true);
////    	actionBar.setIcon(R.drawable.find_gray);
//    	LayoutInflater lf=getLayoutInflater();
//    	View customview=lf.inflate(R.layout.custom_actionbar, null);
//     	actionBar.setCustomView(customview);
    	
    }
    
    
//    @Override
//    public boolean onPrepareOptionsMenu(Menu menu) {
//    	// TODO Auto-generated method stub
//    
//            menu.getItem(1).setIcon(R.drawable.contact_gray);//替换图标
// 
//        
//   
//    	return super.onPrepareOptionsMenu(menu);
//    }
    
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        
//        ShareActionProvider   provider = (ShareActionProvider) menu.findItem(R.id.search_home).getActionProvider();  
//        // Initialize the share intent  
//        Intent intent = new Intent(Intent.ACTION_SEND);  
//        intent.setType("text/plain");  
//        intent.putExtra(Intent.EXTRA_TEXT, "Text I want to share");  
//        provider.setShareIntent(intent);  
        
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.menu_home) {
//        	item.setActionView(R.drawable.contact);
//        	View mView=item.getActionView();
//        	
//        	//popupWindow_home.showAtLocation(mainViewPager, Gravity.TOP,0,0);
//        	popupWindow_home.showAsDropDown(mView);
        	Toast.makeText(MainActivity.this, "click the add", 500).show();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
    
    
   
    
    
    
    
    
    
    
    
    
    
    
    
}
