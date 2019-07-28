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


import jvst.wrapper.valueobjects.*;


/**
 *  VST 2.0 Plug -> Host Communication
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */


public abstract class VSTV20ToHost extends VSTV10ToHost {

  //plugin canDo return values
  public static final int CANDO_DONT_KNOW = 0; //default
  public static final int CANDO_YES = 1;
  public static final int CANDO_NO = -1;

  //Plugin canDo parameters
  public static final String CANDO_PLUG_SEND_VST_EVENTS = "sendVstEvents";
  public static final String CANDO_PLUG_SEND_VST_MIDI_EVENT = "sendVstMidiEvent";
  public static final String CANDO_PLUG_SEND_VST_TIME_INFO = "sendVstTimeInfo";
  public static final String CANDO_PLUG_RECEIVE_VST_EVENTS = "receiveVstEvents";
  public static final String CANDO_PLUG_RECEIVE_VST_MIDI_EVENT = "receiveVstMidiEvent";
  public static final String CANDO_PLUG_REVEIVE_VST_MIDI_EVENT = "receiveVstMidiEvent"; //keep TYPO for backward compatibility!
  public static final String CANDO_PLUG_RECEIVE_VST_TIME_INFO = "receiveVstTimeInfo";
  public static final String CANDO_PLUG_OFFLINE = "offline";
  public static final String CANDO_PLUG_PLUG_AS_CHANNEL_INSERT = "plugAsChannelInsert";
  public static final String CANDO_PLUG_PLUG_AS_SEND = "plugAsSend";
  public static final String CANDO_PLUG_MIX_DRY_WET = "mixDryWet";
  public static final String CANDO_PLUG_NO_REAL_TIME = "noRealTime";
  public static final String CANDO_PLUG_MULTIPASS = "multipass";
  public static final String CANDO_PLUG_METAPASS = "metapass";
  public static final String CANDO_PLUG_1_IN_1_OUT = "1in1out";
  public static final String CANDO_PLUG_1_IN_2_OUT = "1in2out";
  public static final String CANDO_PLUG_2_IN_1_OUT = "2in1out";
  public static final String CANDO_PLUG_2_IN_2_OUT = "2in2out";
  public static final String CANDO_PLUG_2_IN_4_OUT = "2in4out";
  public static final String CANDO_PLUG_4_IN_2_OUT = "4in2out";
  public static final String CANDO_PLUG_4_IN_4_OUT = "4in4out";
  public static final String CANDO_PLUG_4_IN_8_OUT = "4in8out";	// 4:2 matrix to surround bus
  public static final String CANDO_PLUG_8_IN_4_OUT = "8in4out";	// surround bus to 4:2 matrix
  public static final String CANDO_PLUG_8_IN_8_OUT = "8in8out";
  //VST 2.1
  public static final String CANDO_PLUG_MIDI_PROGRAM_NAMES = "midiProgramNames";
  public static final String CANDO_PLUG_CONFORMS_TO_WINDOW_RULES = "conformsToWindowRules";	// mac: doesn't mess with grafport. general: may want
  //VST 2.3
  public static final String CANDO_PLUG_BYPASS = "bypass";


  //Languages
  public static final int VST_LANG_ENGLISH = 1;
  public static final int VST_LANG_GERMAN = 2;
  public static final int VST_LANG_FRENCH = 3;
  public static final int VST_LANG_ITALIAN = 4;
  public static final int VST_LANG_SPANISH = 5;
  public static final int VST_LANG_JAPANESE = 6;




  //********************
  //*** Plug -> Host ***
  //********************

  //vst 2.0 REQUIRED
  //****************
  //Nothing


  //vst 2.0 OPTIONAL
  //****************
  public native int canHostDo(String what);

//We have our own UI handling
/*
 public native int closeWindow(); //deprecated since 2.4 anyways!
 public native int openWindow(); //deprecated since 2.4 anyways!
*/
 public native boolean sizeWindow(int width, int height);


  public native boolean ioChanged();
  public native VSTTimeInfo getTimeInfo(int filter);
  public native boolean sendVstEventsToHost(VSTEvents events);
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native boolean needIdle();
  
  public native double updateSampleRate();
  public native int updateBlockSize();
  public native int getAutomationState();
  //0 : Not supported.
  //1 : Off.
  //2 : Read.
  //3 : Write.
  //4 : Read/write.

  public native int getCurrentProcessLevel();
  //0 : Not supported.
  //1 : Currently in user thread (gui).
  //2 : Currently in audio thread or irq (where process is called).
  //3 : Currently in 'sequencer' thread or irq (midi, timer etc).
  //4 : Currently offline processing and thus in user thread.
  //other : Not defined, but probably pre-empting user thread.


  public native String getDirectory();

  public native String getHostProductString();
  public native int getHostVendorVersion();
  public native String getHostVendorString();
  public native int getHostLanguage();


  public native int getInputLatency();
  public native int getOutputLatency();

  //not supported, cant handle AEffect* pointers --> maybe return a new wrapper instance ...
  //nah! dont need that. deprecated since 2.4 anyways!
  /*
  public native int getNextPlug();
  public native int getPreviousPlug();
  */

  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native int getNumAutomatableParameters();



  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native int getParameterQuantization();
  //0 : Not implemented.
  //1 : Full single float precision is maintained in automation.
  //other : The integer value that represents +1.0.

  
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void hasExternalBuffer(boolean state);

// not supported, void* parameter!
//public native int hostVendorSpecific();

  public native void isSynth(boolean state);
  public native void noTail(boolean state);

  //VST offline interface is currently not supported!
  /*
  public native int offlineGetCurrentMetaPass();
  public native int offlineGetCurrentPass();
  public native int offlineRead();
  public native int offlineStart();
  public native int offlineWrite();
  */
  
  
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void setOutputSamplerate(float rate);
  
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native int tempoAt(int position);
  public native boolean updateDisplay();
  
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void wantAsyncOperation(boolean state);

/*
 Asks the host to send VstEvents, as would request an instrument.
 This call can't be issued at construction time,
 it should be done in the resume method instead.
*/
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native void wantEvents(int filter);
  
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public native int willProcessReplacing();
  //0 : Not implemented. we don't know whether process or processReplacing is called.
  //1 : Indicates that host will call processReplacing .
  //other : process (accumulating) will be used.

}
