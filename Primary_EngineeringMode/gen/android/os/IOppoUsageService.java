/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: /work/Primary_EngineeringMode/src/android/os/IOppoUsageService.aidl
 */
package android.os;
public interface IOppoUsageService extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements android.os.IOppoUsageService
{
private static final java.lang.String DESCRIPTOR = "android.os.IOppoUsageService";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an android.os.IOppoUsageService interface,
 * generating a proxy if needed.
 */
public static android.os.IOppoUsageService asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof android.os.IOppoUsageService))) {
return ((android.os.IOppoUsageService)iin);
}
return new android.os.IOppoUsageService.Stub.Proxy(obj);
}
@Override public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_testSaveSomeData:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
java.lang.String _arg1;
_arg1 = data.readString();
this.testSaveSomeData(_arg0, _arg1);
reply.writeNoException();
return true;
}
case TRANSACTION_getHistoryBootTime:
{
data.enforceInterface(DESCRIPTOR);
java.util.List<java.lang.String> _result = this.getHistoryBootTime();
reply.writeNoException();
reply.writeStringList(_result);
return true;
}
case TRANSACTION_getHistoryImeiNO:
{
data.enforceInterface(DESCRIPTOR);
java.util.List<java.lang.String> _result = this.getHistoryImeiNO();
reply.writeNoException();
reply.writeStringList(_result);
return true;
}
case TRANSACTION_getHistoryPcbaNO:
{
data.enforceInterface(DESCRIPTOR);
java.util.List<java.lang.String> _result = this.getHistoryPcbaNO();
reply.writeNoException();
reply.writeStringList(_result);
return true;
}
case TRANSACTION_getAppUsageHistoryRecordCount:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getAppUsageHistoryRecordCount();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_getAppUsageHistoryRecords:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
int _arg1;
_arg1 = data.readInt();
java.util.List<java.lang.String> _result = this.getAppUsageHistoryRecords(_arg0, _arg1);
reply.writeNoException();
reply.writeStringList(_result);
return true;
}
case TRANSACTION_writeAppUsageHistoryRecord:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
java.lang.String _arg1;
_arg1 = data.readString();
boolean _result = this.writeAppUsageHistoryRecord(_arg0, _arg1);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_getHistoryCountOfSendedMsg:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getHistoryCountOfSendedMsg();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_getHistoryCountOfReceivedMsg:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getHistoryCountOfReceivedMsg();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_accumulateHistoryCountOfSendedMsg:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
boolean _result = this.accumulateHistoryCountOfSendedMsg(_arg0);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_accumulateHistoryCountOfReceivedMsg:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
boolean _result = this.accumulateHistoryCountOfReceivedMsg(_arg0);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_getDialOutDuration:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getDialOutDuration();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_getInComingCallDuration:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getInComingCallDuration();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_accumulateDialOutDuration:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
boolean _result = this.accumulateDialOutDuration(_arg0);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_accumulateInComingCallDuration:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
boolean _result = this.accumulateInComingCallDuration(_arg0);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_getHistoryRecordsCountOfPhoneCalls:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getHistoryRecordsCountOfPhoneCalls();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_getPhoneCallHistoryRecords:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
int _arg1;
_arg1 = data.readInt();
java.util.List<java.lang.String> _result = this.getPhoneCallHistoryRecords(_arg0, _arg1);
reply.writeNoException();
reply.writeStringList(_result);
return true;
}
case TRANSACTION_writePhoneCallHistoryRecord:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
java.lang.String _arg1;
_arg1 = data.readString();
boolean _result = this.writePhoneCallHistoryRecord(_arg0, _arg1);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_shutDown:
{
data.enforceInterface(DESCRIPTOR);
this.shutDown();
reply.writeNoException();
return true;
}
case TRANSACTION_import_config_init:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.import_config_init();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_import_config_loop:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
int _result = this.import_config_loop(_arg0);
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_import_config_done:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.import_config_done();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_export_mos_config:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.export_mos_config();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_getMinChargeTemperature:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getMinChargeTemperature();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_getMaxChargeTemperature:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getMaxChargeTemperature();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_getMaxChargeCurrent:
{
data.enforceInterface(DESCRIPTOR);
int _result = this.getMaxChargeCurrent();
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_updateMinChargeTemperature:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
boolean _result = this.updateMinChargeTemperature(_arg0);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_updateMaxChargeTemperature:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
boolean _result = this.updateMaxChargeTemperature(_arg0);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_updateMaxChargeCurrent:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
boolean _result = this.updateMaxChargeCurrent(_arg0);
reply.writeNoException();
reply.writeInt(((_result)?(1):(0)));
return true;
}
case TRANSACTION_getDownloadStatusString:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
java.lang.String _result = this.getDownloadStatusString(_arg0);
reply.writeNoException();
reply.writeString(_result);
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements android.os.IOppoUsageService
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
@Override public android.os.IBinder asBinder()
{
return mRemote;
}
public @SuppressWarnings("unused")
java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
/**
	 * Internel use, only for test:
	 */
