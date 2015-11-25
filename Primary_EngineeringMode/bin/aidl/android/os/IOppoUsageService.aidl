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

interface IOppoUsageService {
	/**
	 * Internel use, only for test:
	 */
	void testSaveSomeData(int dataType, in String dataContent);
	
	/**
	 * Get all history boot time record, with format:YMD.H:M:S
	 */
	List<String> getHistoryBootTime();
	
	/**
	 * Get all history Imei NO record, include the current Imei NO.
	 */
	List<String> getHistoryImeiNO();
	
	/**
	 * Get all history Pcba NO record, include the current Pcba NO.
	 */
	List<String> getHistoryPcbaNO();
	
	/**
	 * Get the record count of the app usage history records.
	 */
	int getAppUsageHistoryRecordCount();
	
	/**
	 * Get the app usage history records. Use the index params to control the record count when
	 * read the rocords to avoid the return result take too many memory for the reason that the
	 * count of the records maybe too large. So you can get the records in batches.
	 *
	 * Params: as designed, index should meat (startIndex >= 1 && endIndex >= 1 && startIndex <= endIndex).
	 * Return: records list with elememt like this: AppName or pkgName|start time with format YMD,H:M:S.
	 */
	List<String> getAppUsageHistoryRecords(int startIndex, int endIndex);
	
	/**
	 * Save app usage history record.
	 * Param appName: the name of the app or it's package name.
	 * Parma dateTime: the date time string of the app start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
	boolean writeAppUsageHistoryRecord(String appName, String dateTime);
	
	/**
	 * Get or accumulate the history count of msg, sended or received.
	 */
	int getHistoryCountOfSendedMsg();
	int getHistoryCountOfReceivedMsg();
	boolean accumulateHistoryCountOfSendedMsg(int newCountIncrease);
	boolean accumulateHistoryCountOfReceivedMsg(int newCountIncrease);
	
	/**
	 * Get or accumulate the phone call duration records:
	 */
	int getDialOutDuration();
	int getInComingCallDuration();
	boolean accumulateDialOutDuration(int durationInMinute);
	boolean accumulateInComingCallDuration(int durationInMinute);
	
	/**
	 * Get the count of the phone call history records. Include dial out and incoming call.
	 */
	int getHistoryRecordsCountOfPhoneCalls();
	
	/**
	 * Get the phone call history records include dial out and incoming call. Use the index params 
	 * to control the record count when read the rocords to avoid the return result take too many
	 * memory for the reason that the count of the records maybe too large.
	 * So you can get the records in batches.
	 * 
	 * Params: as designed, index should meat (startIndex >= 1 && endIndex >= 1 && startIndex <= endIndex).
	 * Return: records list with elememt like this: phoneNo|occur time with format YMD,H:M:S.
	 */
	List<String> getPhoneCallHistoryRecords(int startIndex, int endIndex);	

	/**
	 * Save phone call history record, include dial out and incoming call.
	 * Param phoneNoStr: the phone call number string.
	 * Parma dateTime: the date time string of the phone call start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
	boolean writePhoneCallHistoryRecord(String phoneNoStr, String dateTime);

	/**
	 * call by system to save shut down time:
	 */
	void shutDown();

    //JianRong.Zheng@Swdp.Android.Power, 2014/09/12, Add for [], {
    int import_config_init();
    int import_config_loop(String content);
    int import_config_done();

    int export_mos_config();
    //} add end for [].

    int getMinChargeTemperature();
    int getMaxChargeTemperature();
    int getMaxChargeCurrent();
    boolean updateMinChargeTemperature(int temp);
    boolean updateMaxChargeTemperature(int temp);
    boolean updateMaxChargeCurrent(int current);
    
	String getDownloadStatusString(int part);
}