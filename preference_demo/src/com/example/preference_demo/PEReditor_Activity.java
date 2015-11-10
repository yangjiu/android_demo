package com.example.preference_demo;


import android.app.Activity;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.preference.Preference;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


/*
 * preference中edit的使用
 * 
 * 1.preference的edit方法获取edit实例
 * 2.edit实例调用putstring方法写入数据，commit方法执行
 * 3.Preference.getString方法读取数据
 * 4.有相应的xml文件会保存在sd卡中
 * 
 * 
 * 
 */

public class PEReditor_Activity extends Activity {

	private SharedPreferences mPreference;
	private SharedPreferences.Editor mEdit;
	private EditText mEditText;
	private Button wButton;
	private Button rButton;
	private TextView mTextView;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pereditor_);
        
        
        mPreference=(SharedPreferences) this.getPreferences(MODE_WORLD_READABLE);
        mEditText=(EditText)findViewById(R.id.editText1);
         wButton=(Button)findViewById(R.id.button1);
         rButton=(Button)findViewById(R.id.button2);
         mTextView=(TextView)findViewById(R.id.textView1);
        mEdit=mPreference.edit();
        
        wButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				mEdit.putString("wenfeng", mEditText.getText().toString());
				mEdit.commit();
			}
		});
      
        rButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				mTextView.setText( mPreference.getString("wenfeng", ""));
			}
		});
        
        
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
