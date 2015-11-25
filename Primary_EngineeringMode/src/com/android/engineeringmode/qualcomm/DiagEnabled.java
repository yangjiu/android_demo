package com.android.engineeringmode.qualcomm;

//import oppo.os.OppoUsbSwitch;//========================================================================by cbt
import java.util.Calendar;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.SystemClock;
import android.os.SystemProperties;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.android.engineeringmode.R;
import android.os.OppoManager;
import com.android.engineeringmode.functions.Light;

import android.hardware.usb.UsbManager;
import android.provider.Settings;

public class DiagEnabled extends Activity implements View.OnClickListener,
		CompoundButton.OnCheckedChangeListener {
	private static final String LOG_TAG = "DiagEnabled";

	private static final String ALLDIAG_USB_CONFIG = "diag,serial_smd,serial_tty,rmnet_bam,mass_storage,adb";

	private static final int DIALOG_PICKER_PASSWORD = 0x0983;
	private static final int DIALOG_ERROR_STATISTICS = 0x0900;

	private static final String DIAG_PROP = "persist.sys.dial.enable";
	public static final String SERIAL_PROP = "persist.sys.serial.enable";
    private static final String ALLDIAG_PROP = "sys.usb.config";
    private static String USB_CONFIG_PROPERTY;

	private EditText mKeyField;

	private ToggleButton mSerial;
	private ToggleButton mDiag;
	private ToggleButton mAllDiag;
	private ToggleButton mRndisAndDiag;
	private ToggleButton mRmnetBam;
	//SheTao@Swdp.Android.Engineer Mode, 2015/03/23, Add for [support odb], {
	private ToggleButton mOdb;
	//} add end for [].
	
	private Button mPrivilege;
	private Boolean mDiagOpened=false;
	private Boolean mSerialOpened=false;
	private Boolean mAllOpened=false;
	private Boolean mRndisAndDiagOpened = false;
	private Boolean mRmnetBamOpened = false;
	private UsbManager mUsbManager;

	private int mAdbEnable = 0;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.diag_enabled);
		mSerial = (ToggleButton) findViewById(R.id.toggle_serial);
		mSerial.setOnCheckedChangeListener(this);
		mDiag = (ToggleButton) findViewById(R.id.toggle_diag);
		mDiag.setOnCheckedChangeListener(this);
		mAllDiag = (ToggleButton) findViewById(R.id.toggle_alldiag);
		mAllDiag.setOnCheckedChangeListener(this);
		mRndisAndDiag = (ToggleButton) findViewById(R.id.rndisAndDiag);
		mRndisAndDiag.setOnCheckedChangeListener(this);
		mRmnetBam = (ToggleButton) findViewById(R.id.rmnet_bam);
		mRmnetBam.setOnCheckedChangeListener(this);
		//SheTao@Swdp.Android.Engineer Mode, 2015/03/23, Add for [support odb], {
		mOdb = (ToggleButton) findViewById(R.id.toggle_odb);
		mOdb.setOnCheckedChangeListener(this);
		//} add end for [].
		mPrivilege = (Button) findViewById(R.id.privilege);
		//SheTao@Swdp.Android.Engineer Mode, 2015/10/15, Add for [remove *#801# -> privilege button ], {
		//if(SystemProperties.getBoolean("ro.build.release_type",false)) {
			((TextView)findViewById(R.id.privilege_text)).setVisibility(View.INVISIBLE);
			mPrivilege.setVisibility(View.INVISIBLE);
		//}
		//} add end for [].
		mPrivilege.setOnClickListener(this);
		mUsbManager = (UsbManager)this.getSystemService(Context.USB_SERVICE);
	}

	@Override
	protected void onResume() {
		super.onResume();
		mSerial.setChecked(SystemProperties.getBoolean(SERIAL_PROP, false));
		mDiag.setChecked(SystemProperties.getBoolean(DIAG_PROP, false));

        USB_CONFIG_PROPERTY = SystemProperties.get(ALLDIAG_PROP);
        boolean isSelected = false;
        if (USB_CONFIG_PROPERTY.equals(ALLDIAG_USB_CONFIG)) {
            isSelected = true;
        }
        Log.d(LOG_TAG, "USB_CONFIG_PROPERTY: " + USB_CONFIG_PROPERTY + "  sys.usb.config  isSeleceted: " + isSelected);
		mAllDiag.setChecked(isSelected);
		//SheTao@Swdp.Android.Engineer Mode, 2015/03/23, Add for [support odb], {
		mOdb.setChecked(SystemProperties.getBoolean("persist.sys.oppo.odb", false));
		//} add end for [].
		updatePrivilegeButton();
        mAdbEnable = Settings.Global.getInt(getContentResolver(),Settings.Global.ADB_ENABLED, 0);
	}

	private boolean checkPrivilege() {
		SharedPreferences sp = getSharedPreferences("privilege", MODE_PRIVATE);

		// Log.d("", "jni check: " + Privilege.isEscalated());
		// Log.d("", "save flag: " + sp.getBoolean("escalated", false));
		return sp.getBoolean("escalated", false) || Privilege.isEscalated();
	}

	private void updatePrivilegeButton() {
		boolean privilege = checkPrivilege();

		mPrivilege.setText(privilege ? R.string.privilege_recover
				: R.string.privilege_escalate);
		mPrivilege.setTag(privilege);
	}

	private boolean escalatedUp(boolean enable, String password) {
		boolean ret = true;

		// Log.d("", "escalated up: " + password);
		if (enable) {
			if (password != null) {
				enable = Privilege.escalate(password);
                                //ifdef VENDOR_EDIT 
                                //Horn@Swdp.Android.Boot, 2015-04-25, for oppo sepolicy 

                                if (enable)
                                {
                                SystemProperties.set("persist.sys.adbroot", "1");
                                }
                                //endif /* VENDOR_EDIT */ 
				Log.d(LOG_TAG, "privilege escalate "
						+ (enable ? "success" : "failed"));
			} else {
				enable = false;
			}
			ret = enable;
		} else {
                        //ifdef VENDOR_EDIT 
                        //Horn@Swdp.Android.Boot, 2015-04-25, for oppo sepolicy 
                        SystemProperties.set("persist.sys.adbroot", "0");
                        //endif /* VENDOR_EDIT */ 
			Privilege.recover();
		}

		// save operation
		SharedPreferences sp = getSharedPreferences("privilege", MODE_PRIVATE);
		SharedPreferences.Editor e = sp.edit();
		e.putBoolean("escalated", enable);
		e.commit();

		// update ui
		updatePrivilegeButton();

		if (ret) {
                        //ifndef VENDOR_EDIT 
                        //Horn@Swdp.Android.Boot, 2015-04-25, for oppo sepolicy 
			//new Thread(new Runnable() {
			//	public void run() {
			//		Log.i(LOG_TAG, "reboot device...");
			//		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
			//		pm.reboot(null);
			//	}
			//}).start();
                        //else
                        // do reboot only when back to safe adb
                        if ("0".equals(SystemProperties.get("persist.sys.adbroot","1")))
                        {
                         new Thread(new Runnable() {
                              public void run() {
                                      Log.i(LOG_TAG, "reboot device...");
                                      PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
                                      pm.reboot(null);
                              }
                        }).start();     
                        }else
                        {
                           SystemProperties.set("ctl.restart", "adbd");
                        }
                        //endif /* VENDOR_EDIT */ 
		}
		return ret;
	}

	@Override
	public Dialog onCreateDialog(int id) {
		final Dialog dialog = new Dialog(this);
		if (id == DIALOG_PICKER_PASSWORD) {
			dialog.setContentView(R.layout.password_picker);
			dialog.setTitle(R.string.dialog_obtain_password_title);

			dialog.findViewById(R.id.password_enter).setOnClickListener(this);
			dialog.findViewById(R.id.password_cancel).setOnClickListener(this);
		}
		return dialog;
	}

	@Override
	public void onPrepareDialog(int id, Dialog dialog) {
		if (id == DIALOG_PICKER_PASSWORD) {
			EditText et = (EditText) dialog.findViewById(R.id.password);
			mKeyField = et;
			et.setText(null);
			et.setOnEditorActionListener(new TextView.OnEditorActionListener() {
				@Override
				public boolean onEditorAction(TextView v, int actionId,
						KeyEvent event) {
					if (actionId == EditorInfo.IME_ACTION_DONE) {
						String key = mKeyField.getText().toString();
						dismissDialog(DIALOG_PICKER_PASSWORD);
						if (escalatedUp(true, key))
							Toast.makeText(DiagEnabled.this,
									R.string.privilege_escalated_success,
									Toast.LENGTH_SHORT).show();
					}
					return true;
				}
			});
		}
		super.onPrepareDialog(id, dialog);
	}

	@Override
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		int id = buttonView.getId();
		switch (id) {
		case R.id.toggle_diag:
            SystemProperties.set("persist.sys.allcommode", Boolean.toString(isChecked));
			//wangw@OnLineRD.DeviceService, 2014/01/06, Use standard api to set usb config 
			if (isChecked) {
			//	OppoUsbSwitch.setFactoryModeEnbaled(1);//===========================================================by cbt	
                 mAdbEnable = Settings.Global.getInt(getContentResolver(),Settings.Global.ADB_ENABLED, 0);
				if (mAdbEnable != 1) {
					Settings.Global.putInt(getContentResolver(),Settings.Global.ADB_ENABLED, 1);
				}				
				SystemProperties.set("persist.sys.adb.engineermode", "0");
                SystemClock.sleep(200);
				SystemProperties.set("persist.sys.usb.config", "diag,adb");//==================by cbt
				Intent intent = new Intent(
						"android.intent.action.LOCKSREEN_ENGINEERINGMODE");
				sendBroadcast(intent);
				// added by songxh for restart ADBD process
				SystemProperties.set("persist.service.adb.enable", "0");
				SystemClock.sleep(100);
				SystemProperties.set("persist.service.adb.enable", "1");
				mDiagOpened=true;
				//mUsbManager.setCurrentFunction("diag,adb", true);
			} else {
                if (mAdbEnable != 1) {
					Settings.Global.putInt(getContentResolver(),Settings.Global.ADB_ENABLED, 0);
				}
				SystemProperties.set("persist.sys.adb.engineermode", "1");
                
				//SystemProperties.set(ALLDIAG_PROP, "mtp,mass_storage,adb");
				//mUsbManager.setCurrentFunction("mtp,mass_storage,adb", true); 
			//	OppoUsbSwitch.setFactoryModeEnbaled(0);//===============================================================by cbt	
			}
			SystemProperties.set(DIAG_PROP, Boolean.toString(isChecked));
			break;

		case R.id.toggle_serial:
			SystemProperties.set(SERIAL_PROP, Boolean.toString(isChecked));
			if (isChecked) {
				Light.openSerialPort();
				mSerialOpened=true;
			} else {
				Light.closeSerialPort();
				mSerialOpened=false;
			}
			break;

		case R.id.toggle_alldiag:
			SystemProperties.set("persist.sys.allcommode", Boolean.toString(isChecked));
			//wangw@OnLineRD.DeviceService, 2014/01/06, Use standard api to set usb config 
			if (isChecked) {
				SystemProperties.set("persist.sys.usb.config", ALLDIAG_USB_CONFIG);
				//"diag,diag_mdm,serial_tty,rmnet_smd,adb"
				Intent intent = new Intent(
						"android.intent.action.LOCKSREEN_ENGINEERINGMODE");
				sendBroadcast(intent);
				SystemProperties.set("persist.service.adb.enable", "0");
				SystemClock.sleep(100);
				SystemProperties.set("persist.service.adb.enable", "1");
				String string=SystemProperties.get("persist.sys.usb.config");
				Log.d(LOG_TAG, string);
				//mUsbManager.setCurrentFunction(ALLDIAG_USB_CONFIG, true);
				mAllOpened=true;
			} else {
				SystemProperties.set(ALLDIAG_PROP, "mtp,mass_storage,adb");
				//mUsbManager.setCurrentFunction("mtp,mass_storage,adb", true);
				mAllOpened=false;
			}
			break;
			
		case R.id.rndisAndDiag:
			SystemProperties.set("sys.usb.engspecialconfig", Boolean.toString(isChecked));
			if (isChecked) {
				for (int i = 0; i < 20; i++) {
					// State transition is done when sys.usb.state is set to the new configuration
					if (SystemProperties.getBoolean("sys.usb.engspecialconfig", false)) {
						setUsbTethering(true);
						mRndisAndDiagOpened = true;
						break;
					}
					SystemClock.sleep(50);
				}
				if (!SystemProperties.getBoolean("sys.usb.engspecialconfig", false)) {
					Toast.makeText(this, "set usb config rndis,diag failed", Toast.LENGTH_LONG).show();
				}
			} else {
				if (mRndisAndDiagOpened) {
					setUsbTethering(false);
				}
				mRndisAndDiagOpened = false;
			}
			break;

		case R.id.rmnet_bam:
			if (isChecked) {
				SystemProperties.set("sys.usb.config", "diag,serial_smd,rmnet_bam,adb");
			} else {
				SystemProperties.set("sys.usb.config", "mtp,mass_storage,adb");
			}
			break;

		//SheTao@Swdp.Android.Engineer Mode, 2015/03/23, Add for [support odb], {
		case R.id.toggle_odb:
			if (isChecked) {
				SystemProperties.set("persist.sys.oppo.odb", "true");
			} else {
				SystemProperties.set("persist.sys.oppo.odb", "false");
			}
			break;
		//} add end for [].

		}

		//SheTao@Swdp.Android.Engineer Mode, 2015/03/23, Add for [support odb], {
		if (R.id.toggle_odb != id) {
			if (isChecked) {
				mOdb.setChecked(false);
			} else {
				mOdb.setChecked(true);
			}
		}
		//} add end for [].
	}

	private void setUsbTethering(boolean enabled) {
		ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
		if (cm.setUsbTethering(enabled) != ConnectivityManager.TETHER_ERROR_NO_ERROR) {
			Toast.makeText(this, "set usb config rndis,diag failed", Toast.LENGTH_LONG).show();
		}
	}
	
	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.privilege:
			if (checkPrivilege()) {
				escalatedUp(false, null);
				Toast.makeText(DiagEnabled.this,
						R.string.privilege_recover_success, Toast.LENGTH_SHORT)
						.show();
			} else {
				// obtain password
				showDialog(DIALOG_PICKER_PASSWORD);
			}
			break;

		case R.id.password_enter:
			String key = mKeyField.getText().toString();
			dismissDialog(DIALOG_PICKER_PASSWORD);
			if (escalatedUp(true, key))
				Toast.makeText(DiagEnabled.this,
						R.string.privilege_escalated_success,
						Toast.LENGTH_SHORT).show();
			break;

		case R.id.password_cancel:
			dismissDialog(DIALOG_PICKER_PASSWORD);
			break;
		default:
			//
		}
	}
	@Override
	protected void onStop() {
		//begin--cbt add for import log.2012/12/18		
				Calendar now=Calendar.getInstance();
				String time=now.get(Calendar.YEAR)+"-"+(now.get(Calendar.MONTH)+1)+
						"-"+now.get(Calendar.DAY_OF_MONTH)+"-"+now.get(Calendar.HOUR_OF_DAY)
						+"-"+now.get(Calendar.MINUTE)+"-"+now.get(Calendar.SECOND);

		if (mDiagOpened) {

			String content = time+"--DiagEnabled--"+"open diag_mdm,adb";
				int size = OppoManager.writeLogToPartition(content+"\n",OppoManager.ENGINEERINGMODE_TEST_TAG);
			if(size<0)
			Log.e(LOG_TAG, "write engineeringmode test failed,content: "+content);
		}
		if (mSerialOpened) {
			 String content = time+"--DiagEnabled--"+"open SerialPort";
				int size = OppoManager.writeLogToPartition(content+"\n",OppoManager.ENGINEERINGMODE_TEST_TAG);
			if(size<0)
			Log.e(LOG_TAG, "write engineeringmode test failed,content: "+content);
		} 
		if (mAllOpened) {
			 String content = time+"--DiagEnabled--"+ " open " + ALLDIAG_USB_CONFIG;
				int size = OppoManager.writeLogToPartition(content+"\n",OppoManager.ENGINEERINGMODE_TEST_TAG);
			if(size<0)
			Log.e(LOG_TAG, "write engineeringmode test failed,content: "+content);
		}
		super.onStop();
	//begin--cbt add for import log.2012/12/18	
	}
}



	