@Override public void testSaveSomeData(int dataType, java.lang.String dataContent) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(dataType);
_data.writeString(dataContent);
mRemote.transact(Stub.TRANSACTION_testSaveSomeData, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
/**
	 * Get all history boot time record, with format:YMD.H:M:S
	 */
@Override public java.util.List<java.lang.String> getHistoryBootTime() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.util.List<java.lang.String> _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getHistoryBootTime, _data, _reply, 0);
_reply.readException();
_result = _reply.createStringArrayList();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Get all history Imei NO record, include the current Imei NO.
	 */
@Override public java.util.List<java.lang.String> getHistoryImeiNO() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.util.List<java.lang.String> _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getHistoryImeiNO, _data, _reply, 0);
_reply.readException();
_result = _reply.createStringArrayList();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Get all history Pcba NO record, include the current Pcba NO.
	 */
@Override public java.util.List<java.lang.String> getHistoryPcbaNO() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.util.List<java.lang.String> _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getHistoryPcbaNO, _data, _reply, 0);
_reply.readException();
_result = _reply.createStringArrayList();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Get the record count of the app usage history records.
	 */
@Override public int getAppUsageHistoryRecordCount() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getAppUsageHistoryRecordCount, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Get the app usage history records. Use the index params to control the record count when
	 * read the rocords to avoid the return result take too many memory for the reason that the
	 * count of the records maybe too large. So you can get the records in batches.
	 *
	 * Params: as designed, index should meat (startIndex >= 1 && endIndex >= 1 && startIndex <= endIndex).
	 * Return: records list with elememt like this: AppName or pkgName|start time with format YMD,H:M:S.
	 */
@Override public java.util.List<java.lang.String> getAppUsageHistoryRecords(int startIndex, int endIndex) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.util.List<java.lang.String> _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(startIndex);
_data.writeInt(endIndex);
mRemote.transact(Stub.TRANSACTION_getAppUsageHistoryRecords, _data, _reply, 0);
_reply.readException();
_result = _reply.createStringArrayList();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Save app usage history record.
	 * Param appName: the name of the app or it's package name.
	 * Parma dateTime: the date time string of the app start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
