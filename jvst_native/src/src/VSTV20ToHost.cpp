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
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------


#include "VSTV20ToHost.h"
#include "VSTV24ToPlug.h"
#include <stdlib.h>
#include "JNIUtils.h" //important, this needs always to be the LAST import!

extern VSTV24ToPlug* getWrapperInstance(JNIEnv *env, jobject obj);



//Impl of Plug -> Host calls
//Java -> Native
//impls for VSTV20ToHost.java native calls
//**********************************************



// XXX: BUG: TODO:	The reference to the TimeInfoObject is global, so, if there is more than one
//					plugin running at the same time, races might occur and
//					the TimeInfo a plugin receives gets mixed up (all plugins get the information
//					written by the last writer).
//
//					Solution: cache the TimeInfoObject at the java side, so that every plugin instance
//					has its own, private copy of the instance and there is no global one.
//
//					If anyone implements this, please send the patch to daniel309@users.sourceforge.net

//cached fieds
//******************************

//TimeInfo
void InitTimeInfoCache(JNIEnv* env);
bool IsTimeInfoCacheInitialised = false;
jclass TimeInfoClass = NULL;
jobject TimeInfoObject = NULL;

jfieldID TimeInfoFieldSamplePos = NULL;
jfieldID TimeInfoFieldSampleRate = NULL;
jfieldID TimeInfoFieldNanoSeconds = NULL;
jfieldID TimeInfoFieldPPQPos = NULL;
jfieldID TimeInfoFieldTempo = NULL;
jfieldID TimeInfoFieldBarStartPos = NULL;
jfieldID TimeInfoFieldCycleStartPos = NULL;
jfieldID TimeInfoFieldCycleEndPos = NULL;
jfieldID TimeInfoFieldTimeSigNumerator = NULL;
jfieldID TimeInfoFieldTimeSigDenominator = NULL;
jfieldID TimeInfoFieldSmpteOffset = NULL;
jfieldID TimeInfoFieldSmpteFrameRate = NULL;
jfieldID TimeInfoFieldSamplesToNextClock = NULL;
jfieldID TimeInfoFieldFlags = NULL;


//Events
void InitEventsCache(JNIEnv* env);
bool IsEventsCacheInitialised = false;

jclass VSTEventsClass = NULL;
jclass VSTEventClass = NULL;
jclass MidiEventClass = NULL;

jfieldID EventsFieldEvents = NULL;
jfieldID EventsFieldNumEvents = NULL;
jfieldID EventsFieldReserved = NULL;

jfieldID EventFieldType = NULL;
jfieldID EventFieldByteSize = NULL;
jfieldID EventFieldDeltaFrames = NULL;
jfieldID EventFieldFlags = NULL;
jfieldID EventFieldData = NULL;
jfieldID MidiEventFieldNoteLength = NULL;
jfieldID MidiEventFieldNoteOffset = NULL;
jfieldID MidiEventFieldDetune = NULL;
jfieldID MidiEventFieldNoteOffVelocity = NULL;
jfieldID MidiEventFieldReserved1 = NULL;
jfieldID MidiEventFieldReserved2 = NULL;


#define VST_EVENTS_MAX 256

struct BigVstEvents
{
    VstInt32 numEvents;
    VstIntPtr reserved;
    VstEvent* events[VST_EVENTS_MAX];
};


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    canHostDo
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_canHostDo
		(JNIEnv * env, jobject obj, jstring str) {
	log("canHostDo");

	char t[255];
	jint ret = -1L;

	if (str==NULL) return ret;

	const char* text = env->GetStringUTFChars(str, NULL);
	vst_strncpy(t, text, 254);
	env->ReleaseStringUTFChars(str, text);

    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) ret = WrapperInstance->canHostDo(t);

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    sizeWindow
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_sizeWindow
(JNIEnv * env, jobject obj, jint width, jint height){
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->sizeWindow(width,height);
	else return false;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    ioChanged
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_ioChanged
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);

	//Refresh the number of parameters
	jmethodID mid = env->GetMethodID(env->GetObjectClass(obj), "getNumParams", "()I");
	if (mid == NULL) {
		log("** ERROR: cannot find effects .getNumParams(I)");
		checkException(env); //print stack trace!
		return -1;
	}
	int num = env->CallIntMethod(obj, mid);
	WrapperInstance->setNumParams(num);

	//inputs, outputs and initial delay can be set from the Java side


	if (WrapperInstance!=NULL) return WrapperInstance->ioChanged();
	else return false;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getTimeInfo
 * Signature: (I)Ljvst/wrapper/valueobjects/VSTTimeInfo;
 */
