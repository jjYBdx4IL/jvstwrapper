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

#include "VSTV23ToHost.h"
#include "VSTV24ToPlug.h"
#include "JNIUtils.h" //important, this needs always to be the LAST import!

extern VSTV24ToPlug* getWrapperInstance(JNIEnv *env, jobject obj);



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV23ToHost.java native calls
//**********************************************


/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    setPanLaw
 * Signature: (IF)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_setPanLaw
  (JNIEnv* env, jobject obj, jint type, jfloat val) {
      VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	  if (WrapperInstance!=NULL) return WrapperInstance->setPanLaw(type, val);
	  else return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    beginLoadBank
 * Signature: (Ljvst/wrapper/valueobjects/VSTPatchChunkInfo;)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_beginLoadBank
  (JNIEnv *env, jobject obj, jobject bank) {
      VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	  return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    beginLoadProgram
 * Signature: (Ljvst/wrapper/valueobjects/VSTPatchChunkInfo;)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_beginLoadProgram
	(JNIEnv *env, jobject obj, jobject prog) {
        VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
		return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    copySpeaker
 * Signature: (Ljvst/wrapper/valueobjects/VSTSpeakerProperties;Ljvst/wrapper/valueobjects/VSTSpeakerProperties;)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_copySpeaker
  (JNIEnv *env, jobject obj, jobject from, jobject to) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	return 0;
}

/*
 * Class:     jvst_wrapper_communication_VSTV23ToHost
 * Method:    matchArrangement
 * Signature: (Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV23ToHost_matchArrangement
  (JNIEnv *env, jobject obj, jobject obj1, jobject obj2) {
  VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
  return 0;
}

