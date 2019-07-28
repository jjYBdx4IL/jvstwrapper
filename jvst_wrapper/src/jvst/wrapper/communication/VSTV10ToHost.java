/* 
 * jVSTwRapper - The Java way into VST world!
 * 
 * jVSTwRapper is an easy and reliable Java Wrapper for the Steinberg VST interface. 
 * It enables you to develop VST 2.3 compatible audio plugins and virtual instruments 
 * plus user interfaces with the Java Programming Language. 3 Demo Plugins(+src) are included!
 * 
 * Copyright (C) 2006  Daniel Martin [daniel309@users.sourceforge.net] 
 * 					   and many others, see CREDITS.txt
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

package jvst.wrapper.communication;

import java.io.*;


/**
 *  VST 1.0 Plug -> Host Communication
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */
public abstract class VSTV10ToHost  {


  //vst 1.0 REQUIRED
  //****************
  //NOTHING


  //vst 1.0 TOOLS
  //****************
  public native void setUniqueID(int id);
  public native void setNumInputs(int num);
  public native void setNumOutputs(int num);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void hasVu(boolean state);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void hasClip(boolean state);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void canMono(boolean state);
  public native void canProcessReplacing(boolean state);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void setRealtimeQualities(int qualities);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void setOfflineQualities(int qualities);
  public native void setInitialDelay(int delay);
  public native void programsAreChunks(boolean state);
  public native float getSampleRate();
  public native int getBlockSize();


  //vst1.0 OPTIONAL
  //****************
  public native void setParameterAutomated(int index, float value);
  public native int getMasterVersion();
  public native int getCurrentUniqueId();
  public native void masterIdle();
  public native boolean isInputConnected(int input);
  public native boolean isOutputConnected(int output);

}
