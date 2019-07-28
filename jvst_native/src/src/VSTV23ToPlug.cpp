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
// A JNI Wrapper 
// for Java as the language to write VSTi Plugins
//-------------------------------------------------------------------------------------------------------

#include "VSTV23ToPlug.h"
#include "JNIUtils.h"


//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV23ToPlug::VSTV23ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV22ToPlug (audioMaster, progs, parms, jvm) {

}

//-----------------------------------------------------------------------------------------
VSTV23ToPlug::~VSTV23ToPlug () {

}

//-----------------------------------------------------------------------------------------
VstInt32 VSTV23ToPlug::setTotalSampleToProcess (VstInt32 value) {	
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setTotalSampleToProcess", "(I)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method setTotalSampleToProcess(I)I"); return value;}
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid, value);
	
	this->checkException(env);

	return ret;
} 

//-----------------------------------------------------------------------------------------
VstInt32 VSTV23ToPlug::getNextShellPlugin (char* name) { 
	if (name==NULL) return 0;

	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getNextShellPlugin", "(Ljava/lang/String;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getNextShellPlugin(Ljava/lang/String;)I"); return -1;}

	jstring str = env->NewStringUTF("dummy - replace me with REAL name!");
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid, str);
	
	if (str!=NULL) {
		const char* jstr = env->GetStringUTFChars(str, NULL);
		vst_strncpy (name, jstr, kVstMaxNameLen-1);
		env->ReleaseStringUTFChars(str, jstr);
		env->DeleteLocalRef(str);
	} else ret = 0L;

	
	this->checkException(env);

	return ret;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTV23ToPlug::startProcess () { 
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "startProcess", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method startProcess()I"); return 0;}

	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
}	

//-----------------------------------------------------------------------------------------
VstInt32 VSTV23ToPlug::stopProcess () { 
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "stopProcess", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method stopProcess()I"); return 0;}

	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;	
}	

