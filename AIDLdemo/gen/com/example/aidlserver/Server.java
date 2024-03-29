/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: D:\\Program Files (x86)\\adt-bundle-windows-x86_64-20140702\\wordspace\\AIDLdemo\\src\\com\\example\\aidlserver\\Server.aidl
 */
package com.example.aidlserver;
public interface Server extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.example.aidlserver.Server
{
private static final java.lang.String DESCRIPTOR = "com.example.aidlserver.Server";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an com.example.aidlserver.Server interface,
 * generating a proxy if needed.
 */
public static com.example.aidlserver.Server asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.example.aidlserver.Server))) {
return ((com.example.aidlserver.Server)iin);
}
return new com.example.aidlserver.Server.Stub.Proxy(obj);
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
case TRANSACTION_getContent:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _result = this.getContent();
reply.writeNoException();
reply.writeString(_result);
return true;
}
case TRANSACTION_modifyContent:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
java.lang.String _result = this.modifyContent(_arg0);
reply.writeNoException();
reply.writeString(_result);
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.example.aidlserver.Server
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
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
@Override public java.lang.String getContent() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.lang.String _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_getContent, _data, _reply, 0);
_reply.readException();
_result = _reply.readString();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public java.lang.String modifyContent(java.lang.String org) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.lang.String _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(org);
mRemote.transact(Stub.TRANSACTION_modifyContent, _data, _reply, 0);
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
static final int TRANSACTION_getContent = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_modifyContent = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
}
public java.lang.String getContent() throws android.os.RemoteException;
public java.lang.String modifyContent(java.lang.String org) throws android.os.RemoteException;
}
