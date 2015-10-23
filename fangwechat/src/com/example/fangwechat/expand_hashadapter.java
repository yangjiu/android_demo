package com.example.fangwechat;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import android.R.integer;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class expand_hashadapter extends BaseExpandableListAdapter {


	
	ArrayList<ArrayList<HashMap<String, Object>>> childrenList;
	 ArrayList<HashMap<String, String>> groudlist;
	 Context mContext;
	
	
	public expand_hashadapter(Context context,ArrayList<ArrayList<HashMap<String, Object>>> childrenList,ArrayList<HashMap<String, String>> groudlist) {
		// TODO Auto-generated constructor stub

		this.groudlist=groudlist;
		this.childrenList=childrenList;
		mContext=context;
	}
		
		
	
	
	
	@Override
	public int getGroupCount() {
		// TODO Auto-generated method stub

		return groudlist.size();
	}

	@Override
	public int getChildrenCount(int groupPosition) {
		// TODO Auto-generated method stub
	
		return childrenList.get(groupPosition).size();
	
	}

	@Override
	public Object getGroup(int groupPosition) {
		// TODO Auto-generated method stub
		return groudlist.get(groupPosition);
	}

	@Override
	public Object getChild(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return childrenList.get(groupPosition).get(childPosition);
	}

	@Override
	public long getGroupId(int groupPosition) {
		// TODO Auto-generated method stub
		return groupPosition;
	}

	@Override
	public long getChildId(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return childPosition;
	}

	@Override
	public boolean hasStableIds() {
		// TODO Auto-generated method stub
		return true;
	}

	@Override
	public View getGroupView(int groupPosition, boolean isExpanded,
			View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		LayoutInflater lf=LayoutInflater.from(mContext);
		
		View groudview=lf.inflate(R.layout.expand_groudview, null);
		TextView groudtext=(TextView)groudview.findViewById(R.id.expandgroud_text);
		groudtext.setText(groudlist.get(groupPosition).get("groudname"));
		
		return groudview;
	}

	@Override
	public View getChildView(int groupPosition, int childPosition,
			boolean isLastChild, View convertView, ViewGroup parent) {
        LayoutInflater lf=LayoutInflater.from(mContext);
		
		View childview=lf.inflate(R.layout.expand_childview, null);
		TextView childtext=(TextView)childview.findViewById(R.id.expandchild_text);
		ImageView childImageView=(ImageView)childview.findViewById(R.id.expandchild_img);
		
		childtext.setText((CharSequence) childrenList.get(groupPosition).get(childPosition).get("character"));
	//	childImageView.setBackgroundResource(R.drawable.contact);
		
		childImageView.setImageDrawable(mContext.getResources().getDrawable( (Integer) childrenList.get(groupPosition).get(childPosition).get("picture")));
		
		
	//	childImageView.setBackgroundDrawable(mContext.getResources().getDrawable( (Integer) childrenList.get(groupPosition).get(childPosition).get("picture")));
		// TODO Auto-generated method stub
		return childview;
	}

	@Override
	public boolean isChildSelectable(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return true;
	}

}

