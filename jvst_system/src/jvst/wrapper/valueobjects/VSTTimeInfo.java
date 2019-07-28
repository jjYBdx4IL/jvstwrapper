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

/**
 *  VstTimeInfo as requested via audioMasterGetTime (getTimeInfo())
 *  refers to the current time slice. note the new slice is
 *  already started when processEvents() is called
 */

public class VSTTimeInfo {
  public static final int VST_TIME_TRANSPORT_CHANGED      = 1; // Indicates that Playing, Cycle or Recording has changed
  public static final int VST_TIME_TRANSPORT_PLAYING      = 1 << 1;
  public static final int VST_TIME_TRANSPORT_CYCLE_ACTIVE = 1 << 2;
  public static final int VST_TIME_TRANSPORT_RECORDING    = 1 << 3;

  public static final int VST_TIME_AUTOMATION_WRITING     = 1 << 6;
  public static final int VST_TIME_AUTOMATION_READING     = 1 << 7;

  // flags which indicate which of the fields in this VstTimeInfo
  // are valid; samplePos and sampleRate are always valid
  public static final int VST_TIME_NANOS_VALID            = 1 << 8;
  public static final int VST_TIME_PPQ_POS_VALID          = 1 << 9;
  public static final int VST_TIME_TEMPO_VALID            = 1 << 10;
  public static final int VST_TIME_BARS_VALID	          = 1 << 11;
  public static final int VST_TIME_CYCLE_POS_VALID        = 1 << 12;	// start and end
  public static final int VST_TIME_TIME_SIG_VALID         = 1 << 13;
  public static final int VST_TIME_SMPTE_VALID            = 1 << 14;
  public static final int VST_TIME_CLOCK_VALID            = 1 << 15;



  private double samplePos;	        // current location
  private double sampleRate;
  private double nanoSeconds;		// system time
  private double ppqPos;		// 1 ppq
  private double tempo;			// in bpm
  private double barStartPos;		// last bar start, in 1 ppq
  private double cycleStartPos;		// 1 ppq
  private double cycleEndPos;		// 1 ppq
  private int timeSigNumerator;	// time signature
  private int timeSigDenominator;
  private int smpteOffset;
  private int smpteFrameRate;		// 0:24, 1:25, 2:29.97, 3:30, 4:29.97 df, 5:30 df
  private int samplesToNextClock;	// midi clock resolution (24 ppq), can be negative
  private int flags;			// see above


  public double getSamplePos() {return this.samplePos;}
  public double getSampleRate() {return this.sampleRate;}
  public double getNanoSeconds() {return this.nanoSeconds;}
  public double getPPQPos() {return this.ppqPos;}
  public double getTempo() {return this.tempo;}
  public double getBarStartPos() {return this.barStartPos;}
  public double getCycleStartPos() {return this.cycleStartPos;}
  public double getCycleEndPos() {return this.cycleEndPos;}

/*
  public void setSamplePos(double d) {this.samplePos = d;}
  public void setSampleRate(double d) {this.sampleRate = d;}
  public void setNanoSeconds(double d) {this.nanoSeconds = d;}
  public void setPPQPos(double d) {this.ppqPos = d;}
  public void setTempo(double d) {this.tempo = d;}
  public void setBarStartPos(double d) {this.barStartPos = d;}
  public void setCycleStartPos(double d) {this.cycleStartPos = d;}
  public void setCycleEndPos(double d) {this.cycleEndPos = d;}
*/

  public int getTimeSigNumerator() {return this.timeSigNumerator;}
  public int getTimeSigDenominator() {return this.timeSigDenominator;}
  public int getSMPTEOffset() {return this.smpteOffset;}
  public int getSMPTEFrameRate() {return this.smpteFrameRate;}
  public int getSamplesToNextClock() {return this.samplesToNextClock;}
  public int getFlags() {return this.flags;}

/*
  public void setTimeSigNumerator(int d) {this.timeSigNumerator = d;}
  public void setTimeSigDenominator(int d) {this.timeSigDenominator = d;}
  public void setSMPTEOffset(int d) {this.smpteOffset = d;}
  public void setSMPTEFrameRate(int d) {this.smpteFrameRate = d;}
  public void setSamplesToNextClock(int d) {this.samplesToNextClock = d;}
  public void setFlags(int d) {this.flags = d;}
*/

}
