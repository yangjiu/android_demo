package com.example.inflat_demo;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.RadioGroup;

/*inflate demo
 * 
 * inflate(int resource, ViewGroup root, boolean attachToRoot)
 * 1.resource是资源文件，如R.layout.test1.xml
 * 2.root是根布局，如R.layout。activity_main
 * 3.attachToRoot是指resource是否加入到root布局中，类似于是否执行addview
 * 
 * 情形一：
 * 当attachToRoot等于false是返回的是resource布局，只不过他最外面的view的参数跟root的一样
 * 情形二：
 * 当attachToRoot等于true是返回的是root布局，而且resource参数不会变
 * 
 * 
 * 
 * 
 * 
 * 
 */



public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        //情形一,看不到R.layout.activity_main中的hello字样
//        LinearLayout mLinearLayout=(LinearLayout) getLayoutInflater().inflate(R.layout.activity_main, null);
//        View lastView=getLayoutInflater().inflate(R.layout.test1, mLinearLayout,false);
//        setContentView(lastView);
        
      //情形二,看到R.layout.activity_main中的hello字样，且R.layout.test1布局参数不变
        LinearLayout mLinearLayout=(LinearLayout) getLayoutInflater().inflate(R.layout.activity_main, null);
        View lastView=getLayoutInflater().inflate(R.layout.test1, mLinearLayout,true);
        setContentView(lastView);
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
