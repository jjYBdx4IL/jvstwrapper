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

import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.gui.VSTPluginGUIRunner;

import net.sourceforge.napkinlaf.NapkinLookAndFeel;



public class DreiNullDreiNapkinGUI extends DreiNullDreiGUI {

	private static final long serialVersionUID = -4934701250252694934L;

	
	public DreiNullDreiNapkinGUI(VSTPluginGUIRunner r, VSTPluginAdapter plug) throws Exception {
		super(r,plug);
		//throw exception that the user gets a message box with the 
		//stack trace if something fails in the constructor here...
		
		//TELL the UI Manager that it should use the Plugins ClassLoader to load UI classes. 
		UIManager.put("ClassLoader", this.getClass().getClassLoader());

		//use the napkin look-and-feel for this plugin!
		UIManager.setLookAndFeel(new NapkinLookAndFeel());
		SwingUtilities.updateComponentTreeUI(this);
	}
	
	
	
	
	
	
	public static void main(String[] args) throws Throwable {
		DEBUG=true;
		
		DreiNullDreiGUI gui = new DreiNullDreiNapkinGUI(null,null);
	}
}
