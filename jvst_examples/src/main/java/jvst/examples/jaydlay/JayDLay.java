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

package jvst.examples.jaydlay;


import jvst.wrapper.*;


public class JayDLay extends VSTPluginAdapter {
  public final static int PARAM_ID_DELAY = 0;
  public final static int PARAM_ID_FEEDBACK = 1;
  public final static int PARAM_ID_OUT = 2;

  public final static int NUM_PARAMS = PARAM_ID_OUT + 1;

  
  private DelayProgram[] programs;
  private float[] buffer;
  private float fDelay, fFeedBack, fOut;
  private int delay;
  private int size;
  private int cursor;
  private int currentProgram;

  JayDLayGUI gui = null; //reference to gui
  
  
  public JayDLay(long wrapper) {
    super(wrapper);
    log("Construktor JayDLay() START!");

    this.size = 44100;
    this.cursor = 0;
    this.delay = 0;
    this.buffer = new float[this.size];

    this.programs = new DelayProgram[16];
    for (int i = 0; i < this.programs.length; i++) {
      this.programs[i] = new DelayProgram();
    }
    this.fDelay = this.fFeedBack = 0F;
    this.fOut = 1.0F;

    
    //this is bad practice and causes npes on the mac (threading issue)
    //dont call gui stuff before the gui is initialized (which is not the case in the plug constructor)
    this.setProgram(0);

    //communicate with the host
    this.setNumInputs(1);// mono input
    this.setNumOutputs(1);// mono output
    //this.hasVu(false); //deprecated as of vst2.4
    this.canProcessReplacing(true);//mandatory for vst 2.4!
    this.setUniqueID('j'<<24 | 'D'<<16 | 'l'<<8 | 'y');//random unique number registered at steinberg (4 byte)

    this.canMono(true); 

    log("Construktor JayDLay() INVOKED!");
  }


  /**
   * to avoid that strange sounds appear if playback is restarted after stop.
   */
  public void resume() {
    log("resuming...");
    for (int i = 0; i < this.buffer.length; i++) this.buffer[i] = 0.0F;
  }


  public int canDo(String feature) {
	//the host asks us here what we are able to do
    int ret = JayDLay.CANDO_NO;

    //if (feature.equals(JayDLay.CANDO_PLUG_1_IN_2_OUT)) ret = JayDLay.CANDO_YES;
    if (feature.equals(JayDLay.CANDO_PLUG_1_IN_1_OUT)) ret = JayDLay.CANDO_YES;
    if (feature.equals(JayDLay.CANDO_PLUG_PLUG_AS_CHANNEL_INSERT)) ret = JayDLay.CANDO_YES;
    if (feature.equals(JayDLay.CANDO_PLUG_PLUG_AS_SEND)) ret = JayDLay.CANDO_YES;

    log("canDo: " + feature + " = " + ret);
    return ret;
  }

  public boolean setBypass(boolean value) {
    log("setBypass");
    //do not support soft bypass!
    return false;
  }

  public boolean string2Parameter(int index, String value) {
    log("string2parameter index=" + index + " value=" + value);
    boolean ret = false;

    try {
      if (value != null) this.setParameter(index, Float.parseFloat(value));
      ret=true;
    }
    catch(Exception e) {   //ignore
    }

    return ret;
  }

  public String getVendorString() { return "jVSTwRapper"; }

  public int getPlugCategory() {
    log("getPlugCategory");
    return VSTPluginAdapter.PLUG_CATEG_EFFECT;
  }

  public int getProgram() { return this.currentProgram; }

  public int getVendorVersion() {
    log("getVendor Version");
    return 1000;
  }

  public int getNumPrograms() {
    log("getNumPrograms");
    return this.programs.length;
  }

  public int getNumParams() {
    log("getNumParams");
    return NUM_PARAMS;
  }

  public float getParameter(int index) {
    float v = 0;

    switch (index) {
        case PARAM_ID_DELAY:
          v = this.fDelay;
          break;
        case PARAM_ID_FEEDBACK:
          v = this.fFeedBack;
          break;
        case PARAM_ID_OUT:
          v = this.fOut;
          break;
    }

    return v;
  }

  public String getParameterName(int index) {
    String label = "";

    switch (index) {
        case PARAM_ID_DELAY:
          label = "Delay";
          break;
        case PARAM_ID_FEEDBACK:
          label = "FeedBack";
          break;
        case PARAM_ID_OUT:
          label = "Volume";
          break;
    }

    return label;
  }

  public String getProductString() {
    return "JayDLay 1.0";
  }

  public void setProgram(int index) {
    DelayProgram dp = this.programs[index];

    //log("setProgram index=" + index);
    
    this.currentProgram = index;

    this.setParameter(PARAM_ID_DELAY, dp.getDelay());
    this.setParameter(PARAM_ID_FEEDBACK, dp.getFeedback());
    this.setParameter(PARAM_ID_OUT, dp.getOut());
    
    updateGUI();
  }

  public void setParameter(int index, float value) {
    DelayProgram dp = this.programs[this.currentProgram];

    //log("setParameter index=" + index + " value=" + value);
    
    switch (index) {
        case PARAM_ID_DELAY:
          this.setDelay(value);
          break;
        case PARAM_ID_FEEDBACK:
          this.fFeedBack = value;
          dp.setFeedback(value);
          break;
        case PARAM_ID_OUT:
          this.fOut = value;
          dp.setOut(value);
          break;
    }
    
    updateGUI();
  }

