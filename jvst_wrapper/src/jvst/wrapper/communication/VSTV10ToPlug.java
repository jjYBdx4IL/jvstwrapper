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


/**
 *  VST 1.0 Host -> Plug Communication
 *
 *@author     daniel
 *@created    14. Oktober 2003
 */
public interface VSTV10ToPlug {

  //not supported
  //virtual long dispatcher (long opCode, long index, long value, void *ptr, float opt);


  //VST 1.0 REQUIRED
  //*****************
  public void setParameter(int index, float value);
  public float getParameter(int index);
 
  /**@deprecated
   * deprecated as of vst sdk version 2.4 
   * please see the original vst documenation for details
   * 
   * USE processReplacing() and/or processDoubleReplacing() INSTEAD!
   */
  public void process(float[][] inputs, float[][] outputs, int sampleFrames);
 
  public void processReplacing(float[][] inputs, float[][] outputs, int sampleFrames);
  public int getProgram();
  public void setProgram(int index);
  public void setProgramName(String name);
  public String getProgramName();
  public String getParameterName(int index);
  public String getParameterDisplay(int index);
  public String getParameterLabel(int index);


  //VST 1.0 OPTIONAL
  //*****************
  public void open();
  public void close();
  public void suspend();
  public void resume();
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public float getVu();
  public int getChunk(byte data[][], boolean isPreset);
  public int setChunk(byte data[], int byteSize, boolean isPreset);
  public void setBlockSize(int blockSize);
  public void setSampleRate(float sampleRate);


  //custom enhancements by dm
  //**************************

  /**
   *  because setting this param in constructor is bad design!
   */
  public int getNumPrograms();


  /**
   *  because setting this param in constructor is bad design!
   */
  public int getNumParams();

}
