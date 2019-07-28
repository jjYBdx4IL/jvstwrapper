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
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------

#include "VSTV10ToHost.h"
#include "VSTV24ToPlug.h"
#include "JNIUtils.h" //important, this needs always to be the LAST import!


#if defined(MACX) || defined(linux)
#include <pthread.h>
#endif


VSTV24ToPlug* getWrapperInstance(JNIEnv *env, jobject obj) {
	jclass plugclass=env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(plugclass, "WrapperInstance", "J");
	if (fid == NULL) {
		log("** ERROR: cannot find effects WrappeInstance");
		checkAndThrowException(env);
		return 0;
	}
	jlong wri=env->GetLongField(obj,fid);
	if (checkAndThrowException(env)) return 0;
	return (VSTV24ToPlug*)wri;
}



//Impl of Plug -> Host calls
//Java -> Native
//**********************************************


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setUniqueID
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setUniqueID(JNIEnv *env, jobject obj, jint id) {
	log("setUniqueID=%X",id);
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setUniqueID(id);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setNumInputs
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setNumInputs(JNIEnv *env, jobject obj, jint num) {
	log("setNumInputs=%i", num);
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setNumInputs(num);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setNumOutputs
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setNumOutputs(JNIEnv *env, jobject obj, jint num) {
	log("setNumOutputs=%i", num);
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL)WrapperInstance->setNumOutputs(num);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    hasVu
 * Signature: ()V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_hasVu(JNIEnv *env, jobject obj, jboolean state) {
	log("hasVu");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->hasVu(state != 0 ? true : false);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    hasClip
 * Signature: ()Z
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_hasClip(JNIEnv *env, jobject obj, jboolean state) {
	log("hasClip");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->hasClip(state != 0 ? true : false);
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    canMono
 * Signature: ()Z
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_canMono(JNIEnv *env, jobject obj, jboolean state) {
	log("canMono");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->canMono(state != 0 ? true : false);
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    canProcessReplacing
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_canProcessReplacing(JNIEnv *env, jobject obj, jboolean state) {
	bool test = state != 0 ? true : false;
	log("canProcessReplacing=%i", test);
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->canProcessReplacing(state != 0 ? true : false);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setRealtimeQualities
 * Signature: (I)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setRealtimeQualities(JNIEnv *env, jobject obj, jint quality) {
	log("setRealtimeQualities");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setRealtimeQualities(quality);
}



//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setOfflineQualities
 * Signature: (I)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setOfflineQualities(JNIEnv *env, jobject obj, jint quality) {
	log("setOfflineQualities");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setOfflineQualities(quality);
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setInitialDelay
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setInitialDelay(JNIEnv *env, jobject obj, jint delay) {
	log("setInitialDelay");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setInitialDelay(delay);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    programsAreChunks
 * Signature: ()Z
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_programsAreChunks(JNIEnv *env, jobject obj, jboolean state) {
	log("programsAreChunks");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->programsAreChunks(state != 0 ? true : false);
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getSampleRate
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getSampleRate(JNIEnv *env, jobject obj) {
	log("getSampleRate");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getSampleRate();
	else return -1.0F;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getBlockSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getBlockSize(JNIEnv *env, jobject obj) {
	log("getBlockSize");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getBlockSize();
	else return 0;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    setParameterAutomated
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_setParameterAutomated(JNIEnv *env, jobject obj, jint index, jfloat value) {
	//log("setParameterAutomated");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);

#ifdef WIN32
	//Workaround for energyXT: Save Thread for setParameterAutomated
	WrapperInstance->ToHostThread=GetCurrentThreadId();
#endif
#if defined(MACX) || defined(linux)
	WrapperInstance->ToHostThread=pthread_self();
#endif

	log("setParameterAutomated(%i,%f)", index, value);
	if (WrapperInstance!=NULL) WrapperInstance->setParameterAutomated(index, value);

    WrapperInstance->ToHostThread=0;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getMasterVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getMasterVersion(JNIEnv *env, jobject obj) {
	log("getMasterVersion");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getMasterVersion();
	else return -1;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    getCurrentUniqueId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_getCurrentUniqueId(JNIEnv *env, jobject obj) {
	log("getCurrentUniqueId");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getCurrentUniqueId();
	else return -1;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    masterIdle
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_masterIdle(JNIEnv *env, jobject obj) {
	//log("masterIdle");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->masterIdle();
}


//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    isInputConnected
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_isInputConnected(JNIEnv *env, jobject obj, jint input) {
	log("isInputConnected");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->isInputConnected(input);
	else return false;
}

//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    isOutputConnected
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_isOutputConnected(JNIEnv *env, jobject obj, jint output) {
	log("isOutputConnected");
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->isOutputConnected(output);
	else return false;
}







//------------------------------------------------------------------------
/*
 * Class:     VSTV10ToHost
 * Method:    sayHello
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV10ToHost_sayHello(JNIEnv *, jobject) {
	printf("\nC++ sagt Hallo!");
}
