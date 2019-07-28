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

#include "VSTV24ToPlug.h"
#include "JNIUtils.h"


//Impl of Host -> Plug calls
//Native -> Java
//-----------------------------------------------------------------------------
VSTV24ToPlug::VSTV24ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm)
	: VSTV23ToPlug (audioMaster, progs, parms, jvm) {

	this->ProcessDoubleReplacingMethodID = NULL;
	this->JavaDoubleClass = NULL;

	ProcessDoubleReplacingLastSampleFrames=-1;
	ProcessDoubleReplacingJInputs=NULL;
	ProcessDoubleReplacingJOutputs=NULL;
	for (int i=0;i<8;i++) {
		ProcessDoubleReplacingInArrays[i]=NULL;
		ProcessDoubleReplacingOutArrays[i]=NULL;
	}
}

//-----------------------------------------------------------------------------------------
VSTV24ToPlug::~VSTV24ToPlug () {

}


//-----------------------------------------------------------------------------------------
bool VSTV24ToPlug::setProcessPrecision (VstInt32 precision) {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setProcessPrecision", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method setProcessPrecision(I)Z"); return false;}

	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, precision);

	if (this->checkException(env)) return false;

	return ret!=0;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTV24ToPlug::getNumMidiInputChannels() {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getNumMidiInputChannels", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getNumMidiInputChannels()I"); return false;}

	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);

	this->checkException(env);

	return ret;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTV24ToPlug::getNumMidiOutputChannels() {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getNumMidiOutputChannels", "()I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getNumMidiOutputChannels()I"); return false;}

	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);

	this->checkException(env);

	return ret;
}

//-----------------------------------------------------------------------------------------
void VSTV24ToPlug::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames) {

	if (sampleFrames<=0) return; // do nothing when no samples are given

	//if (!this->isProcessing) this->resume(); //TODO: maybe implement the isProcessing logic here (as in VSTV10ToPlug)?

	JNIEnv* env = this->ensureJavaThreadAttachment();

	if(this->JavaDoubleClass == NULL) {
		this->JavaDoubleClass = env->FindClass("[D");
		this->JavaDoubleClass = (jclass) env->NewGlobalRef(this->JavaDoubleClass);
		if (this->JavaDoubleClass == NULL) log("** ERROR: cannot find class [D");
	}

#if 1
// Code donated by Normen Hansen: significantly reduces GC activity

	if (ProcessDoubleReplacingLastSampleFrames<sampleFrames) {
		ProcessDoubleReplacingLastSampleFrames=sampleFrames;

		//delete old java arrays if existing
		for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
			//if (ProcessDoubleReplacingInArrays[i]) env->DeleteLocalRef(ProcessDoubleReplacingInArrays[i]);
			if (ProcessDoubleReplacingInArrays[i]) env->DeleteGlobalRef(ProcessDoubleReplacingInArrays[i]);
		}
		for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
			//if (ProcessDoubleReplacingOutArrays[i]) env->DeleteLocalRef(ProcessDoubleReplacingOutArrays[i]);
			if (ProcessDoubleReplacingOutArrays[i]) env->DeleteGlobalRef(ProcessDoubleReplacingOutArrays[i]);
		}
		//if (ProcessDoubleReplacingJInputs) env->DeleteLocalRef(ProcessDoubleReplacingJInputs);
		//if (ProcessDoubleReplacingJOutputs) env->DeleteLocalRef(ProcessDoubleReplacingJOutputs);
		if (ProcessDoubleReplacingJInputs) env->DeleteGlobalRef(ProcessDoubleReplacingJInputs);
		if (ProcessDoubleReplacingJOutputs) env->DeleteGlobalRef(ProcessDoubleReplacingJOutputs);

		ProcessDoubleReplacingJInputs = env->NewObjectArray(this->getAeffect()->numInputs, JavaDoubleClass, NULL);
		ProcessDoubleReplacingJOutputs = env->NewObjectArray(this->getAeffect()->numOutputs, JavaDoubleClass, NULL);
		if (ProcessDoubleReplacingJInputs == NULL) log("** ERROR: out of memory! jinputs");
		if (ProcessDoubleReplacingJOutputs == NULL) log("** ERROR: out of memory! joutputs");

		//create global refs
		ProcessDoubleReplacingJInputs = (jobjectArray) env->NewGlobalRef(ProcessDoubleReplacingJInputs);
		ProcessDoubleReplacingJOutputs = (jobjectArray) env->NewGlobalRef(ProcessDoubleReplacingJOutputs);

		//create new java float arrays of bufferSize
		for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
			//ProcessDoubleReplacingInArrays[i]=env->NewDoubleArray(sampleFrames);
			jdoubleArray localref = env->NewDoubleArray(sampleFrames);
			if (localref == NULL) log("** ERROR: out of memory! inarrays localref");
			ProcessDoubleReplacingInArrays[i]= (jdoubleArray) env->NewGlobalRef(localref);
		}
		for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
			//ProcessDoubleReplacingOutArrays[i]=env->NewDoubleArray(sampleFrames);
			jdoubleArray localref = env->NewDoubleArray(sampleFrames);
			if (localref == NULL) log("** ERROR: out of memory! outarrays localref");
			ProcessDoubleReplacingOutArrays[i]= (jdoubleArray) env->NewGlobalRef(localref);
		}
	}

	jdouble *jval = NULL;
	//copy c array to java array
	for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
		//jval = env->GetDoubleArrayElements(ProcessDoubleReplacingInArrays[i], NULL);
		jval = (jdouble*)env->GetPrimitiveArrayCritical(ProcessDoubleReplacingInArrays[i], NULL);
		memcpy(jval, inputs[i], sampleFrames * sizeof(double));
		//env->ReleaseDoubleArrayElements(ProcessDoubleReplacingInArrays[i], jval, 0);
		env->ReleasePrimitiveArrayCritical(ProcessDoubleReplacingInArrays[i], jval, 0);
		env->SetObjectArrayElement(ProcessDoubleReplacingJInputs, i, ProcessDoubleReplacingInArrays[i]);
	}
	for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
		//processReplacing replaces the output
		//--> do not copy output from native to java, no need for that since it is replaced anyways
		//--> well, do it anyways... fixes the hanging note problem...
		//next experiment: insert 0.0d for each double of the output array --> will be replaced by the plugin

		//jval = env->GetDoubleArrayElements(ProcessDoubleReplacingOutArrays[i], NULL);
		jval = (jdouble*)env->GetPrimitiveArrayCritical(ProcessDoubleReplacingOutArrays[i], NULL);

		//memcpy(jval, outputs[i], sampleFrames * sizeof(double));
		memset(jval, 0, sampleFrames * sizeof(double));
		//TODO: 0 may not be 0.0d at all platforms? --> http://bytes.com/forum/thread222353.html

		env->ReleasePrimitiveArrayCritical(ProcessDoubleReplacingOutArrays[i], jval, 0);
		//env->ReleaseFloatArrayElements(ProcessDoubleReplacingOutArrays[i], jval, 0);

		env->SetObjectArrayElement(ProcessDoubleReplacingJOutputs, i, ProcessDoubleReplacingOutArrays[i]);
	}


	//call java method
	if(this->ProcessDoubleReplacingMethodID == NULL) {
		log("creating new processDoubleRplacing mid");
		this->ProcessDoubleReplacingMethodID = env->GetMethodID(this->JavaPlugClass, "processDoubleReplacing", "([[D[[DI)V");
		if (this->ProcessDoubleReplacingMethodID == NULL) log("** ERROR: cannot find effects .processDoubleReplacing(...)");
	}
	env->CallVoidMethod(this->JavaPlugObj, this->ProcessDoubleReplacingMethodID, ProcessDoubleReplacingJInputs, ProcessDoubleReplacingJOutputs, (jint)sampleFrames);

	//copy java array to c array
	for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
		//jval = env->GetFloatArrayElements(ProcessDoubleReplacingOutArrays[i], NULL);
		jval = (jdouble*)env->GetPrimitiveArrayCritical(ProcessDoubleReplacingOutArrays[i], NULL);
		memcpy(outputs[i], jval, sampleFrames * sizeof(double));
		//env->ReleaseFloatArrayElements(ProcessDoubleReplacingOutArrays[i], jval, 0); //!!! use JNI_ABORT as last param? should be more efficient, we dont need the changes to jval to be copied back
		env->ReleasePrimitiveArrayCritical(ProcessDoubleReplacingOutArrays[i], jval, JNI_ABORT);
	}

