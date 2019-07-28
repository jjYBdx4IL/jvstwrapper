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
// JNI utility functions
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#ifndef __JNIUtils__
#define __JNIUtils__

#include "public.sdk/vst2.x/audioeffectx.h"

#include <jni.h>


//we still want all VST2.4 deprecated methods to be backward compatible
#undef VST_FORCE_DEPRECATED
#define VST_FORCE_DEPRECATED 0

//string lengths
#define JVST_PATH_MAX 5000
#define JVST_FILE_MAX 200

#if defined(MACX) || defined(linux)
	//patch original JNIEXPORT to use -fvisibility=hidden compiler option 
	//so that default visibility for symbols is "hidden"
	#undef JNIEXPORT
	#define JNIEXPORT __attribute__ ((visibility ("default")))
#endif


jvalue JNU_CallJavaMethod(	JNIEnv *env,
							jobject obj, 
							jmethodID mid, 
							const char *name,
							const char *descriptor, 
							...);

int log(const char* c, ...);
char *trim (char *string);
char *replace(char *string, char *oldpiece, char *newpiece);
bool checkException(JNIEnv *env);
bool checkAndThrowException(JNIEnv *env);
JNIEnv* ensureJavaThreadAttachment(JavaVM* vm);


#ifdef WIN32
	//stuff to load the jvm.dll dynamically
	//this removes the need to add the path to jvm.dll to the PATH variable
	int initJVMFunctionPointers(char *vmlibpath);
	char* readJVMLibLocation(char* requestedJVMVersion, char* customRegKey);
#endif
#ifdef linux
	//stuff to load the jvm.dll dynamically
	int initJVMFunctionPointers(char *vmlibpath);
	char* readJVMLibLocation(char* requestedJVM);
	
	char* find_exe_for_symbol (const void *symbol);
#endif
#ifdef MACX
	int checkJVMVersionRequest(char* requestedJVMVersion);
	void printCurrentThreadID();
#endif


#endif