JNIEXPORT jobject JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getTimeInfo
		(JNIEnv* env, jobject obj, jint filt) {

    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) {
		if (IsTimeInfoCacheInitialised==false) InitTimeInfoCache(env);

		VstTimeInfo* ti = WrapperInstance->getTimeInfo(filt);

		//if getTimeInfo NOT support by host, then NULL is returned!
		if (ti!=NULL) {
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldSamplePos, ti->samplePos);
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldSampleRate, ti->sampleRate);
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldNanoSeconds, ti->nanoSeconds);
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldPPQPos, ti->ppqPos);
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldTempo, ti->tempo);
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldBarStartPos, ti->barStartPos);
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldCycleStartPos, ti->cycleStartPos);
			env->SetDoubleField(TimeInfoObject, TimeInfoFieldCycleEndPos, ti->cycleEndPos);

			env->SetIntField(TimeInfoObject, TimeInfoFieldTimeSigNumerator, ti->timeSigNumerator);
			env->SetIntField(TimeInfoObject, TimeInfoFieldTimeSigDenominator, ti->timeSigDenominator);
			env->SetIntField(TimeInfoObject, TimeInfoFieldSmpteFrameRate, ti->smpteFrameRate);
			env->SetIntField(TimeInfoObject, TimeInfoFieldSmpteOffset, ti->smpteOffset);
			env->SetIntField(TimeInfoObject, TimeInfoFieldSamplesToNextClock, ti->samplesToNextClock);
			env->SetIntField(TimeInfoObject, TimeInfoFieldFlags, ti->flags);

			checkAndThrowException(env);

			return TimeInfoObject;
		}
		else return NULL;
	}
	else return NULL;
}




#ifndef min
	int min(int a, int b) {return a < b ? a : b;}
#endif

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    sendVstEventsToHost
 * Signature: (Ljvst/wrapper/valueobjects/VSTEvents;)Z
 */

JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_sendVstEventsToHost
	(JNIEnv* env, jobject obj, jobject events) {

	VSTV24ToPlug* WrapperInstance = getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL && events!=NULL) {

		//alloc events
        BigVstEvents vstEventsToHost;
        VstMidiSysexEvent vstMidiSysexEventsToHost[VST_EVENTS_MAX];
		VstMidiEvent vstMidiEventsToHost[VST_EVENTS_MAX];
        VstEvent vstEventToHost[VST_EVENTS_MAX];

		if (IsEventsCacheInitialised==false) InitEventsCache(env);


		VstEvents* ret = (VstEvents*)&vstEventsToHost;
		ret->numEvents = env->GetIntField(events, EventsFieldNumEvents);
		ret->reserved = env->GetIntField(events, EventsFieldReserved);

		jobjectArray jevents = (jobjectArray)env->GetObjectField(events, EventsFieldEvents);
		if (jevents==NULL) return 0;


		int len = min (env->GetArrayLength(jevents), ret->numEvents); // better be save. If somebody initializes a big array,
																	  // but only initializes the first el. we crash later
																	  // when accessing the second element. Solution: hopefully
																	  // the field numEvents was used. We simply use the smaller
																	  // one of both hints to be save.
		for (int i=0; i < len; i++) {
			//CAUTION: I only VST_EVENTS_MAX events will be transmited to Host
			if (i>=VST_EVENTS_MAX) break;

			VstEvent* vstevent;
			jobject jevent = env->GetObjectArrayElement(jevents, i);

			jint typ = env->GetIntField(jevent, EventFieldType);

			if (typ==kVstMidiType) {
				VstMidiEvent* mevent = &(vstMidiEventsToHost[i]);

				mevent->type = typ;
				mevent->byteSize = env->GetIntField(jevent, EventFieldByteSize);
				mevent->deltaFrames = env->GetIntField(jevent, EventFieldDeltaFrames);
				mevent->flags = env->GetIntField(jevent, EventFieldFlags);

				jbyteArray jdata = (jbyteArray)env->GetObjectField(jevent, EventFieldData);
				jbyte* elmts = env->GetByteArrayElements(jdata, NULL);
				for (int j=0; j<env->GetArrayLength(jdata); j++) {
					if (j>=3) break; // 3 bytes of midi data
					mevent->midiData[j] = elmts[j];
				}
				mevent->midiData[3] = 0; //reserved
				env->ReleaseByteArrayElements(jdata, elmts, 0);
				env->DeleteLocalRef(jdata);

				mevent->noteLength = env->GetIntField(jevent, MidiEventFieldNoteLength);
				mevent->noteOffset = env->GetIntField(jevent, MidiEventFieldNoteOffset);

				mevent->detune = env->GetByteField(jevent, MidiEventFieldDetune);
				mevent->noteOffVelocity = env->GetByteField(jevent, MidiEventFieldNoteOffVelocity);
				mevent->reserved1 = env->GetByteField(jevent, MidiEventFieldReserved1);
				mevent->reserved2 = env->GetByteField(jevent, MidiEventFieldReserved2);

				vstevent = (VstEvent*)mevent;
			}
			else if (typ==kVstSysExType) {
				VstMidiSysexEvent* sysevent = &(vstMidiSysexEventsToHost[i]);

				sysevent->type = typ;
				sysevent->byteSize = env->GetIntField(jevent, EventFieldByteSize);
				sysevent->deltaFrames = env->GetIntField(jevent, EventFieldDeltaFrames);
				sysevent->flags = env->GetIntField(jevent, EventFieldFlags);

				jbyteArray jdata = (jbyteArray)env->GetObjectField(jevent, EventFieldData);
				jbyte* elmts = env->GetByteArrayElements(jdata, NULL);
				jsize len = env->GetArrayLength(jdata);

				sysevent->dumpBytes = len;
				sysevent->sysexDump = (char*)malloc(len);
				memcpy(sysevent->sysexDump, elmts, len);
				
				env->ReleaseByteArrayElements(jdata, elmts, 0);
				env->DeleteLocalRef(jdata);

				vstevent = (VstEvent*)sysevent;
			}
			else {
				vstevent = &(vstEventToHost[i]);

				vstevent->type = typ;
				vstevent->byteSize = env->GetIntField(jevent, EventFieldByteSize);
				vstevent->deltaFrames = env->GetIntField(jevent, EventFieldDeltaFrames);
				vstevent->flags = env->GetIntField(jevent, EventFieldFlags);

				jbyteArray jdata = (jbyteArray)env->GetObjectField(jevent, EventFieldData);
				jbyte* elmts = env->GetByteArrayElements(jdata, NULL);
				
				for (int j=0; j<env->GetArrayLength(jdata); j++) {
					if (j>=16) break; //16 Bytes Data
					vstevent->data[j] = elmts[j];
				}

				env->ReleaseByteArrayElements(jdata, elmts, 0);
				env->DeleteLocalRef(jdata);
			}

			ret->events[i] = vstevent;
		}

		checkAndThrowException(env);
		return WrapperInstance->sendVstEventsToHost(ret);
	}
	else return 0;
 }




/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    needIdle
 * Signature: ()Z
 */

