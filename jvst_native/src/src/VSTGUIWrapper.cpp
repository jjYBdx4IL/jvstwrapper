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


#include "VSTGUIWrapper.h"
#include "VSTV10ToPlug.h"
#include "ConfigFileReader.h"
#include "JNIUtils.h"


#if defined(WIN32) || defined(linux)
	#include "jawt_md.h"
#endif
#if defined(MACX) || defined(linux)
	#include <pthread.h>
#endif
#ifdef MACX
	#include "VSTGUIWrapperMAC.h"
#endif
#ifdef WIN32
	#include <windows.h>
#endif
#ifdef linux
	#include <unistd.h>

	// images
	#include "background.xpm"
	#include "button.xpm"
#endif



//-----------------------------------------------------------------------------
enum {
	// resource id's
	kBackgroundID = 128,
	kButtonID,

	// tags
	kButtonTag = 42,

	// positions
	kButtonX = 228,
	kButtonY = 13,
};

#ifdef linux
	CResTable xpmResources = {
		{kBackgroundID, (char**) background_xpm},
		{kButtonID,     (char**) button_xpm},
		{0,             0}
	};

	//Global display --> defined by VSTGUI
	extern Display* display;
#endif



VSTGUIWrapper::VSTGUIWrapper (AudioEffect *effect, jclass guiRunnerClass, jstring guiclazz)
	: AEffGUIEditor (effect) {

	log("GUI wrapper init");

	this->Jvm = ((VSTV10ToPlug*)effect)->Jvm;
	this->JavaPlugObj = ((VSTV10ToPlug*)effect)->JavaPlugObj;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	this->JavaPlugGUIClass = (jclass) env->NewGlobalRef(guiRunnerClass);
	this->JavaPlugGUIString = (jstring) env->NewGlobalRef(guiclazz);

	this->IsInitialized=false;
	this->IdleMethodID=NULL;

	this->AttachWindow=false;
	ConfigFileReader *cfg = new ConfigFileReader();
	if(cfg!=NULL) {
		this->AttachWindow=(cfg->AttachToNativePluginWindow==1);
		delete cfg;
	}

#if defined (WIN32) || defined(linux)
	this->JavaWindowHandle = 0;
#endif
#ifdef MACX
	//no window embedding on the mac for now...
	this->AttachWindow=0;
#endif
#ifdef linux
	// XInitThreads(); this is causing major problems with QT4 based apps (Qtractor)
	// --> disabled. still works fine with Jost, Renoise, eXT2
	// DO NOT ENABLE AGAIN!
	/*
	Status s = XInitThreads();
	log("XInitThreads()=%i", s);
	*/

	//this->ParentWindow = 0;
	this->JavaDisplay=NULL;
#endif

	hBackground = NULL;
	hButton = NULL;

	// load the background bitmap --> only used if attachwindow==false
	hBackground = new CBitmap (kBackgroundID);
}


//-----------------------------------------------------------------------------
void VSTGUIWrapper::idle () {
	if (this->IsInitialized==false) return;
	JNIEnv* env = this->ensureJavaThreadAttachment();

	if (this->IdleMethodID == NULL) this->IdleMethodID = env->GetMethodID(this->JavaPlugGUIClass, "idle", "()V");
	if (this->IdleMethodID == NULL) log("** ERROR: cannot find GUI instance-method idle()V");
	env->CallVoidMethod(this->JavaPlugGUIObj, this->IdleMethodID);
	this->checkException(env);
}


//-----------------------------------------------------------------------------
bool VSTGUIWrapper::getRect (ERect **ppErect) {
	if (ppErect==NULL) return false; //sanity check parameter

	log("GUI wrapper getRect");
	if(this->AttachWindow) {
		JNIEnv* env = this->ensureJavaThreadAttachment();

		if (this->IsInitialized==false) {
#ifdef MACX
			//well, mac window embedding isnt implemented, so we wont come to this point anyways...
			performOnAnotherThread(this, NULL, NULL, GuiWrapperInitJavaSide, false);
			return false;
#else
			initJavaSide();
#endif
		}

		//sanity check
		if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return false;

		jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "getWidth", "()I");
		if (mid == NULL) log("** ERROR: cannot find GUI instance-method getWidth()I");
		jint width=env->CallIntMethod(this->JavaPlugGUIObj, mid);
		this->checkException(env);

		mid = env->GetMethodID(this->JavaPlugGUIClass, "getHeight", "()I");
		if (mid == NULL) log("** ERROR: cannot find GUI instance-method getHeight()I");
		jint height=env->CallIntMethod(this->JavaPlugGUIObj, mid);
		this->checkException(env);

		rect.left   = 0;
		rect.top    = 0;
		rect.right  = (short)width;
		rect.bottom = (short)height;
	}
	else {
		rect.left   = 0;
		rect.top    = 0;
		rect.right  = (short)hBackground->getWidth();
		rect.bottom = (short)hBackground->getHeight();
	}
	*ppErect = &rect;
	return true;
}

