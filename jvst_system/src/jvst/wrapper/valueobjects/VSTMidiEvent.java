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


public class VSTMidiEvent extends VSTEvent {
  protected int noteLength;	// (in sample frames) of entire note, if available, else 0
  protected int noteOffset;	// offset into note from note start if available, else 0

  //private byte midiData[4];	// 1 thru 3 midi bytes; midiData[3] is reserved (zero)
  //ATTENTION! use data[] instead!
  protected byte detune;		// -64 to +63 cents; for scales other than 'well-tempered' ('microtuning')
  protected byte noteOffVelocity;
  protected byte reserved1;	// zero
  protected byte reserved2;	// zero


  public static final int FLAG_MidiEventIsRealtime = 1 << 0;	///< means that this event is played life (not in playback)

  
  

  public VSTMidiEvent() {
    //private byte midiData[4];	// 1 thru 3 midi bytes; midiData[3] is reserved (zero)
    //--> correct the data size...
    this.data = new byte[4];
  }

  public int getNoteLength() {
    return this.noteLength;
  }

  public void setNoteLength(int noteLength) {
    this.noteLength = noteLength;
  }

  public int getNoteOffset() {
    return this.noteOffset;
  }

  public void setNoteOffset(int noteOffset) {
    this.noteOffset = noteOffset;
  }

  public byte getDetune() {
    return this.detune;
  }

  public void setDetune(byte detune) {
    this.detune = detune;
  }

  public byte getNoteOffVelocity() {
    return this.noteOffVelocity;
  }

  public void setNoteOffVelocity(byte noteOffVelocity) {
    this.noteOffVelocity = noteOffVelocity;
  }

  public byte getReserved1() {
    return this.reserved1;
  }

  public void setReserved1(byte reserved1) {
    this.reserved1 = reserved1;
  }

  public byte getReserved2() {
    return this.reserved2;
  }

  public void setReserved2(byte reserved2) {
    this.reserved2 = reserved2;
  }


}