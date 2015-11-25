/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: /work/Primary_EngineeringMode/src/android/os/IOppoService.aidl
 */
package android.os;
/**
 * @hide
 * @author clee
 * licx@OnLineRD.framework.oppo_service
 */
public interface IOppoService extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements android.os.IOppoService
{
private static final java.lang.String DESCRIPTOR = "android.os.IOppoService";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an android.os.IOppoService interface,
 * generating a proxy if needed.
 */
public static android.os.IOppoService asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof android.os.IOppoService))) {
return ((android.os.IOppoService)iin);
}
return new android.os.IOppoService.Stub.Proxy(obj);
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
case TRANSACTION_readRawPartition:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
int _arg1;
_arg1 = data.readInt();
java.lang.String _result = this.readRawPartition(_arg0, _arg1);
reply.writeNoException();
reply.writeString(_result);
return true;
}
case TRANSACTION_writeRawPartition:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
int _result = this.writeRawPartition(_arg0);
reply.writeNoException();
reply.writeInt(_result);
return true;
}
case TRANSACTION_readCriticalData:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
int _arg1;
_arg1 = data.readInt();
java.lang.String _result = this.readCriticalData(_arg0, _arg1);
reply.writeNoException();
reply.writeString(_result);
return true;
}
case TRANSACTION_writeCriticalData:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
java.lang.String _arg1;
_arg1 = data.readString();
int _result = this.writeCriticalData(_arg0, _arg1);
reply.writeNoException();
reply.writeInt(_result);
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements android.os.IOppoService
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
/******************************************************
     * 
     * This part is for writing raw partition
     * 
     */
@Override public java.lang.String readRawPartition(int offset, int size) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.lang.String _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(offset);
_data.writeInt(size);
mRemote.transact(Stub.TRANSACTION_readRawPartition, _data, _reply, 0);
_reply.readException();
_result = _reply.readString();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int writeRawPartition(java.lang.String content) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(content);
mRemote.transact(Stub.TRANSACTION_writeRawPartition, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
/******************************************************
     * 
     * This part is for writing critical data
     * 
     */
@Override public java.lang.String readCriticalData(int id, int size) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.lang.String _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(id);
_data.writeInt(size);
mRemote.transact(Stub.TRANSACTION_readCriticalData, _data, _reply, 0);
_reply.readException();
_result = _reply.readString();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public int writeCriticalData(int id, java.lang.String content) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
int _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(id);
_data.writeString(content);
mRemote.transact(Stub.TRANSACTION_writeCriticalData, _data, _reply, 0);
_reply.readException();
_result = _reply.readInt();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
}
static final int TRANSACTION_readRawPartition = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_writeRawPartition = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
static final int TRANSACTION_readCriticalData = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
static final int TRANSACTION_writeCriticalData = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
}
/******************************************************
     * 
     * This part is for writing raw partition
     * 
     */
public java.lang.String readRawPartition(int offset, int size) throws android.os.RemoteException;
public int writeRawPartition(java.lang.String content) throws android.os.RemoteException;
/******************************************************
     * 
     * This part is for writing critical data
     * 
     */
public java.lang.String readCriticalData(int id, int size) throws android.os.RemoteException;
public int writeCriticalData(int id, java.lang.String content) throws android.os.RemoteException;
}