//-----------------------------------------------------------------------------
VSTGUIWrapper::~VSTGUIWrapper () {
	log("GUI wrapper destroy");

	if (this->IsInitialized==true) {
		JNIEnv* env = this->ensureJavaThreadAttachment();

		//destroy() der gui aufrufen
		//macx tends to block forever here...
		jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "destroy", "()V");
		if (mid == NULL) log("** ERROR: cannot find GUI instance-method destroy()V");
		env->CallVoidMethod(this->JavaPlugGUIObj, mid);
		this->checkException(env);

		//free global reference
		env->DeleteGlobalRef(this->JavaPlugGUIObj);
	}

	// free the background bitmap
	if (hBackground) hBackground->forget ();
	hBackground = 0;

	log("GUI Wrapper destroyed!");
}


#ifdef linux
int xErrorHandler(Display *dp, XErrorEvent *e) {
	char text[1024];
	XGetErrorText(dp, e->error_code, text, sizeof(text));
	text[1023]='\0';
	log("**XError: display=%p, code=%i, minor=%i, request=%i, ressource=%i, serial=%i, type=%i, text=%s",
		dp, e->error_code, e->minor_code, e->request_code, e->resourceid, e->serial, e->type, text);

	//TODO: maybe set this->AttachWindow to false if a bad window error occurs???
	return 0;
}
#endif


