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
// Definitions for the Java VSTi Wrapper class
//-
// 2003,2004 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#ifndef __VSTV10ToPlug__
#define __VSTV10ToPlug__

#include "public.sdk/vst2.x/audioeffectx.h"
#include "VSTGUIWrapper.h"

#if defined(MACX) || defined(linux)
	#include <pthread.h>
#endif

#include <jni.h>


//------------------------------------------------------------------------
class VSTV10ToPlug : public AudioEffectX {
	friend class VSTGUIWrapper;

	public:
		VSTV10ToPlug (audioMasterCallback audioMaster, int prg, int prm, JavaVM *jvm);
		virtual ~VSTV10ToPlug ();


		//HOST -> PLUG


		//vst 1.0 REQUIRED
		//*****************
		virtual void setParameter (VstInt32 index, float value);
		virtual float getParameter (VstInt32 index);
		
		virtual void process (float **inputs, float **outputs, VstInt32 sampleframes);
		virtual void processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames);
		
		virtual VstInt32 getProgram();
		virtual void setProgram (VstInt32 program);
		virtual void setProgramName (char* name);
		virtual void getProgramName (char* name);

		virtual void getParameterLabel (VstInt32 index, char* label);
		virtual void getParameterDisplay (VstInt32 index, char* text);
		virtual void getParameterName (VstInt32 index, char* text);


		//vst 1.0 OPTIONAL
		//*****************
		virtual void resume ();	// Called when Plugin is switched to On
		virtual void suspend ();// Called when Plugin is switched to Off
		virtual void open ();
		virtual void close ();

		virtual float getVu();
		virtual VstInt32 getChunk (void** data, bool isPreset = false);
		virtual VstInt32 setChunk (void* data, VstInt32 byteSize, bool isPreset = false);
		virtual void setBlockSize (VstInt32 blockSize);
		virtual void setSampleRate(float sampleRate);

		
		//UTILITY
		int initJavaSide(jclass plug);
		void setNumParams(VstInt32 num);
#ifdef WIN32
		DWORD ToHostThread;
#endif
#if defined(MACX) || defined(linux)
		pthread_t ToHostThread;
#endif

	protected:
		JNIEnv* ensureJavaThreadAttachment();
		bool checkException(JNIEnv* env);

		//JNIEnv *JEnv;
		JavaVM *Jvm;
		jobject JavaPlugObj;
		jclass JavaPlugClass;
		jclass JavaFloatClass;

	private:
		jmethodID ProcessMethodID;
		jmethodID ProcessReplacingMethodID;
		bool isProcessing;

		//cached methods ids;
		jmethodID GetParameterMethod;
		jmethodID SetParameterMethod;

		//cached pointer to chunk-data
		void* chunkdata;
		long chunksize;

		//cached array for process()
		jobjectArray  ProcessJInputs;
		jobjectArray  ProcessJOutputs;
		jfloatArray ProcessInArrays[8];//max channels=8
		jfloatArray ProcessOutArrays[8];
		VstInt32 ProcessLastSampleFrames;

		jobjectArray  ProcessReplacingJInputs;
		jobjectArray  ProcessReplacingJOutputs;
		jfloatArray ProcessReplacingInArrays[8];//max channels=8
		jfloatArray ProcessReplacingOutArrays[8];
		VstInt32 ProcessReplacingLastSampleFrames;
};

#endif
