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
// A simple JNI Wrapper
// for Java as the language to write VSTi Plugins
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#include "VSTV10ToPlug.h"
#include "ConfigFileReader.h"
#include "JNIUtils.h"

#include <stdio.h>
#include <string.h>

#ifdef linux
	#include <stdlib.h>
#endif


extern char DllFileName[JVST_FILE_MAX];


//Impl of Host -> Plug calls
//Native -> Java
//-----------------------------------------------------------------------------
VSTV10ToPlug::VSTV10ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm)
	: AudioEffectX (audioMaster, progs, parms) {

	log("VSTV10ToPlug KONSTRUCTOR!!!");


	this->Jvm = jvm;

	//init cached fields
	this->JavaPlugObj = NULL;
	this->ProcessMethodID = NULL;
	this->ProcessReplacingMethodID = NULL;
	this->JavaFloatClass = NULL;

	this->isProcessing=false; //fix to ensure that resume was called before process*

	//init cached fields
	GetParameterMethod = NULL;
	SetParameterMethod = NULL;

	chunkdata = NULL;
	chunksize = 0;

	ProcessLastSampleFrames=-1;
	ProcessJInputs=NULL;
	ProcessJOutputs=NULL;
	for (int i=0;i<8;i++) {
		ProcessInArrays[i]=NULL;
		ProcessOutArrays[i]=NULL;
	}

	ProcessReplacingLastSampleFrames=-1;
	ProcessReplacingJInputs=NULL;
	ProcessReplacingJOutputs=NULL;
	for (int i=0;i<8;i++) {
		ProcessReplacingInArrays[i]=NULL;
		ProcessReplacingOutArrays[i]=NULL;
	}
 }

//------------------------------------------------------------------------
VSTV10ToPlug::~VSTV10ToPlug () {
	log("Destroying Wrapper");
	JNIEnv* env = this->ensureJavaThreadAttachment();

	//Delete global reference
	env->DeleteGlobalRef(this->JavaPlugObj);

	if(chunkdata!=NULL) { free(chunkdata); chunkdata = NULL; chunksize=0; }

	//hier wird nur gewartet bis der haupt jvm thread der einzigste
	//java thread ist. die jvm wird aber trotzdem nicht entladen...
	//Jvm->DestroyJavaVM();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setProgram (VstInt32 program) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setProgram", "(I)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setProgram(I)V");

	env->CallVoidMethod(this->JavaPlugObj, mid, (jint)program);

	this->checkException(env);
}

VstInt32 VSTV10ToPlug::getProgram () {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getProgram", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getProgram()I");

	return env->CallIntMethod(this->JavaPlugObj, mid);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setProgramName (char *name) {
	if (name==NULL) return;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setProgramName", "(Ljava/lang/String;)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setProgramName(Ljava/lang/String;)V");
	
	char newname[kVstMaxNameLen];
	vst_strncpy(newname, name, kVstMaxNameLen-1); //cut chars
	jstring arg = env->NewStringUTF(newname);

	env->CallVoidMethod(this->JavaPlugObj, mid, arg);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getProgramName (char *name) {
	if (name==NULL) return;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getProgramName", "()Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getProgramName(Ljava/lang/String;)V");

	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid);
	if (ret==NULL) { strcpy (name, ""); return;}

	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxProgNameLen) log("* WARNING: program name '%s' too long (max %i)", jstr, kVstMaxProgNameLen);
	vst_strncpy (name, jstr, kVstMaxProgNameLen-1);
	env->ReleaseStringUTFChars(ret, jstr);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setParameter (VstInt32 index, float value) {
//Workaround for energyXT: Don't call setParameter from setParameterAutomated
// Disabled for now: prevented param automation from working, i.e. calls to setParameterAutomated()
// didnt end up calling setParameter (what they should do)
/*
#ifdef WIN32
	if(this->ToHostThread!=GetCurrentThreadId()) {
#endif
#if defined(MACX) || defined(linux)
	if(this->ToHostThread!=pthread_self()) {
#endif
*/
	JNIEnv* env = this->ensureJavaThreadAttachment();

	if (this->SetParameterMethod==NULL) {
		this->SetParameterMethod = env->GetMethodID(this->JavaPlugClass, "setParameter", "(IF)V");
		if (this->SetParameterMethod == NULL) log("** ERROR: cannot find instance-method setParameter(IF)V");
	}

	log("setParameter(%i,%f)", index, value);
	env->CallVoidMethod(this->JavaPlugObj, this->SetParameterMethod, (jint)index, (jfloat)value);

	this->checkException(env);
//	}
}