bool VSTGUIWrapper::open (void *ptr) {
	log("GUI wrapper open %p", ptr);

	if (this->IsInitialized==false) {
#ifdef MACX
		//well, mac window embedding isnt implemented, so we wont come to this point anyways...
		performOnAnotherThread(this, NULL, NULL, GuiWrapperInitJavaSide, false);
		//return true; //commented, because this also skips the creation of the VSTGUI part --> empty ui on first open()

		//this way, we miss the first call to open()!
		//--> we fix this on the java side: last step of gui initilialization there is opening the gui
#else
		initJavaSide();
		//can continue from here since we did a invokeAndWait to init the java frame
#endif
	}

	//sanity check
	//if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return false;

	//!!! always call this !!!
	AEffGUIEditor::open(ptr);
	JNIEnv* env = this->ensureJavaThreadAttachment();


#ifdef linux
	//install error handler to be notified if anything goes wrong in the win embedding code...
	XSetErrorHandler(xErrorHandler);
#endif

#if defined(WIN32) || defined(linux)
	ConfigFileReader *cfg = new ConfigFileReader();
	JAWT awt;
	jboolean result;
	bool isAttached = false;

	if (this->AttachWindow && ptr!=NULL) {
		log("attaching");
		this->AttachWindow=false; //if something goes wrong, dont try again
		jfieldID libraryOk = env->GetFieldID(this->JavaPlugGUIClass, "libraryOk", "Z");
		this->checkException(env);

		if (libraryOk != NULL) {
			jboolean lOk = env->GetBooleanField(this->JavaPlugGUIObj, libraryOk);
			this->checkException(env);

			if(lOk==JNI_TRUE) {
				log("successfully loaded the jawt library");
				// Get the AWT
				awt.version = JAWT_VERSION_1_3;
				result = JAWT_GetAWT(env, &awt);

				if(result != JNI_FALSE) {
					log("JAWT structs initialised!");
					//Inform the class
					jfieldID attachField = env->GetFieldID(this->JavaPlugGUIClass, "WindowAttached", "Z");
					if (attachField != NULL) env->SetBooleanField(this->JavaPlugGUIObj, attachField, JNI_TRUE);
					this->checkException(env);

					isAttached=true;
					this->AttachWindow=true;
				}
				else log("** Error: JAWT_GetAWT returned with error!");
			}
			else log("** Error: Could not load the JAWT library on the java side!");
		}
	}
	else this->AttachWindow=false; //ptr was NULL, dont trust this host any more!


	if ((!isAttached) && (cfg->CloseNativePluginWindow==1)) {
#ifdef WIN32
		DestroyWindow(GetParent((HWND)ptr));
#endif
#ifdef linux
		/* Not implemented on Linux! */
		//if (::GlobalDisplay!=NULL)
			//XDestroyWindow(::GlobalDisplay, (Window)ptr);
			//XUnmapWindow(::GlobalDisplay, (Window)ptr);
#endif
	}
	if (cfg) delete cfg;


	if(isAttached) {
		jfieldID guiField = env->GetFieldID(this->JavaPlugGUIClass, "gui", "Ljvst/wrapper/VSTPluginGUIAdapter;");
		this->checkException(env);
		jobject guiObject = env->GetObjectField(this->JavaPlugGUIObj, guiField);
		this->checkException(env);
		if (guiField==NULL || guiObject==NULL) {
			log("** ERROR: guiField==NULL || guiObject==NULL");
			return false;
		}

		// Call Undecorate
		this->undecorateJavaWindow();

		//Try to attach the Window
		log("Attaching Window");
		JAWT_DrawingSurface* ds;
		JAWT_DrawingSurfaceInfo* dsi;
#ifdef WIN32
		JAWT_Win32DrawingSurfaceInfo* dsi_win;
#endif
#ifdef linux
		JAWT_X11DrawingSurfaceInfo* dsi_win;
#endif
		jint lock;
  	    // Get the drawing surface (jframe)
		ds = awt.GetDrawingSurface(env, guiObject);
		if(ds != NULL){
			log("obtained drawing surface");
			// Lock the drawing surface
			lock = ds->Lock(ds);
			if((lock & JAWT_LOCK_ERROR) == 0) {
				log("locked the drawing surface");
   				// Get the drawing surface info
				dsi = ds->GetDrawingSurfaceInfo(ds);

				if(dsi!=NULL) {
					log("obtained ds info");
					// Get the platform-specific drawing info
#ifdef WIN32
					dsi_win = (JAWT_Win32DrawingSurfaceInfo*)dsi->platformInfo;
#endif
#ifdef linux
					dsi_win = (JAWT_X11DrawingSurfaceInfo*)dsi->platformInfo;
#endif

#ifdef WIN32
					//Get Java Window-Handle
					this->JavaWindowHandle=dsi_win->hwnd;

					//window style
					LONG style;

					//http://msdn.microsoft.com/en-us/library/ms633541%28VS.85%29.aspx
					//do what msdn suggests: clear a possible WS_POPUP hint here, and add WS_CHILD
					//java window
					//style=GetWindowLong((HWND)JavaWindowHandle, GWL_STYLE);
					//SetWindowLong((HWND)JavaWindowHandle, GWL_STYLE, (style & ~WS_POPUP) | WS_CHILD);
					//--> IMPORTANT: reverse these styles again when unembedding the window!
					// CAUSES DEADLOCK (threading issue? --> dont do this for now...)

					//also, add hint so that drawing stuff works correctly
					//host window
					//style=(LONG)GetWindowLong(((HWND)ptr), GWL_STYLE);
					//SetWindowLong((HWND)ptr, GWL_STYLE, style | WS_CLIPCHILDREN);
					// --> not needed, set by default

					//reparent windows
					HWND ret = SetParent((HWND)JavaWindowHandle, (HWND)ptr);
					log("reparent=%p", ret);

					//style=GetWindowLong((HWND)JavaWindowHandle, GWL_STYLE);
					//SetWindowLong((HWND)JavaWindowHandle, GWL_STYLE, (style & ~WS_CHILD) | WS_POPUP);
#endif
#ifdef linux
					//Get Java Window-Handle
					this->JavaWindowHandle=dsi_win->drawable;  //cast drawable to window! (is only a handle anyways...)
					this->JavaDisplay = dsi_win->display;
					log("Java Display = %p", this->JavaDisplay);

					//reparent window

					//works only approx. 1 out of 7 times
					//int ret = XReparentWindow (this->JavaDisplay, this->JavaWindowHandle, (Window)ptr, 0, 0);

					//this is much better (works reliable with Renoise and Jost, only EnergyXT makes problems)
					int ret = XReparentWindow (::display, this->JavaWindowHandle, (Window)ptr, 0, 0);

					//old reparent
					/*
					//Save current parent window in order to be able to restore it on close()
					this->ParentWindow = getParentWindow(::display, this->JavaWindowHandle);
					log("Parent Window=%i", this->ParentWindow);
					int ret = XReparentWindow (::display, this->JavaWindowHandle, (Window)ptr, 0, 0);
					*/

					log("win reparent=%i", ret);
#endif

					// Free the drawing surface info
					ds->FreeDrawingSurfaceInfo(dsi);
				}
				else log("**Error: Unable to retrieve drawing surface INFO!");

				// Unlock the drawing surface
				ds->Unlock(ds);
			}
			else log("**Error: Unable to LOCK the drawing surface!");

			// Free the drawing surface
			awt.FreeDrawingSurface(ds);
		}
		else log("**Error: Unable to retrieve the drawing surface!");

	} //isAttached
#endif


	if (this->IsInitialized==true) {
		// Call Open in the java side
		if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return false;
		jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "open", "()V");
		if (mid == NULL) log("** ERROR: cannot find GUI instance-method open()V");
		if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return false;
		env->CallVoidMethod(this->JavaPlugGUIObj, mid);
	}

	//TODO: or even to this line here

