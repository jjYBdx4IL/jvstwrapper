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


public class VSTSpeakerArrangement {
  //Speaker Arrangement Types
  public static final int SpeakerArrUserDefined = -2;
  public static final int SpeakerArrEmpty = -1;
  public static final int SpeakerArrMono  =  0;	        // M

  public static final int SpeakerArrStereoM = 1;	// L R
  public static final int SpeakerArrStereoSurround = 2;	// Ls Rs
  public static final int SpeakerArrStereoCenter = 3;	// Lc Rc
  public static final int SpeakerArrStereoSide = 4;	// Sl Sr
  public static final int SpeakerArrStereoCLfe = 5;	// C Lfe

  public static final int SpeakerArr30Cine = 6;		// L R C
  public static final int SpeakerArr30Music = 7;	// L R S
  public static final int SpeakerArr31Cine = 8;		// L R C Lfe
  public static final int SpeakerArr31Music = 9;	// L R Lfe S

  public static final int SpeakerArr40Cine = 10;	// L R C   S (LCRS)
  public static final int SpeakerArr40Music = 11;	// L R Ls  Rs (Quadro)
  public static final int SpeakerArr41Cine = 12;	// L R C   Lfe S (LCRS+Lfe)
  public static final int SpeakerArr41Music = 13;	// L R Lfe Ls Rs (Quadro+Lfe)

  public static final int SpeakerArr50 = 14;		// L R C Ls  Rs
  public static final int SpeakerArr51 = 15;		// L R C Lfe Ls Rs

  public static final int SpeakerArr60Cine = 16;	// L R C   Ls  Rs Cs
  public static final int SpeakerArr60Music = 17;	// L R Ls  Rs  Sl Sr
  public static final int SpeakerArr61Cine = 18;	// L R C   Lfe Ls Rs Cs
  public static final int SpeakerArr61Music = 19;	// L R Lfe Ls  Rs Sl Sr

  public static final int SpeakerArr70Cine = 20;	// L R C Ls  Rs Lc Rc
  public static final int SpeakerArr70Music = 21;	// L R C Ls  Rs Sl Sr
  public static final int SpeakerArr71Cine = 22;	// L R C Lfe Ls Rs Lc Rc
  public static final int SpeakerArr71Music = 23;	// L R C Lfe Ls Rs Sl Sr

  public static final int SpeakerArr80Cine = 24;	// L R C Ls  Rs Lc Rc Cs
  public static final int SpeakerArr80Music = 25;	// L R C Ls  Rs Cs Sl Sr
  public static final int SpeakerArr81Cine = 26;	// L R C Lfe Ls Rs Lc Rc Cs
  public static final int SpeakerArr81Music = 27;	// L R C Lfe Ls Rs Cs Sl Sr

  public static final int SpeakerArr102 = 28;		// L R C Lfe Ls Rs Tfl Tfc Tfr Trl Trr Lfe2
  public static final int NumSpeakerArr = 29;




  private int type;			    // (was float lfeGain; // LFE channel gain is adjusted [dB] higher than other channels)
  private int numChannels;		    // number of channels in this speaker arrangement
  private VSTSpeakerProperties speakers[];  // variable

  public VSTSpeakerArrangement() {
  }

  public int getType() {
    return this.type;
  }

  public void setType(int type) {
    this.type = type;
  }

  public int getNumChannels() {
    return this.numChannels;
  }

  public void setNumChannels(int numChannels) {
    this.numChannels = numChannels;
  }

  public VSTSpeakerProperties[] getSpeakers() {
    return this.speakers;
  }

  public void setSpeakers(VSTSpeakerProperties[] speakers) {
    this.speakers = speakers;
  }


}