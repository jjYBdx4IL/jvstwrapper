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
//
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#include "VSTV20ToPlug.h"
#include "JNIUtils.h"

#if defined(MACX) || defined(linux)
	#include <pthread.h>
#endif


//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV20ToPlug::VSTV20ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV10ToPlug (audioMaster, progs, parms, jvm) {

		this->isProcessEventsCacheInitialised = false;

		//init cached fields
		this->ProcessEventsMethodID = NULL;
		this->EventConstructorMethodID = NULL;
		this->MidiEventConstructorMethodID = NULL;

		this->VSTEventsClass = NULL;
		this->VSTEventClass = NULL;
		this->VSTMidiEventClass = NULL;

		this->VSTEventsObject = NULL;
	
		this->VSTEventsFieldEvents = NULL;
		this->VSTEventsFieldNumEvents = NULL;
		this->VSTEventsFieldReserved = NULL;

		this->VSTEventFieldType = NULL;
		this->VSTEventFieldByteSize = NULL;
		this->VSTEventFieldDeltaFrames = NULL;
		this->VSTEventFieldFlags = NULL;
		this->VSTEventFieldData = NULL;
		this->VSTMidiEventFieldNoteLength = NULL;
		this->VSTMidiEventFieldNoteOffset = NULL;
		this->VSTMidiEventFieldDetune = NULL;
		this->VSTMidiEventFieldNoteOffVelocity = NULL;
		this->VSTMidiEventFieldReserved1 = NULL;
		this->VSTMidiEventFieldReserved2 = NULL;


		this->isVarIoCacheInitialised = false;
		this->ProcessVarIoMethodID = NULL;

		this->VarIoClass = NULL;
		this->VarIoObject = NULL;

		this->VarIoFieldInputs = NULL;
		this->VarIoFieldOutputs = NULL;
		this->VarIoFieldNumSamplesInput = NULL;
		this->VarIoFieldNumSamplesOutput = NULL;
		this->VarIoFieldNumSamplesInputProcessed = NULL;
		this->VarIoFieldNumSamplesOutputProcessed = NULL;



		this->isSpeakerCacheInitialised = false;
		this->jSpeakerArrClass = NULL;	
		this->jSpeakerPropsClass = NULL; 
		
		this->SpeakerPropsConstructor = NULL;
		this->SetSpeakerArrMethod = NULL;

		this->jSpeakerArrInObject = NULL;
		this->jSpeakerArrOutObject = NULL; 

		this->typeField = NULL; 
		this->numChannelsField = NULL; 
		this->speakersField = NULL; 
			
		this->azimuthField = NULL; 
		this->elevationField = NULL; 
		this->radiusField = NULL; 
		this->reservedField = NULL; 
		this->nameField = NULL; 
		this->propsTypeField = NULL;
		this->futureField = NULL; 

}

//-----------------------------------------------------------------------------------------
VSTV20ToPlug::~VSTV20ToPlug () {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	//Delete global references
	if (this->VSTEventsObject!=NULL) env->DeleteGlobalRef(this->VSTEventsObject);
	if (this->VarIoObject!=NULL) env->DeleteGlobalRef(this->VarIoObject);

	if (this->jSpeakerArrInObject!=NULL) env->DeleteGlobalRef(this->jSpeakerArrInObject);
	if (this->jSpeakerArrOutObject!=NULL) env->DeleteGlobalRef(this->jSpeakerArrOutObject); 
}




//-----------------------------------------------------------------------------------------
bool VSTV20ToPlug::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text) {
	if (text==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getProgramNameIndexed", "(II)Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getProgramNameIndexed(II)Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid, (jint)category, (jint)index);
	if (ret==NULL) return false;

	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxProgNameLen) log("* WARNING: program name indexed '%s' too long (max %i)", jstr, kVstMaxProgNameLen);
	vst_strncpy (text, jstr, kVstMaxProgNameLen-1);

	if (this->checkException(env)) return false;

	return true;
}

//------------------------------------------------------------------------
bool VSTV20ToPlug::getProductString (char* text) {
	if (text==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getProductString", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getProductString()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid);
	if (ret==NULL) return false;
	
	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxProductStrLen) log("* WARNING: product string '%s' too long (max %i)", jstr, kVstMaxProductStrLen);
	vst_strncpy (text, jstr, kVstMaxProductStrLen-1);

	if (this->checkException(env)) return false;

	return true;
}

//------------------------------------------------------------------------
bool VSTV20ToPlug::getVendorString (char* text) {
	if (text==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getVendorString", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getVendorString()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid);
	if (ret==NULL) return false;

	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxVendorStrLen) log("* WARNING: vendor string '%s' too long (max %i)", jstr, kVstMaxVendorStrLen);
	vst_strncpy (text, jstr, kVstMaxVendorStrLen-1);

	if (this->checkException(env)) return false;

	return true;
}

//------------------------------------------------------------------------
VstPlugCategory VSTV20ToPlug::getPlugCategory() { 
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getPlugCategory", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getPlugCategory()I");
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	switch (ret) {
		case kPlugCategUnknown:
			return kPlugCategUnknown;
			break;
		case kPlugCategEffect:
			return kPlugCategEffect;
			break;
		case kPlugCategSynth:
			return kPlugCategSynth;		
			break;
		case kPlugCategAnalysis:
			return kPlugCategAnalysis;
			break;
		case kPlugCategMastering:
			return kPlugCategMastering;
			break;
		case kPlugCategSpacializer:
			return kPlugCategSpacializer;
			break;
		case kPlugCategRoomFx:
			return kPlugCategRoomFx;
			break;
		case kPlugSurroundFx:	
			return kPlugSurroundFx;
			break;
		case kPlugCategRestoration:
			return kPlugCategRestoration;
			break;
		case kPlugCategOfflineProcess:
			return kPlugCategOfflineProcess;
			break;
		case kPlugCategShell:
			return kPlugCategShell;
			break;
		case kPlugCategGenerator:
			return kPlugCategGenerator;
			break;
		default:
			return kPlugCategUnknown;
			break;
	}
}

VstInt32 VSTV20ToPlug::canDo (char* text) {
	if (text==NULL) return 0; //dont know

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "canDo", "(Ljava/lang/String;)I");
	if (mid == NULL) log("** ERROR: cannot find instance-method canDo(Ljava/lang/String;)I");
	
	jstring arg = env->NewStringUTF(text);

	jint ret = env->CallIntMethod(this->JavaPlugObj, mid, arg);
	
	this->checkException(env);

	return ret;
}

