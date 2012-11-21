/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package djondb;

public class DjondbConnection {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public DjondbConnection(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(DjondbConnection obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        djonwrapperJNI.delete_DjondbConnection(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public DjondbConnection(String host) {
    this(djonwrapperJNI.new_DjondbConnection__SWIG_0(host), true);
  }

  public DjondbConnection(String host, int port) {
    this(djonwrapperJNI.new_DjondbConnection__SWIG_1(host, port), true);
  }

  public DjondbConnection(DjondbConnection orig) {
    this(djonwrapperJNI.new_DjondbConnection__SWIG_2(DjondbConnection.getCPtr(orig), orig), true);
  }

  public boolean open() {
    return djonwrapperJNI.DjondbConnection_open(swigCPtr, this);
  }

  public void close() {
    djonwrapperJNI.DjondbConnection_close(swigCPtr, this);
  }

  public void internalClose() {
    djonwrapperJNI.DjondbConnection_internalClose(swigCPtr, this);
  }

  public boolean isOpen() {
    return djonwrapperJNI.DjondbConnection_isOpen(swigCPtr, this);
  }

  public boolean shutdown() {
    return djonwrapperJNI.DjondbConnection_shutdown(swigCPtr, this);
  }

  public boolean insert(String db, String ns, String json) {
    return djonwrapperJNI.DjondbConnection_insert__SWIG_0(swigCPtr, this, db, ns, json);
  }

  public boolean insert(String db, String ns, BSONObj obj) {
    return djonwrapperJNI.DjondbConnection_insert__SWIG_1(swigCPtr, this, db, ns, BSONObj.getCPtr(obj), obj);
  }

  public BSONObj findByKey(String db, String ns, String select, String id) {
    long cPtr = djonwrapperJNI.DjondbConnection_findByKey__SWIG_0(swigCPtr, this, db, ns, select, id);
    return (cPtr == 0) ? null : new BSONObj(cPtr, false);
  }

  public BSONObj findByKey(String db, String ns, String id) {
    long cPtr = djonwrapperJNI.DjondbConnection_findByKey__SWIG_1(swigCPtr, this, db, ns, id);
    return (cPtr == 0) ? null : new BSONObj(cPtr, false);
  }

  public BSONObjVectorPtr find(String db, String ns, String select, String filter) {
    long cPtr = djonwrapperJNI.DjondbConnection_find__SWIG_0(swigCPtr, this, db, ns, select, filter);
    return (cPtr == 0) ? null : new BSONObjVectorPtr(cPtr, false);
  }

  public BSONObjVectorPtr find(String db, String ns, String filter) {
    long cPtr = djonwrapperJNI.DjondbConnection_find__SWIG_1(swigCPtr, this, db, ns, filter);
    return (cPtr == 0) ? null : new BSONObjVectorPtr(cPtr, false);
  }

  public boolean update(String db, String ns, String json) {
    return djonwrapperJNI.DjondbConnection_update__SWIG_0(swigCPtr, this, db, ns, json);
  }

  public boolean update(String db, String ns, BSONObj bson) {
    return djonwrapperJNI.DjondbConnection_update__SWIG_1(swigCPtr, this, db, ns, BSONObj.getCPtr(bson), bson);
  }

  public boolean remove(String db, String ns, String id, String revision) {
    return djonwrapperJNI.DjondbConnection_remove(swigCPtr, this, db, ns, id, revision);
  }

  public boolean dropNamespace(String db, String ns) {
    return djonwrapperJNI.DjondbConnection_dropNamespace(swigCPtr, this, db, ns);
  }

  public StringVector dbs() {
    long cPtr = djonwrapperJNI.DjondbConnection_dbs(swigCPtr, this);
    return (cPtr == 0) ? null : new StringVector(cPtr, false);
  }

  public StringVector namespaces(String db) {
    long cPtr = djonwrapperJNI.DjondbConnection_namespaces(swigCPtr, this, db);
    return (cPtr == 0) ? null : new StringVector(cPtr, false);
  }

  public String host() {
    return djonwrapperJNI.DjondbConnection_host(swigCPtr, this);
  }

}