//dm: deprecated as of vst2.4
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_needIdle
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->needIdle();
	else return false;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateSampleRate
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateSampleRate
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->updateSampleRate();
	else return 0.00;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateBlockSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateBlockSize
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->updateBlockSize();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getAutomationState
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getAutomationState
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getAutomationState();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getCurrentProcessLevel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getCurrentProcessLevel
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getCurrentProcessLevel();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getDirectory
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getDirectory
		(JNIEnv* env, jobject obj) {

	jstring ret = NULL;

#ifdef WIN32
	//TODO:
	//Mac hosts return a FSSpec* here!!! (instead of char*)
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) ret = env->NewStringUTF((char *)WrapperInstance->getDirectory());
#endif

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostProductString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostProductString
		(JNIEnv* env, jobject obj) {

	jstring ret = NULL;
	//text 	String of maximum 64 char
	char text[255]={'\0'};
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->getHostProductString(text);
	if (WrapperInstance!=NULL) ret = env->NewStringUTF(text);

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostVendorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostVendorVersion
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getHostVendorVersion();
	else return -1L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostVendorString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostVendorString
		(JNIEnv* env, jobject obj) {

	jstring ret = NULL;
	//text 	String of maximum 64 char
	char text[255]={'\0'};
	VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->getHostVendorString(text);
	if (WrapperInstance!=NULL) ret = env->NewStringUTF(text);

	return ret;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getHostLanguage
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getHostLanguage
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getHostLanguage();
	else return 0L;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getInputLatency
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getInputLatency
		(JNIEnv* env, jobject obj) {
   VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getInputLatency();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getOutputLatency
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getOutputLatency
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getOutputLatency();
	else return 0L;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getNumAutomatableParameters
 * Signature: ()I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getNumAutomatableParameters
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getNumAutomatableParameters();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    getParameterQuantization
 * Signature: ()I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_getParameterQuantization
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->getParameterQuantization();
	else return 0L;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    hasExternalBuffer
 * Signature: (Z)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_hasExternalBuffer
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->hasExternalBuffer(state != 0 ? true : false);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    isSynth
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_isSynth
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->isSynth(state != 0 ? true : false);
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    noTail
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_noTail
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->noTail(state != 0 ? true : false);
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    setOutputSamplerate
 * Signature: (F)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_setOutputSamplerate
		(JNIEnv* env, jobject obj, jfloat sr) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->setOutputSamplerate(sr);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    tempoAt
 * Signature: (I)I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_tempoAt
		(JNIEnv* env, jobject obj, jint pos) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->tempoAt(pos);
	else return 0L;
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    updateDisplay
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_updateDisplay
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->updateDisplay();
	else return false;
}

/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    wantAsyncOperation
 * Signature: (Z)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_wantAsyncOperation
		(JNIEnv* env, jobject obj, jboolean state) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->wantAsyncOperation(state != 0 ? true : false);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    wantEvents
 * Signature: (I)V
 */

//dm: deprecated as of vst2.4
JNIEXPORT void JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_wantEvents
		(JNIEnv* env, jobject obj, jint fil) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) WrapperInstance->wantEvents(fil);
}


/*
 * Class:     jvst_wrapper_communication_VSTV20ToHost
 * Method:    willProcessReplacing
 * Signature: ()I
 */

//dm: deprecated as of vst2.4
JNIEXPORT jint JNICALL Java_jvst_wrapper_communication_VSTV20ToHost_willProcessReplacing
		(JNIEnv* env, jobject obj) {
    VSTV24ToPlug* WrapperInstance=getWrapperInstance(env,obj);
	if (WrapperInstance!=NULL) return WrapperInstance->willProcessReplacing();
	else return 0L;
}








