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

package jvst.wrapper.valueobjects;


public class VSTEvent {
  //VstEvent Types
  public static final int VST_EVENT_MIDI_TYPE = 1;	  // midi event, can be cast as VstMidiEvent (see below)
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_AUDIO_TYPE = 2;	  // audio
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_VIDEO_TYPE = 3;	  // video
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_PARAMETER_TYPE = 4; // parameter
  /**@deprecated
   * deprecated as of vst sdk version 2.4
   * please see the original vst documenation for details
   */
  public static final int VST_EVENT_TRIGGER_TYPE = 5;	  // trigger
  public static final int VST_EVENT_SYSEX_TYPE = 6;	  // midi system exclusive
  // ...etc


  protected int type;		// see enum above
  protected int byteSize;		// of this event, excl. type and byteSize
  protected int deltaFrames;	// sample frames related to the current block start sample position
  protected int flags;		// generic flags, used for midieventflags since 2.4
  protected byte[] data = new byte[16];		// size may vary but is usually 16

  public VSTEvent() {
  }

  public int getType() {
    return this.type;
  }

  public void setType(int type) {
    this.type = type;
  }

  public int getByteSize() {
    return this.byteSize;
  }

  public void setByteSize(int byteSize) {
    this.byteSize = byteSize;
  }

  public int getDeltaFrames() {
    return this.deltaFrames;
  }

  public void setDeltaFrames(int deltaFrames) {
    this.deltaFrames = deltaFrames;
  }

  public int getFlags() {
    return this.flags;
  }

  public void setFlags(int flags) {
    this.flags = flags;
  }

  public byte[] getData() {
    return this.data;
  }

  public void setData(byte[] data) {
    this.data = data;
  }

}