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
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class RotaryKnobPlusText extends Box implements ChangeListener {

	private static final long serialVersionUID = -114796980764805052L;

    private RotaryKnob knob;   
    private JTextField text;
    private JLabel label;
    
    public RotaryKnobPlusText() {
    	this("unnamed");
    }
    
	public RotaryKnobPlusText(String name) {
		super(BoxLayout.Y_AXIS);
		
		this.label = new JLabel(name);
		this.label.setAlignmentX(CENTER_ALIGNMENT);
		this.add(this.label);
		
		this.knob = new RotaryKnob();
		this.knob.setAlignmentX(CENTER_ALIGNMENT);
		this.add(this.knob);
		
		this.text = new JTextField("0.00");
		this.text.setEditable(false); //TODO: make editable --> more fancy!
		this.text.setAlignmentX(CENTER_ALIGNMENT);
		this.text.setMaximumSize(new Dimension(5000, 18));
	    this.add(this.text);
	    
	    this.knob.setValue(0);
	    this.knob.addChangeListener(this); //default
	}

	
	//default hander!
	public void stateChanged(ChangeEvent e) {
    	this.text.setText(Float.toString(((int)(this.knob.getValue()*100f))/100f));
	}

	//remove default listener and add custom one
	public void addChangeListener(ChangeListener cl)  {
		this.knob.removeChangeListener(this);
		this.knob.addChangeListener(cl);		
	}
	
	
	public RotaryKnob getKnob() {
		return knob;
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
		JFrame win = new JFrame("KnobPlusText");
		win.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
	    win.getContentPane().setLayout(new BorderLayout());
	    win.setSize(100,140);
	    win.getContentPane().add(new RotaryKnobPlusText("test"), BorderLayout.CENTER);
	    win.setVisible(true);
	}
}
