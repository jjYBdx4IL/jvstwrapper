package org.tango.JaVaSTyle;
import org.tango.JaVaSTyle.*;
import org.tango.jniTools.*;

/**
 * <p>Title: JaVaSTyle</p>
 * <p>Description: Java interface for VST plugin hosting</p>
 * <p>Copyright: Copyright (c) Tango 2003</p>
 * <p>Company: Tango</p>
 * @author Greg
 * @version 1.0
 */

public class JVSTPlugin {

  private long libHandle;
  private long aEffectPtr;
  private JVSTHost host;

  private native long instanciate(JVSTHost host, long libHandle);

  // VERSION 1.0 Methods
  public native void process(
      NativePointer inputs,
      NativePointer outputs,
      int sampleFrames);
  public native void processReplacing(
      NativePointer inputs,
      NativePointer outputs,
      int sampleFrames);
  public native void setParameter(int index, float value);
  public native float getParameter(int index);
  public native void getParameterDisplay(int index, StringBuffer text);
  public native void getParameterName(int index, StringBuffer text);
  public native void getParameterLabel(int index, StringBuffer label);
  public native int getProgram();
  public native void setProgram(int prog);
  public native void setProgramName(String name);
  public native void getProgramName(StringBuffer name);
  public native int dispatcher(
      int opdcode,
      int index,
      int value,
      NativePointer ptr,
      float opt);
  public native void open();
  public native void close();
  public native float getVu();
  public native int getChunk(NativePointer data, boolean isPreset);
  public native int setChunk(NativePointer data, int byteSize, boolean isPreset);
  public int getChunk(NativePointer data) {
    return getChunk(data, false);
  }
  public int setChunk(NativePointer data, int bytesize) {
    return setChunk(data, bytesize, false);
  }
  public native void setSampleRate(float sampleRate);
  public native void setBlockSize(int blocksize);
  public native void suspend();
  public native void resume();


  // VERSION 2.0 + Methods
  /*
  public native boolean canParameterBeAutomated(int index);
  public native boolean string2parameter(int index, String text);
  public native float getChannelParameter(int channel, int index);
  public native int getNumCategories();
  public native boolean getProgramNameIndexed(int category,
                                              int index,
                                              StringBuffer text);
  public native boolean copyProgram(int destination);
  public native boolean beginSetProgram();
  public native boolean endSetProgram();
  public native void inputConnected(int index, boolean state);
  public native void outputConnected(int index, boolean state);
  public native boolean getInputProperties(int index,
                                           VstPinProperties properties);
  public native boolean getOutputProperties(int index,
                                           VstPinProperties properties);
  public native VstPlugCategory getPlugCategory();
  public native int setTotalSampleToProcess(int value);
  public native int getNextShellPlugin(StringBuffer name);
  public native int reportCurrentPosition();
  public native NativeFloatArray reportDestinationBuffer();
  public native int startProcess();
  public native int stopProcess();
  public native int getMidiProgramName(int channel,
                                       MidiProgramName midiProgramName);
  public native int getCurrentMidiProgram(int channel,
                                          MidiProgramName currentProgram);
  public native int getMidiProgramCategory(int channel,
                                           MidiProgramCategory category);
  public native boolean hasMidiProgramsChanged(int channel);
  public native boolean getMidiKeyName(int channel, StringBuffer keyName);
  public native boolean processVariableIO(VstVariableIo variable);
  public native boolean setSpeakerArrangement(
      VstSpeakerArrangement pluginInput,
      VstSpeakerArrangement pluginOutput);
  public native boolean getSpeakerArrangement(
      JavaReference puginInput,
      JavaReference puginOutput);
  public native void setBlockSizeAndSampleRate(int blockSize, float sampleRate);
  public native boolean setBypass(boolean onOff);
  public native boolean getEffectName(StringBuffer name);
  public native boolean getErrorText(StringBuffer text);
  public native boolean getVendorString(StringBuffer text);
  public native boolean getProductString(StringBuffer text);
  public native int getVendorVersion();
  public native int vendorSpecific(int lArg,
                                   int lArg2,
                                   NativePointer ptrArg,
                                   float floatArg);
  public native int canDo(String text);
  public native NativePointer getIcon();
  public native boolean setViewPosition(int x, int y);
  public native int getTailSize();
  public native int fxIdle();
  public native boolean getParameterProperties(int index,
                                               VstParameterProperties p);
  public native boolean beginEdit(int index);
  public native boolean endEdit(int index);
  public native boolean keysRequired();
  public native int getVstVersion();
  public native boolean openFileSelector(VstFileSelect ptr);
  public native boolean closeFileSelector(VstFileSelect ptr);
  public native boolean getChunkFile(StringBuffer nativePath);
  public native boolean allocateArrangement(
      JavaReference arrangement, int nbChannels);
  public native boolean deallocateArrangement(
      VstSpeakerArrangement arrangement);
  public native boolean copySpeaker(VstSpeakerProperties to,
                                    VstSpeakerProperties from);
  public native boolean matchArrangement(
      JavaReference to,
      VstSpeakerArrangement from);
*/

  static {
    System.loadLibrary("JVSTlib");
  }

  protected JVSTPlugin(JVSTHost host, long libHandle) {
    this.libHandle = libHandle;
    this.host = host;
    this.aEffectPtr = instanciate(host, libHandle);
  }

  public long getLibHandle() {
    return libHandle;
  }

  public long getAEffectPtr() {
    return aEffectPtr;
  }

}