#else
//############################################################################################################
// old version

	jobjectArray  jinputs;
	jobjectArray  joutputs;

	jinputs = env->NewObjectArray(this->getAeffect()->numInputs, this->JavaDoubleClass, NULL);
	joutputs = env->NewObjectArray(this->getAeffect()->numOutputs, this->JavaDoubleClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		double* in = inputs[i];
		jdoubleArray darr = env->NewDoubleArray(sampleFrames);

		env->SetDoubleArrayRegion(darr, 0, sampleFrames, in);
        env->SetObjectArrayElement(jinputs, i, darr);
        env->DeleteLocalRef(darr);
	}

	//processDoubleReplacing replaces the output
	//--> send emtpy output (do not copy output from native to java)
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		//create empty float array
		jdoubleArray darr = env->NewDoubleArray(sampleFrames);
		env->SetObjectArrayElement(joutputs, i, darr);
		env->DeleteLocalRef(darr);
	}



	//call java method
	if(this->ProcessDoubleReplacingMethodID == NULL) {
		log("creating new processDoubleRplacing mid");
		this->ProcessDoubleReplacingMethodID = env->GetMethodID(this->JavaPlugClass, "processDoubleReplacing", "([[D[[DI)V");
		if (this->ProcessDoubleReplacingMethodID == NULL) log("** ERROR: cannot find effects .processDoubleReplacing(...)");
	}
	env->CallVoidMethod(this->JavaPlugObj, this->ProcessDoubleReplacingMethodID, jinputs, joutputs, (jint)sampleFrames);



	//jetzt elemente von joutput nach output umkopieren...
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		double* out = outputs[i];
		jdoubleArray jout;
		jdouble *jval;

		jout = (jdoubleArray)env->GetObjectArrayElement(joutputs, i);
		jval = env->GetDoubleArrayElements(jout, NULL);

		memcpy(out, jval, sampleFrames * sizeof(double));

		env->ReleaseDoubleArrayElements(jout, jval, 0);
		env->DeleteLocalRef(jout);
	}

	//ARRAYS mit deletelocalref wieder zerstoeren...
	env->DeleteLocalRef(jinputs);
	env->DeleteLocalRef(joutputs);
#endif


	::checkException(env);
}



 #ifdef linux

 	//Global display --> defined by VSTGUI
	extern Display* display;

	//workaround to access the display on linux
	//-----------------------------------------------------------------------------------------
	VstInt32 VSTV24ToPlug::dispatcher (VstInt32 opCode, VstInt32 index, VstInt32 value, void *ptr, float opt) {
	    int result = 0;

	    switch (opCode) {
	   		case effEditOpen:
	   			log("Opened Display = %p", value);
	        	if (display == 0) {
	           		display = (Display*) value;
	        	}
	        	log("Native Display = %p", display);
	    	default:
	        	result = AudioEffectX::dispatcher (opCode, index, value, ptr, opt);
	    }
	    return result;
	}
#endif
