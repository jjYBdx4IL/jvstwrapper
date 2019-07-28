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

package jvst.wrapper.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class JSliderPlusText extends Box implements ChangeListener {

	private static final long serialVersionUID = 4797525904877917491L;

    private JSlider slider;   
    private JTextField text;
    private JLabel label;
    
    public JSliderPlusText() {
    	this("unnamed");
    }
    
	public JSliderPlusText(String name) {
		super(BoxLayout.Y_AXIS);
		
		this.label = new JLabel(name);
		this.label.setAlignmentX(CENTER_ALIGNMENT);
		this.add(this.label);
		
		this.slider = new JSlider(JSlider.VERTICAL);
		this.slider.setAlignmentX(CENTER_ALIGNMENT);
		this.add(this.slider);
		
		this.text = new JTextField("0.00");
		this.text.setEditable(false);
		this.text.setAlignmentX(CENTER_ALIGNMENT);
		this.text.setMaximumSize(new Dimension(5000, 18));
	    this.add(this.text);
	    
	    this.slider.setValue(0);	    
	    this.slider.addChangeListener(this); //default
	}


	public void stateChanged(ChangeEvent e) {
		this.text.setText(Integer.toString(this.slider.getValue()));
	}

	//remove default listener and add custom one
	public void addChangeListener(ChangeListener cl)  {
		this.slider.removeChangeListener(this);
		this.slider.addChangeListener(cl);		
	}

	public JSlider getSlider() {
		return slider;
	}
	public JTextField getTextField() {
		return text;
	}
	
	public void setName(String name) {
		this.label.setText(name);
	}
	public String getName() {
		return this.label.getText();
	}
	
	
	
	

	public static void main(String[] args) {
		JFrame win = new JFrame("SliderPlusText");
		win.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
	    win.getContentPane().setLayout(new BorderLayout());
	    win.setSize(40,140);
	    win.getContentPane().add(new JSliderPlusText("test"), BorderLayout.CENTER);
	    win.setVisible(true);
	}
}
