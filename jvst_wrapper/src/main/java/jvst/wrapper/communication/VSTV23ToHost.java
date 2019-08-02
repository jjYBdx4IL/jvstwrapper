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


public abstract class VSTV23ToHost extends VSTV22ToHost {

  public native boolean setPanLaw (int type, float val);
  // Set the Panning Law used by the Host

  public native int beginLoadBank (VSTPatchChunkInfo ptr);
  // Called before a Bank is loaded.
  // returns -1 if the Bank cannot be loaded, returns 1 if it can be loaded else 0 (for compatibility)

  public native int beginLoadProgram (VSTPatchChunkInfo ptr);
  // Called before a Program is loaded. (called before beginSetProgram)
  // returns -1 if the Program cannot be loaded, returns 1 if it can be loaded else 0 (for compatibility)


  //VST 2.3 TOOLS
  //**********************


   //NOT IMPORTANT --> we have a gc!!!
   /*
   public native bool allocateArrangement (VstSpeakerArrangement** arrangement, long nbChannels);
    // Allocate memory for a VstSpeakerArrangement containing the given
    // number of channels

   public native bool deallocateArrangement(VstSpeakerArrangement * * arrangement);
   // Delete/free memory for a speaker arrangement
   */

   public native boolean copySpeaker(VSTSpeakerProperties to, VSTSpeakerProperties from);
   // Feed the "to" speaker properties with the same values than "from"'s ones.
   // It is assumed here that "to" exists yet, ie this function won't
   // allocate memory for the speaker (this will prevent from having
   // a difference between an Arrangement's number of channels and
   // its actual speakers...)

   public native boolean matchArrangement(VSTSpeakerArrangement to, VSTSpeakerArrangement from);
   // "to" is deleted, then created and initialized with the same values as "from" ones ("from" must exist).


}