//------------------------------------------------------------------------
float VSTV10ToPlug::getParameter (VstInt32 index)   {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	if (this->GetParameterMethod==NULL) {
		this->GetParameterMethod = env->GetMethodID(this->JavaPlugClass, "getParameter", "(I)F");
		if (this->GetParameterMethod == NULL) log("** ERROR: cannot find instance-method getParameter(I)F");
	}

	jfloat ret = env->CallFloatMethod(this->JavaPlugObj, this->GetParameterMethod, (jint)index);

	this->checkException(env);

	return (float)ret;
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getParameterName(VstInt32 index, char *label) {
	if (label==NULL) return;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getParameterName", "(I)Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getParameterName(I)Ljava/lang/String;");

	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (ret==NULL) {strcpy (label, ""); return;}

	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxParamStrLen) log("* WARNING: parameter name '%s' is too long (max %i)", jstr, kVstMaxParamStrLen);
	vst_strncpy (label, jstr, kVstMaxParamStrLen-1);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getParameterDisplay (VstInt32 index, char *text) {
	if (text==NULL) return;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getParameterDisplay", "(I)Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getParameterDisplay(I)Ljava/lang/String;");

	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (ret==NULL) {strcpy (text, ""); return;}

	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxParamStrLen) log("* WARNING: parameter display '%s' is too long (max %i)", jstr, kVstMaxParamStrLen);
	vst_strncpy (text, jstr, kVstMaxParamStrLen-1);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::getParameterLabel (VstInt32 index, char *label) {
	if (label==NULL) return;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getParameterLabel", "(I)Ljava/lang/String;");
	if (mid == NULL) log("** ERROR: cannot find instance-method getParameterLabel(I)Ljava/lang/String;");

	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (ret==NULL) {strcpy (label, ""); return;}

	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxParamStrLen) log("* WARNING: parameter label '%s' too long (max %i)", jstr, kVstMaxParamStrLen);
	vst_strncpy (label, jstr, kVstMaxParamStrLen-1);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::suspend () {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	//call to overwritten method, this is done in all steinberg sample plugs,
	//so we do it too...
	AudioEffectX::suspend();

	// Called when Plugin is switched to Off
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "suspend", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method suspend()V");

	env->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException(env);

	this->isProcessing=false;
}

//------------------------------------------------------------------------
void VSTV10ToPlug::resume () {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	this->isProcessing=true;

	// Called when Plugin is switched to On
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "resume", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method resume()V");

	env->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException(env);

	//call to overwritten method, this is done in all steinberg sample plugs,
	//so we do it too...
	AudioEffectX::resume();
}

//------------------------------------------------------------------------
void VSTV10ToPlug::open() {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "open", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method open()V");

	env->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::close() {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "close", "()V");
	if (mid == NULL) log("** ERROR: cannot find instance-method close()V");

	env->CallVoidMethod(this->JavaPlugObj, mid);

	this->checkException(env);
}

//------------------------------------------------------------------------
float VSTV10ToPlug::getVu() {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getVu", "()F");
	if (mid == NULL) log("** ERROR: cannot find instance-method getVu()F");

	jfloat ret =  env->CallFloatMethod(this->JavaPlugObj, mid);

	this->checkException(env);

	return ret;
}

//------------------------------------------------------------------------
VstInt32 VSTV10ToPlug::getChunk(void** data, bool isPreset) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getChunk", "([[BZ)I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getChunk([[BZ)I");

	jobjectArray  jdata;//dummy
	jclass byteClass;
	jbyte buf[1] = {-1};//dummy

	byteClass = env->FindClass("[B");
	if (byteClass==NULL) log("** ERROR: cannot find Class [B");
	jdata = env->NewObjectArray(1, byteClass, NULL);
	jbyteArray barr = env->NewByteArray(1);
	env->SetByteArrayRegion(barr, 0, 1, buf);
    env->SetObjectArrayElement(jdata, 0, barr);
    env->DeleteLocalRef(barr);

	jint data_len = env->CallIntMethod(this->JavaPlugObj, mid, jdata, (jboolean)isPreset);

	//jetzt elemente von jdata[][] nach **data umkopieren...
	//for (int i=0; i<data_len; i++) {
	for (int i=0; i<env->GetArrayLength(jdata) ; i++) {
		if(chunksize!=data_len){
			if(chunkdata!=NULL) free(chunkdata);
			chunkdata=malloc(data_len*sizeof(jbyte));
			chunksize=data_len;
		}
		data[i] = chunkdata;
		jbyte* dat = (jbyte*)data[i];
		jbyteArray jdat;
		jbyte *jval;

		jdat = (jbyteArray)env->GetObjectArrayElement(jdata, i);
		jval = env->GetByteArrayElements(jdat, NULL);

		memcpy(dat, jval, data_len * sizeof(jbyte));

		env->ReleaseByteArrayElements(jdat, jval, 0);
		env->DeleteLocalRef(jdat);
	}

	this->checkException(env);

	return data_len*sizeof(jbyte);
}

//------------------------------------------------------------------------
VstInt32 VSTV10ToPlug::setChunk(void* data, VstInt32 byteSize, bool isPreset) {
	if (data==NULL) return 0;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jint ret = -1;

	jbyteArray jdata = env->NewByteArray(byteSize/sizeof(jbyte));
	env->SetByteArrayRegion(jdata, 0, byteSize/sizeof(jbyte), (jbyte*)data);

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setChunk", "([BIZ)I");
	if (mid == NULL) log("** ERROR: cannot find instance-method setChunk([BIZ)I");
	ret = env->CallIntMethod(this->JavaPlugObj, mid, jdata, (jint)(byteSize/sizeof(jbyte)), (jboolean)isPreset);

    env->DeleteLocalRef(jdata);

	this->checkException(env);

	return ret;
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setBlockSize(VstInt32 blockSz) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setBlockSize", "(I)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setBlockSize(I)V");

	env->CallVoidMethod(this->JavaPlugObj, mid, blockSz);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::setSampleRate(float sampleRt) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setSampleRate", "(F)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setSampleRate(F)V");

	env->CallVoidMethod(this->JavaPlugObj, mid, sampleRt);

	this->checkException(env);
}

//------------------------------------------------------------------------
void VSTV10ToPlug::process (float** inputs, float** outputs, VstInt32 sampleFrames) {
	//if (inputs==NULL || outputs==NULL) return; //sanity check parameters

	if (sampleFrames<=0) return; // do nothing when no samples are given

	if (!this->isProcessing) this->resume();

	JNIEnv* env = this->ensureJavaThreadAttachment();

	if(this->JavaFloatClass == NULL) {
		this->JavaFloatClass = env->FindClass("[F");
		this->JavaFloatClass = (jclass) env->NewGlobalRef(this->JavaFloatClass);
		if (this->JavaFloatClass == NULL) log("** ERROR: cannot find class [F");
	}


#if 1
// Code donated by Normen Hansen: significantly reduces GC activity

	if (ProcessLastSampleFrames<sampleFrames) {
		ProcessLastSampleFrames=sampleFrames;

		//delete old java arrays if existed
		for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
			//if (ProcessInArrays[i]) env->DeleteLocalRef(ProcessInArrays[i]);
			if (ProcessInArrays[i]) env->DeleteGlobalRef(ProcessInArrays[i]);
		}
		for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
			//if (ProcessOutArrays[i]) env->DeleteLocalRef(ProcessOutArrays[i]);
			if (ProcessOutArrays[i]) env->DeleteGlobalRef(ProcessOutArrays[i]);
		}
		//if (ProcessJInputs) env->DeleteLocalRef(ProcessJInputs);
		//if (ProcessJOutputs) env->DeleteLocalRef(ProcessJOutputs);
		if (ProcessJInputs) env->DeleteGlobalRef(ProcessJInputs);
		if (ProcessJOutputs) env->DeleteGlobalRef(ProcessJOutputs);

		ProcessJInputs = env->NewObjectArray(this->getAeffect()->numInputs, JavaFloatClass, NULL);
		ProcessJOutputs = env->NewObjectArray(this->getAeffect()->numOutputs, JavaFloatClass, NULL);
		if (ProcessJInputs == NULL) log("** ERROR: out of memory! jinputs");
		if (ProcessJOutputs == NULL) log("** ERROR: out of memory! joutputs");
		
		//create global refs
		ProcessJInputs = (jobjectArray) env->NewGlobalRef(ProcessJInputs);
		ProcessJOutputs = (jobjectArray) env->NewGlobalRef(ProcessJOutputs);

		//create new java float arrays of bufferSize
		for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
			//ProcessInArrays[i]=env->NewFloatArray(sampleFrames);
			jfloatArray localref = env->NewFloatArray(sampleFrames);
			if (localref == NULL) log("** ERROR: out of memory! inarrays localref");
			ProcessInArrays[i]= (jfloatArray) env->NewGlobalRef(localref);
		}
		for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
			//ProcessOutArrays[i]=env->NewFloatArray(sampleFrames);
			jfloatArray localref = env->NewFloatArray(sampleFrames);
			if (localref == NULL) log("** ERROR: out of memory! outarrays localref");
			ProcessOutArrays[i]= (jfloatArray) env->NewGlobalRef(localref);
		}
	}

	jfloat *jval;
	//copy c array to java array
	for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
		//jval = env->GetFloatArrayElements(ProcessInArrays[i], NULL);
		jval = (jfloat*)env->GetPrimitiveArrayCritical(ProcessInArrays[i], NULL);
		memcpy(jval, inputs[i], sampleFrames * sizeof(float));
		env->ReleasePrimitiveArrayCritical(ProcessInArrays[i], jval, 0);
		//env->ReleaseFloatArrayElements(ProcessInArrays[i], jval, 0);

		env->SetObjectArrayElement(ProcessJInputs, i, ProcessInArrays[i]);
	}
	for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
		//jval = env->GetFloatArrayElements(ProcessOutArrays[i], NULL);
		jval = (jfloat*)env->GetPrimitiveArrayCritical(ProcessOutArrays[i], NULL);
		memcpy(jval, outputs[i], sampleFrames * sizeof(float));
		env->ReleasePrimitiveArrayCritical(ProcessOutArrays[i], jval, 0);
		//env->ReleaseFloatArrayElements(ProcessOutArrays[i], jval, 0);

		env->SetObjectArrayElement(ProcessJOutputs, i, ProcessOutArrays[i]);
	}

	//call java method
	if(this->ProcessMethodID == NULL) {
		log("creating new process mid");
		this->ProcessMethodID = env->GetMethodID(this->JavaPlugClass, "process", "([[F[[FI)V");
		if (this->ProcessMethodID == NULL) log("** ERROR: cannot find effects .process(...)");
	}
	env->CallVoidMethod(this->JavaPlugObj, this->ProcessMethodID, ProcessJInputs, ProcessJOutputs, (jint)sampleFrames);

	//copy java array to c array
	for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
		//jval = env->GetFloatArrayElements(ProcessOutArrays[i], NULL);
		jval = (jfloat*)env->GetPrimitiveArrayCritical(ProcessOutArrays[i], NULL);
		memcpy(outputs[i], jval, sampleFrames * sizeof(float));
		//env->ReleaseFloatArrayElements(ProcessOutArrays[i], jval, JNI_ABORT); //!!! use JNI_ABORT as last param? should be more efficient, we dont need the changes to jval to be copied back
		env->ReleasePrimitiveArrayCritical(ProcessOutArrays[i], jval, JNI_ABORT);
	}


