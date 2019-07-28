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

package jvst.wrapper;


import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.text.DecimalFormat;

import jvst.wrapper.communication.VSTV24ToHost;
import jvst.wrapper.communication.VSTV24ToPlug;
import jvst.wrapper.valueobjects.MidiKeyName;
import jvst.wrapper.valueobjects.MidiProgramCategory;
import jvst.wrapper.valueobjects.MidiProgramName;
import jvst.wrapper.valueobjects.VSTEvents;
import jvst.wrapper.valueobjects.VSTParameterProperties;
import jvst.wrapper.valueobjects.VSTPinProperties;
import jvst.wrapper.valueobjects.VSTSpeakerArrangement;
import jvst.wrapper.valueobjects.VSTVariableIO;

/**
 *  Adapter to ignore VST 1.0, VST 2.0, VST 2.1, VST 2.2, VST 2.3, VST 2.4
 *  OPTIONAL methods --> return defaults
 *  and some utility methods (number conversions...)
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */
public abstract class VSTPluginAdapter extends VSTV24ToHost implements VSTV24ToPlug {


  //C++ pointer to the plugin instance
  protected long WrapperInstance = 0;
  
  private static String logBasePath = "c:/";
  private static String logFileName = "JAVA_STD_OUT.txt";
  private static boolean logEnabled = false;
  
  
  
  //VST Constants
  //************************************
  public static final int PAN_LAW_LINEAR = 0;	  // L = pan * M; R = (1 - pan) * M;
  public static final int PAN_LAW_EQUAL_POWER = 1;// L = pow (pan, 0.5) * M; R = pow ((1 - pan), 0.5) * M;

  
  public VSTPluginAdapter(long Wrapper) {
  	this.WrapperInstance=Wrapper;
  	
  	//since processReplacing is MANDATORY from 2.4 on
  	canProcessReplacing(true);
  	
  	//were vst2.4 compatible: no Vu, no Clip
  	hasVu(false); hasClip(false);
  }


  
  //*** process is DEPRECATED since 2.4! ***
  //--> make it optional and mark as deprecated
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   * 
   * USE processReplacing() and/or processDoubleReplacing() INSTEAD!
   */
  public void process(float[][] inputs, float[][] outputs, int sampleFrames){}
  
  
  
  //provide defaults for vst 1.0 OPTIONAL methods
  //*************************************
  public void open() { }
  public void close() { }
  public void suspend() { }
  public void resume() { }
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public float getVu() {return 0F;}
  public int getChunk(byte[][] data, boolean isPreset) {return 0;}
  public int setChunk(byte data[], int byteSize, boolean isPreset) {return 0;}
  public void setBlockSize(int blockSize) { }
  public void setSampleRate(float sampleRate) { }


  //provide defaults for vst 2.0 OPTIONAL methods
  //********************************
  public String getEffectName() {return "nix";}
  public int getVendorVersion() {return 1000;}
  public boolean canParameterBeAutomated(int index) {return true;}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public boolean copyProgram(int destination) {return false;}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public int fxIdle() {return 0;}
  public float getChannelParameter(int channel, int index) {return 0.F;}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public int getNumCategories() {return 1;}

  public VSTPinProperties getInputProperties(int index) {return null;}
  public VSTPinProperties getOutputProperties(int index) {return null;}
  
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public String getErrorText() {return null;}
  public int getGetTailSize() {return 0;}
  public VSTParameterProperties getParameterProperties(int index) {return null;}

  public int getVstVersion () {return 2400;} 
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public void inputConnected (int index, boolean state) {}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public void outputConnected (int index, boolean state) {}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public boolean keysRequired () {return false;}

  public int processEvents (VSTEvents e) {return 0;}
  public boolean processVariableIo (VSTVariableIO vario) {return false;}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public int reportCurrentPosition () {return 0;}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public float[] reportDestinationBuffer () {return new float[]{0.F};}
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public void setBlockSizeAndSampleRate(int blockSize, float sampleRate) {}

  public boolean setSpeakerArrangement (VSTSpeakerArrangement pluginInput, VSTSpeakerArrangement pluginOutput) {
    return false;
  }
  public boolean getSpeakerArrangement (VSTSpeakerArrangement pluginInput, VSTSpeakerArrangement pluginOutput) {
	return false;
  }

