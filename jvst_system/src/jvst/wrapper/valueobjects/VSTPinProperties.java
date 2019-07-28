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


public class VSTPinProperties {
  //pin properties flags
  public static final int VST_PIN_IS_ACTIVE = 1;
  public static final int VST_PIN_IS_STEREO = 2;
  public static final int VST_PIN_USE_SPEAKER = 4;


  private String Label = "";
  private int Flags;         // see pin properties flags
  private int ArrangementType;
  private String ShortLabel = ""; // recommended: 6 + delimiter


  public VSTPinProperties() {
  }

  public VSTPinProperties(String lab, int fla, int arr, String sho) {
    this.Label = lab;
    this.Flags = fla;
    this.ArrangementType = arr;
    this.ShortLabel = sho;
  }


  public String getShortLabel() {
    return this.ShortLabel;
  }

  public void setShortLabel(String shortLabel) {
    if (shortLabel.length()>6) this.ShortLabel = shortLabel.substring(0,6);
    else this.ShortLabel = shortLabel;
  }

  public int getArrangementType() {
    return this.ArrangementType;
  }

  public void setArrangementType(int ArrangementType) {
    this.ArrangementType = ArrangementType;
  }

  public int getFlags() {
    return this.Flags;
  }

  public void setFlags(int Flags) {
    this.Flags = Flags;
  }

  public String getLabel() {
    return this.Label;
  }

  public void setLabel(String label) {
    if (label.length()>63) this.Label = label.substring(0,63);
    else this.Label = label;
  }
}