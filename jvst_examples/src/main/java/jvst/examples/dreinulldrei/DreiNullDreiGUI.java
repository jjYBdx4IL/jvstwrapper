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

import java.awt.GridLayout;

import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSlider;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.VSTPluginGUIAdapter;
import jvst.wrapper.gui.RotaryKnob;
import jvst.wrapper.gui.RotaryKnobPlusText;
import jvst.wrapper.gui.JSliderPlusText;
import jvst.wrapper.gui.VSTPluginGUIRunner;

//Created using Eclipse GUI Editor!
public class DreiNullDreiGUI extends VSTPluginGUIAdapter implements ChangeListener {

	private static final long serialVersionUID = -2702280782127069661L;
	private JPanel jContentPane = null;
	private JPanel VolumePanel = null;
	private JPanel OscPanel = null;
	private JPanel EnvPanel = null;
	private JPanel FilterPane = null;
	private JPanel FilterPane2 = null;
	private JRadioButton saw = null;
	private JRadioButton wave = null;
	private RotaryKnobPlusText accent = null;
	private RotaryKnobPlusText glide = null;
	private RotaryKnobPlusText envmod = null;
	private RotaryKnobPlusText envdecay = null;
	private RotaryKnobPlusText cutoff = null;
	private RotaryKnobPlusText reso = null;
	private JSliderPlusText volume = null;
	
	
	private VSTPluginAdapter pPlugin;
	protected static boolean DEBUG = false;
	  
	
	
	public void init() {
		if (!DEBUG) {
			//init gui with plugs parameters
			this.volume.getSlider().setValue((int)this.pPlugin.getParameter(DreiNullDreiProgram.PARAM_ID_VOLUME));
		    this.accent.getKnob().setValue(this.pPlugin.getParameter(DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT));
		    this.glide.getKnob().setValue(this.pPlugin.getParameter(DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED));
		    this.envmod.getKnob().setValue(this.pPlugin.getParameter(DreiNullDreiProgram.PARAM_ID_ENV_MOD));
		    this.envdecay.getKnob().setValue(this.pPlugin.getParameter(DreiNullDreiProgram.PARAM_ID_ENV_DECAY));
		    this.cutoff.getKnob().setValue(this.pPlugin.getParameter(DreiNullDreiProgram.PARAM_ID_CUT_OFF));
		    this.reso.getKnob().setValue(this.pPlugin.getParameter(DreiNullDreiProgram.PARAM_ID_RESONANCE));
		
		    //add new listener to gui components (and remove default one!)
		    this.volume.addChangeListener(this);
		    this.accent.addChangeListener(this);
		    this.glide.addChangeListener(this);
		    this.envmod.addChangeListener(this);
		    this.envdecay.addChangeListener(this);
		    this.cutoff.addChangeListener(this);
		    this.reso.addChangeListener(this);
		}
	}
	
