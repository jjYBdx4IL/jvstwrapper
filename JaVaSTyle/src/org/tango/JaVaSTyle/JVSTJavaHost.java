package org.tango.JaVaSTyle;

/**
 * <p>Titre : JaVaSTyle</p>
 * <p>Description : A Java Interface for hosting VST plugins</p>
 * <p>Copyright : Copyright (c) Tango 2003</p>
 * <p>Soci�t� : Tango</p>
 * @author non attribuable
 * @version 1.0
 */

public abstract class JVSTJavaHost implements JVSTHost {

  private long callback;

  private static native void initJNI();
  private native long declareHost();

  public long getCallback() {
    return callback;
  }

  static {
    System.loadLibrary("JVSTLib");
    initJNI();
  }

  public JVSTJavaHost() {
    callback = declareHost();
  }
}