#if defined(WIN32) || defined(linux)
	//Check exceptions in open, if exception, then unattach immediately
	if(env->ExceptionCheck()==JNI_TRUE) {
		//If a Exception occured close the gui again
		log("Exception in Open!");
		this->checkException(env); //log and clear exception

		if (isAttached) {
	        //Close Gui
			jmethodID midClose = env->GetMethodID(this->JavaPlugGUIClass, "close", "()V");
		    if (midClose == NULL) log("** ERROR: cannot find GUI instance-method close()V");
		    env->CallVoidMethod(this->JavaPlugGUIObj, midClose);
			this->checkException(env);

			log("* WARNING: Exception occured in GUI open() --> disabling native window attachment");
			this->detachWindow(); //detach window

			this->undecorateJavaWindow(); //call undecorate (this now redecorates the window)

			//Call open again
			jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "open", "()V");
			if (mid == NULL) log("** ERROR: cannot find GUI instance-method open()V");
	 	    env->CallVoidMethod(this->JavaPlugGUIObj, mid);
	 	    this->checkException(env);
		}

		this->AttachWindow=false; //dont attach again!
	}
#endif

	this->checkException(env);

	if (this->AttachWindow==false) {
		// if something went wrong with the attaching process, or attaching is deactivated,
		// show a small gui here that allows to show/hide the java gui

		log("creating custom view...");
		CRect size (0, 0, hBackground->getWidth(), hBackground->getHeight());

		log("new cframe");
		CFrame* lFrame = new CFrame (size, ptr, this);
		log("cframe OK!");

		lFrame->setBackground (hBackground);

		CBitmap* hButtonBG = new CBitmap (kButtonID);
		size (kButtonX, kButtonY, kButtonX + hButtonBG->getWidth(), kButtonY + hButtonBG->getHeight()/2);
		hButton = new COnOffButton(size, this, kButtonTag, hButtonBG);
		lFrame->addView (hButton);

		hButtonBG->forget();

		frame=lFrame;
		log("custom view ready!");
	}

	return true;
}


void VSTGUIWrapper::valueChanged (CDrawContext* context, CControl* control) {
	if (this->IsInitialized==false) return;

	log("Value Changed! tag=%i value=%f", control->getTag(), control->getValue());

	long tag = control->getTag();

	switch (tag) {
	case kButtonTag:
		float val = control->getValue();
		JNIEnv* env = this->ensureJavaThreadAttachment();
		if (val==1.0) {
			//hide
			if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return;
			jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "close", "()V");
			if (mid == NULL) log("** ERROR: cannot find GUI instance-method close()V");
			if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return;
			env->CallVoidMethod(this->JavaPlugGUIObj, mid);
			this->checkException(env);
		}
		else {
			//show
			if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return;
			jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "open", "()V");
			if (mid == NULL) log("** ERROR: cannot find GUI instance-method open()V");
			if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return;
			env->CallVoidMethod(this->JavaPlugGUIObj, mid);
			this->checkException(env);
		}
		break;
	}

