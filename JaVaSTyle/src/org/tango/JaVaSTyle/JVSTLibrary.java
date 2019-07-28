package org.tango.JaVaSTyle;
import java.util.*;

/**
 * <p>Title: JaVaSTyle</p>
 * <p>Description: Java interface for VST plugin hosting</p>
 * <p>Copyright: Copyright (c) Tango 2003</p>
 * <p>Company: Tango</p>
 * @author Greg
 * @version 1.0
 */

public class JVSTLibrary {

  private HashMap theLibrarian;

  private native long nativeLoadVST(String libName);
  private native boolean nativeUnloadVST(long handle);

  static {
    System.loadLibrary("JVSTlib");
  }

  public JVSTLibrary() {
    theLibrarian = new HashMap();
  }

  public long loadVST(String libName) {
    long handle = nativeLoadVST(libName);
    theLibrarian.put(libName, new Long(handle));
    return handle;
  }

  public long getHandle(String libName) {
    return ((Long)theLibrarian.get(libName)).longValue();
  }

  public boolean unloadVST(long handle) {
    return nativeUnloadVST(handle);
  }

  public boolean unloadVST(String libName) {
    if (theLibrarian.containsKey(libName)) {
      return unloadVST(((Long) theLibrarian.get(libName)).longValue());
    } else {
      return false;
    }
  }

  public JVSTPlugin instanciate(JVSTHost host, String libName) {
    return new JVSTPlugin(host, getHandle(libName));
  }
}