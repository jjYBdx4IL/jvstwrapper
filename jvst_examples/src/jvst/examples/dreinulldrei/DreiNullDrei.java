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

package jvst.examples.dreinulldrei;



import jvst.wrapper.*;
import jvst.wrapper.valueobjects.*;


public class DreiNullDrei extends VSTPluginAdapter {
  private static final float ENV_INC = 64;

  private static final int NUM_PROGRAMS = 16;
  private static final int NUM_OUTPUTS = 1;


  private DreiNullDreiProgram[] programs = new DreiNullDreiProgram[NUM_PROGRAMS];
  private int channelPrograms[] = new int[NUM_PROGRAMS];
  private int currentProgram;


  private boolean IsNoteOn = false;
  private int currentNote;

  private float srate;

  private float vco_inc_src, vco_inc, vco_inc_dest, vco_k, vco_wav;

  private float vcf_envmod, vcf_envdecay, vcf_envdecayi, vcf_reso, vcf_rescoeff,
                vcf_cutoff, vcf_e0, vcf_e1, vcf_c0, vcf_d1, vcf_d2,
                vcf_a, vcf_b, vcf_c, vcf_envpos;

  private float vca_attack, vca_decay, vca_a0, vca_a, vca_mode, vca_accamt;

  private float sld_spd;
  private int cnt, cntmax;

  private float volume;




  public DreiNullDrei(long wrapper) {
    super(wrapper);
    log("Construktor DreiNullDrei() START!");


    for (int i = 0; i < this.programs.length; i++) this.programs[i] = new DreiNullDreiProgram();
    for (int i = 0; i < this.channelPrograms.length; i++) this.channelPrograms[i] = i;

    this.setProgram(0);


    this.setNumInputs(0);// no input
    this.setNumOutputs(NUM_OUTPUTS);// mono output
    //this.hasVu(false); //deprecated as of vst2.4
    //this.hasClip(false); //deprecated as of vst2.4
    this.canProcessReplacing(true);

    this.isSynth(true);

    this.setUniqueID('j'<<24 | 'D'<<16 | 'r'<<8 | 'e');

    this.srate = 44100f;
    this.vco_inc_dest = 440f / this.srate;
    this.vco_inc = this.vco_inc_dest;
    this.vco_wav = 0f;
    this.vco_k = this.vcf_cutoff = this.vcf_envmod = this.vcf_reso = 0f;
    this.vco_inc_dest = this.vcf_envdecay = 0f;
    this.vcf_envpos = ENV_INC;

    this.vcf_a = this.vcf_b = this.vcf_d1 = this.vcf_d2 = this.vcf_c0 = this.vcf_e0 = this.vcf_e1 = 0;
    this.vca_mode = 2;
    this.vca_a = 0f;
    this.vcf_rescoeff = 1f;
    this.vca_attack = 1f - 0.94406088f;
    this.vca_decay = 0.99897516f;
    this.vca_a0 = 0.5f;
    this.vca_accamt = 0.5f;
    this.sld_spd = 0.1f;
    this.setCutoff(0.9f);
    this.setResonance(0.1f);
    this.setEnvMod(1f);
    this.setEnvDecay(0.1f);
    this.cntmax = 1;
    this.volume = 1f;

    this.suspend();

    log("Construktor DreiNullDrei() INVOKED!");
  }


  public void setSampleRate(float s) {this.srate = s;}
  public void resume() { 
	  this.wantEvents(1); //deprecated as of vst2.4 
	  					  //but keep it --> backward compatibility!!!
  }