bool VSTV20ToPlug::string2parameter (VstInt32 index, char *text) {
	//vst spec: text==0 is to be expected to check the capability (returns true) 
	if (text==NULL) return true;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "string2Parameter", "(ILjava/lang/String;)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method string2Parameter(ILjava/lang/String;)Z"); return false;}

	if (text==NULL) return false;
	jstring arg = env->NewStringUTF(text);

	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, (jint)index, arg);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}

bool VSTV20ToPlug::setBypass (bool onOff) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setBypass", "(Z)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method setBypass(Z)Z"); return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, onOff);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}


//OPTIONAL


VstInt32 VSTV20ToPlug::getVendorVersion() {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getVendorVersion", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getVendorVersion()I");
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
}



bool VSTV20ToPlug::getEffectName (char* name)   {
	if (name==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getEffectName", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getEffectName()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid);
	if (ret==NULL) return false;
	
	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>kVstMaxEffectNameLen) log("* WARNING: effect name '%s' is too long (max %i)", jstr, kVstMaxEffectNameLen);
	vst_strncpy (name, jstr, kVstMaxEffectNameLen-1);
	
	if (this->checkException(env)) return false;

	return true;
}

bool VSTV20ToPlug::canParameterBeAutomated(VstInt32 index) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "canParameterBeAutomated", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method canParameterBeAutomated(I)Z"); return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, (jint)index);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}

bool VSTV20ToPlug::copyProgram(VstInt32 destination) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "copyProgram", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method copyProgram(I)Z");  return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, (jint)destination);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}

VstInt32 VSTV20ToPlug::fxIdle() {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "fxIdle", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method fxIdle()I");
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
}

float VSTV20ToPlug::getChannelParameter(VstInt32 channel, VstInt32 index) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getChannelParameter", "(II)F");
	if (mid == NULL) log("** ERROR: cannot find instance-method getChannelParameter(II)F");
	
	jfloat ret = env->CallFloatMethod(this->JavaPlugObj, mid, (jint)channel, (jint)index);
	
	this->checkException(env);

	return ret;
}

VstInt32 VSTV20ToPlug::getNumCategories() {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getNumCategories", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getNumCategories()I");
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
}

bool VSTV20ToPlug::getInputProperties (VstInt32 index, VstPinProperties *props) {
	if (props==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getInputProperties", "(I)Ljvst/wrapper/valueobjects/VSTPinProperties;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getInputProperties(I)Ljvst/wrapper/valueobjects/VSTPinProperties;"); return false;}
	
	jobject obj = env->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (obj == NULL) {
		return false;
	}


	jfieldID fid;
	jstring jstr;
	const char *str;
	jclass cls = env->GetObjectClass(obj);

	fid = env->GetFieldID(cls, "ArrangementType", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id ArrangementType"); return false;}
	props->arrangementType = env->GetIntField(obj, fid);
	
	fid = env->GetFieldID(cls, "Flags", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id Flags"); return false;}
	props->flags = env->GetIntField(obj, fid);
	
	fid = env->GetFieldID(cls, "Label", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id Label"); return false;}
	jstr = (jstring)env->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = env->GetStringUTFChars(jstr, 0);
		vst_strncpy(props->label,str, kVstMaxLabelLen-1);
	}
	
	fid = env->GetFieldID(cls, "ShortLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id ShortLabel"); return false;}
	jstr = (jstring)env->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = env->GetStringUTFChars(jstr, 0);
		vst_strncpy(props->shortLabel,str, kVstMaxShortLabelLen-1);
	}

	if (this->checkException(env)) return false;

	return true;
}

bool VSTV20ToPlug::getOutputProperties (VstInt32 index, VstPinProperties * props) {	
	if (props==NULL) return false;
	
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getOutputProperties", "(I)Ljvst/wrapper/valueobjects/VSTPinProperties;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getOutputProperties(I)Ljvst/wrapper/valueobjects/VSTPinProperties;"); return false;}
	
	jobject obj = env->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	if (obj==NULL) return false;

	jfieldID fid;
	jstring jstr;
	const char *str;
	jclass cls = env->GetObjectClass(obj);

	fid = env->GetFieldID(cls, "ArrangementType", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id ArrangementType"); return false;}
	props->arrangementType = env->GetIntField(obj, fid);
	
	fid = env->GetFieldID(cls, "Flags", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id Flags"); return false;}
	props->flags = env->GetIntField(obj, fid);
	
	fid = env->GetFieldID(cls, "Label", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id Label"); return false;}
	jstr = (jstring)env->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = env->GetStringUTFChars(jstr, 0);
		vst_strncpy(props->label,str, kVstMaxLabelLen-1);
	}
	
	fid = env->GetFieldID(cls, "ShortLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id ShortLabel"); return false;}
	jstr = (jstring)env->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = env->GetStringUTFChars(jstr, 0);
		vst_strncpy(props->shortLabel,str, kVstMaxShortLabelLen-1);
	}

	if (this->checkException(env)) return false;

	return true;
}

bool VSTV20ToPlug::getErrorText (char* text) {
	if (text==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getErrorText", "()Ljava/lang/String;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getErrorText()Ljava/lang/String;"); return false;}
	
	jstring ret = (jstring)env->CallObjectMethod(this->JavaPlugObj, mid);
	if (ret==NULL) return false;
	
	const char* jstr = env->GetStringUTFChars(ret, NULL);
	if (strlen(jstr)>50) log("* WARNING: error text '%s' too long (max %i)", jstr, 50);
	vst_strncpy (text, jstr, 50-1);

	if (this->checkException(env)) return false;

	return true;
}

VstInt32 VSTV20ToPlug::getGetTailSize () {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getGetTailSize", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getGetTailSize()I");
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
}

bool VSTV20ToPlug::getParameterProperties (VstInt32 index, VstParameterProperties *p) {
	if (p==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getParameterProperties", "(I)Ljvst/wrapper/valueobjects/VSTParameterProperties;");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getParameterProperties(I)Ljvst/wrapper/valueobjects/VSTParameterProperties;"); return false;}
	
	jobject obj = env->CallObjectMethod(this->JavaPlugObj, mid, (jint)index);
	//null returned --> method not supported by plugin
	if (obj==NULL) return false;

	jfieldID fid;
	jstring jstr;
	const char *str;
	jclass cls = env->GetObjectClass(obj);

	fid = env->GetFieldID(cls, "stepFloat", "F");
	if (fid == NULL) {log("** ERROR: cannot find field-id stepFloat"); return false;}
	p->stepFloat = env->GetFloatField(obj, fid);

	fid = env->GetFieldID(cls, "smallStepFloat", "F");
	if (fid == NULL) {log("** ERROR: cannot find field-id smallStepFloat"); return false;}
	p->smallStepFloat = env->GetFloatField(obj, fid);

	fid = env->GetFieldID(cls, "largeStepFloat", "F");
	if (fid == NULL) {log("** ERROR: cannot find field-id largeStepFloat"); return false;}
	p->largeStepFloat = env->GetFloatField(obj, fid);

	fid = env->GetFieldID(cls, "label", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id label"); return false;}
	jstr = (jstring)env->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = env->GetStringUTFChars(jstr, 0);
		vst_strncpy(p->label,str,kVstMaxLabelLen-1);
	}

	fid = env->GetFieldID(cls, "flags", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id flags"); return false;}
	p->flags = env->GetIntField(obj, fid);

	fid = env->GetFieldID(cls, "minInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id minInteger"); return false;}
	p->minInteger = env->GetIntField(obj, fid);

	fid = env->GetFieldID(cls, "maxInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id maxInteger"); return false;}
	p->maxInteger = env->GetIntField(obj, fid);

	fid = env->GetFieldID(cls, "stepInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id stepInteger"); return false;}
	p->stepInteger = env->GetIntField(obj, fid);

	fid = env->GetFieldID(cls, "largeStepInteger", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id largeStepInteger"); return false;}
	p->largeStepInteger = env->GetIntField(obj, fid);


	fid = env->GetFieldID(cls, "shortLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id shortLabel"); return false;}
	jstr = (jstring)env->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = env->GetStringUTFChars(jstr, 0);
		vst_strncpy(p->label,str,kVstMaxShortLabelLen-1);
	}

	fid = env->GetFieldID(cls, "displayIndex", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id displayIndex"); return false;}
	p->displayIndex = (short)env->GetIntField(obj, fid);

	fid = env->GetFieldID(cls, "category", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id category"); return false;}
	p->category = (short)env->GetIntField(obj, fid);

	fid = env->GetFieldID(cls, "numParametersInCategory", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id numParametersInCategory"); return false;}
	p->numParametersInCategory = (short)env->GetIntField(obj, fid);

	fid = env->GetFieldID(cls, "reserved", "I");
	if (fid == NULL) {log("** ERROR: cannot find field-id reserved"); return false;}
	p->reserved = (short)env->GetIntField(obj, fid);


	fid = env->GetFieldID(cls, "categoryLabel", "Ljava/lang/String;");
	if (fid == NULL) {log("** ERROR: cannot find field-id categoryLabel"); return false;}
	jstr = (jstring)env->GetObjectField(obj, fid);
	if (jstr!=NULL) {
		str = env->GetStringUTFChars(jstr, 0);
		vst_strncpy(p->categoryLabel,str,kVstMaxCategLabelLen-1);
	}
	
	fid = env->GetFieldID(cls, "future", "[C");
	if (fid == NULL) {log("** ERROR: cannot find param props field-id future"); return false;}
	jcharArray jin = (jcharArray)env->GetObjectField(obj, fid);
	jchar* jval = env->GetCharArrayElements(jin, NULL);
	for (int j=0; j<env->GetArrayLength(jin) && j<16; j++) 
		p->future[j] = jval[j];
	env->ReleaseCharArrayElements(jin, jval, 0);
	env->DeleteLocalRef(jin);


	if (this->checkException(env)) return false;

	return true;
}


VstInt32 VSTV20ToPlug::getVstVersion () {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "getVstVersion", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method getVstVersion()I");
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
}

void VSTV20ToPlug::inputConnected (VstInt32 index, bool state) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "inputConnected", "(IZ)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method inputConnected(IZ)V");
	
	env->CallVoidMethod(this->JavaPlugObj, mid, index, state);

	this->checkException(env);
}

