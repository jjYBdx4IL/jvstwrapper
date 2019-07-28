package org.tango.JaVaSTyle;

/**
 * <p>Titre : JaVaSTyle</p>
 * <p>Description : A Java Interface for hosting VST plugins</p>
 * <p>Copyright : Copyright (c) Tango 2003</p>
 * <p>Soci�t� : Tango</p>
 * @author non attribuable
 * @version 1.0
 */

public interface JVSTHost {

  public long getCallback();

  public void setParameterAutomated(JVSTPlugin plug, int index, int value);
  public int getMasterVersion(JVSTPlugin plug);
  public int getCurrentUniqueId(JVSTPlugin plug);
  public void masterIdle(JVSTPlugin plug);
  public boolean isInputConnected(JVSTPlugin plug, int pin);
}