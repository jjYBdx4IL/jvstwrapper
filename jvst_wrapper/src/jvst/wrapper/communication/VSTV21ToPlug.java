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


public interface VSTV21ToPlug extends VSTV20ToPlug {
  //VST 2.1 Required
  //nothing

  //VST2.1 Optional
  public int getMidiProgramName(int channel, MidiProgramName midiProgramName);
  // Struct will be filled with information for 'thisProgramIndex'.
  // returns number of used programIndexes.
  // If 0 is returned, no MidiProgramNames supported.

  public int getCurrentMidiProgram(int channel, MidiProgramName currentProgram);
  // Struct will be filled with information for the current program.
  // Returns the programIndex of the current program. -1 means not supported.

  public int getMidiProgramCategory(int channel, MidiProgramCategory category);
  // Struct will be filled with information for 'thisCategoryIndex'.
  // returns number of used categoryIndexes.
  // if 0 is returned, no MidiProgramCategories supported/used.

  public boolean hasMidiProgramsChanged(int channel);
  // Returns true if the MidiProgramNames, MidiKeyNames or
  // MidiControllerNames had changed on this channel.

  public boolean getMidiKeyName(int channel, MidiKeyName keyName);
  // Struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'
  // if keyName is "" the standard name of the key will be displayed.
  // if false is returned, no MidiKeyNames defined for 'thisProgramIndex'.

  public boolean beginSetProgram();
  // Called before a program is loaded

  public boolean endSetProgram();
  // Called after...

}