#else
//############################################################################################################
// old version

	jobjectArray  jinputs;
	jobjectArray  joutputs;

	jinputs = env->NewObjectArray(this->getAeffect()->numInputs, this->JavaFloatClass, NULL);
	joutputs = env->NewObjectArray(this->getAeffect()->numOutputs, this->JavaFloatClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = inputs[i];
		jfloatArray farr = env->NewFloatArray(sampleFrames);

		env->SetFloatArrayRegion(farr, 0, sampleFrames, in);
        env->SetObjectArrayElement(jinputs, i, farr);
        env->DeleteLocalRef(farr);
	}

	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = outputs[i];
		jfloatArray farr = env->NewFloatArray(sampleFrames);

		env->SetFloatArrayRegion(farr, 0, sampleFrames, out);
		env->SetObjectArrayElement(joutputs, i, farr);
		env->DeleteLocalRef(farr);
	}

	//call java method
	if(this->ProcessMethodID == NULL) {
		log("creating new process mid");
		this->ProcessMethodID = env->GetMethodID(this->JavaPlugClass, "process", "([[F[[FI)V");
		if (this->ProcessMethodID == NULL) log("** ERROR: cannot find effects .process(...)");
	}
	env->CallVoidMethod(this->JavaPlugObj, this->ProcessMethodID, jinputs, joutputs, (jint)sampleFrames);


	//jetzt elemente von joutput nach output umkopieren...
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = outputs[i];
		jfloatArray jout;
		jfloat *jval;

		jout = (jfloatArray)env->GetObjectArrayElement(joutputs, i);
		jval = env->GetFloatArrayElements(jout, NULL);

		memcpy(out, jval, sampleFrames * sizeof(float));

		env->ReleaseFloatArrayElements(jout, jval, 0);
		env->DeleteLocalRef(jout);
	}

	//ARRAYS mit deletelocalref wieder zerstoeren...
	env->DeleteLocalRef(jinputs);
	env->DeleteLocalRef(joutputs);