void VSTV20ToPlug::outputConnected (VstInt32 index, bool state) {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "outputConnected", "(IZ)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method outputConnected(IZ)V");
	
	env->CallVoidMethod(this->JavaPlugObj, mid, index, state);

	this->checkException(env);
}

bool VSTV20ToPlug::keysRequired () {
	JNIEnv* env = this->ensureJavaThreadAttachment();
	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "keysRequired", "()Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method keysRequired()Z");  return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}


VstInt32 VSTV20ToPlug::processEvents (VstEvents* events) {
	if (events==NULL) return 0; //sanity check parameter
	
	if (this->isProcessEventsCacheInitialised==false) this->initProcessEventsCache();

	JNIEnv* env = this->ensureJavaThreadAttachment();

	env->SetIntField(this->VSTEventsObject, this->VSTEventsFieldNumEvents, events->numEvents);
	env->SetIntField(this->VSTEventsObject, this->VSTEventsFieldReserved, events->reserved);

	//create events array
	//TODO: need to check if this array here can be reused (similar to the algorithm used in process() / processReplacing())
	//      in order to prevent garbage collection. the way it is now, the event array is dereferenced after calling the 
	//      java side, gc needs to clean it up. Reusing the array and the event objects would save a lot of GC runs...
	jobjectArray jevents = env->NewObjectArray(events->numEvents, this->VSTEventClass, NULL);
	if (jevents == NULL) {
		log("** ERROR: out of memory! jevents");
		return 0;
	}

	for(int i=0; i<events->numEvents; i++) {
		VstEvent* e = events->events[i];
		jobject jevent;

		if (e->type==kVstMidiType) {
			//Midi Event
			jevent = env->NewObject(this->VSTMidiEventClass, this->MidiEventConstructorMethodID);
			if (jevent == NULL) {
				log("** ERROR: cannot create VSTMidiEvent Object!");
				return 0;
			}
			
			VstMidiEvent* me = (VstMidiEvent*)e;
			env->SetIntField(jevent, this->VSTEventFieldType, me->type);
			env->SetIntField(jevent, this->VSTEventFieldByteSize, me->byteSize);
			env->SetIntField(jevent, this->VSTEventFieldDeltaFrames, me->deltaFrames);
			env->SetIntField(jevent, this->VSTEventFieldFlags, me->flags);
			
			env->SetIntField(jevent, this->VSTMidiEventFieldNoteLength, me->noteLength);
			env->SetIntField(jevent, this->VSTMidiEventFieldNoteOffset, me->noteOffset);
			env->SetByteField(jevent, this->VSTMidiEventFieldDetune, me->detune);
			env->SetByteField(jevent, this->VSTMidiEventFieldNoteOffVelocity, me->noteOffVelocity);
			env->SetByteField(jevent, this->VSTMidiEventFieldReserved1, me->reserved1);
			env->SetByteField(jevent, this->VSTMidiEventFieldReserved2, me->reserved2);
			
			//midi data --> 4 byte
			jbyteArray barr = env->NewByteArray(4);
			env->SetByteArrayRegion(barr, 0, 4, (jbyte*)me->midiData);
			env->SetObjectField(jevent, this->VSTEventFieldData, barr);	
			env->DeleteLocalRef(barr);
		}
		else {
			//any other event
			jevent = env->NewObject(this->VSTEventClass, this->EventConstructorMethodID);
			if (jevent == NULL) {
				log("** ERROR: cannot create VSTEvent Object!");
				return 0;
			}

			env->SetIntField(jevent, this->VSTEventFieldType, e->type);
			env->SetIntField(jevent, this->VSTEventFieldByteSize, e->byteSize);
			env->SetIntField(jevent, this->VSTEventFieldDeltaFrames, e->deltaFrames);
			env->SetIntField(jevent, this->VSTEventFieldFlags, e->flags);
			
			if (e->type==kVstSysExType) {
				//sysex data --> variable size
				VstMidiSysexEvent* se = (VstMidiSysexEvent*)e;
				jbyteArray barr = env->NewByteArray(se->dumpBytes);
				env->SetByteArrayRegion(barr, 0, se->dumpBytes, (jbyte*)se->sysexDump);
				env->SetObjectField(jevent, this->VSTEventFieldData, barr);	
				env->DeleteLocalRef(barr);
			}
			else {
				//generic vst event --> 16 byte
				jbyteArray barr = env->NewByteArray(16);
				env->SetByteArrayRegion(barr, 0, 16, (jbyte*)e->data);
				env->SetObjectField(jevent, this->VSTEventFieldData, barr);	
				env->DeleteLocalRef(barr);
			}
		}

		env->SetObjectArrayElement(jevents, i, jevent);
		env->DeleteLocalRef(jevent); 
	}

	env->SetObjectField(this->VSTEventsObject, this->VSTEventsFieldEvents, jevents);

	//call java method
	jint ret = env->CallIntMethod(this->JavaPlugObj, this->ProcessEventsMethodID, this->VSTEventsObject);

	env->DeleteLocalRef(jevents);

	::checkException(env);

	return ret;
}


