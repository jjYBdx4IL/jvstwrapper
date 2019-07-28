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

//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Wrapper for VSTi GUI
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTGUIWrapper__
#define __VSTGUIWrapper__


#include <jni.h>

//MACX is defined by vstgui.h!
#if defined(linux) 
	#include "vstgui_linux/vstgui.h"
#else
	#include "vstgui/vstgui.h"
#endif

#if defined(MACX) || defined(linux)
	#include <pthread.h>
#endif
#ifdef linux
  	#include <X11/Xlib.h>
  	//#include <X11/Xutil.h>
  	//#include <X11/Xatom.h>
  	#undef KeyPress
#endif


//-----------------------------------------------------------------------------
class VSTGUIWrapper : public AEffGUIEditor, public CControlListener {

	public:
		VSTGUIWrapper (AudioEffect *effect, jclass guiRunnerClass, jstring guiclazz);	
		virtual ~VSTGUIWrapper ();

		virtual bool open (void *ptr);
		virtual void close ();
		virtual bool getRect (ERect **ppRect);

		virtual void valueChanged (CDrawContext* context, CControl* control);

		//Utility
		int initJavaSide();	
		void idle();

#ifdef WIN32
		HWND JavaWindowHandle;
#endif
#ifdef linux
		Window JavaWindowHandle;
		//Window ParentWindow;
		Display* JavaDisplay;
#endif

	protected:
		bool checkException(JNIEnv* env);


	private:
		JNIEnv* ensureJavaThreadAttachment();

#if defined(WIN32) || defined(linux)
		void detachWindow();
		void undecorateJavaWindow();
#endif
#ifdef linux
		Window getParentWindow(Display *d, Window w);
#endif

		JavaVM *Jvm;
		jobject JavaPlugObj;
		jobject JavaPlugGUIObj;
		jclass JavaPlugGUIClass;
		jstring JavaPlugGUIString;
		jmethodID IdleMethodID;
		
		bool IsInitialized;
		bool AttachWindow;

		
		//VSTGUI stuff
		COnOffButton *hButton;
		CBitmap *hBackground;
};

#endif