	public void stateChanged(ChangeEvent e) {
		Object o = e.getSource();
	    if (!DEBUG) {
		    if (o.equals(this.volume.getSlider())) {
		      JSlider s = (JSlider)o;
		      this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_VOLUME, (float)s.getValue() / 100F);
		      this.volume.getTextField().setText(this.pPlugin.getParameterDisplay(DreiNullDreiProgram.PARAM_ID_VOLUME));
		    }
		    else if (o.equals(this.accent.getKnob())) {
		      RotaryKnob rk = (RotaryKnob)o;
		      this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT, rk.getValue());
		      this.accent.getTextField().setText(this.pPlugin.getParameterDisplay(DreiNullDreiProgram.PARAM_ID_ACC_AMOUNT));
		    }
		    else if (o.equals(this.glide.getKnob())) {
		      RotaryKnob rk = (RotaryKnob)o;
		      this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED, rk.getValue());
		      this.glide.getTextField().setText(this.pPlugin.getParameterDisplay(DreiNullDreiProgram.PARAM_ID_GLIDE_SPEED));
		 	}
		    else if (o.equals(this.envmod.getKnob())) {
		      RotaryKnob rk = (RotaryKnob)o;
		      this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_ENV_MOD, rk.getValue());
		      this.envmod.getTextField().setText(this.pPlugin.getParameterDisplay(DreiNullDreiProgram.PARAM_ID_ENV_MOD));
		    }
		    else if (o.equals(this.envdecay.getKnob())) {
		      RotaryKnob rk = (RotaryKnob)o;
		      this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_ENV_DECAY, rk.getValue());
		      this.envdecay.getTextField().setText(this.pPlugin.getParameterDisplay(DreiNullDreiProgram.PARAM_ID_ENV_DECAY));
		    }
		    else if (o.equals(this.cutoff.getKnob())) {
		      RotaryKnob rk = (RotaryKnob)o;
		      this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_CUT_OFF, rk.getValue());
		      this.cutoff.getTextField().setText(this.pPlugin.getParameterDisplay(DreiNullDreiProgram.PARAM_ID_CUT_OFF));
		    }
		    else if (o.equals(this.reso.getKnob())) {
		      RotaryKnob rk = (RotaryKnob)o;
		      this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_RESONANCE, rk.getValue());
		      this.reso.getTextField().setText(this.pPlugin.getParameterDisplay(DreiNullDreiProgram.PARAM_ID_RESONANCE));
		    }
		    else if (o.equals(this.wave)) {
		      JRadioButton r = (JRadioButton)o;
		      if (r.isEnabled()) this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_WAVEFORM, 0.9F);
		    }
		    else if (o.equals(this.saw)) {
		      JRadioButton r = (JRadioButton)o;
		      if (r.isEnabled()) this.pPlugin.setParameter(DreiNullDreiProgram.PARAM_ID_WAVEFORM, 0.1F);
		    }
	    }
	}
	
	
	
	
	
	/**
	 * This method initializes VolumePanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getVolumePanel() {
		if (VolumePanel == null) {
			VolumePanel = new JPanel();
			VolumePanel.setLayout(new BoxLayout(getVolumePanel(), BoxLayout.Y_AXIS));
			VolumePanel.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Amp", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Dialog", java.awt.Font.BOLD, 12), new java.awt.Color(51,51,51)));
			VolumePanel.add(getSliderPlusText(), null);
		}
		return VolumePanel;
	}

	/**
	 * This method initializes OscPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getOscPanel() {
		if (OscPanel == null) {
			OscPanel = new JPanel();
			OscPanel.setLayout(new BoxLayout(getOscPanel(), BoxLayout.Y_AXIS));
			OscPanel.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Osc", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, null, null));
			OscPanel.add(getSaw(), null);
			OscPanel.add(getWave(), null);
			ButtonGroup bg = new ButtonGroup();
			bg.add(getSaw());
			bg.add(getWave());
		}
		return OscPanel;
	}

	/**
	 * This method initializes EnvPanel	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getEnvPanel() {
		if (EnvPanel == null) {
			EnvPanel = new JPanel();
			EnvPanel.setLayout(new BoxLayout(getEnvPanel(), BoxLayout.Y_AXIS));
			EnvPanel.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Envelope", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Dialog", java.awt.Font.BOLD, 12), new java.awt.Color(51,51,51)));
			EnvPanel.add(getRotaryKnobPlusText2(), null);
			EnvPanel.add(getRotaryKnobPlusText3(), null);
		}
		return EnvPanel;
	}

	/**
	 * This method initializes FilterPane	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getFilterPane() {
		if (FilterPane == null) {
			FilterPane = new JPanel();
			FilterPane.setLayout(new BoxLayout(getFilterPane(), BoxLayout.Y_AXIS));
			FilterPane.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Filter", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, null, null));
			FilterPane.add(getRotaryKnobPlusText4(), null);
			FilterPane.add(getRotaryKnobPlusText5(), null);
		}
		return FilterPane;
	}

	/**
	 * This method initializes FilterPane2	
	 * 	
	 * @return javax.swing.JPanel	
	 */
	private JPanel getFilterPane2() {
		if (FilterPane2 == null) {
			FilterPane2 = new JPanel();
			FilterPane2.setLayout(new BoxLayout(getFilterPane2(), BoxLayout.Y_AXIS));
			FilterPane2.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Accent", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, null, null));
			FilterPane2.add(getRotaryKnobPlusText(), null);
			FilterPane2.add(getRotaryKnobPlusText1(), null);
		}
		return FilterPane2;
	}

	/**
	 * This method initializes saw	
	 * 	
	 * @return javax.swing.JRadioButton	
	 */
	private JRadioButton getSaw() {
		if (saw == null) {
			saw = new JRadioButton();
			saw.setName(null);
			saw.setText("saw");
			saw.setSelected(true);
		}
		return saw;
	}

	/**
	 * This method initializes wave	
	 * 	
	 * @return javax.swing.JRadioButton	
	 */
	private JRadioButton getWave() {
		if (wave == null) {
			wave = new JRadioButton();
			wave.setText("pulse");
			wave.setName("wave");
		}
		return wave;
	}

	/**
	 * This method initializes rotaryKnobPlusText	
	 * 	
	 * @return jvst.wrapper.gui.RotaryKnobPlusText	
	 */
	private RotaryKnobPlusText getRotaryKnobPlusText() {
		if (accent == null) {
			accent = new RotaryKnobPlusText();
			accent.setName("Accent");
			accent.setAlignmentX(0.5F);
		}
		return accent;
	}

	/**
	 * This method initializes rotaryKnobPlusText1	
	 * 	
	 * @return jvst.wrapper.gui.RotaryKnobPlusText	
	 */
	private RotaryKnobPlusText getRotaryKnobPlusText1() {
		if (glide == null) {
			glide = new RotaryKnobPlusText();
			glide.setName("Glide");
		}
		return glide;
	}

	/**
	 * This method initializes rotaryKnobPlusText2	
	 * 	
	 * @return jvst.wrapper.gui.RotaryKnobPlusText	
	 */
	private RotaryKnobPlusText getRotaryKnobPlusText2() {
		if (envmod == null) {
			envmod = new RotaryKnobPlusText();
			envmod.setName("Mod");
		}
		return envmod;
	}

	/**
	 * This method initializes rotaryKnobPlusText3	
	 * 	
	 * @return jvst.wrapper.gui.RotaryKnobPlusText	
	 */
	private RotaryKnobPlusText getRotaryKnobPlusText3() {
		if (envdecay == null) {
			envdecay = new RotaryKnobPlusText();
			envdecay.setName("Decay");
		}
		return envdecay;
	}

	/**
	 * This method initializes rotaryKnobPlusText4	
	 * 	
	 * @return jvst.wrapper.gui.RotaryKnobPlusText	
	 */
	private RotaryKnobPlusText getRotaryKnobPlusText4() {
		if (cutoff == null) {
			cutoff = new RotaryKnobPlusText();
			cutoff.setName("Cutoff");
			cutoff.setVisible(true);
		}
		return cutoff;
	}

	/**
	 * This method initializes rotaryKnobPlusText5	
	 * 	
	 * @return jvst.wrapper.gui.RotaryKnobPlusText	
	 */
	private RotaryKnobPlusText getRotaryKnobPlusText5() {
		if (reso == null) {
			reso = new RotaryKnobPlusText();
			reso.setName("Reso");
		}
		return reso;
	}

	/**
	 * This method initializes sliderPlusText	
	 * 	
	 * @return jvst.wrapper.gui.SliderPlusText	
	 */
	private JSliderPlusText getSliderPlusText() {
		if (volume == null) {
			volume = new JSliderPlusText();
			volume.setName("Volume");
		}
		return volume;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Throwable {
		DEBUG=true;
		
	    DreiNullDreiGUI gui = new DreiNullDreiGUI(null,null);
	    gui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	/**
	 * This is the default constructor
	 * @throws UnsupportedLookAndFeelException 
	 * @throws IllegalAccessException 
	 * @throws InstantiationException 
	 * @throws Exception 
	 */
	public DreiNullDreiGUI(VSTPluginGUIRunner r, VSTPluginAdapter plug) throws Exception {
		super(r,plug);
		this.pPlugin=plug;
		initialize();
		this.init();
		//this is needed on the mac only, 
	    //java guis are handled there in a pretty different way than on win/linux
	    //XXX
	    if (RUNNING_MAC_X) show();
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(415, 258);
		//*** IMPORTANT *** dont set the close behavior, this is done in some super class already!!!
		//this.setDefaultCloseOperation(javax.swing.JFrame.HIDE_ON_CLOSE);
		this.setContentPane(getJContentPane());
		this.setTitle("DreiNullDrei");
		//*** IMPORTANT *** this stuff is all covered by the underlying class (VSTPluginGUIAdapter)
		//this.setVisible(true);
		
		this.setResizable(false);
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			GridLayout gridLayout = new GridLayout();
			gridLayout.setRows(1);
			gridLayout.setColumns(5);
			jContentPane = new JPanel();
			jContentPane.setLayout(gridLayout);
			jContentPane.add(getVolumePanel(), null);
			jContentPane.add(getOscPanel(), null);
			jContentPane.add(getEnvPanel(), null);
			jContentPane.add(getFilterPane(), null);
			jContentPane.add(getFilterPane2(), null);
		}
		return jContentPane;
	}




}  //  @jve:decl-index=0:visual-constraint="137,6"