#endif


	::checkException(env);
}

//---------------------------------------------------------------------------
void VSTV10ToPlug::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames) {
	//if (inputs==NULL || outputs==NULL) return; //sanity check parameters --> does not work! vsthost does not generate output then!

	if (sampleFrames<=0) return; // do nothing when no samples are given

	if (!this->isProcessing) this->resume();

	JNIEnv* env = this->ensureJavaThreadAttachment();

	if(this->JavaFloatClass == NULL) {
		this->JavaFloatClass = env->FindClass("[F");
		this->JavaFloatClass = (jclass) env->NewGlobalRef(this->JavaFloatClass);
		if (this->JavaFloatClass == NULL) log("** ERROR: cannot find class [F");
	}

#if 1
// Code donated by Normen Hansen: significantly reduces GC activity

	if (ProcessReplacingLastSampleFrames<sampleFrames) {
		ProcessReplacingLastSampleFrames=sampleFrames;

		//delete old java arrays if existing
		for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
			//if (ProcessReplacingInArrays[i]) env->DeleteLocalRef(ProcessReplacingInArrays[i]);
			if (ProcessReplacingInArrays[i]) env->DeleteGlobalRef(ProcessReplacingInArrays[i]);
		}
		for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
			//if (ProcessReplacingOutArrays[i]) env->DeleteLocalRef(ProcessReplacingOutArrays[i]);
			if (ProcessReplacingOutArrays[i]) env->DeleteGlobalRef(ProcessReplacingOutArrays[i]);
		}
		//if (ProcessReplacingJInputs) env->DeleteLocalRef(ProcessReplacingJInputs);
		//if (ProcessReplacingJOutputs) env->DeleteLocalRef(ProcessReplacingJOutputs);
		if (ProcessReplacingJInputs) env->DeleteGlobalRef(ProcessReplacingJInputs);
		if (ProcessReplacingJOutputs) env->DeleteGlobalRef(ProcessReplacingJOutputs);

		ProcessReplacingJInputs = env->NewObjectArray(this->getAeffect()->numInputs, JavaFloatClass, NULL);
		ProcessReplacingJOutputs = env->NewObjectArray(this->getAeffect()->numOutputs, JavaFloatClass, NULL);
		if (ProcessReplacingJInputs == NULL) log("** ERROR: out of memory! jinputs");
		if (ProcessReplacingJOutputs == NULL) log("** ERROR: out of memory! joutputs");

		//create global refs
		ProcessReplacingJInputs = (jobjectArray) env->NewGlobalRef(ProcessReplacingJInputs);
		ProcessReplacingJOutputs = (jobjectArray) env->NewGlobalRef(ProcessReplacingJOutputs);


		//create new java float arrays of bufferSize
		for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
			//ProcessReplacingInArrays[i]=env->NewFloatArray(sampleFrames);
			jfloatArray localref = env->NewFloatArray(sampleFrames);
			if (localref == NULL) log("** ERROR: out of memory! inarrays localref");
			ProcessReplacingInArrays[i]= (jfloatArray) env->NewGlobalRef(localref);
		}
		for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
			//ProcessReplacingOutArrays[i]=env->NewFloatArray(sampleFrames);
			jfloatArray localref = env->NewFloatArray(sampleFrames);
			if (localref == NULL) log("** ERROR: out of memory! outarrays localref");
			ProcessReplacingOutArrays[i]= (jfloatArray) env->NewGlobalRef(localref);
		}
	}

	jfloat *jval = NULL;
	//copy c array to java array
	for (int i = 0; (i < this->getAeffect()->numInputs) && (i<8); i++) {
		//jval = env->GetFloatArrayElements(ProcessReplacingInArrays[i], NULL);
		jval = (jfloat*)env->GetPrimitiveArrayCritical(ProcessReplacingInArrays[i], NULL);
		memcpy(jval, inputs[i], sampleFrames * sizeof(float));
		env->ReleasePrimitiveArrayCritical(ProcessReplacingInArrays[i], jval, 0);
		//env->ReleaseFloatArrayElements(ProcessReplacingInArrays[i], jval, 0);

		env->SetObjectArrayElement(ProcessReplacingJInputs, i, ProcessReplacingInArrays[i]);
	}
	for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
		//processReplacing replaces the output
		//--> do not copy output from native to java, no need for that since it is replaced anyways
		//--> well, do it anyways... fixes the hanging note problem...
		//next experiment: insert 0.0f for each float of the output array --> will be replaced by the plugin

		//jval = env->GetFloatArrayElements(ProcessReplacingOutArrays[i], NULL);
		jval = (jfloat*)env->GetPrimitiveArrayCritical(ProcessReplacingOutArrays[i], NULL);

		//memcpy(jval, outputs[i], sampleFrames * sizeof(float));
		memset(jval, 0, sampleFrames * sizeof(float));
		//TODO: 0 may not be 0.0f at all platforms? --> http://bytes.com/forum/thread222353.html

		env->ReleasePrimitiveArrayCritical(ProcessReplacingOutArrays[i], jval, 0);
		//env->ReleaseFloatArrayElements(ProcessReplacingOutArrays[i], jval, 0);

		env->SetObjectArrayElement(ProcessReplacingJOutputs, i, ProcessReplacingOutArrays[i]);
	}


	//call java method
	if(this->ProcessReplacingMethodID == NULL) {
		log("creating new processRplacing mid");
		this->ProcessReplacingMethodID = env->GetMethodID(this->JavaPlugClass, "processReplacing", "([[F[[FI)V");
		if (this->ProcessReplacingMethodID == NULL) log("** ERROR: cannot find effects .processReplacing(...)");
	}
	env->CallVoidMethod(this->JavaPlugObj, this->ProcessReplacingMethodID, ProcessReplacingJInputs, ProcessReplacingJOutputs, (jint)sampleFrames);

	//copy java array to c array
	for (int i = 0; (i < this->getAeffect()->numOutputs) && (i<8); i++) {
		//jval = env->GetFloatArrayElements(ProcessReplacingOutArrays[i], NULL);
		jval = (jfloat*)env->GetPrimitiveArrayCritical(ProcessReplacingOutArrays[i], NULL);
		memcpy(outputs[i], jval, sampleFrames * sizeof(float));
		//env->ReleaseFloatArrayElements(ProcessReplacingOutArrays[i], jval, 0); //!!! use JNI_ABORT as last param? should be more efficient, we dont need the changes to jval to be copied back
		env->ReleasePrimitiveArrayCritical(ProcessReplacingOutArrays[i], jval, JNI_ABORT);
	}