bool VSTV20ToPlug::processVariableIo ( VstVariableIo* varIo) {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	//TODO:
	//This code seems to contain some severe errors!
	//If anybody debugs this method here and fixes it, please send it to me
	//I just didnt fix it mself since this method is used very rarely in plugins...

	if (varIo==NULL) 
		return false;

	if (this->isVarIoCacheInitialised==false) this->initVarIoCache();

	if(this->JavaFloatClass == NULL) {
		this->JavaFloatClass = env->FindClass("[F");
		this->JavaFloatClass = (jclass) env->NewGlobalRef(this->JavaFloatClass);
		if (this->JavaFloatClass == NULL) log("** ERROR: cannot find class [F in VarIo");
	}

	jobjectArray  jinputs;
	jobjectArray  joutputs;
	jboolean ret;

	jinputs = env->NewObjectArray(this->getAeffect()->numInputs, this->JavaFloatClass, NULL);
	joutputs = env->NewObjectArray(this->getAeffect()->numOutputs, this->JavaFloatClass, NULL);
	if (jinputs == NULL) log("** ERROR: out of memory! vario jinputs");
	if (joutputs == NULL) log("** ERROR: out of memory! vario joutputs");

	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = varIo->inputs[i];
		jfloatArray farr = env->NewFloatArray(varIo->numSamplesInput);

		env->SetFloatArrayRegion(farr, 0, varIo->numSamplesInput, in);
		env->SetObjectArrayElement(jinputs, i, farr);
		env->DeleteLocalRef(farr);
	}

	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = varIo->outputs[i];
		jfloatArray farr = env->NewFloatArray(varIo->numSamplesOutput);

		env->SetFloatArrayRegion(farr, 0, varIo->numSamplesOutput, out);
		env->SetObjectArrayElement(joutputs, i, farr);
		env->DeleteLocalRef(farr);
	}
	//set vario object props
	env->SetObjectField(this->VarIoObject, this->VarIoFieldInputs, jinputs);
	env->SetObjectField(this->VarIoObject, this->VarIoFieldOutputs, joutputs);
	env->SetIntField(this->VarIoObject, this->VarIoFieldNumSamplesInput, varIo->numSamplesInput);
	env->SetIntField(this->VarIoObject, this->VarIoFieldNumSamplesOutput, varIo->numSamplesOutput);

	ret = env->CallBooleanMethod(this->JavaPlugObj, this->ProcessVarIoMethodID, this->VarIoObject);

	if (this->checkException(env)) return false;
	env->DeleteLocalRef(jinputs);
	env->DeleteLocalRef(joutputs);

	//copy values back to varIo struct
	jinputs = (jobjectArray)env->GetObjectField(this->VarIoObject, this->VarIoFieldInputs);
	joutputs = (jobjectArray)env->GetObjectField(this->VarIoObject, this->VarIoFieldOutputs);
	for (int i=0; i<this->getAeffect()->numInputs; i++) {
		float* in = varIo->inputs[i];
		jfloatArray jin;
		jfloat *jval;

		jin = (jfloatArray)env->GetObjectArrayElement(jinputs, i);
		jval = env->GetFloatArrayElements(jin, NULL);

		memcpy(in, jval, varIo->numSamplesInput * sizeof(float));

		env->ReleaseFloatArrayElements(jin, jval, 0);
		env->DeleteLocalRef(jin);
	}
	for (int i=0; i<this->getAeffect()->numOutputs; i++) {
		float* out = varIo->outputs[i];
		jfloatArray jout;
		jfloat *jval;

		jout = (jfloatArray)env->GetObjectArrayElement(joutputs, i);
		jval = env->GetFloatArrayElements(jout, NULL);

		memcpy(out, jval, varIo->numSamplesOutput * sizeof(float));

		env->ReleaseFloatArrayElements(jout, jval, 0);
		env->DeleteLocalRef(jout);
	}
	VstInt32 x = 0;
	varIo->numSamplesInput = env->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesInput);
	x = env->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesInputProcessed);
	varIo->numSamplesInputProcessed = &x;
	varIo->numSamplesOutput = env->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesOutput);
	VstInt32 xx = env->GetIntField(this->VarIoObject, this->VarIoFieldNumSamplesOutputProcessed);
	varIo->numSamplesOutputProcessed = &xx;

	env->DeleteLocalRef(jinputs);
	env->DeleteLocalRef(joutputs);


	if (this->checkException(env)) return false;

	return ret==JNI_TRUE;
}

