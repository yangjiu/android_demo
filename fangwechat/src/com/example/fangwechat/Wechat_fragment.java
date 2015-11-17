package com.example.fangwechat;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.SimpleAdapter;


public class Wechat_fragment extends Fragment {

	


	Context mContext;
	public Wechat_fragment(Context context) {
		// TODO Auto-generated constructor stub
		mContext=context;
	}
	
	
	@Override
	public void onCreate(@Nullable Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
	
	}
	
	@Override
	@Nullable
	public View onCreateView(LayoutInflater inflater,
			@Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View fragmentView= inflater.inflate(R.layout.wechat_fragment, null);
		ListView mListView=(ListView)fragmentView.findViewById(R.id.wechat_list);
		
		String user="张翠花";
		String subtitle="快点还钱给我，没钱用了！";
		String time="周二";
		int mresid=R.drawable.userhead;
		
		  ArrayList<Map<String, Object>> maplist =new ArrayList<Map<String,Object>>();
		 HashMap<String, Object> map;
		    for(int i=0;i<10;i++)
		    {
		    	map=new HashMap<String, Object>();
		    	map.put("user", user);
		    	map.put("subtitle", subtitle);
		    	map.put("time", time);
		    	map.put("mresid", mresid);
		    	maplist.add(map);
		    	
		    	
		    }
		
		SimpleAdapter wechatlistAdapter=new SimpleAdapter(mContext, maplist, R.layout.wechatlist_item, 
				new String[]{"user","subtitle","time","mresid"}, new int[]{R.id.wechat_usename,R.id.wechat_subtitle,R.id.wechat_time,R.id.wechat_imageview});
		
		mListView.setAdapter(wechatlistAdapter);
		return fragmentView;
}
	
}