void InitTimeInfoCache(JNIEnv* env) {
	TimeInfoClass = env->FindClass("jvst/wrapper/valueobjects/VSTTimeInfo");
	if (TimeInfoClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTTimeInfo");
		return;
	}
	TimeInfoClass = (jclass) env->NewGlobalRef(TimeInfoClass);


	//Call JAVA Konstruktor
	jmethodID mid = env->GetMethodID(TimeInfoClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for TimeInfo!");
		return;
	}
	TimeInfoObject = env->NewObject(TimeInfoClass, mid);
	if (TimeInfoObject == NULL) {
		log("** ERROR: cannot create VST TimeInfo Object!");
		return;
	}
	//create global reference!
	TimeInfoObject = env->NewGlobalRef(TimeInfoObject);


	TimeInfoFieldSamplePos = env->GetFieldID(TimeInfoClass, "samplePos", "D");
	if (TimeInfoFieldSamplePos == NULL) {
		log("** ERROR: cannot find field-id samplePos (D)");
		return;
	}
	TimeInfoFieldSampleRate = env->GetFieldID(TimeInfoClass, "sampleRate", "D");
	if (TimeInfoFieldSampleRate == NULL) {
		log("** ERROR: cannot find field-id sampleRate (D)");
		return;
	}
	TimeInfoFieldNanoSeconds = env->GetFieldID(TimeInfoClass, "nanoSeconds", "D");
	if (TimeInfoFieldNanoSeconds == NULL) {
		log("** ERROR: cannot find field-id nanoSeconds (D)");
		return;
	}
	TimeInfoFieldPPQPos = env->GetFieldID(TimeInfoClass, "ppqPos", "D");
	if (TimeInfoFieldPPQPos == NULL) {
		log("** ERROR: cannot find field-id ppqPos (D)");
		return;
	}
	TimeInfoFieldTempo = env->GetFieldID(TimeInfoClass, "tempo", "D");
	if (TimeInfoFieldTempo == NULL) {
		log("** ERROR: cannot find field-id tempo (D)");
		return;
	}
	TimeInfoFieldBarStartPos = env->GetFieldID(TimeInfoClass, "barStartPos", "D");
	if (TimeInfoFieldBarStartPos == NULL) {
		log("** ERROR: cannot find field-id barStartPos (D)");
		return;
	}
	TimeInfoFieldCycleStartPos = env->GetFieldID(TimeInfoClass, "cycleStartPos", "D");
	if (TimeInfoFieldCycleStartPos == NULL) {
		log("** ERROR: cannot find field-id cycleStartPos (D)");
		return;
	}
	TimeInfoFieldCycleEndPos = env->GetFieldID(TimeInfoClass, "cycleEndPos", "D");
	if (TimeInfoFieldCycleEndPos == NULL) {
		log("** ERROR: cannot find field-id cycleEndPos (D)");
		return;
	}

	TimeInfoFieldTimeSigNumerator = env->GetFieldID(TimeInfoClass, "timeSigNumerator", "I");
	if (TimeInfoFieldTimeSigNumerator == NULL) {
		log("** ERROR: cannot find field-id timeSigNumerator (I)");
		return;
	}
	TimeInfoFieldTimeSigDenominator = env->GetFieldID(TimeInfoClass, "timeSigDenominator", "I");
	if (TimeInfoFieldTimeSigDenominator == NULL) {
		log("** ERROR: cannot find field-id timeSigDenominator (I)");
		return;
	}
	TimeInfoFieldSmpteOffset = env->GetFieldID(TimeInfoClass, "smpteOffset", "I");
	if (TimeInfoFieldSmpteOffset == NULL) {
		log("** ERROR: cannot find field-id smpteOffset (I)");
		return;
	}
	TimeInfoFieldSmpteFrameRate = env->GetFieldID(TimeInfoClass, "smpteFrameRate", "I");
	if (TimeInfoFieldSmpteFrameRate == NULL) {
		log("** ERROR: cannot find field-id smpteFrameRate (I)");
		return;
	}
	TimeInfoFieldSamplesToNextClock = env->GetFieldID(TimeInfoClass, "samplesToNextClock", "I");
	if (TimeInfoFieldSamplesToNextClock == NULL) {
		log("** ERROR: cannot find field-id samplesToNextClock (I)");
		return;
	}
	TimeInfoFieldFlags = env->GetFieldID(TimeInfoClass, "flags", "I");
	if (TimeInfoFieldFlags == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return;
	}

	IsTimeInfoCacheInitialised = true;
}