VstInt32 VSTV20ToPlug::reportCurrentPosition () {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "reportCurrentPosition", "()I");
	if (mid == NULL) log("** ERROR: cannot find instance-method reportCurrentPosition()I");
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid);
	
	this->checkException(env);

	return ret;
}

float* VSTV20ToPlug::reportDestinationBuffer () {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "reportDestinationBuffer", "()[F");
	if (mid == NULL) log("** ERROR: cannot find instance-method reportDestinationBuffer()[F");
	
	jfloatArray jret = (jfloatArray)env->CallObjectMethod(this->JavaPlugObj, mid);
	jfloat* jval = env->GetFloatArrayElements(jret, NULL);
	//do memcopy and free array ressources here?
	/*
	jval = env->GetFloatArrayElements(jout, NULL);
	memcpy(out, jval, varIo->numSamplesOutput * sizeof(float));
	env->ReleaseFloatArrayElements(jout, jval, 0);
	env->DeleteLocalRef(jout);
	*/
	
	this->checkException(env);

	return jval;
}

void VSTV20ToPlug::setBlockSizeAndSampleRate (VstInt32 bs, float sr) {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "setBlockSizeAndSampleRate", "(IF)V");
	if (mid == NULL) log("** ERROR: cannot find instance-method setBlockSizeAndSampleRate(IF)V");
	
	env->CallVoidMethod(this->JavaPlugObj, mid, bs, sr);

	this->checkException(env);	
}

bool VSTV20ToPlug::setSpeakerArrangement (VstSpeakerArrangement* pluginInput, VstSpeakerArrangement *pluginOutput) {
	//return true; -->OK!
	
	if (pluginInput==NULL || pluginOutput==NULL) 
		return false;
	
	JNIEnv* env = this->ensureJavaThreadAttachment();

	if (this->isSpeakerCacheInitialised==false) this->initSpeakerCache();
	
	env->SetIntField(this->jSpeakerArrInObject, this->typeField, pluginInput->type);
	env->SetIntField(this->jSpeakerArrOutObject, this->typeField, pluginOutput->type);
	env->SetIntField(this->jSpeakerArrInObject, this->numChannelsField, pluginInput->numChannels);
	env->SetIntField(this->jSpeakerArrOutObject, this->numChannelsField, pluginOutput->numChannels);
	

	if (pluginInput->numChannels > 100) {
		log("* Warning: too many input channels (setSpeakerArrangement)");
		return false;
	}
	jobjectArray jInProps = env->NewObjectArray(pluginInput->numChannels, this->jSpeakerPropsClass, NULL);

	for (int i=0; i<pluginInput->numChannels; i++) {
		jobject jSpeakerPropsObject = env->NewObject(this->jSpeakerPropsClass, this->SpeakerPropsConstructor);
		if (jSpeakerPropsObject == NULL) {
			log("** ERROR: cannot create VSTSpeakerProperties Object!");
			break;
		}

		env->SetFloatField(jSpeakerPropsObject, this->azimuthField, pluginInput->speakers[i].azimuth);
		env->SetFloatField(jSpeakerPropsObject, this->elevationField, pluginInput->speakers[i].elevation);
		env->SetFloatField(jSpeakerPropsObject, this->radiusField, pluginInput->speakers[i].radius);
		env->SetFloatField(jSpeakerPropsObject, this->reservedField, pluginInput->speakers[i].reserved);		
		env->SetIntField(jSpeakerPropsObject, this->propsTypeField, pluginInput->speakers[i].type);

		jstring jstr = env->NewStringUTF(pluginInput->speakers[i].name);
		env->SetObjectField(jSpeakerPropsObject, this->nameField, jstr);
		
		//hack! convert array
		jbyte* c = new jbyte[28];
		for (int j=0; j<28; j++) c[j]=pluginInput->speakers[i].future[j]; 

		jbyteArray farr = env->NewByteArray(28);
		env->SetByteArrayRegion(farr, 0, 28, c);
		env->SetObjectField(jSpeakerPropsObject, this->futureField, farr);
		env->DeleteLocalRef(farr);


		env->SetObjectArrayElement(jInProps, i, jSpeakerPropsObject);

		env->DeleteLocalRef(jSpeakerPropsObject);
		delete [] c;
	}


	if (pluginOutput->numChannels > 100) {
		log("* Warning: too many output channels (setSpeakerArrangement)");
		return false;
	}
	jobjectArray jOutProps = env->NewObjectArray(pluginOutput->numChannels, this->jSpeakerPropsClass, NULL);

	for (int i=0; i<pluginOutput->numChannels; i++) {
		jobject jSpeakerPropsObject = env->NewObject(this->jSpeakerPropsClass, this->SpeakerPropsConstructor);
		if (jSpeakerPropsObject == NULL) {
			log("** ERROR: cannot create VSTSpeakerProperties Object!");
			break;
		}

		env->SetFloatField(jSpeakerPropsObject, this->azimuthField, pluginOutput->speakers[i].azimuth);
		env->SetFloatField(jSpeakerPropsObject, this->elevationField, pluginOutput->speakers[i].elevation);
		env->SetFloatField(jSpeakerPropsObject, this->radiusField, pluginOutput->speakers[i].radius);
		env->SetFloatField(jSpeakerPropsObject, this->reservedField, pluginOutput->speakers[i].reserved);		
		env->SetIntField(jSpeakerPropsObject, this->propsTypeField, pluginOutput->speakers[i].type);

		jstring jstr = env->NewStringUTF(pluginOutput->speakers[i].name);
		env->SetObjectField(jSpeakerPropsObject, this->nameField, jstr);
		
		//hack! convert array
		jbyte* c = new jbyte[28];
		for (int j=0; j<28; j++) c[j]=pluginOutput->speakers[i].future[j];

		jbyteArray farr = env->NewByteArray(28);
		env->SetByteArrayRegion(farr, 0, 28, c);
		env->SetObjectField(jSpeakerPropsObject, this->futureField, farr);
		env->DeleteLocalRef(farr);


		env->SetObjectArrayElement(jOutProps, i, jSpeakerPropsObject);

		env->DeleteLocalRef(jSpeakerPropsObject);
		delete [] c;
	}

	env->SetObjectField(this->jSpeakerArrInObject, this->speakersField, jInProps);
	env->SetObjectField(this->jSpeakerArrOutObject, this->speakersField, jOutProps);


	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, this->SetSpeakerArrMethod, this->jSpeakerArrInObject, this->jSpeakerArrOutObject);

	env->DeleteLocalRef(jInProps);
	env->DeleteLocalRef(jOutProps);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}


