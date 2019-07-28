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
// Java -> Native calls impls
//-------------------------------------------------------------------------------------------------------


#include "VSTV21ToHost.h"
#include "VSTV24ToPlug.h"
#include "JNIUtils.h" //important, this needs always to be the LAST import!

extern VSTV24ToPlug* getWrapperInstance(JNIEnv *env, jobject obj);



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV21ToHost.java native calls
//**********************************************


/*
 * Class:     jvst_wrapper_communication_VSTV21ToHost
 * Method:    beginEdit
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV21ToHost_beginEdit
	(JNIEnv *env, jobject obj, jint index) {
		log("beginEdit(%i)", index);

        VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj); 
		if (WrapperInstance!=NULL) return WrapperInstance->beginEdit(index);
		else return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV21ToHost
 * Method:    endEdit
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV21ToHost_endEdit
  (JNIEnv *env, jobject obj, jint index) {
	    log("endEdit(%i)", index);

        VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
		if (WrapperInstance!=NULL) return WrapperInstance->endEdit(index);
		else return 0;
}

