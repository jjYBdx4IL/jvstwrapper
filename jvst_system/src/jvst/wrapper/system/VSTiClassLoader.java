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


package jvst.wrapper.system;

import java.net.URL;
import java.net.URLClassLoader;


public class VSTiClassLoader extends URLClassLoader {

	public VSTiClassLoader(URL[] urls) {
		super(urls);
	}
	
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#findClass(java.lang.String)
	 */
	protected Class findClass(String arg0) throws ClassNotFoundException {
		//VSTiClassLoaderManager.log("findClass: " + arg0); 
		
		// TODO Auto-generated method stub
		return super.findClass(arg0);
	}
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#findLibrary(java.lang.String)
	 */
	protected String findLibrary(String arg0) {
		//VSTiClassLoaderManager.log("findLibrary: " + arg0);
		
		// TODO Auto-generated method stub
		return super.findLibrary(arg0);
	}
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#loadClass(java.lang.String, boolean)
	 */
	protected synchronized Class loadClass(String arg0, boolean arg1)
			throws ClassNotFoundException {
		//VSTiClassLoaderManager.log("loadClass: " + arg0 + " bool=" + arg1);
		
		// TODO Auto-generated method stub
		return super.loadClass(arg0, arg1);
	}
	/* (non-Javadoc)
	 * @see java.lang.ClassLoader#loadClass(java.lang.String)
	 */
	public Class loadClass(String arg0) throws ClassNotFoundException {
		//VSTiClassLoaderManager.log("loadClass: " + arg0);
		
		// TODO Auto-generated method stub
		return super.loadClass(arg0);
	}
}