@Override public boolean writeAppUsageHistoryRecord(java.lang.String appName, java.lang.String dateTime) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(appName);
_data.writeString(dateTime);
mRemote.transact(Stub.TRANSACTION_writeAppUsageHistoryRecord, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Get or accumulate the history count of msg, sended or received.
	 */
@Override public int getHistoryCountOfSendedMsg() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getHistoryCountOfSendedMsg, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int getHistoryCountOfReceivedMsg() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getHistoryCountOfReceivedMsg, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public boolean accumulateHistoryCountOfSendedMsg(int newCountIncrease) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(newCountIncrease);
mRemote.transact(Stub.TRANSACTION_accumulateHistoryCountOfSendedMsg, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public boolean accumulateHistoryCountOfReceivedMsg(int newCountIncrease) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(newCountIncrease);
mRemote.transact(Stub.TRANSACTION_accumulateHistoryCountOfReceivedMsg, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Get or accumulate the phone call duration records:
	 */
@Override public int getDialOutDuration() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getDialOutDuration, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int getInComingCallDuration() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getInComingCallDuration, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public boolean accumulateDialOutDuration(int durationInMinute) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(durationInMinute);
mRemote.transact(Stub.TRANSACTION_accumulateDialOutDuration, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public boolean accumulateInComingCallDuration(int durationInMinute) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(durationInMinute);
mRemote.transact(Stub.TRANSACTION_accumulateInComingCallDuration, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Get the count of the phone call history records. Include dial out and incoming call.
	 */
@Override public int getHistoryRecordsCountOfPhoneCalls() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getHistoryRecordsCountOfPhoneCalls, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
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
@Override public java.util.List<java.lang.String> getPhoneCallHistoryRecords(int startIndex, int endIndex) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.util.List<java.lang.String> _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(startIndex);
_data.writeInt(endIndex);
mRemote.transact(Stub.TRANSACTION_getPhoneCallHistoryRecords, _data, _reply, 0);
_reply.readException();
_result = _reply.createStringArrayList();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * Save phone call history record, include dial out and incoming call.
	 * Param phoneNoStr: the phone call number string.
	 * Parma dateTime: the date time string of the phone call start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
@Override public boolean writePhoneCallHistoryRecord(java.lang.String phoneNoStr, java.lang.String dateTime) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(phoneNoStr);
_data.writeString(dateTime);
mRemote.transact(Stub.TRANSACTION_writePhoneCallHistoryRecord, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/**
	 * call by system to save shut down time:
	 */
@Override public void shutDown() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_shutDown, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
//JianRong.Zheng@Swdp.Android.Power, 2014/09/12, Add for [], {

@Override public int import_config_init() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_import_config_init, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int import_config_loop(java.lang.String content) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(content);
mRemote.transact(Stub.TRANSACTION_import_config_loop, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int import_config_done() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_import_config_done, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int export_mos_config() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_export_mos_config, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
//} add end for [].

@Override public int getMinChargeTemperature() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getMinChargeTemperature, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int getMaxChargeTemperature() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getMaxChargeTemperature, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int getMaxChargeCurrent() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getMaxChargeCurrent, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public boolean updateMinChargeTemperature(int temp) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(temp);
mRemote.transact(Stub.TRANSACTION_updateMinChargeTemperature, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public boolean updateMaxChargeTemperature(int temp) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(temp);
mRemote.transact(Stub.TRANSACTION_updateMaxChargeTemperature, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public boolean updateMaxChargeCurrent(int current) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
boolean _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(current);
mRemote.transact(Stub.TRANSACTION_updateMaxChargeCurrent, _data, _reply, 0);
_reply.readException();
_result = (0!=_reply.readInt());
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public java.lang.String getDownloadStatusString(int part) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.lang.String _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(part);
mRemote.transact(Stub.TRANSACTION_getDownloadStatusString, _data, _reply, 0);
_reply.readException();
_result = _reply.readString();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
}
static final int TRANSACTION_testSaveSomeData = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_getHistoryBootTime = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
static final int TRANSACTION_getHistoryImeiNO = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
static final int TRANSACTION_getHistoryPcbaNO = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
static final int TRANSACTION_getAppUsageHistoryRecordCount = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
static final int TRANSACTION_getAppUsageHistoryRecords = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
static final int TRANSACTION_writeAppUsageHistoryRecord = (android.os.IBinder.FIRST_CALL_TRANSACTION + 6);
static final int TRANSACTION_getHistoryCountOfSendedMsg = (android.os.IBinder.FIRST_CALL_TRANSACTION + 7);
static final int TRANSACTION_getHistoryCountOfReceivedMsg = (android.os.IBinder.FIRST_CALL_TRANSACTION + 8);
static final int TRANSACTION_accumulateHistoryCountOfSendedMsg = (android.os.IBinder.FIRST_CALL_TRANSACTION + 9);
static final int TRANSACTION_accumulateHistoryCountOfReceivedMsg = (android.os.IBinder.FIRST_CALL_TRANSACTION + 10);
static final int TRANSACTION_getDialOutDuration = (android.os.IBinder.FIRST_CALL_TRANSACTION + 11);
static final int TRANSACTION_getInComingCallDuration = (android.os.IBinder.FIRST_CALL_TRANSACTION + 12);
static final int TRANSACTION_accumulateDialOutDuration = (android.os.IBinder.FIRST_CALL_TRANSACTION + 13);
static final int TRANSACTION_accumulateInComingCallDuration = (android.os.IBinder.FIRST_CALL_TRANSACTION + 14);
static final int TRANSACTION_getHistoryRecordsCountOfPhoneCalls = (android.os.IBinder.FIRST_CALL_TRANSACTION + 15);
static final int TRANSACTION_getPhoneCallHistoryRecords = (android.os.IBinder.FIRST_CALL_TRANSACTION + 16);
static final int TRANSACTION_writePhoneCallHistoryRecord = (android.os.IBinder.FIRST_CALL_TRANSACTION + 17);
static final int TRANSACTION_shutDown = (android.os.IBinder.FIRST_CALL_TRANSACTION + 18);
static final int TRANSACTION_import_config_init = (android.os.IBinder.FIRST_CALL_TRANSACTION + 19);
static final int TRANSACTION_import_config_loop = (android.os.IBinder.FIRST_CALL_TRANSACTION + 20);
static final int TRANSACTION_import_config_done = (android.os.IBinder.FIRST_CALL_TRANSACTION + 21);
static final int TRANSACTION_export_mos_config = (android.os.IBinder.FIRST_CALL_TRANSACTION + 22);
static final int TRANSACTION_getMinChargeTemperature = (android.os.IBinder.FIRST_CALL_TRANSACTION + 23);
static final int TRANSACTION_getMaxChargeTemperature = (android.os.IBinder.FIRST_CALL_TRANSACTION + 24);
static final int TRANSACTION_getMaxChargeCurrent = (android.os.IBinder.FIRST_CALL_TRANSACTION + 25);
static final int TRANSACTION_updateMinChargeTemperature = (android.os.IBinder.FIRST_CALL_TRANSACTION + 26);
static final int TRANSACTION_updateMaxChargeTemperature = (android.os.IBinder.FIRST_CALL_TRANSACTION + 27);
static final int TRANSACTION_updateMaxChargeCurrent = (android.os.IBinder.FIRST_CALL_TRANSACTION + 28);
static final int TRANSACTION_getDownloadStatusString = (android.os.IBinder.FIRST_CALL_TRANSACTION + 29);
}
/**
	 * Internel use, only for test:
	 */
public void testSaveSomeData(int dataType, java.lang.String dataContent) throws android.os.RemoteException;
/**
	 * Get all history boot time record, with format:YMD.H:M:S
	 */
public java.util.List<java.lang.String> getHistoryBootTime() throws android.os.RemoteException;
/**
	 * Get all history Imei NO record, include the current Imei NO.
	 */
public java.util.List<java.lang.String> getHistoryImeiNO() throws android.os.RemoteException;
/**
	 * Get all history Pcba NO record, include the current Pcba NO.
	 */
public java.util.List<java.lang.String> getHistoryPcbaNO() throws android.os.RemoteException;
/**
	 * Get the record count of the app usage history records.
	 */
public int getAppUsageHistoryRecordCount() throws android.os.RemoteException;
/**
	 * Get the app usage history records. Use the index params to control the record count when
	 * read the rocords to avoid the return result take too many memory for the reason that the
	 * count of the records maybe too large. So you can get the records in batches.
	 *
	 * Params: as designed, index should meat (startIndex >= 1 && endIndex >= 1 && startIndex <= endIndex).
	 * Return: records list with elememt like this: AppName or pkgName|start time with format YMD,H:M:S.
	 */
public java.util.List<java.lang.String> getAppUsageHistoryRecords(int startIndex, int endIndex) throws android.os.RemoteException;
/**
	 * Save app usage history record.
	 * Param appName: the name of the app or it's package name.
	 * Parma dateTime: the date time string of the app start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
public boolean writeAppUsageHistoryRecord(java.lang.String appName, java.lang.String dateTime) throws android.os.RemoteException;
/**
	 * Get or accumulate the history count of msg, sended or received.
	 */
public int getHistoryCountOfSendedMsg() throws android.os.RemoteException;
public int getHistoryCountOfReceivedMsg() throws android.os.RemoteException;
public boolean accumulateHistoryCountOfSendedMsg(int newCountIncrease) throws android.os.RemoteException;
public boolean accumulateHistoryCountOfReceivedMsg(int newCountIncrease) throws android.os.RemoteException;
/**
	 * Get or accumulate the phone call duration records:
	 */
public int getDialOutDuration() throws android.os.RemoteException;
public int getInComingCallDuration() throws android.os.RemoteException;
public boolean accumulateDialOutDuration(int durationInMinute) throws android.os.RemoteException;
public boolean accumulateInComingCallDuration(int durationInMinute) throws android.os.RemoteException;
/**
	 * Get the count of the phone call history records. Include dial out and incoming call.
	 */
public int getHistoryRecordsCountOfPhoneCalls() throws android.os.RemoteException;
/**
	 * Get the phone call history records include dial out and incoming call. Use the index params 
	 * to control the record count when read the rocords to avoid the return result take too many
	 * memory for the reason that the count of the records maybe too large.
	 * So you can get the records in batches.
	 * 
	 * Params: as designed, index should meat (startIndex >= 1 && endIndex >= 1 && startIndex <= endIndex).
	 * Return: records list with elememt like this: phoneNo|occur time with format YMD,H:M:S.
	 */
public java.util.List<java.lang.String> getPhoneCallHistoryRecords(int startIndex, int endIndex) throws android.os.RemoteException;
/**
	 * Save phone call history record, include dial out and incoming call.
	 * Param phoneNoStr: the phone call number string.
	 * Parma dateTime: the date time string of the phone call start time, recomend get from
	 * DateFormat.getDateTimeInstance().format(Date date). If empty, will use current time as default.
	 *
	 * Return result: return true if save successful, false if failed.
	 */
public boolean writePhoneCallHistoryRecord(java.lang.String phoneNoStr, java.lang.String dateTime) throws android.os.RemoteException;
/**
	 * call by system to save shut down time:
	 */
public void shutDown() throws android.os.RemoteException;
//JianRong.Zheng@Swdp.Android.Power, 2014/09/12, Add for [], {

public int import_config_init() throws android.os.RemoteException;
public int import_config_loop(java.lang.String content) throws android.os.RemoteException;
public int import_config_done() throws android.os.RemoteException;
public int export_mos_config() throws android.os.RemoteException;
//} add end for [].

public int getMinChargeTemperature() throws android.os.RemoteException;
public int getMaxChargeTemperature() throws android.os.RemoteException;
public int getMaxChargeCurrent() throws android.os.RemoteException;
public boolean updateMinChargeTemperature(int temp) throws android.os.RemoteException;
public boolean updateMaxChargeTemperature(int temp) throws android.os.RemoteException;
public boolean updateMaxChargeCurrent(int current) throws android.os.RemoteException;
public java.lang.String getDownloadStatusString(int part) throws android.os.RemoteException;
}
