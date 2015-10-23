package com.example.fangwechat;

import java.util.ArrayList;
import java.util.HashMap;

import android.R.integer;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.ExpandableListView;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.Toast;



public class Contact_fragment extends Fragment {
    Context mContext;    
	
	public Contact_fragment(Context context) {
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
		View contact_fragmentview=inflater.inflate(R.layout.contact_fragment, null);
		FrameLayout contact_rootview=(FrameLayout)contact_fragmentview.findViewById(R.id.contact_linear);
			
		final ExpandableListView mExpandableListView=(ExpandableListView)contact_fragmentview.findViewById(R.id.contact_expandview);
        expand_hashadapter mHashadapter=new expand_hashadapter(mContext, WC_data.get_contactChildlist(), WC_data.get_contactgroudlist());
	    mExpandableListView.setAdapter(mHashadapter);
	    
	    Contact_SideBar contact_SideBar=(Contact_SideBar)contact_fragmentview.findViewById(R.id.contact_sidebar);
		
		for(int i=0;i< WC_data.get_contactgroudlist().size();i++)
		{
		mExpandableListView.expandGroup(i);
		}
		mExpandableListView.setGroupIndicator(null);
		
		
		
		final ArrayList<HashMap<String, String>> contactgroud_list=WC_data.get_contactgroudlist();
	    
		contact_SideBar.setOnTouchingLetterChangedListener(new OnTouchingLetterChangedListener() {
			
			@Override
			public void onTouchingLetterChanged(String s) {
				Toast.makeText(mContext, s, 500).show();
				// TODO Auto-generated method stub
				for(int i=0;i<contactgroud_list.size();i++)
				{
					if(contactgroud_list.get(i).get("groudname").equals(s))
					{
						mExpandableListView.setSelectedGroup(i);	
					}
					//
				}
				
				
			}
		});
		//LayoutParams params_bar=new LayoutParams(20, LayoutParams.WRAP_CONTENT);
		//contact_SideBar.setLayoutParams(params_bar);
		//contact_SideBar.setLeft(120);
		
		//contact_rootview.addView(contact_SideBar,params_bar);
		
		
		
		

		
		
	
		//mExpandableListView.setDivider(null);
		//mExpandableListView.setChildDivider(getResources().getDrawable(R.drawable.addfriend));
		//mExpandableListView.setChildDivider(getResources().getDrawable(R.drawable.help));
		
		return contact_fragmentview;
	}
	
	
}
