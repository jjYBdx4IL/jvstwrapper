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

public class MidiProgramCategory {
  private int thisCategoryIndex;        // >= 0. fill struct for this category index.
  private String name = "";             //[64];
  private int parentCategoryIndex = -1; // -1:no parent category
  private int flags = 0;                // reserved, none defined yet, zero.


  public int getThisCategoryIndex() {return this.thisCategoryIndex;}
  public int getParentCategoryIndex() {return this.parentCategoryIndex;}
  public int getFlags() {return this.flags;}
  public String getName() {return this.name;}


  public void setThisCategoryIndex(int p) {this.thisCategoryIndex = p;}
  public void setParentCategoryIndex(int p) {this.parentCategoryIndex = p;}
  public void setFlags(int p) {this.flags = p;}
  public void setName(String p) {this.name = p;}

}