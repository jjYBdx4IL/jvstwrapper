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

//import java.io.BufferedOutputStream;
import java.io.File;
//import java.io.FileOutputStream;
//import java.io.PrintStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;

/*
 * the reason why we need this class is that classloaders provide means to 
 * enforce native library separation. See 
 * http://java.sun.com/docs/books/jni/html/design.html
 * for more information.   
 */
 
public class VSTiClassLoaderManager {

	private static Map classLoaders = new HashMap(10);
	
	/*
	private static PrintStream str;
	static {
		try {
			str = new PrintStream(new BufferedOutputStream(
					new FileOutputStream("VSTiLoader.txt", false)));
			System.setErr(str);
			System.setOut(str);
		} catch (Throwable t) {
			//ignore
		}
	}

	public static void log(String m) {
		str.println(m);
		str.flush();
	}
	*/
	
	
	public static Object createVSTiInstance(Class clazz)
			throws InstantiationException, IllegalAccessException {
		Object instance = clazz.newInstance();
		return instance;
	}

	public static Class loadVSTiClass(String nativeLib, String fqClassName,
			String classPath) throws ClassNotFoundException {
		if (fqClassName == null)
			throw new IllegalArgumentException("fqClassName is NULL!");
		if (nativeLib == null)
			throw new IllegalArgumentException("nativeLib is NULL!");
		if (classPath == null)
			throw new IllegalArgumentException("classPath is NULL!");

		
		ArrayList urlList = new ArrayList(10);
		StringTokenizer strTok = new StringTokenizer(classPath, System.getProperty("path.separator", ";"), false);
		while (strTok.hasMoreTokens()) {
			String tok = strTok.nextToken();
			File f = new File(tok);
			if (!f.exists())
				continue;

			try {
				//log("Adding URL:" + f.toURL().toString());
				urlList.add(f.toURL());
			} catch (MalformedURLException e) {
				throw new IllegalArgumentException(e.toString());
			}
		}

		URL[] urls = (URL[])urlList.toArray(new URL[1]);
		
		fqClassName = fqClassName.replaceAll("/", ".");

		//The virtual machine does not allow a given JNI native library to be loaded 
		//by more than one class loader. Attempting to load the same native library 
		//by multiple class loaders causes an UnsatisfiedLinkError to be thrown
		ClassLoader loader = (ClassLoader) classLoaders.get(nativeLib);

		if (loader == null) {
			//log("new Loader for: " + nativeLib);

			//no associated cl so far. plug is being loaded for the first time!
			loader = new VSTiClassLoader(urls);
			Class clazz = loader.loadClass(fqClassName);

			classLoaders.put(nativeLib, loader);

			//log("Class Loader=" + clazz.getClassLoader());

			return clazz;
		} else {
			//log("Reusing Loader for: " + nativeLib);

			//this plug has been loaded.. reuse class loader so that the native
			//calls are being redirected to the correct library!
			Class clazz = loader.loadClass(fqClassName);

			//log("Class Loader=" + clazz.getClassLoader());

			return clazz;
		}
	}

}