#ifdef linux
	//tip from lucio --> force GUI update here
	//most linux hosts seem not to do this on their own, so manually update gui here...
	update();
#endif
}



//-----------------------------------------------------------------------------
void VSTGUIWrapper::close () {
	log("closing GUI");

	if (this->IsInitialized==false) return;

	JNIEnv* env = this->ensureJavaThreadAttachment();

//LINUX:
//This is the not so nice solution (undecorated java window shows up briefly before hide() is called).
//However, this does work on energyXT2 and Renoise, with the restriction that the (java) call to hide()
//does not have any effect (dont know why...). This means that after close, the java window is detached,
//but is not hidden, e.g. it stays visible.
#if defined(WIN32) || defined(linux)
	this->detachWindow(); //first detach, then close
#endif


	//if native window attaching is NOT used, dont call close on the java window
	//the java windows has its own "x" button to be closed. users might want to close the native stub
	//window (show/hide button and jvst logo) without closing the java plugin gui window
	//if attaching is on however, we need to close also the java window (since it is embedded in the native window)
	if (this->AttachWindow==true) {
		jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "close", "()V");
		if (mid == NULL) log("** ERROR: cannot find GUI instance-method close()V");
		env->CallVoidMethod(this->JavaPlugGUIObj, mid);
	}
	bool error = this->checkException(env);


// LINUX:
// well, this would look best (the window is made invisible first, then the java win is detached from
// the native win. Thus no flickering of the java window occurs since detaching causes the java win to
// be displayed in undecorated form at its original location (upper left corner of the screen).
// However, this DOES NOT WORK in energyXT2 or Renoise. Somehow the java window gets corrupted
// and will never show up again when calling show();

//#if defined(linux)
//	this->detachWindow(); //first detach, then close
//#endif

#if defined(WIN32) || defined(linux)
	if (error) {
		log("* WARNING: Exception occured in GUI close() --> disabling native window attachment");
		this->undecorateJavaWindow(); //redecorate the unattached window
		this->AttachWindow=false; //error in close --> dont attach to window again!
	}
#endif
}



// *** Performed on a DIFFERENT THREAD ***
// starting this thread and initializing the AWT is done as late as possible in order to
// allow the test routines of the host (that only instantiate the plug, not the gui)
// can run synchroneous and exit gracefully. is also much faster because the awt doesnt have to be initialized

// KEEP IN MIND: a very bad behaving host may quickly init the gui, but then terminate immediately,
// leaving this thread running alone, and destroying THIS instance.
// --> sanity check each access of 'this' and member vars
//-----------------------------------------------------------------------------
int VSTGUIWrapper::initJavaSide() {
	if (this->IsInitialized==true) return -1;

	log("GUI wrapper initJavaSide");
	if (!this) return -1;
	JNIEnv* env = this->ensureJavaThreadAttachment();

	log("WITHIN gui initjavaside");
	//log("Current thread=%i", pthread_self());

	log("creating instance of GUIRunner");
	if (!this) return -1;
	jmethodID mid = env->GetMethodID(this->JavaPlugGUIClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find constructor of VSTPluginGUIRunner");
		this->checkException(env); //print stack trace!
		effect->setEditor(NULL); //disable custom gui
		return -1;
	}

	log("instantiating GUIRunner");
	if (!this) return -1;
	this->JavaPlugGUIObj = env->NewObject(this->JavaPlugGUIClass, mid);
	if (this->JavaPlugGUIObj == NULL) {
		log("** ERROR: cannot create VSTPluginGUIRunner Object");
		this->checkException(env); //print stack trace!
		effect->setEditor(NULL); //disable custom gui
		return -1;
	}

	if (!this) return -1;
	//create global reference --> this obj is used accross different threads
	this->JavaPlugGUIObj = env->NewGlobalRef(this->JavaPlugGUIObj);


	log("creating instance of GUI class using GUI Runner, also call open() when gui ready");
	if (!this) return -1;
	mid = env->GetMethodID(this->JavaPlugGUIClass, "loadVSTGUI", "(Ljava/lang/String;Ljvst/wrapper/VSTPluginAdapter;)V");
	if (mid == NULL) {
		log("** ERROR: cannot find loadVSTGUI in VSTPluginGUIRunner");
		this->checkException(env); //print stack trace!
		effect->setEditor(NULL); //disable custom gui
		return -1;
	}
	if (!this) return -1;
	env->CallVoidMethod(this->JavaPlugGUIObj, mid, this->JavaPlugGUIString, this->JavaPlugObj);
	if (this->checkException(env)) {
		//error occured when loading gui class
		log("* WARNING: An exception occurred when the gui class was loaded, goin back to stadard gui rendered by the host application");
		effect->setEditor(NULL); //disable custom gui
		return -1;
	}

	if (!this) return -1;
	if (this->checkException(env)) return -1;

	log("GUI initJavaSide OK -- ready for GUI calls!");

	this->IsInitialized=true;
	return 0;
}