  //provide defaults for vst 2.1 OPTIONAL methods
  //********************************
  public int getMidiProgramName (int channel, MidiProgramName midiProgramName) { return 0; }
  public int getCurrentMidiProgram(int channel, MidiProgramName currentProgram) { return -1; }
  public int getMidiProgramCategory(int channel,MidiProgramCategory category) { return 0; }
  public boolean hasMidiProgramsChanged(int channel) { return false; }
  public boolean getMidiKeyName(int channel, MidiKeyName keyName) { return false; }
  public boolean beginSetProgram() { return false;}
  public boolean endSetProgram() { return false;}


  //provide defaults for vst 2.3 OPTIONAL methods
  //********************************
  public int setTotalSampleToProcess (int value) { return value; }
  public int getNextShellPlugin(String name) { return 0; }
  public int startProcess() { return 0;}
  public int stopProcess() { return 0;}


  //provide defaults for vst 2.4 OPTIONAL methods
  //********************************
  public void processDoubleReplacing (double[][] inputs, double[][] outputs, int sampleFrames) {}
  
  public boolean setProcessPrecision (int precision){return false;}
  public int getNumMidiInputChannels(){return 0;}
  public int getNumMidiOutputChannels(){return 0;}

  

  //utility
  //***********************************************************************

  protected String dbToString(float value) {
    String ret;
    DecimalFormat form = new DecimalFormat("#0.0");

    if (value <= 0F) ret = "-oo";
    else ret = form.format(20.0F * (float) (Math.log(value) / Math.log(10)) ); //log10(value)!

    return ret;
  }

  protected String hzToString(float value, int sampleRate) {
    String ret;
    DecimalFormat form = new DecimalFormat("#0");

    if (value != 0F) ret = form.format((float)sampleRate / value);
    else ret = "0";

    return ret;
  }

  protected String msToString(float value, int sampleRate) {
    String ret;
    DecimalFormat form = new DecimalFormat("#0");

    if (sampleRate!=0) ret = form.format(value * 1000.0F / (float)sampleRate);
    else ret = "0";

    return ret;
  }

  protected String formatFloatParam(float value) {
    DecimalFormat form = new DecimalFormat("#0.0#");
    return form.format(value);
  }  
  
  
  
  //jVSTwRapper java-side init and log stuff
  //**************

  /**
   *  is called by the plugin wrapper lib BEFORE plugins construktor!
   */
  protected static void _initPlugFromNative(String dllLocation, boolean log) {
    File nativeLib = new File(dllLocation);
  	if (nativeLib.isDirectory() || !nativeLib.exists()) 
  		throw new IllegalArgumentException("Illegal native lib location: '" + dllLocation + "'");
    
  	logBasePath = nativeLib.getParent();
    logEnabled = log;

    String dllFileName = nativeLib.getName();
    int point = dllFileName.lastIndexOf(".");
    if (point == -1) {
      throw new IllegalStateException("Could not find '.' in dllFileName " + dllFileName);
    }
    String LibName = dllFileName.substring(0, point);
    logFileName = LibName + "_java_stdout.txt";

    try {
      File logf = new File(new File(logBasePath), logFileName);
      PrintStream str = new PrintStream(new BufferedOutputStream(new FileOutputStream(logf, true)));
      System.setOut(str);
      System.setErr(str);
    }
    catch (Exception e) {
      e.printStackTrace();
      throw new IllegalStateException(e.toString());
    }

    log("registering native Library '" + dllLocation + "'");
    System.load(dllLocation); //this load() gets delegated to the VSTiClassLoader, 
    						  //so that native calls get dispatched correctly!
  }
  

  /**
   * logging
   */
  public static void log(String s) {
    if (isLogEnabled()) {
      System.out.println(s);
      System.out.flush();
    }
  }

  protected static boolean isLogEnabled() {
    return logEnabled;
  }
  
  protected static void setIsLogEnabled(boolean b) {
	logEnabled=b;
  }

  protected static String getLogBasePath() {
    return logBasePath;
  }

  protected static String getLogFileName() {
    return logFileName;
  }
  
}
