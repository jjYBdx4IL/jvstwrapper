#ifndef __JAVASTYLE_GLOBALS_H__
#define __JAVASTYLE_GLOBALS_H__

#include "jni.h"

#include "StringBuffer.h"

namespace jvst {

	extern jclass JVSTLibrary_class;

	extern jclass JVSTPlugin_class;
	extern jfieldID JVSTPlugin_libHandle;
	extern jfieldID JVSTPlugin_aEffectPtr;
	extern jfieldID JVSTPlugin_host;

	extern jclass JVSTHost_class;
	extern jmethodID JVSTHost_getCallback;

	extern jclass JVSTJavaHost_class;

	extern jclass NativePointer_class;
	extern jfieldID NativePointer_ptr;

	extern jclass NativeFloatArray_class;
	extern jfieldID NativeFloatArray_ptr;
	extern jfieldID NativeFloatArray_size;

	extern JavaVM * jvm;
	extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved); 
	extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved); 

};

#endif