//-----------------------------------------------------------------------------
JNIEnv* VSTGUIWrapper::ensureJavaThreadAttachment() {
	return ::ensureJavaThreadAttachment(this->Jvm);
}

bool VSTGUIWrapper::checkException(JNIEnv* env) {
	return ::checkException(env);
}


#if defined(WIN32) || defined(linux)
void VSTGUIWrapper::detachWindow() {

	if (this->AttachWindow==false) return;

	log("detaching java window from native win");
	if(this==NULL || this->JavaPlugGUIObj==NULL || this->JavaPlugGUIClass==NULL) return;

	JNIEnv* env = this->ensureJavaThreadAttachment();

	// Detach the Window
	if(this->JavaWindowHandle!=0) {
#ifdef WIN32

		HWND ret = SetParent(this->JavaWindowHandle,NULL);
		log("reparent=%p", ret);

		this->JavaWindowHandle=NULL;

		//http://msdn.microsoft.com/en-us/library/ms633541%28VS.85%29.aspx
		//do what msdn suggests: clear a possible WS_CHILD hint here, and add WS_POPUP AFTER repanting the window
		//java window
		//LONG style = GetWindowLong((HWND)JavaWindowHandle, GWL_STYLE);
		//SetWindowLong((HWND)JavaWindowHandle, GWL_STYLE, (style & ~WS_CHILD) | WS_POPUP);
		//--> CAUSES DEADLOCK!

#endif
#ifdef linux
		//does work only approx. 1 out of 7 times
		//int ret = XReparentWindow (this->JavaDisplay, this->JavaWindowHandle, XDefaultRootWindow(this->JavaDisplay), 0, 0);

		//is this better? --> jupp, see comment in open() (when attaching)
		int ret = XReparentWindow (::display, this->JavaWindowHandle, XDefaultRootWindow(this->JavaDisplay), 0, 0);

		//reparent with saved parent window
		//int ret = XReparentWindow (this->JavaDisplay, this->JavaWindowHandle, this->ParentWindow, 0, 0);

		log("remap=%i", ret);
		this->JavaWindowHandle=0;
#endif
	}
	if(frame!=NULL) {
		delete frame;
		frame=NULL;
	}

	//Inform the class
	jfieldID attachField = env->GetFieldID(this->JavaPlugGUIClass, "WindowAttached", "Z");
	if (attachField != NULL) env->SetBooleanField(this->JavaPlugGUIObj, attachField, JNI_FALSE);
	this->checkException(env);
}

void VSTGUIWrapper::undecorateJavaWindow() {

	if (this->AttachWindow==false) return;

	log("undecorate / redecorating Java window");

	JNIEnv* env = this->ensureJavaThreadAttachment();

	// Call Undecorate
    jmethodID midUndeco = env->GetMethodID(this->JavaPlugGUIClass, "undecorate", "()V");
    if (midUndeco == NULL) log("** ERROR: cannot find GUI instance-method undecorate()V");
    env->CallVoidMethod(this->JavaPlugGUIObj, midUndeco);
    this->checkException(env);
}
#endif

#ifdef linux
Window VSTGUIWrapper::getParentWindow(Display *d, Window w) {
    Window       root   = 0;
    Window       parent = 0;
    Window      *children;
    unsigned int num_children;

    if (XQueryTree(d, w, &root, &parent, &children, &num_children))
        XFree(children);

    return parent;
}
#endif

