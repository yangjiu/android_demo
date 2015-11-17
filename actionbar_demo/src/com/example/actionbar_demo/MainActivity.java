package com.example.actionbar_demo;

import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.Activity;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;

public class MainActivity extends Activity {

	private ActionBar mActionBar;
	private Mtablistener mtablistener;
	private Display_fragment mDisplay_fragment;
	private General_fragment mGeneral_fragment;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
	//	setContentView(R.layout.activity_main);

		mtablistener = new Mtablistener(); 

//
//		mActionBar.show();                                                        //4.显示actionbar
		//setDefaultFragment();
		setContentView(R.layout.activity_main);
		
		mActionBar = getActionBar();                                               //1.获取actionbar
     	mActionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);              //2.设置导航模式为tab

		mActionBar.addTab(mActionBar.newTab().setText(R.string.General)            //3.actionbar增加tab，tab规定要增加监听器
				.setTabListener(mtablistener));
		mActionBar.addTab(mActionBar.newTab().setText(R.string.Sound)
				.setTabListener(mtablistener));
		mActionBar.addTab(mActionBar.newTab().setText(R.string.Display)
				.setTabListener(mtablistener));
//		FragmentManager fragmentManager = getFragmentManager();  
//        FragmentTransaction fragmentTransaction = fragmentManager  
//                .beginTransaction();  
//		
//		 WindowManager wm = getWindowManager();  
//	        Display d = wm.getDefaultDisplay();  
//	        if (d.getWidth() > d.getHeight()) {  
//	            // ---landscape mode---  
//	            Display_fragment fragment1 = new Display_fragment();  
//	            // android.R.id.content refers to the content  
//	            // view of the activity  
//	            fragmentTransaction.replace(android.R.id.content, fragment1);  
//	        } else {  
//	            // ---portrait mode---  
//	            General_fragment fragment2 = new General_fragment();  
//	            fragmentTransaction.replace(android.R.id.content, fragment2);  
//	        }  
//	        // ---add to the back stack---  
//	        fragmentTransaction.addToBackStack(null);  
//	        fragmentTransaction.commit();  
	//	setDefaultFragment();
	     
	}

	class Mtablistener implements ActionBar.TabListener {

		@Override
		public void onTabSelected(Tab tab, FragmentTransaction ft) {
			// TODO Auto-generated method stub
			FragmentManager fm = getFragmentManager();
			FragmentTransaction transaction = fm.beginTransaction();   
		    Log.v("debug",(String) tab.getText());
			if(tab.getText().toString().equals("general"))
                  {
				     Log.v("debug","gen");
                	  if(mGeneral_fragment==null)
                	  {
                		  mGeneral_fragment=new General_fragment();
                	  }
               	  transaction.replace(R.id.FrameLayout1, mGeneral_fragment);
                	  
                	  
                	  
                  }
		
			
			if(tab.getText().toString().equals("display"))
				
            {
				 Log.v("debug","dis");
          	  if(mDisplay_fragment==null)
          	  {
          		  mDisplay_fragment=new Display_fragment();
          	  }
          	  transaction.replace(R.id.FrameLayout1, mDisplay_fragment);
          	  
          	  
          	
            }
	
			transaction.commit();
		
		
		
		}

		@Override
		public void onTabUnselected(Tab tab, FragmentTransaction ft) {
			// TODO Auto-generated method stub

		}

		@Override
		public void onTabReselected(Tab tab, FragmentTransaction ft) {
			// TODO Auto-generated method stub

		}

	}

	
	
	private void setDefaultFragment()
	{
		FragmentManager fm = getFragmentManager();
		FragmentTransaction transaction = fm.beginTransaction();
		mDisplay_fragment = new Display_fragment();
		transaction.replace(R.id.FrameLayout1, mDisplay_fragment);
		transaction.commit();
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