bool VSTV20ToPlug::getSpeakerArrangement (VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput) {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	if (this->isSpeakerCacheInitialised==false) this->initSpeakerCache();

	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, this->GetSpeakerArrMethod, this->jSpeakerArrInObject, this->jSpeakerArrOutObject);
	if (ret==JNI_FALSE) {
		*pluginInput = 0; 
		*pluginOutput = 0;
		return false;
	}


	//alloc
	VstSpeakerArrangement *input = new VstSpeakerArrangement;
	VstSpeakerArrangement *output= new VstSpeakerArrangement;

	input->type = env->GetIntField(this->jSpeakerArrInObject, this->typeField);
	output->type = env->GetIntField(this->jSpeakerArrOutObject, this->typeField);
	input->numChannels = env->GetIntField(this->jSpeakerArrInObject, this->numChannelsField);
	output->numChannels = env->GetIntField(this->jSpeakerArrOutObject, this->numChannelsField);
	

	jobjectArray jinputs = (jobjectArray)env->GetObjectField(this->jSpeakerArrInObject, this->speakersField);
	jobjectArray joutputs = (jobjectArray)env->GetObjectField(this->jSpeakerArrOutObject, this->speakersField);
	if (jinputs==NULL || joutputs==NULL) {
		*pluginInput = 0;
		*pluginOutput = 0;
		return false;
	}

	for (int i=0; i<env->GetArrayLength(jinputs) && i<8; i++) {
		jobject jSpeakerPropsObject = (jobject)env->GetObjectArrayElement(jinputs, i);
		if (jSpeakerPropsObject == NULL) {
			log("** ERROR: cannot create VSTSpeakerProperties Object!");
			break;
		}
		
		VstSpeakerProperties props;
		props.azimuth = env->GetFloatField(jSpeakerPropsObject, this->azimuthField);
		props.elevation = env->GetFloatField(jSpeakerPropsObject, this->elevationField);
		props.radius = env->GetFloatField(jSpeakerPropsObject, this->radiusField);
		props.reserved = env->GetFloatField(jSpeakerPropsObject, this->reservedField);		
		props.type = env->GetIntField(jSpeakerPropsObject, this->propsTypeField);
	
		jstring str = (jstring)env->GetObjectField(jSpeakerPropsObject, this->nameField);
		if (str==NULL) break;
		const char* jstr = env->GetStringUTFChars(str, NULL);
		vst_strncpy (props.name, jstr, kVstMaxNameLen-1);
		if(this->checkException(env)) break;

		jcharArray jin = (jcharArray)env->GetObjectField(jSpeakerPropsObject, this->futureField);
		jchar* jval = env->GetCharArrayElements(jin, NULL);
		for (int j=0; j<env->GetArrayLength(jin) && j<28; j++) 
			props.future[j] = jval[j];
		env->ReleaseCharArrayElements(jin, jval, 0);
		env->DeleteLocalRef(jin);

		input->speakers[i] = props;
	}

	for (int i=0; i<env->GetArrayLength(joutputs) && i<8; i++) {
		jobject jSpeakerPropsObject = (jobject)env->GetObjectArrayElement(joutputs, i);
		if (jSpeakerPropsObject == NULL) {
			log("** ERROR: cannot create VSTSpeakerProperties Object!");
			break;
		}
		
		VstSpeakerProperties props;
		props.azimuth = env->GetFloatField(jSpeakerPropsObject, this->azimuthField);
		props.elevation = env->GetFloatField(jSpeakerPropsObject, this->elevationField);
		props.radius = env->GetFloatField(jSpeakerPropsObject, this->radiusField);
		props.reserved = env->GetFloatField(jSpeakerPropsObject, this->reservedField);		
		props.type = env->GetIntField(jSpeakerPropsObject, this->propsTypeField);
	
		jstring str = (jstring)env->GetObjectField(jSpeakerPropsObject, this->nameField);
		if (str==NULL) break;
		const char* jstr = env->GetStringUTFChars(str, NULL);
		vst_strncpy (props.name, jstr, kVstMaxNameLen-1);
		if(this->checkException(env)) break;

		jcharArray jin = (jcharArray)env->GetObjectField(jSpeakerPropsObject, this->futureField);
		jchar* jval = env->GetCharArrayElements(jin, NULL);
		for (int j=0; j<env->GetArrayLength(jin) && j<28; j++) 
			props.future[j] = jval[j];
		env->ReleaseCharArrayElements(jin, jval, 0);
		env->DeleteLocalRef(jin);

		output->speakers[i] = props;
	}

	env->DeleteLocalRef(jinputs);
	env->DeleteLocalRef(joutputs);
	
	if (this->checkException(env)) {
		*pluginInput = 0;
		*pluginOutput = 0;
		return false;
	}
	
	pluginInput = &input;
	pluginOutput = &output;

	return ret==JNI_TRUE;
}






//TODO: maybe implement that later, so that swing ui can be positioned by native app...
//but this is deprecated since 2.3 anyways, so I dont know if any of the hosts use it
/*
bool VSTV20ToPlug::setViewPosition (long x, long y) {
	return false;
}
*/