  public void setProgram(int index) {
    if (index < 0 || index >= NUM_PROGRAMS) return;

    DreiNullDreiProgram dp = this.programs[index];
    this.currentProgram = index;

    this.setParameter(DreiNullDreiProgram.PARAM_ID_VOLUME, dp.getVolume());

    this.setParameter(DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT, dp.getAccAmount());
    this.setParameter(DreiNullDreiProgram.PARAM_ID_CUT_OFF, dp.getCutoff());
    this.setParameter(DreiNullDreiProgram.PARAM_ID_ENV_DECAY, dp.getEnvDecay());
    this.setParameter(DreiNullDreiProgram.PARAM_ID_ENV_MOD, dp.getEnvMod());
    this.setParameter(DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED, dp.getGlideSpeed());
    this.setParameter(DreiNullDreiProgram.PARAM_ID_RESONANCE, dp.getResonance());
    this.setParameter(DreiNullDreiProgram.PARAM_ID_WAVEFORM, dp.getWaveform());
  }

  public void setProgramName(String name) {
    this.programs[this.currentProgram].setName(name);
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

  public String getParameterLabel(int index) {
    String label = "";

    switch (index) {
      case DreiNullDreiProgram.PARAM_ID_WAVEFORM:
        label = "Shape";
        break;
      case DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT:
      case DreiNullDreiProgram.PARAM_ID_CUT_OFF:
      case DreiNullDreiProgram.PARAM_ID_ENV_DECAY:
      case DreiNullDreiProgram.PARAM_ID_ENV_MOD:
      case DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED:
      case DreiNullDreiProgram.PARAM_ID_RESONANCE:
        label = "";
        break;
      case DreiNullDreiProgram.PARAM_ID_VOLUME:
        label = "dB";
        break;
    }

    return label;
  }

  public String getParameterDisplay(int index) {
    String text = "";

    switch (index) {
      case DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT: {
        text = Float.toString(this.getAccAmt());
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_CUT_OFF:{
        text = Float.toString(this.getCutoff());
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_ENV_DECAY: {
        text = Float.toString(this.getEnvDecay());
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_ENV_MOD:{
        text = Float.toString(this.getEnvMod());
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED: {
        text = this.dbToString(this.getGlideSpeed());
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_RESONANCE: {
        text = this.dbToString(this.getResonance());
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_VOLUME: {
        text = this.dbToString(this.volume);
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_WAVEFORM: {
        text = Float.toString(this.getWaveform());
        break;
      }
    }

    return text;
  }

  public String getParameterName(int index) {
    String label = "";

   switch (index) {
      case DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT: label = "Acc Amount"; break;
      case DreiNullDreiProgram.PARAM_ID_CUT_OFF: label = "Cutoff"; break;
      case DreiNullDreiProgram.PARAM_ID_ENV_DECAY: label = "Env Decay"; break;
      case DreiNullDreiProgram.PARAM_ID_ENV_MOD: label = "Env Mod"; break;
      case DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED: label = "Glide Speed"; break;
      case DreiNullDreiProgram.PARAM_ID_RESONANCE: label = "Resonance"; break;
      case DreiNullDreiProgram.PARAM_ID_VOLUME: label = "Volume"; break;
      case DreiNullDreiProgram.PARAM_ID_WAVEFORM: label = "Waveform"; break;

    }

    return label;
  }

  public void setParameter(int index, float value) {
    DreiNullDreiProgram dp = this.programs[this.currentProgram];

    switch (index) {
      case DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT: {
        dp.setAccAmount(value);
        this.setAccAmt(value);
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_CUT_OFF: {
        dp.setCutoff(value);
        this.setCutoff(value);
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_ENV_DECAY: {
        dp.setEnvDecay(value);
        this.setEnvDecay(value);
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_ENV_MOD: {
        dp.setEnvMod(value);
        this.setEnvMod(value);
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED: {
        dp.setGlideSpeed(value);
        this.setGlideSpeed(value);
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_RESONANCE: {
        dp.setResonance(value);
        this.setResonance(value);
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_VOLUME: {
        dp.setVolume(value);
        this.volume = value;
        break;
      }
      case DreiNullDreiProgram.PARAM_ID_WAVEFORM: {
        dp.setWaveform(value);
        this.setWaveform(value);
        break;
      }
    }

  }

  public float getParameter(int index) {
    float v = 0;

    switch (index) {
      case DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT: v = this.getAccAmt(); break;
      case DreiNullDreiProgram.PARAM_ID_CUT_OFF: v = this.getCutoff(); break;
      case DreiNullDreiProgram.PARAM_ID_ENV_DECAY: v = this.getEnvDecay(); break;
      case DreiNullDreiProgram.PARAM_ID_ENV_MOD: v = this.getEnvMod(); break;
      case DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED: v = this.getGlideSpeed(); break;
      case DreiNullDreiProgram.PARAM_ID_RESONANCE: v = this.getResonance(); break;
      case DreiNullDreiProgram.PARAM_ID_VOLUME: v = this.volume; break;
      case DreiNullDreiProgram.PARAM_ID_WAVEFORM: v = this.getWaveform(); break;
    }
    return v;
  }

  public VSTPinProperties getOutputProperties (int index) {
    VSTPinProperties ret = null;

    if (index < NUM_OUTPUTS) {
      ret = new VSTPinProperties();
      ret.setLabel("DreiNullDrei " + (index + 1) + "d");
      ret.setFlags(VSTPinProperties.VST_PIN_IS_ACTIVE);
    }

    return ret;
  }

  public String getProgramNameIndexed(int category, int index) {
    String text = "";
    if (index < this.programs.length) text = this.programs[index].getName();
    if ("Init".equals(text)) text = text + " " + index;
    return text;
  }

  public boolean copyProgram (int destination) {
    if (destination < NUM_PROGRAMS) {
      this.programs[destination] = this.programs[this.currentProgram];
      return true;
    }
    return false;
  }


  public String getEffectName() { return "DreiNullDrei v0.6"; }
  public String getVendorString() { return "jVSTwRapper"; }
  public String getProductString() { return "DreiNullDrei"; }
  public int getNumPrograms() { return NUM_PROGRAMS; }
  public int getNumParams() { return DreiNullDreiProgram.NUM_PARAMS; }
  public boolean setBypass(boolean value) { return false; }
  public int getProgram() { return this.currentProgram; }
  public int getPlugCategory() { return VSTPluginAdapter.PLUG_CATEG_SYNTH; }

  public int canDo(String feature) {
    int ret = DreiNullDrei.CANDO_NO;

    if (DreiNullDrei.CANDO_PLUG_RECEIVE_VST_EVENTS.equals(feature)) ret = DreiNullDrei.CANDO_YES;
    if (DreiNullDrei.CANDO_PLUG_RECEIVE_VST_MIDI_EVENT.equals(feature)) ret = DreiNullDrei.CANDO_YES;
    if (DreiNullDrei.CANDO_PLUG_MIDI_PROGRAM_NAMES.equals(feature)) ret = DreiNullDrei.CANDO_YES;

    return ret;
  }

  public boolean string2Parameter(int index, String value) {
    boolean ret = false;

    try {
      if (value != null) this.setParameter(index, Float.parseFloat(value));
      ret=true;
    } catch(Exception e) {log(e.toString());}

    return ret;
  }


// midi program names:
// as an example, GM names are used here. in fact, this Synth doesn't even support
// multi-timbral operation so it's really just for demonstration.
// a 'real' instrument would have a number of voices which use the
// programs[channelProgram[channel]] parameters when it receives
// a note on message.

  public int getMidiProgramName(int channel, MidiProgramName mpn) {
    int prg = mpn.getThisProgramIndex();

    if (prg < 0 || prg >= 128) return 0;
    this.fillProgram(channel, prg, mpn);
    if (channel == 9) return 1;

    return 128;
  }

  public int getCurrentMidiProgram (int channel, MidiProgramName mpn) {
    if (channel < 0 || channel >= 16 || mpn==null) return -1;

    int prg = this.channelPrograms[channel];
    mpn.setThisProgramIndex(prg);
    fillProgram (channel, prg, mpn);

    return prg;
  }

  public int getMidiProgramCategory (int channel, MidiProgramCategory cat) {
    cat.setParentCategoryIndex(-1);	// -1:no parent category
    cat.setFlags(0);			// reserved, none defined yet, zero.

    int category = cat.getThisCategoryIndex();
    if (channel == 9) {
      cat.setName("Drums");
      return 1;
    }
    if (category >= 0 && category < GMNames.NUM_GM_CATEGORIES)
      cat.setName(GMNames.GM_CATEGORIES[category]);

    return GMNames.NUM_GM_CATEGORIES;
  }

  public boolean hasMidiProgramsChanged (int channel) {
    return false;
  }

// struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'
// if keyName is "" the standard name of the key will be displayed.
// if false is returned, no MidiKeyNames defined for 'thisProgramIndex'.
  public boolean getMidiKeyName (long channel, MidiKeyName key) {
    return false;
  }

  


  
  
//DEPRECATED since 2.4
  public void process(float[][] input, float[][] output, int samples) {
      float w, k, result;
      float[] out1 = output[0];

      if (this.IsNoteOn) {
        for (int j = 0; j < samples; j++) {

          if (this.vcf_envpos >= ENV_INC) {
            w = this.vcf_e0 + this.vcf_c0;
            k = (float) Math.exp( -w / this.vcf_rescoeff);
            this.vcf_c0 = this.vcf_c0 * this.vcf_envdecay;
            this.vcf_a = 2f * (float) Math.cos(2f * w) * k;
            this.vcf_b = -k * k;
            this.vcf_c = 1f - this.vcf_a - this.vcf_b;
            this.vcf_envpos = 0;
          }

          if (this.vco_wav > 0.5f) {
            result = this.vcf_a * this.vcf_d1 + this.vcf_b * this.vcf_d2 +
                this.vcf_c * this.rct(this.vco_k) * this.vca_a;
          }
          else {
            result = this.vcf_a * this.vcf_d1 + this.vcf_b * this.vcf_d2 +
                this.vcf_c * this.vco_k * this.vca_a;
          }

          this.vcf_d2 = this.vcf_d1;
          this.vcf_envpos = this.vcf_envpos + 1;
          this.vcf_d1 = result;

          this.cnt++;
          w = (float)this.cnt / (float)this.cntmax;

          if (w < 1f) {
            k = this.vco_inc_src * (1f - w) + w * this.vco_inc_dest;
          }
          else {
            this.vco_inc = this.vco_inc_dest;
            k = this.vco_inc;
          }

          this.vco_k = this.vco_k + k;
          if (this.vco_k > 0.5f) this.vco_k -= 1;

          if (this.vca_mode == 0) {
            this.vca_a = this.vca_a + ((this.vca_a0 - this.vca_a) * this.vca_attack);
          }
          else if (this.vca_mode == 1) {
            this.vca_a = this.vca_a * this.vca_decay;

            if (this.vca_a < (1f / 65536f)) {
              this.vca_a = 0;
              this.vca_mode = 2;
            }
          }

          out1[j] += result * this.volume;
        }
      }

  }
  
  public void processReplacing(float[][] input, float[][] output, int samples) {
    float w, k, result;
    float[] out1 = output[0];

    if (this.IsNoteOn) {
      for (int j = 0; j < samples; j++) {

        if (this.vcf_envpos >= ENV_INC) {
          w = this.vcf_e0 + this.vcf_c0;
          k = (float) Math.exp( -w / this.vcf_rescoeff);
          this.vcf_c0 = this.vcf_c0 * this.vcf_envdecay;
          this.vcf_a = 2f * (float) Math.cos(2f * w) * k;
          this.vcf_b = -k * k;
          this.vcf_c = 1f - this.vcf_a - this.vcf_b;
          this.vcf_envpos = 0;
        }

        if (this.vco_wav > 0.5f) {
          result = this.vcf_a * this.vcf_d1 + this.vcf_b * this.vcf_d2 +
              this.vcf_c * this.rct(this.vco_k) * this.vca_a;
        }
        else {
          result = this.vcf_a * this.vcf_d1 + this.vcf_b * this.vcf_d2 +
              this.vcf_c * this.vco_k * this.vca_a;
        }

        this.vcf_d2 = this.vcf_d1;
        this.vcf_envpos = this.vcf_envpos + 1;
        this.vcf_d1 = result;

        this.cnt++;
        w = (float)this.cnt / (float)this.cntmax;

        if (w < 1f) {
          k = this.vco_inc_src * (1f - w) + w * this.vco_inc_dest;
        }
        else {
          this.vco_inc = this.vco_inc_dest;
          k = this.vco_inc;
        }

        this.vco_k = this.vco_k + k;
        if (this.vco_k > 0.5f) this.vco_k -= 1;

        if (this.vca_mode == 0) {
          this.vca_a = this.vca_a + ((this.vca_a0 - this.vca_a) * this.vca_attack);
        }
        else if (this.vca_mode == 1) {
          this.vca_a = this.vca_a * this.vca_decay;

          if (this.vca_a < (1f / 65536f)) {
            this.vca_a = 0;
            this.vca_mode = 2;
          }
        }

        out1[j] = result * this.volume;
      }
    }

  }

 public int processEvents (VSTEvents ev) {
    for (int i = 0; i < ev.getNumEvents(); i++) {
      if (ev.getEvents()[i].getType() != VSTEvent.VST_EVENT_MIDI_TYPE) continue;

      VSTMidiEvent event = (VSTMidiEvent)ev.getEvents()[i];
      byte[] midiData = event.getData();
      int status = midiData[0] & 0xf0;// ignoring channel

      if (status == 0x90 || status == 0x80) {
        // we only look at notes
        int note = midiData[1] & 0x7f;
        int velocity = midiData[2] & 0x7f;
        if (status == 0x80) velocity = 0;	// note off by velocity 0

        if (velocity==0 && (note == currentNote)) this.noteOff();
        else this.noteOn (note, velocity >= 64, false); //FIXME: do not auto glide=true!!!
      }
      else if (status == 0xb0) {
        // all notes off
        if (midiData[1] == 0x7e || midiData[1] == 0x7b)	this.noteOff();
      }
    }

    return 1;	// want more
  }







  private void setWaveform(float w) {this.vco_wav = w;}
  private float getWaveform() {return this.vco_wav;}

  private void setGlideSpeed(float s) {this.sld_spd = s;}
  private float getGlideSpeed() {return this.sld_spd;}

  private void setAccAmt(float a) {this.vca_accamt = a;}
  private float getAccAmt() {return this.vca_accamt;}

  private void setCutoff(float c) {
    this.vcf_cutoff = c;
    this.recalc();
  }
  private float getCutoff() {return this.vcf_cutoff;}

  private void setResonance(float r) {
    this.vcf_reso = r;
    this.vcf_rescoeff = (float)Math.exp(-1.20f + 3.455f * this.vcf_reso);
    this.recalc();
  }
  private float getResonance() {return this.vcf_reso;};

  private void setEnvMod(float e) {
    this.vcf_envmod = e;
    this.recalc();
  }
  private float getEnvMod() {return this.vcf_envmod;}


  private void setEnvDecay(float d) {
      this.vcf_envdecayi = d;
      this.vcf_envdecay = (0.2f + (2.3f * vcf_envdecayi)) * this.srate;

      if (this.vcf_envdecay < 1) this.vcf_envdecay = 1;

      this.vcf_envdecay = (float)Math.pow(0.1f, 1f / (this.vcf_envdecay * ENV_INC));
  }
  private float getEnvDecay() {return this.vcf_envdecayi;}


  private void recalc() {
    this.vcf_e1 = (float)Math.exp(6.109f + 1.5876f * this.vcf_envmod + 2.1553f * this.vcf_cutoff - 1.2f * (1.0f - this.vcf_reso));
    this.vcf_e0 = (float)Math.exp(5.613f - 0.8f * this.vcf_envmod + 2.1553f * this.vcf_cutoff - 0.7696f * (1.0f - this.vcf_reso));
    this.vcf_e0 = this.vcf_e0 * (float)Math.PI / this.srate;
    this.vcf_e1 = this.vcf_e1 * (float)Math.PI / this.srate;
    this.vcf_e1 = this.vcf_e1 - this.vcf_e0;
    this.vcf_envpos = ENV_INC;
  }


  private float rct(float x) { if(x < 0f) return -0.5f; else return 0.5f; }


  private void noteOn(int note, boolean acc, boolean glide) {
  	currentNote = note;
  	
  	this.vco_inc_src = this.vco_inc;
    this.vco_inc_dest = (440f / this.srate) * (float)Math.pow(2f ,((float)note - 57f) * (1f / 12f));

    this.cntmax = Math.round(this.srate * this.sld_spd);
    if (glide) this.cnt = 0; else this.cnt = this.cntmax - 1;

    this.vca_mode = 0;
    this.vcf_c0 = this.vcf_e1;

    this.vcf_envpos = ENV_INC;

    if (acc) this.vca_a0 = 0.5f + this.vca_accamt * 0.5f;
    else this.vca_a0 = 0.5f;

    this.IsNoteOn = true;
  }

  private void noteOff() {
    this.vca_a = 0;
    this.vca_mode = 2;
    this.IsNoteOn = false;
  }

  private void fillProgram (int channel, int prg, MidiProgramName mpn) {
    if (channel == 9) {
      //drums
      mpn.setName("Standard");
      mpn.setMidiProgram((byte)0);
      mpn.setParentCategoryIndex(0);
    }
    else {
      mpn.setName(GMNames.GM_NAMES[prg]);
      mpn.setMidiProgram((byte)prg);
      mpn.setParentCategoryIndex(-1);	// for now

      for (int i = 0; i < GMNames.NUM_GM_CATEGORIES; i++) {
        if (prg >= GMNames.GM_CATEGORIES_FIRST_INDICES[i] &&
            prg < GMNames.GM_CATEGORIES_FIRST_INDICES[i + 1]) {
          mpn.setParentCategoryIndex(i);
          break;
        }
      }

    }

  }

}



class DreiNullDreiProgram {
  public final static int PARAM_ID_VOLUME = 0;
  public final static int PARAM_ID_WAVEFORM = 1;
  public final static int PARAM_ID_GLIDE_SPEED = 2;
  public final static int PARAM_ID_ACC_AMOUNT = 3;
  public final static int PARAM_ID_CUT_OFF = 4;
  public final static int PARAM_ID_RESONANCE = 5;
  public final static int PARAM_ID_ENV_MOD = 6;
  public final static int PARAM_ID_ENV_DECAY = 7;

  public final static int NUM_PARAMS = PARAM_ID_ENV_DECAY + 1;


  private String name = "Init";

  private float volume = 1f;
  private float waveForm = 1;
  private float glideSpeed = 0.1f;
  private float accAmount = 0.5f;
  private float cutOff = 0.9f;
  private float reso = 0.1f;
  private float envMod = 1f;
  private float envDecay = 0.1f;


  public String getName() { return this.name; }
  public void setName(String name) { this.name = name; }

  public float getVolume() { return this.volume; }
  public void setVolume(float v) { this.volume = v; }

  public float getWaveform() { return this.waveForm; }
  public void setWaveform(float v) { this.waveForm = v; }

  public float getGlideSpeed() { return this.glideSpeed; }
  public void setGlideSpeed(float v) { this.glideSpeed = v; }

  public float getAccAmount() { return this.accAmount; }
  public void setAccAmount(float v) { this.accAmount = v; }

  public float getCutoff() { return this.cutOff; }
  public void setCutoff(float v) { this.cutOff = v; }

  public float getResonance() { return this.reso; }
  public void setResonance(float v) { this.reso = v; }

  public float getEnvMod() { return this.envMod; }
  public void setEnvMod(float v) { this.envMod = v; }

  public float getEnvDecay() { return this.envDecay; }
  public void setEnvDecay(float v) { this.envDecay = v; }

}
