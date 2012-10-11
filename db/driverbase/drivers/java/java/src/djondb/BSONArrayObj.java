/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package djondb;

public class BSONArrayObj {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public BSONArrayObj(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(BSONArrayObj obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        djonwrapperJNI.delete_BSONArrayObj(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public BSONArrayObj() {
    this(djonwrapperJNI.new_BSONArrayObj__SWIG_0(), true);
  }

  public BSONArrayObj(BSONArrayObj orig) {
    this(djonwrapperJNI.new_BSONArrayObj__SWIG_1(BSONArrayObj.getCPtr(orig), orig), true);
  }

  public int length() {
    return djonwrapperJNI.BSONArrayObj_length(swigCPtr, this);
  }

  public void add(BSONObj obj) {
    djonwrapperJNI.BSONArrayObj_add(swigCPtr, this, BSONObj.getCPtr(obj), obj);
  }

  public BSONObj get(int index) {
    long cPtr = djonwrapperJNI.BSONArrayObj_get(swigCPtr, this, index);
    return (cPtr == 0) ? null : new BSONObj(cPtr, false);
  }

  public String toChar() {
    return djonwrapperJNI.BSONArrayObj_toChar(swigCPtr, this);
  }

  public BSONArrayObj select(String select) {
    long cPtr = djonwrapperJNI.BSONArrayObj_select(swigCPtr, this, select);
    return (cPtr == 0) ? null : new BSONArrayObj(cPtr, false);
  }

  public SWIGTYPE_p_std__vectorT_BSONObj_p_t__iterator begin() {
    return new SWIGTYPE_p_std__vectorT_BSONObj_p_t__iterator(djonwrapperJNI.BSONArrayObj_begin(swigCPtr, this), true);
  }

  public SWIGTYPE_p_std__vectorT_BSONObj_p_t__iterator end() {
    return new SWIGTYPE_p_std__vectorT_BSONObj_p_t__iterator(djonwrapperJNI.BSONArrayObj_end(swigCPtr, this), true);
  }

}