  public String getEffectName() {
    return "JayDLay";
  }

  public String getParameterLabel(int index) {
    String label = "";

    switch (index) {
        case PARAM_ID_DELAY:
          label = "";
          break;
        case PARAM_ID_FEEDBACK:
          label = "";
          break;
        case PARAM_ID_OUT:
          label = "dB";
          break;
    }

    return label;
  }

  public void setProgramName(String name) {
    this.programs[this.currentProgram].setName(name);
  }

  public String getParameterDisplay(int index) {
    String text = "";

    switch (index) {
        case PARAM_ID_DELAY:
          text = Integer.toString(this.delay) + this.getParameterLabel(PARAM_ID_DELAY);
          break;
        case PARAM_ID_FEEDBACK:
          text = this.formatFloatParam(this.fFeedBack) + this.getParameterLabel(PARAM_ID_FEEDBACK);
          break;
        case PARAM_ID_OUT:
          text = this.dbToString(this.fOut) + this.getParameterLabel(PARAM_ID_OUT);
          break;
    }

    return text;
  }

  public String getProgramName() {
    String name;

    if (this.programs[this.currentProgram].getName().equals("Init")) {
      name = this.programs[this.currentProgram].getName() + " " + (this.currentProgram + 1);
    }
    else {
      name = this.programs[this.currentProgram].getName();
    }

    return name;
  }

  public String getProgramNameIndexed(int category, int index) {
    String text = "";
    if (index < this.programs.length) text = this.programs[index].getName();
    if ("Init".equals(text)) text = text + " " + index;

    return text;
  }

  //processReplacing is REPLACING the calculated floats to the output
  public void processReplacing(float[][] inputs, float[][] outputs, int sampleFrames) {
    float[] in = inputs[0];
    float[] out1 = outputs[0];
    //float[] out2 = outputs[1];

    for (int i = 0; i < sampleFrames; i++) {
      float x = in[i];
      float y = this.buffer[this.cursor] * this.fOut;

      this.buffer[this.cursor++] = x + y * this.fFeedBack;
      if (this.cursor >= this.delay) this.cursor = 0;

      out1[i] = y; //IMPORTANT!!! Here is the difference between process() [accumulation]
      //out2[i] = y; //and processReplacing() [replacement]
    }
  }

  //DEPRECATED SINCE 2.4!
  //process is ACCUMULATING the calculated floats to the output
  //BUT STILL, leave it there for backward compatibility (some hosts only call this one
  //and are not aware of processReplacing...)
  public void process(float[][] inputs, float[][] outputs, int sampleFrames) {
    float[] in = inputs[0];
    float[] out1 = outputs[0];
    //float[] out2 = outputs[1];

    for (int i = 0; i < sampleFrames; i++) {
      float x = in[i];
      float y = this.buffer[this.cursor] * this.fOut;

      this.buffer[this.cursor++] = x + y * this.fFeedBack;
      if (this.cursor >= this.delay) this.cursor = 0;

      out1[i] += y; //IMPORTANT!!! Here is the difference between process() [accumulation]
      //out2[i] += y; //and processReplacing() [replacement]
    }
  }




  private void setDelay(float fdelay) {
    this.fDelay = fdelay;
    this.programs[this.currentProgram].setDelay(fdelay);
    //this.cursor = 0;
    this.delay = (int)(fdelay * (float)(size - 1));
  }
  
  
  protected void updateGUI() {
	//only access gui elemts if the gui was fully initialized
	//this is to prevent a threading issue on the mac that may cause a npe because the sliders 
	//arent there yet (the constructor of the plugin is called, when the gui is not initialized yet)
	//for thread savety on the mac, never call gui stuff in the constructor of the plugin
	//init the gui defaults always when the gui is loaded, not when the plug is loaded.
	
	if (	gui!=null && 
			gui.DelaySlider!=null && 
			gui.DelayText!=null && 
			gui.FeedbackSlider!=null && 
			gui.FeedbackText!=null && 
			gui.VolumeSlider!=null && 
			gui.VolumeText!=null) {
	    gui.DelaySlider.setValue((int)(this.getParameter(PARAM_ID_DELAY) * 100F));
	    gui.DelayText.setText(this.getParameterDisplay(PARAM_ID_DELAY));
	    
	    gui.FeedbackSlider.setValue((int)(this.getParameter(PARAM_ID_FEEDBACK) * 100F));
	    gui.FeedbackText.setText(this.getParameterDisplay(PARAM_ID_FEEDBACK));
	    
	    gui.VolumeSlider.setValue((int)(this.getParameter(PARAM_ID_OUT) * 100F));
	    gui.VolumeText.setText(this.getParameterDisplay(PARAM_ID_OUT)); 
	}
  }
  
}


/**
 * Helper Class (VO) for the support of different presets for the plug
 * if you are using no gui (comment PluginUIClass out in jVSTwRapper.ini)
 * @author dm
 * @version 1.0
 */
class DelayProgram {
  private String name = "Init";
  private float delay = 0.5F;
  private float feedback = 0.5F;
  private float out = 1F;



  public String getName() { return this.name; }
  public void setName(String name) { this.name = name; }

  public float getDelay() { return this.delay; }
  public void setDelay(float delay) { this.delay = delay; }

  public float getFeedback() { return this.feedback; }
  public void setFeedback(float feedback) { this.feedback = feedback; }

  public float getOut() { return this.out; }
  public void setOut(float out) { this.out = out; }
}