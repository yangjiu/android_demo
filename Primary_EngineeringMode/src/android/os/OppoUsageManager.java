/************************************************************
 * Copyright 2014 OPPO Mobile Comm Corp., Ltd.
 * All rights reserved.
 * 
 * Description	   : Provide a function to indentify the usage of the phone.
 *
 * History		  :( ID, Date, Author, Description)
 * v1.0, 2014-07-22,  Peirs@Swdp.Android.FrameworkUi, create
 ************************************************************/

package android.os;

import android.os.IOppoUsageService;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

import java.util.List;


public final class OppoUsageManager {
	public static final String TAG = "OppoUsageManager";
	private static final boolean DEBUG = false;
	private static final boolean DEBUG_W = false || DEBUG;
	private static final boolean DEBUG_E = false || DEBUG;
	
	public static final String SERVICE_NAME = "usage";
	private IOppoUsageService mOppoUsageService = null;

	private static OppoUsageManager mInstance = null;

	private OppoUsageManager() {
		IBinder serviceBinder = ServiceManager.getService(SERVICE_NAME);
		mOppoUsageService = IOppoUsageService.Stub.asInterface(serviceBinder);
	}

	public static OppoUsageManager getOppoUsageManager() {
		if (null == mInstance) {
			mInstance = new OppoUsageManager();
		}
		return mInstance;
	}

	public void testSaveSomeData(int dataType, String dataContent) {
		if (null != mOppoUsageService) {
			try {
				mOppoUsageService.testSaveSomeData(dataType, dataContent);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "testSaveSomeData failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "testSaveSomeData:service not publish!");
		}
	}

	//--------------------------
	/**
	 * Get all history boot time records, with format:YMD.H:M:S
	 */
	public List<String> getHistoryBootTime() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getHistoryBootTime();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getHistoryBootTime failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getHistoryBootTime:service not publish!");
		}

