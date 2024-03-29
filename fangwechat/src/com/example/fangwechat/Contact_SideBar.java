package com.example.fangwechat;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;

class Contact_SideBar extends View {

	  private TextView textView;// 显示框

	  private boolean showBkg = false;
	  // 触摸事件
	  OnTouchingLetterChangedListener onTouchingLetterChangedListener;
	  // 26个字母
	  public static String[] b = { "#", "A", "B", "C", "D", "E", "F", "G", "H",
	      "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	      "V", "W", "X", "Y", "Z" };
	  int choose = -1;// 选中
	  Paint paint = new Paint();
	  Context mContext;

	  // 自定义view 最好注明这三个构造函数
	  public Contact_SideBar(Context context, AttributeSet attrs, int defStyle) {
	    super(context, attrs, defStyle);
	  }

	  public Contact_SideBar(Context context, AttributeSet attrs) {
	    super(context, attrs);
	  }

	  public Contact_SideBar(Context context) {
	    super(context);
	    mContext=context;
	  }

	
	  /**
	   * 重写这个方法
	   */
	  protected void onDraw(Canvas canvas) {
	    super.onDraw(canvas);
	    // 获取焦点改变背景颜色.
	    if (showBkg) {
	      canvas.drawColor(Color.parseColor("#40000000"));
	      
	    }
	    int height = getHeight();// 获取对应高度
	    int width = getWidth(); // 获取对应宽度
	    int singleHeight = height / b.length;// 获取每一个字母的高度

	    for (int i = 0; i < b.length; i++) {
	      paint.setColor(Color.BLACK);
	      // paint.setColor(Color.WHITE);
	      paint.setTypeface(Typeface.DEFAULT_BOLD);
	      paint.setAntiAlias(true);
	      paint.setTextSize(20);
	      // 选中的状态
	      if (i == choose) {
	        paint.setColor(Color.parseColor("#3399ff"));
	        paint.setFakeBoldText(true);
	      }
	      // x坐标等于中间-字符串宽度的一半.
	      float xPos = width - paint.measureText(b[i])-15;
	      float yPos = singleHeight * i +  singleHeight;
	      canvas.drawText(b[i], xPos, yPos, paint);
	      paint.reset();// 重置画笔
	    }

	  }

	  @Override
	  public boolean dispatchTouchEvent(MotionEvent event) {
	    final int action = event.getAction();
	    final float y = event.getY();// 点击y坐标
	    final int oldChoose = choose;
	    final OnTouchingLetterChangedListener listener = onTouchingLetterChangedListener;
	    final int c = (int) (y / getHeight() * b.length);// 点击y坐标所占总高度的比例*b数组的长度就等于点击b中的个数.
	 
	    switch (action) {
	    case MotionEvent.ACTION_DOWN:
	    Log.v("debug", "down");
	      showBkg = true;
	      if (oldChoose != c && listener != null) {
	        if (c > 0 && c < b.length) {
	       // 	Toast.makeText(mContext, "down", 500).show();
	        listener.onTouchingLetterChanged(b[c]);// 执行onTouchingLetterChanged事件.
	          choose = c;// 选项
	          invalidate();// 刷新
	        }
	      }

	      break;
	    case MotionEvent.ACTION_MOVE:
	    	   Log.v("debug", "move");
	      if (oldChoose != c && listener != null) {
	        if (c > 0 && c < b.length) {
	       // 	Toast.makeText(mContext, "move", 500).show();
	        listener.onTouchingLetterChanged(b[c]);
	          choose = c;
	          invalidate();
	        }
	      }
	      break;
	    case MotionEvent.ACTION_UP:
	    	Log.v("debug", "up");
	    	//Toast.makeText(mContext, "up", 500).show();
	     showBkg = false;
	      choose = -1;//
	      invalidate();
	   //   textView.setVisibility(View.GONE);
	      break;
	    }
	    return true;
	  }

//	  public void setTextView(TextView textView) {
//	    this.textView = textView;
//	  }

	  /**
	   * 向外公开的方法
	   * 
	   * @param onTouchingLetterChangedListener
	   */
	  public void setOnTouchingLetterChangedListener(
	      OnTouchingLetterChangedListener onTouchingLetterChangedListener) {
	    this.onTouchingLetterChangedListener = onTouchingLetterChangedListener;
	  }

	  /**
	   * 接口
	   * 
	   * @author coder
	   * 
	   */
	

	}