#else
//############################################################################################################
// old version

	jobjectArray  jinputs;
	jobjectArray  joutputs;

	jinputs = env->NewObjectArray(this->getAeffect()->numInputs, this->JavaFloatClass, NULL);
	joutputs = env->NewObjectArray(this->getAeffect()->numOutputs, this->JavaFloatClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = inputs[i];
		jfloatArray farr = env->NewFloatArray(sampleFrames);

		env->SetFloatArrayRegion(farr, 0, sampleFrames, in);
        env->SetObjectArrayElement(jinputs, i, farr);
        env->DeleteLocalRef(farr);
	}

	//processReplacing replaces the output
	//--> send emtpy output (do not copy output from native to java)
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		//create empty float array
		jfloatArray farr = env->NewFloatArray(sampleFrames);
		env->SetObjectArrayElement(joutputs, i, farr);
		env->DeleteLocalRef(farr);
	}



	//call java method
	if(this->ProcessReplacingMethodID == NULL) {
		log("creating new processRplacing mid");
		this->ProcessReplacingMethodID = env->GetMethodID(this->JavaPlugClass, "processReplacing", "([[F[[FI)V");
		if (this->ProcessReplacingMethodID == NULL) log("** ERROR: cannot find effects .processReplacing(...)");
	}
	env->CallVoidMethod(this->JavaPlugObj, this->ProcessReplacingMethodID, jinputs, joutputs, (jint)sampleFrames);



	//jetzt elemente von joutput nach output umkopieren...
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = outputs[i];
		jfloatArray jout;
		jfloat *jval;

		jout = (jfloatArray)env->GetObjectArrayElement(joutputs, i);
		jval = env->GetFloatArrayElements(jout, NULL);

		memcpy(out, jval, sampleFrames * sizeof(float));

		env->ReleaseFloatArrayElements(jout, jval, 0);
		env->DeleteLocalRef(jout);
	}

	//ARRAYS mit deletelocalref wieder zerstoeren...
	env->DeleteLocalRef(jinputs);
	env->DeleteLocalRef(joutputs);
