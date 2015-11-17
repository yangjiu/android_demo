package com.example.expanlist_demo;

import android.R.integer;
import android.app.Activity;
import android.app.ExpandableListActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ExpandableListView;

/*
 * 分类列表的demo
 * 
 * 1.xml中加入expanablelist
 * 2.新建groud，与child的布局
 * 3.新建一个adapter类
 * 
 */



public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ExpandableListView mListView=(ExpandableListView)findViewById(R.id.myeplv);
        String[] general=new String[]{"人渣","好人","暖男"};
        String[][] children=new String[][]{
        		
        		{"曾令科"},
        		
        		{"李文峰"},
        		
        		{"陈健强"}
        		
        };
        
        int[][] childimgid=new int[][]
        		{
        		{R.drawable.ic_launcher},
        		{R.drawable.ic_launcher},
        		{R.drawable.ic_launcher},
        		};
        
        WCexpanlist_adapter wcAdapter=new WCexpanlist_adapter(MainActivity.this, general, children, childimgid);
        
        mListView.setAdapter(wcAdapter);
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
