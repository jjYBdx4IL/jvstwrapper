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

package jvst.wrapper;

import java.awt.Dimension;
import java.awt.Toolkit;
import javax.swing.JFrame;
import jvst.wrapper.gui.VSTPluginGUI;
import jvst.wrapper.gui.VSTPluginGUIRunner;

public abstract class VSTPluginGUIAdapter extends JFrame implements VSTPluginGUI {

	// borrowed from apple developer connection
	public static final boolean RUNNING_MAC_X = System.getProperty("os.name").toLowerCase().startsWith("mac os x");
	
	public VSTPluginGUIRunner runner = null;
	protected VSTPluginAdapter plugin = null;
	
	
	public VSTPluginGUIAdapter(VSTPluginGUIRunner r, VSTPluginAdapter plugin) {
		this.runner=r;
		this.plugin=plugin;
		
		//return gracefully when everything is null --> DEBUG mode
		if (r==null && plugin==null) return;
		
		// center window
		/*
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		Dimension frameSize = this.getSize();
		if (frameSize.height > screenSize.height) frameSize.height = screenSize.height;
		if (frameSize.width > screenSize.width) frameSize.width = screenSize.width;
		this.setLocation((screenSize.width - frameSize.width) / 2, (screenSize.height - frameSize.height) / 2);
		*/
		
		// Prepare attaching window
		runner.WindowAttached = false;
		if (!runner.libraryOk) {
			// Load library for native-awt methods
			try {
				if(!RUNNING_MAC_X) { //doesnt work on mac
					System.loadLibrary("jawt");
					runner.libraryOk = true;
				}
			} catch (Exception e) {
				runner.libraryOk = false;
				log("caught exception while trying to load jawt, but thats not so bad, loading gui in a separate window then. exception below:");
				e.printStackTrace();
			} catch (Error e) {
				// Maybe library is already loaded
				//TODO: find a better way!
				if (e.getMessage().indexOf("already loaded") != -1) runner.libraryOk = true;
				else {
					runner.libraryOk = false;
					e.printStackTrace();
				}
			}
		}
		
		// close behavior
		this.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
		//this.hide(); // keep hide() to be backward compatible to 1.4!

		log("\nJAVA GUI Plugin intitialised properly!");
	}

	public void undecorate() {
		if ((runner.WindowAttached) && (!this.isUndecorated())) {
			javax.swing.JFrame f = new javax.swing.JFrame();
			f.pack();
			this.dispose();
			this.setUndecorated(true);
			this.pack();
			f.dispose();
			
			java.awt.Rectangle bounds = this.getBounds();
			bounds.x = 0;
			bounds.y = 0;
			this.setBounds(bounds);
		} 
		else if ((!runner.WindowAttached) && (this.isUndecorated())) {
			// Redecorate the window
			javax.swing.JFrame f = new javax.swing.JFrame();
			f.pack();
			this.dispose();
			this.setUndecorated(false);
			this.pack();
			f.dispose();
			
			// center window
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
			Dimension frameSize = this.getSize();

			if (frameSize.height > screenSize.height) frameSize.height = screenSize.height;
			if (frameSize.width > screenSize.width) frameSize.width = screenSize.width;
			
			java.awt.Rectangle bounds = this.getBounds();
			bounds.x = (screenSize.width - frameSize.width) / 2;
			bounds.y = (screenSize.height - frameSize.height) / 2;
			this.setBounds(bounds);
		}
	}

	// *********************************************************************
	public void open() {
		log("GUI open");
		this.show(); // keep show() to be backward compatible to 1.4!
		this.toFront();
	}

	public void close() {
		this.log("GUI close");
		this.hide(); // keep hide() to be backward compatible to 1.4!
	}

	public void destroy() {
		//MAC was deadlocking when calling destroy() --> well, then simply hide the window instead :-)
		//TODO: still the case ???
		if(!RUNNING_MAC_X) {
			this.log("GUI destroy");
			this.close(); //first close --> hide window before destroy
			this.dispose();
		}
		else {
			this.close();
		}
	}
	
	public void idle() {
		//empty by default, should be used for plug-->host calls (setParameterAutomated, beginEdit, endEdit, ...) ONLY!
		//absolutely DO NOT ATTEMPT ANY GUI related calls here, these must all be decoupled with invokeLater()...
	}

	// ***********************************************************************
	protected void log(String s) {
		VSTPluginAdapter.log(s);
	}

}