		return null;
	}
	
	/**
	 * Get all history Imei NO record, include the current Imei NO.
	 */
	public List<String> getHistoryImeiNO() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getHistoryImeiNO();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getHistoryImeiNO failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getHistoryImeiNO:service not publish!");
		}

		return null;
	}
	
	/**
	 * Get all history Pcba NO record, include the current Pcba NO.
	 */
	public List<String> getHistoryPcbaNO() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getHistoryPcbaNO();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getHistoryPcbaNO failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getHistoryPcbaNO:service not publish!");
		}

		return null;
	}
	
	/**
	 * Get the record count of the app usage history records.
	 */
	public int getAppUsageHistoryRecordCount() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getAppUsageHistoryRecordCount();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getAppUsageHistoryRecordCount failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getAppUsageHistoryRecordCount:service not publish!");
		}

		return 0;
	}
	
	/**
	 * Get the app usage history records. Use the index params to control the record count when
	 * read the rocords to avoid the return result take too many memory for the reason that the
	 * count of the records maybe too large. So you can get the records in batches.
	 *
	 * Params: as designed, index should meat (startIndex >= 1 && endIndex >= 1 && startIndex <= endIndex).
	 * Return: records list with elememt like this: AppName or pkgName|start time with format YMD,H:M:S.
	 */
	public List<String> getAppUsageHistoryRecords(int startIndex, int endIndex) {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getAppUsageHistoryRecords(startIndex, endIndex);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getAppUsageHistoryRecords failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getAppUsageHistoryRecords:service not publish!");
		}

		return null;
	}
	
	/**
	 * Save app usage history record.
	 * Param appName: the name of the app or it's package name.
	 * Parma dateTime: the date time string of the app start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
	public boolean writeAppUsageHistoryRecord(String appName, String dateTime) {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.writeAppUsageHistoryRecord(appName, dateTime);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getAppUsageHistoryRecords failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getAppUsageHistoryRecords:service not publish!");
		}

		return false;
	}
	
	/**
	 * Get the history count of msg sended.
	 */
	public int getHistoryCountOfSendedMsg() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getHistoryCountOfSendedMsg();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getHistoryCountOfSendedMsg failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getHistoryCountOfSendedMsg:service not publish!");
		}

		return 0;
	}

	/**
	 * Get the history count of msg received.
	 */
	public int getHistoryCountOfReceivedMsg() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getHistoryCountOfReceivedMsg();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getHistoryCountOfReceivedMsg failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getHistoryCountOfReceivedMsg:service not publish!");
		}

		return 0;
	}

	/**
	 * Accumulate the history count of msg sended.
	 */
	public boolean accumulateHistoryCountOfSendedMsg(int newCountIncrease) {
		if (newCountIncrease <= 0) {
			if (DEBUG_W) Log.w(TAG, "accumulateHistoryCountOfSendedMsg:illegal param!");
			return false;
		}
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.accumulateHistoryCountOfSendedMsg(newCountIncrease);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "accumulateHistoryCountOfSendedMsg failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "accumulateHistoryCountOfSendedMsg:service not publish!");
		}

		return false;
	}

	/**
	 * Accumulate the history count of msg received.
	 */
	public boolean accumulateHistoryCountOfReceivedMsg(int newCountIncrease) {
		if (newCountIncrease <= 0) {
			if (DEBUG_W) Log.w(TAG, "accumulateHistoryCountOfReceivedMsg:illegal param!");
			return false;
		}

		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.accumulateHistoryCountOfReceivedMsg(newCountIncrease);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "accumulateHistoryCountOfReceivedMsg failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "accumulateHistoryCountOfReceivedMsg:service not publish!");
		}

		return false;
	}
	
	/**
	 * Get the phone call duration records, type is dial out:
	 */
	public int getDialOutDuration() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getDialOutDuration();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getDialOutDuration failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getDialOutDuration:service not publish!");
		}

		return 0;
	}
	
	/**
	 * Get the phone call duration records, type is incoming call:
	 */
	public int getInComingCallDuration() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getInComingCallDuration();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getInComingCallDuration failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getInComingCallDuration:service not publish!");
		}

		return 0;
	}

	/**
	 * Accumulate the phone call duration records, type is dial out:
	 */
	public boolean accumulateDialOutDuration(int durationInMinute) {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.accumulateDialOutDuration(durationInMinute);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "accumulateDialOutDuration failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "accumulateDialOutDuration:service not publish!");
		}

		return false;
	}

	/**
	 * Accumulate the phone call duration records, type is incoming call:
	 */
	public boolean accumulateInComingCallDuration(int durationInMinute) {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.accumulateInComingCallDuration(durationInMinute);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "accumulateInComingCallDuration failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "accumulateInComingCallDuration:service not publish!");
		}

		return false;
	}
	
	/**
	 * Get the count of the phone call history records, include dial out and incoming call.
	 */
	public int getHistoryRecordsCountOfPhoneCalls() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getHistoryRecordsCountOfPhoneCalls();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getHistoryRecordsCountOfPhoneCalls failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getHistoryRecordsCountOfPhoneCalls:service not publish!");
		}

		return 0;
	}
	
	/**
	 * Get the phone call history records include dial out and incoming call. Use the index params 
	 * to control the record count when read the rocords to avoid the return result take too many
	 * memory for the reason that the count of the records maybe too large.
	 * So you can get the records in batches.
	 * 
	 * Params: as designed, index should meat (startIndex >= 1 && endIndex >= 1 && startIndex <= endIndex).
	 * Return: records list with elememt like this: phoneNo|occur time with format YMD,H:M:S.
	 */
	public List<String> getPhoneCallHistoryRecords(int startIndex, int endIndex) {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getPhoneCallHistoryRecords(startIndex, endIndex);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getPhoneCallHistoryRecords failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getPhoneCallHistoryRecords:service not publish!");
		}

		return null;
	}

	/**
	 * Save phone call history record, include dial out and incoming call.
	 * Param phoneNoStr: the phone call number string.
	 * Parma dateTime: the date time string of the phone call start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
	public boolean writePhoneCallHistoryRecord(String phoneNoStr, String dateTime) {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.writePhoneCallHistoryRecord(phoneNoStr, dateTime);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "writePhoneCallHistoryRecord failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "writePhoneCallHistoryRecord:service not publish!");
		}

		return false;
	}

    //JianRong.Zheng@Swdp.Android.Power, 2014/09/12, Add for [], {
	public int import_config_init() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.import_config_init();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "import_config_init failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "import_config_init:service not publish!");
		}

		return -1;
	}
	
	public int import_config_loop(String content) {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.import_config_loop(content);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "import_config_loop failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "import_config_loop:service not publish!");
		}

		return -1;
	}
	
	public int import_config_done() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.import_config_done();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "import_config_done failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "import_config_done:service not publish!");
		}

		return -1;
	}
    
	public int export_mos_config() {
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.export_mos_config();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "export_mos_config failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "export_mos_config:service not publish!");
		}

		return -1;
	}
    //} add end for [].


    public boolean updateMaxChargeCurrent(int current){
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.updateMaxChargeCurrent(current);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "updateMaxChargeCurrent failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "updateMaxChargeCurrent:service not publish!");
		}

		return false;
    }

    public boolean updateMaxChargeTemperature(int temp){
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.updateMaxChargeCurrent(temp);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "updateMaxChargeTemperature failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "updateMaxChargeTemperature:service not publish!");
		}

		return false;
    }

    public boolean updateMinChargeTemperature(int temp){
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.updateMinChargeTemperature(temp);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "updateMinChargeTemperature failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "updateMinChargeTemperature:service not publish!");
		}

		return false;
    }


    public int getMaxChargeCurrent(){
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getMaxChargeCurrent();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getMaxChargeCurrent failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getMaxChargeCurrent:service not publish!");
		}

		return 0;
    }

    public int getMaxChargeTemperature(){
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getMaxChargeTemperature();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getMaxChargeTemperature failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getMaxChargeTemperature:service not publish!");
		}

		return 0;
    }

    public int getMinChargeTemperature(){
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getMinChargeTemperature();
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getMinChargeTemperature failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getMinChargeTemperature:service not publish!");
		}

		return 0;
    }
    
    public String getDownloadStatusString(int part){
		if (null != mOppoUsageService) {
			try {
				return mOppoUsageService.getDownloadStatusString(part);
			} catch(RemoteException exce) {
				if (DEBUG_E) Log.e(TAG, "getDownloadStatusString failed!", exce);
			}
		} else {
			if (DEBUG_W) Log.w(TAG, "getDownloadStatusString:service not publish!");
		}

		return null;
    }
}
