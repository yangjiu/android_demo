package com.example.expanlist_demo;

import android.R.integer;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class WCexpanlist_adapter extends BaseExpandableListAdapter {

	String[] generalStrings;
	String[][] childrenStrings;
	int[][] imgresid_ints;
	Context mContext;
	
	
	public WCexpanlist_adapter(Context context,String[] general,String[][] children,int[][] imgresid) {
		// TODO Auto-generated constructor stub
		generalStrings=general;
		childrenStrings=children;
		imgresid_ints=imgresid;
		mContext=context;
	}
	
	
	@Override
	public int getGroupCount() {
		// TODO Auto-generated method stub
		return generalStrings.length;
	}

	@Override
	public int getChildrenCount(int groupPosition) {
		// TODO Auto-generated method stub
		return childrenStrings[groupPosition].length;
	}

	@Override
	public Object getGroup(int groupPosition) {
		// TODO Auto-generated method stub
		return generalStrings[groupPosition];
	}

	@Override
	public Object getChild(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return childrenStrings[groupPosition][childPosition];
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
		
		View groudview=lf.inflate(R.layout.groudview, null);
		TextView groudtext=(TextView)groudview.findViewById(R.id.groud_text);
		groudtext.setText(generalStrings[groupPosition]);
		
		return groudview;
	}

	@Override
	public View getChildView(int groupPosition, int childPosition,
			boolean isLastChild, View convertView, ViewGroup parent) {
        LayoutInflater lf=LayoutInflater.from(mContext);
		
		View childview=lf.inflate(R.layout.childview, null);
		TextView childtext=(TextView)childview.findViewById(R.id.child_text);
		ImageView childImageView=(ImageView)childview.findViewById(R.id.child_img);
		
		childtext.setText(childrenStrings[groupPosition][childPosition]);
		childImageView.setBackgroundResource(imgresid_ints[groupPosition][childPosition]);
		// TODO Auto-generated method stub
		return childview;
	}

	@Override
	public boolean isChildSelectable(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return true;
	}

}