#endif


	::checkException(env);
}







//---------------------------------------------------------------------------
int VSTV10ToPlug::initJavaSide(jclass effectClass) {
	JNIEnv* env = this->ensureJavaThreadAttachment();


	if(effectClass==NULL) return -1;
	//Create global reference --> use this object accross different threads
	this->JavaPlugClass = (jclass) env->NewGlobalRef(effectClass);

    jlong wri=((jlong)((long)this));
	//JAVA Konstruktor
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "<init>", "(J)V");
	if (mid == NULL) {
		log("** ERROR: cannot find contructor <init>(long)");
		this->checkException(env); //print stack trace!
		return -1;
	}

	if (this->checkException(env)) return -1;

	this->JavaPlugObj = env->NewObject(this->JavaPlugClass, mid,wri);
	if (this->JavaPlugObj == NULL) {
		log("** ERROR: cannot instantiate Java Plugin Object, \nException occured in constructor?\nPlease look at the generated LOG files.");
		this->checkException(env); //print stack trace!
		return -1;
	}

	if (this->checkException(env)) return -1;

	//Create global reference --> use this object accross different threads
	this->JavaPlugObj = env->NewGlobalRef(this->JavaPlugObj);

	if (this->checkException(env)) return -1;



	//setting correct numPrograms value
	mid = env->GetMethodID(this->JavaPlugClass, "getNumPrograms", "()I");
	if (mid == NULL) {
		log("** ERROR: cannot find effects .getNumPrograms(I)");
		this->checkException(env); //print stack trace!
		return -1;
	}
	jint num = env->CallIntMethod(this->JavaPlugObj, mid);
	this->getAeffect()->numPrograms = num;
	this->numPrograms = num;

	if (this->checkException(env)) return -1;


	//setting correct numParams value
	mid = env->GetMethodID(this->JavaPlugClass, "getNumParams", "()I");
	if (mid == NULL) {
		log("** ERROR: cannot find effects .getNumParams(I)");
		this->checkException(env); //print stack trace!
		return -1;
	}
	num = env->CallIntMethod(this->JavaPlugObj, mid);
	this->getAeffect()->numParams = num;
	this->numParams = num;


	log("Plugin initJavaSide ok!");
	if (this->checkException(env)) return -1;
	else return 0;
}

//-----------------------------------------------------------------------------
JNIEnv* VSTV10ToPlug::ensureJavaThreadAttachment() {
	return ::ensureJavaThreadAttachment(this->Jvm);
}

void VSTV10ToPlug::setNumParams(VstInt32 num){
	this->getAeffect()->numParams=num;
	this->numParams=num;
}


bool VSTV10ToPlug::checkException(JNIEnv* env) {
	return ::checkException(env);
}
