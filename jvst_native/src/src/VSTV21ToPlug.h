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

//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV21ToPlug__
#define __VSTV21ToPlug__

#include "VSTV20ToPlug.h"


//------------------------------------------------------------------------
class VSTV21ToPlug : public VSTV20ToPlug {

	public:
		VSTV21ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV21ToPlug ();

		//VST 2.1 Required
		//nothing


		//VST2.1 Optional
		//**********************

		virtual VstInt32 getMidiProgramName (VstInt32 channel, MidiProgramName* midiProgramName);
		// Struct will be filled with information for 'thisProgramIndex'.
		// returns number of used programIndexes.
		// If 0 is returned, no MidiProgramNames supported.
		
		virtual VstInt32 getCurrentMidiProgram (VstInt32 channel, MidiProgramName* currentProgram);
		// Struct will be filled with information for the current program.
		// Returns the programIndex of the current program. -1 means not supported.
		
		virtual VstInt32 getMidiProgramCategory (VstInt32 channel, MidiProgramCategory* category);
		// Struct will be filled with information for 'thisCategoryIndex'.
		// returns number of used categoryIndexes. 
		// if 0 is returned, no MidiProgramCategories supported/used.
		
		virtual bool hasMidiProgramsChanged (VstInt32 channel);
		// Returns true if the MidiProgramNames, MidiKeyNames or 
		// MidiControllerNames had changed on this channel.
		
		virtual bool getMidiKeyName (VstInt32 channel, MidiKeyName* keyName);
		// Struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'
		// if keyName is "" the standard name of the key will be displayed.
		// if false is returned, no MidiKeyNames defined for 'thisProgramIndex'.

		virtual bool beginSetProgram ();
		// Called before a program is loaded

		virtual bool endSetProgram ();  
		// Called after...

};


#endif
