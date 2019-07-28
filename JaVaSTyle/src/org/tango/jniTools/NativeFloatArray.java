package org.tango.jniTools;

/**
 * <p>Titre : JaVaSTyle</p>
 * <p>Description : A Java Interface for hosting VST plugins</p>
 * <p>Copyright : Copyright (c) Tango 2003</p>
 * <p>Société : Tango</p>
 * @author Greg
 * @version 1.0
 */

public class NativeFloatArray {

  long ptr;
  int size;

  private static native void initJNI();

  public native float get(int index);
  public native boolean copyToJava(float[] dest);
  public native boolean copyFromJava(float[] dest);
  public native boolean copyToJava(float[] dest, int startN, int startJ, int length);
  public native boolean copyFromJava(float[] dest, int startN, int startJ, int end);

  static {
    System.loadLibrary("JNItools");
    initJNI();
  }

  public NativeFloatArray() {
  }

}