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


public class VSTVariableIO {

  private float[][] inputs = new float[1][1];
  private float[][] outputs = new float[1][1];
  private int numSamplesInput;
  private int numSamplesOutput;
  private int numSamplesInputProcessed;
  private int numSamplesOutputProcessed;

  public VSTVariableIO() {
  }

  public float[][] getInputs() {
    return this.inputs;
  }

  public void setInputs(float[][] inputs) {
    this.inputs = inputs;
  }

  public float[][] getOutputs() {
    return this.outputs;
  }

  public void setOutputs(float[][] outputs) {
    this.outputs = outputs;
  }

  public int getNumSamplesInput() {
    return this.numSamplesInput;
  }

  public void setNumSamplesInput(int numSamplesInput) {
    this.numSamplesInput = numSamplesInput;
  }

  public int getNumSamplesOutput() {
    return this.numSamplesOutput;
  }

  public void setNumSamplesOutput(int numSamplesOutput) {
    this.numSamplesOutput = numSamplesOutput;
  }

  public int getNumSamplesInputProcessed() {
    return this.numSamplesInputProcessed;
  }

  public void setNumSamplesInputProcessed(int numSamplesInputProcessed) {
    this.numSamplesInputProcessed = numSamplesInputProcessed;
  }

  public int getNumSamplesOutputProcessed() {
    return this.numSamplesOutputProcessed;
  }

  public void setNumSamplesOutputProcessed(int numSamplesOutputProcessed) {
    this.numSamplesOutputProcessed = numSamplesOutputProcessed;
  }

}