//************************************************************************************
void VSTV20ToPlug::initProcessEventsCache () {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	//cache fields
	this->ProcessEventsMethodID = env->GetMethodID(this->JavaPlugClass, "processEvents", "(Ljvst/wrapper/valueobjects/VSTEvents;)I");
	if (this->ProcessEventsMethodID == NULL) {
		log("** ERROR: cannot find instance-method processEvents(Ljvst/wrapper/valueobjects/VSTEvents;)I");  
		return;
	}

	this->VSTEventsClass = env->FindClass("jvst/wrapper/valueobjects/VSTEvents");		
	if (this->VSTEventsClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvents");
		return;
	}
	this->VSTEventsClass = (jclass) env->NewGlobalRef(this->VSTEventsClass);

	this->VSTEventClass = env->FindClass("jvst/wrapper/valueobjects/VSTEvent");		
	if (this->VSTEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvent");
		return;
	}
	this->VSTEventClass = (jclass) env->NewGlobalRef(this->VSTEventClass);

	this->VSTMidiEventClass = env->FindClass("jvst/wrapper/valueobjects/VSTMidiEvent");		
	if (this->VSTMidiEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTMidiEvent");
		return;
	}
	this->VSTMidiEventClass = (jclass) env->NewGlobalRef(this->VSTMidiEventClass);


	//Call VSTEvents JAVA Konstruktor
	jmethodID mid = env->GetMethodID(this->VSTEventsClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for VSTEvents!");
		return;
	}
	this->VSTEventsObject = env->NewObject(this->VSTEventsClass, mid);
	if (this->VSTEventsObject == NULL) {
		log("** ERROR: cannot create VST Events Object!");
		return;
	}
	//Create global reference --> use this object accross different threads
	this->VSTEventsObject = env->NewGlobalRef(this->VSTEventsObject);



	this->VSTEventsFieldEvents = env->GetFieldID(this->VSTEventsClass, "events", "[Ljvst/wrapper/valueobjects/VSTEvent;");
	if (this->VSTEventsFieldEvents == NULL) {
		log("** ERROR: cannot find field-id events ([Ljvst/wrapper/valueobjects/VSTEvent;)");
		return;
	}
	this->VSTEventsFieldNumEvents = env->GetFieldID(this->VSTEventsClass, "numEvents", "I");
	if (this->VSTEventsFieldNumEvents == NULL) {
		log("** ERROR: cannot find field-id numEvents");
		return;
	}
	this->VSTEventsFieldReserved = env->GetFieldID(this->VSTEventsClass, "reserved", "I");
	if (this->VSTEventsFieldReserved == NULL) {
		log("** ERROR: cannot find field-id reserved(I) (VSTEvents)");
		return;
	}



	this->EventConstructorMethodID = env->GetMethodID(this->VSTEventClass, "<init>", "()V");
	if (this->EventConstructorMethodID == NULL) {
		log("** ERROR: cannot find default contructor for Event!");
		return;
	}
	this->MidiEventConstructorMethodID = env->GetMethodID(this->VSTMidiEventClass, "<init>", "()V");
	if (this->MidiEventConstructorMethodID == NULL) {
		log("** ERROR: cannot find default contructor for MidiEvent!");
		return;
	}


	this->VSTEventFieldType = env->GetFieldID(this->VSTEventClass, "type", "I");
	if (this->VSTEventFieldType == NULL) {
		log("** ERROR: cannot find field-id type(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldByteSize = env->GetFieldID(this->VSTEventClass, "byteSize", "I");
	if (this->VSTEventFieldByteSize == NULL) {
		log("** ERROR: cannot find field-id byteSize(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldDeltaFrames = env->GetFieldID(this->VSTEventClass, "deltaFrames", "I");
	if (this->VSTEventFieldDeltaFrames == NULL) {
		log("** ERROR: cannot find field-id deltaFrames(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldFlags = env->GetFieldID(this->VSTEventClass, "flags", "I");
	if (this->VSTEventFieldFlags == NULL) {
		log("** ERROR: cannot find field-id flags(I) (VSTEvent)");
		return;
	}
	this->VSTEventFieldData = env->GetFieldID(this->VSTEventClass, "data", "[B");
	if (this->VSTEventFieldData == NULL) {
		log("** ERROR: cannot find field-id data(I) (VSTEvent)");
		return;
	}
	
	

	this->VSTMidiEventFieldNoteLength = env->GetFieldID(this->VSTMidiEventClass, "noteLength", "I");
	if (this->VSTMidiEventFieldNoteLength == NULL) {
		log("** ERROR: cannot find field-id noteLength(I) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldNoteOffset = env->GetFieldID(this->VSTMidiEventClass, "noteOffset", "I");
	if (this->VSTMidiEventFieldNoteOffset == NULL) {
		log("** ERROR: cannot find field-id noteOffset(I) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldDetune = env->GetFieldID(this->VSTMidiEventClass, "detune", "B");
	if (this->VSTMidiEventFieldDetune == NULL) {
		log("** ERROR: cannot find field-id detune(B) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldNoteOffVelocity = env->GetFieldID(this->VSTMidiEventClass, "noteOffVelocity", "B");
	if (this->VSTMidiEventFieldNoteOffVelocity == NULL) {
		log("** ERROR: cannot find field-id noteOffVelocity(B) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldReserved1 = env->GetFieldID(this->VSTMidiEventClass, "reserved1", "B");
	if (this->VSTMidiEventFieldReserved1 == NULL) {
		log("** ERROR: cannot find field-id reserved1(B) (VSTMidiEvent)");
		return;
	}
	this->VSTMidiEventFieldReserved2 = env->GetFieldID(this->VSTMidiEventClass, "reserved2", "B");
	if (this->VSTMidiEventFieldReserved2 == NULL) {
		log("** ERROR: cannot find field-id reserved2(B) (VSTMidiEvent)");
		return;
	}


	this->isProcessEventsCacheInitialised = true;

	this->checkException(env);
}


//************************************************************************************
void VSTV20ToPlug::initVarIoCache () {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	this->VarIoClass = env->FindClass("jvst/wrapper/valueobjects/VSTVariableIO");		
	if (this->VarIoClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTVariableIO");
		return;
	}
	this->VarIoClass = (jclass) env->NewGlobalRef(this->VarIoClass);


	//Call VarIo JAVA Konstruktor
	jmethodID mid = env->GetMethodID(this->VarIoClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for VarIo!");
		return;
	}
	this->VarIoObject = env->NewObject(this->VarIoClass, mid);
	if (this->VarIoObject == NULL) {
		log("** ERROR: cannot create VST VarIo Object!");
		return;
	}

	this->checkException(env);

	//Create global reference --> use this object accross different threads
	this->VarIoObject = env->NewGlobalRef(this->VarIoObject);

	this->checkException(env);

	this->ProcessVarIoMethodID = env->GetMethodID(this->JavaPlugClass, "processVariableIo", "(Ljvst/wrapper/valueobjects/VSTVariableIO;)Z");
	if (mid == NULL) {
		log("** ERROR: cannot find ProcessVarIo MethodID!");
		return;
	}

	this->checkException(env);
	this->VarIoFieldInputs = env->GetFieldID(this->VarIoClass, "inputs", "[[F");
	if (this->VarIoFieldInputs == NULL) {
		log("** ERROR: cannot find field-id inputs ([[F");
		return;
	}
	this->VarIoFieldOutputs = env->GetFieldID(this->VarIoClass, "outputs", "[[F");
	if (this->VarIoFieldOutputs == NULL) {
		log("** ERROR: cannot find field-id outputs ([[F");
		return;
	}
	this->VarIoFieldNumSamplesInput = env->GetFieldID(this->VarIoClass, "numSamplesInput", "I");
	if (this->VarIoFieldNumSamplesInput == NULL) {
		log("** ERROR: cannot find field-id numSamplesInput (I");
		return;
	}
	this->VarIoFieldNumSamplesOutput = env->GetFieldID(this->VarIoClass, "numSamplesOutput", "I");
	if (this->VarIoFieldNumSamplesOutput == NULL) {
		log("** ERROR: cannot find field-id numSamplesOutput (I");
		return;
	}
	this->VarIoFieldNumSamplesInputProcessed = env->GetFieldID(this->VarIoClass, "numSamplesInputProcessed", "I");
	if (this->VarIoFieldNumSamplesInputProcessed == NULL) {
		log("** ERROR: cannot find field-id numSamplesInputProcessed (I");
		return;
	}
	this->VarIoFieldNumSamplesOutputProcessed = env->GetFieldID(this->VarIoClass, "numSamplesOutputProcessed", "I");
	if (this->VarIoFieldNumSamplesOutputProcessed == NULL) {
		log("** ERROR: cannot find field-id numSamplesOutputProcessed (I");
		return;
	}


	this->isVarIoCacheInitialised = true;

	this->checkException(env);
}


//************************************************************************************
void VSTV20ToPlug::initSpeakerCache() {
	JNIEnv* env = this->ensureJavaThreadAttachment();

	this->jSpeakerArrClass = env->FindClass("jvst/wrapper/valueobjects/VSTSpeakerArrangement");
	if (this->jSpeakerArrClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTSpeakerArrangement");
		return;
	}
	this->jSpeakerArrClass = (jclass) env->NewGlobalRef(this->jSpeakerArrClass);


	//Call JAVA Konstruktor
	jmethodID mid = env->GetMethodID(this->jSpeakerArrClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for VSTSpeakerArrangement!");
		return;
	}
	this->jSpeakerArrInObject = env->NewObject(this->jSpeakerArrClass, mid);
	if (this->jSpeakerArrInObject == NULL) {
		log("** ERROR: cannot create VSTSpeakerArrangement Object!");
		return;
	}
	this->jSpeakerArrInObject = env->NewGlobalRef(this->jSpeakerArrInObject);

	this->jSpeakerArrOutObject = env->NewObject(this->jSpeakerArrClass, mid);
	if (this->jSpeakerArrOutObject == NULL) {
		log("** ERROR: cannot create VSTSpeakerArrangement Object!");
		return;
	}
	this->jSpeakerArrOutObject = env->NewGlobalRef(this->jSpeakerArrOutObject);


	this->jSpeakerPropsClass = env->FindClass("jvst/wrapper/valueobjects/VSTSpeakerProperties");
	if (this->jSpeakerPropsClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTSpeakerProperties");
		return;
	}	
	this->jSpeakerPropsClass = (jclass) env->NewGlobalRef(this->jSpeakerPropsClass);

	this->SpeakerPropsConstructor = env->GetMethodID(this->jSpeakerPropsClass, "<init>", "()V");
	if (this->SpeakerPropsConstructor == NULL) {
		log("** ERROR: cannot find default contructor for VSTSpeakerProperties!");
		return;
	}

	this->SetSpeakerArrMethod = env->GetMethodID(this->JavaPlugClass, "setSpeakerArrangement", "(Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z");
	if (this->SetSpeakerArrMethod == NULL) {
		log("** ERROR: cannot find instance-method setSpeakerArrangement(Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z");
		return;
	}
	this->GetSpeakerArrMethod = env->GetMethodID(this->JavaPlugClass, "getSpeakerArrangement", "(Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z");
	if (this->SetSpeakerArrMethod == NULL) {
		log("** ERROR: cannot find instance-method getSpeakerArrangement(Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;Ljvst/wrapper/valueobjects/VSTSpeakerArrangement;)Z");
		return;
	}

	 
	this->typeField = env->GetFieldID(this->jSpeakerArrClass, "type", "I");
	if (this->typeField == NULL) {
		log("** ERROR: cannot find field-id type (I)");
		return;
	}
	this->numChannelsField = env->GetFieldID(this->jSpeakerArrClass, "numChannels", "I");
	if (this->numChannelsField == NULL) {
		log("** ERROR: cannot find field-id numChannels (I)");
		return;
	}
	this->speakersField = env->GetFieldID(this->jSpeakerArrClass, "speakers", "[Ljvst/wrapper/valueobjects/VSTSpeakerProperties;");
	if (this->speakersField == NULL) {
		log("** ERROR: cannot find field-id speakers ([Ljvst/wrapper/valueobjects/VSTSpeakerProperties)");
		return;
	}
	
	this->azimuthField = env->GetFieldID(this->jSpeakerPropsClass, "azimuth", "F");
	if (this->azimuthField == NULL) {
		log("** ERROR: cannot find field-id azimuth (F)");
		return;
	}
	this->elevationField = env->GetFieldID(this->jSpeakerPropsClass, "elevation", "F");
	if (this->elevationField == NULL) {
		log("** ERROR: cannot find field-id elevation (F)");
		return;
	}
	this->radiusField = env->GetFieldID(this->jSpeakerPropsClass, "radius", "F");
	if (this->radiusField == NULL) {
		log("** ERROR: cannot find field-id radius (F)");
		return;
	}
	this->reservedField = env->GetFieldID(this->jSpeakerPropsClass, "reserved", "F");
	if (this->reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (F)");
		return;
	}
	this->nameField = env->GetFieldID(this->jSpeakerPropsClass, "name", "Ljava/lang/String;");
	if (this->nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return;
	}
	this->propsTypeField = env->GetFieldID(this->jSpeakerPropsClass, "type", "I");
	if (this->typeField == NULL) {
		log("** ERROR: cannot find field-id type (I)");
		return;
	}
	this->futureField = env->GetFieldID(this->jSpeakerPropsClass, "future", "[C");
	if (this->futureField == NULL) {
		log("** ERROR: cannot find field-id future ([C)");
		return;
	}

	
	this->isSpeakerCacheInitialised = true;

	this->checkException(env);
}
