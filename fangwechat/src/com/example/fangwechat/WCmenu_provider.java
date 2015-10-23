package com.example.fangwechat;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import android.R.integer;
import android.R.menu;
import android.content.Context;

import android.view.ActionProvider;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.SimpleAdapter;

public class WCmenu_provider extends ActionProvider{

	Context mContext;
	
	public WCmenu_provider(Context context) {
		super(context);
		mContext=context;
		// TODO Auto-generated constructor stub
	}

	@Override
	public View onCreateActionView() {
		// TODO Auto-generated method stub
	    final LayoutInflater lf=LayoutInflater.from(mContext);
	    View wcmenu=lf.inflate(R.layout.anchor, null);
	    final ImageView anchorView=(ImageView)wcmenu.findViewById(R.id.menu_anchor_imageView);
	    
	    View view_menuWindow=lf.inflate(R.layout.popwindow_home, null);
	    ListView mListView=new ListView(mContext);
	    String[] strings={"a","b","c"};
	 
	   // ArrayAdapter<String> mArrayAdapter=new ArrayAdapter<String>(mContext, R.layout.menulist_item, R.id.item_text, strings);
	    
	    
	    ArrayList<Map<String, Object>> maplist =new ArrayList<Map<String,Object>>();
	    String[] menuname={"发起群聊","添加朋友","扫一扫","收钱","帮助和反馈"};
	    int[] menuviewid={R.drawable.groudchat,R.drawable.addfriend,R.drawable.scan,R.drawable.getmoney,R.drawable.help};
	    HashMap<String, Object> map;
	    for(int i=0;i<menuname.length;i++)
	    {
	    	map=new HashMap<String, Object>();
	    	map.put("menuname", menuname[i]);
	    	map.put("menuviewid", menuviewid[i]);
	    	maplist.add(map);
	    	
	    	
	    }
	    
	   SimpleAdapter mSimpleAdapter=new SimpleAdapter(mContext, maplist, R.layout.menulist_item,new String[]{"menuname","menuviewid"}, new int[]{R.id.item_menutext,R.id.item_menuimage});
	    
	 //  SimpleAdapter ddAdapter=new SimpleAdapter(context, data, resource, from, to) 
	    mListView.setAdapter(mSimpleAdapter);
	    
	    WindowManager wm=(WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE);
	    
	    int popwindow_width=(wm.getDefaultDisplay().getWidth())/2;
	    final PopupWindow menuWindow=new PopupWindow(mListView,popwindow_width+25,  LayoutParams.WRAP_CONTENT, true);
	    
	    
	    menuWindow.setBackgroundDrawable(mContext.getResources().getDrawable(R.color.actionbar_color));
	    
	    anchorView.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				int[] location=new int[2];
				anchorView.getLocationInWindow(location);
				View rootview=lf.inflate(R.layout.activity_main, null);
			    menuWindow.showAtLocation(rootview, Gravity.RIGHT | Gravity.TOP, 20, 145);
			}
		});
	    
		return wcmenu;
	}

	

	
}
