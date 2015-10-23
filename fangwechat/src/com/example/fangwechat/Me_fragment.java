package com.example.fangwechat;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ExpandableListView;

public class Me_fragment extends Fragment{

	Context mContext;
	public Me_fragment(Context context) {
		mContext=context;
		// TODO Auto-generated constructor stub
	}
	
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View findView=inflater.inflate(R.layout.me_fragment, null);
		ExpandableListView mExpandableListView=(ExpandableListView)findView.findViewById(R.id.me_expandview);
		
		expand_hashadapter madapter=new expand_hashadapter(mContext, WC_data.get_meChildlist(), WC_data.get_megroudlist());
		
		mExpandableListView.setAdapter(madapter);
		for(int i=0;i< WC_data.get_megroudlist().size();i++)  //展开所有
		{
		mExpandableListView.expandGroup(i);
		}
		mExpandableListView.setGroupIndicator(null);           //去除指针
		return findView;
	}
	
	
}
