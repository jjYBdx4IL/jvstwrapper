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


public class MidiProgramName {
  public static final int MIDI_IS_OMNI = 1;
  // default is multi. for omni mode, channel 0
  // is used for inquiries and program changes


  private int thisProgramIndex;		// >= 0. fill struct for this program index.
  private String name = "";             //[64];
  private byte midiProgram = -1;	// -1:off, 0-127
  private byte midiBankMsb = -1;        // -1:off, 0-127
  private byte midiBankLsb = -1;        // -1:off, 0-127
  private byte reserved = 0;		// zero
  private int parentCategoryIndex = -1;	// -1:no parent category
  private int flags = 0;		// omni etc, see above



  public int getThisProgramIndex() {return this.thisProgramIndex;}
  public int getParentCategoryIndex() {return this.parentCategoryIndex;}
  public int getFlags() {return this.flags;}
  public String getName() {return this.name;}
  public byte getReserved() {return this.reserved;}
  public byte getMidiProgram() {return this.midiProgram;}
  public byte getMidiBankMsb() {return this.midiBankMsb;}
  public byte getMidiBankLsb() {return this.midiBankLsb;}


  public void setReserved(byte p) {this.reserved = p;}
  public void setThisProgramIndex(int p) {this.thisProgramIndex = p;}
  public void setParentCategoryIndex(int p) {this.parentCategoryIndex = p;}
  public void setFlags(int p) {this.flags = p;}
  public void setName(String p) {this.name = p;}
  public void setMidiProgram(byte p) {this.midiProgram = p;}
  public void setMidiBankMsb(byte p) {this.midiBankMsb = p;}
  public void setMidiBankLsb(byte p) {this.midiBankLsb = p;}

}