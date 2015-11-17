package com.example.synctask_demo;

import android.app.Activity;
import android.database.CursorJoiner.Result;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;


/*
 * 异步任务的demo
 * 1.建立一个类继承与AsyncTask
 * 2.重写方法
 * 
 * 
 * 
 * 
 */


public class MainActivity extends Activity {

	TextView mTextView;
	Button mButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
         mTextView=(TextView)findViewById(R.id.tv);
         mButton=(Button)findViewById(R.id.button1);
         
         mButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				NetTask mNetTask=new NetTask();
				mNetTask.execute("hello");
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
    
    
    class Httpoperator 
    {
    	public void operat()
    	{
    		try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	
    }
    
    class NetTask extends AsyncTask<String, String, String>
    {

		@Override
		protected String doInBackground(String... params) {
			// TODO Auto-generated method stub
			Httpoperator httpoperator=new Httpoperator();
			int i=0;
			while(i<100)
			{
			httpoperator.operat();
			i++;
			publishProgress(""+i);           //这个函数启用onProgressUpdate方法
			
			}
			
			return "OK";
			
		}
		
	  @Override
	protected void onProgressUpdate(String... values) {
		// TODO Auto-generated method stub
		Toast.makeText(MainActivity.this, values[0], 200).show();
	}
		
    	@Override
    	protected void onPostExecute(String result) {          //执行完毕的方法
    		// TODO Auto-generated method stub
    		mTextView.setText(result);
    	}
	  
	  
    }
    
    
}