void InitEventsCache(JNIEnv* env) {
	VSTEventsClass = env->FindClass("jvst/wrapper/valueobjects/VSTEvents");
	if (VSTEventsClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvents");
		return;
	}
	VSTEventsClass = (jclass) env->NewGlobalRef(VSTEventsClass);

	VSTEventClass = env->FindClass("jvst/wrapper/valueobjects/VSTEvent");
	if (VSTEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTEvent");
		return;
	}
	VSTEventClass = (jclass) env->NewGlobalRef(VSTEventClass);

	MidiEventClass = env->FindClass("jvst/wrapper/valueobjects/VSTMidiEvent");
	if (MidiEventClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.VSTMidiEvent");
		return;
	}
	MidiEventClass = (jclass) env->NewGlobalRef(MidiEventClass);


	EventsFieldEvents = env->GetFieldID(VSTEventsClass, "events", "[Ljvst/wrapper/valueobjects/VSTEvent;");
	if (EventsFieldEvents == NULL) {
		log("** ERROR: cannot find field-id events ([Ljvst/wrapper/valueobjects/VSTEvent;)");
		return;
	}
	EventsFieldNumEvents = env->GetFieldID(VSTEventsClass, "numEvents", "I");
	if (EventsFieldNumEvents == NULL) {
		log("** ERROR: cannot find field-id numEvents");
		return;
	}
	EventsFieldReserved = env->GetFieldID(VSTEventsClass, "reserved", "I");
	if (EventsFieldReserved == NULL) {
		log("** ERROR: cannot find field-id reserved(I) (Events)");
		return;
	}


	EventFieldType = env->GetFieldID(VSTEventClass, "type", "I");
	if (EventFieldType == NULL) {
		log("** ERROR: cannot find field-id type(I) (Event)");
		return;
	}
	EventFieldByteSize = env->GetFieldID(VSTEventClass, "byteSize", "I");
	if (EventFieldByteSize == NULL) {
		log("** ERROR: cannot find field-id byteSize(I) (Event)");
		return;
	}
	EventFieldDeltaFrames = env->GetFieldID(VSTEventClass, "deltaFrames", "I");
	if (EventFieldDeltaFrames == NULL) {
		log("** ERROR: cannot find field-id deltaFrames(I) (Event)");
		return;
	}
	EventFieldFlags = env->GetFieldID(VSTEventClass, "flags", "I");
	if (EventFieldFlags == NULL) {
		log("** ERROR: cannot find field-id flags(I) (Event)");
		return;
	}
	EventFieldData = env->GetFieldID(VSTEventClass, "data", "[B");
	if (EventFieldData == NULL) {
		log("** ERROR: cannot find field-id data(I) (Event)");
		return;
	}
	MidiEventFieldNoteLength = env->GetFieldID(MidiEventClass, "noteLength", "I");
	if (MidiEventFieldNoteLength == NULL) {
		log("** ERROR: cannot find field-id noteLength(I) (MidiEvent)");
		return;
	}
	MidiEventFieldNoteOffset = env->GetFieldID(MidiEventClass, "noteOffset", "I");
	if (MidiEventFieldNoteOffset == NULL) {
		log("** ERROR: cannot find field-id noteOffset(I) (MidiEvent)");
		return;
	}
	MidiEventFieldDetune = env->GetFieldID(MidiEventClass, "detune", "B");
	if (MidiEventFieldDetune == NULL) {
		log("** ERROR: cannot find field-id detune(B) (MidiEvent)");
		return;
	}
	MidiEventFieldNoteOffVelocity = env->GetFieldID(MidiEventClass, "noteOffVelocity", "B");
	if (MidiEventFieldNoteOffVelocity == NULL) {
		log("** ERROR: cannot find field-id noteOffVelocity(B) (MidiEvent)");
		return;
	}
	MidiEventFieldReserved1 = env->GetFieldID(MidiEventClass, "reserved1", "B");
	if (MidiEventFieldReserved1 == NULL) {
		log("** ERROR: cannot find field-id reserved1(B) (MidiEvent)");
		return;
	}
	MidiEventFieldReserved2 = env->GetFieldID(MidiEventClass, "reserved2", "B");
	if (MidiEventFieldReserved2 == NULL) {
		log("** ERROR: cannot find field-id reserved2(B) (MidiEvent)");
		return;
	}

	IsEventsCacheInitialised = true;
}
