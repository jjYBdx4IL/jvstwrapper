
#include "org_tango_JaVaSTyle_JVSTPlugin.h"
#include "AEffect.h"

#if BEOS
#	define VST_PLUG_MAIN "main_plugin"
#elif MACX
#	define VST_PLUG_MAIN "main_macho"
#else
#	define VST_PLUG_MAIN "main"
#endif

typedef AEffect* (*vstMainCall) (audioMasterCallback audioMaster);

#include "dynlib_support.h"
#include "PluginLibrary.h"
#include "globals.h"

using namespace jvst;


/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    instanciate
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_instanciate
  (JNIEnv * env, jobject This, jobject host, jlong libHandle) {

	vstMainCall vstMain;

	#if WIN32
	vstMain = (vstMainCall)GetProcAddress((HMODULE)libHandle, VST_PLUG_MAIN);

	#elif POSIX
	vstMain = dlsym(JLONG_TO_PTR(libHandle), VST_PLUG_MAIN);

	#endif

	audioMasterCallback callback = (audioMasterCallback)
	JLONG_TO_PTR(env->CallLongMethod(host, JVSTHost_getCallback));

	/* Register the null host as this one ... */
	JVST::jvstLibrary[NULL].plugin = env->NewGlobalRef(This);
	JVST::jvstLibrary[NULL].host = env->NewGlobalRef(host);

	/* Retrieve the pointer to plugin */
	AEffect * the_plugin = vstMain(callback);

	/* Register it into the library */
	JVST::jvstLibrary[the_plugin].plugin = env->NewGlobalRef(This);
	JVST::jvstLibrary[the_plugin].host = env->NewGlobalRef(host);

	return PTR_TO_JLONG(the_plugin);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    process
 * Signature: (Lorg/tango/jniTools/NativePointer;Lorg/tango/jniTools/NativePointer;I)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_process
(JNIEnv * env, jobject This, jobject in, jobject out, jint sampleframes) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	float ** input_buf = (float**)JLONG_TO_PTR(env->GetLongField(in, NativePointer_ptr));
	float ** output_buf = (float**)JLONG_TO_PTR(env->GetLongField(out, NativePointer_ptr));
	aEff->process(aEff, input_buf, output_buf, sampleframes);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    processReplacing
 * Signature: (Lorg/tango/jniTools/NativePointer;Lorg/tango/jniTools/NativePointer;I)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_processReplacing
(JNIEnv * env, jobject This, jobject in, jobject out, jint sampleframes) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	float ** input_buf = (float**)JLONG_TO_PTR(env->GetLongField(in, NativePointer_ptr));
	float ** output_buf = (float**)JLONG_TO_PTR(env->GetLongField(out, NativePointer_ptr));
	aEff->processReplacing(aEff, input_buf, output_buf, sampleframes);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setParameter
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setParameter
(JNIEnv * env, jobject This, jint index, jfloat value) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->setParameter(aEff, index, value);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameter
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameter
(JNIEnv * env, jobject This, jint index) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	return aEff->getParameter(aEff, index);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameterDisplay
 * Signature: (ILjava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameterDisplay
(JNIEnv * env, jobject This, jint index, jobject strBuf) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	char text[256];
	aEff->dispatcher(aEff, effGetParamDisplay, index, 0, (void*)text, 0);
	jstring jstr = env->NewStringUTF(text);
	env->CallObjectMethod(strBuf, java_lang::StringBuffer_append__Ljava_lang_String, jstr);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameterName
 * Signature: (ILjava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameterName
(JNIEnv * env, jobject This, jint index, jobject strBuf) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	char text[256];
	aEff->dispatcher(aEff, effGetParamName, index, 0, (void*)text, 0);
	jstring jstr = env->NewStringUTF(text);
	env->CallObjectMethod(strBuf, java_lang::StringBuffer_append__Ljava_lang_String, jstr);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameterLabel
 * Signature: (ILjava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameterLabel
(JNIEnv * env, jobject This, jint index, jobject strBuf) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	char text[256];
	aEff->dispatcher(aEff, effGetParamLabel, index, 0, (void*)text, 0);
	jstring jstr = env->NewStringUTF(text);
	env->CallObjectMethod(strBuf, java_lang::StringBuffer_append__Ljava_lang_String, jstr);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getProgram
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getProgram
(JNIEnv * env, jobject This) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	return (jint)aEff->dispatcher(aEff, effGetProgram, 0, 0, NULL, 0);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setProgram
 * Signature: ()I
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setProgram
(JNIEnv * env, jobject This, jint prog) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->dispatcher(aEff, effSetProgram, 0, prog, NULL, 0);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setProgramName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setProgramName
(JNIEnv * env, jobject This, jstring name) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
    char buf[128]; /* VST program name allowed up to 24 chars */
    const char *str = env->GetStringUTFChars(name, 0);
	strcpy(buf, str);
	aEff->dispatcher(aEff, effSetProgramName, 0, 0, (void*)buf, 0);
    env->ReleaseStringUTFChars(name, str);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getProgramName
 * Signature: (Ljava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getProgramName
(JNIEnv * env, jobject This, jobject strBuf) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	char text[128]; /* VST program name allowed up to 24 chars */
	aEff->dispatcher(aEff, effGetProgramName, 0, 0, (void*)text, 0);
	jstring jstr = env->NewStringUTF(text);
	env->CallObjectMethod(strBuf, java_lang::StringBuffer_append__Ljava_lang_String, jstr);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    dispatcher
 * Signature: (IIILorg/tango/jniTools/NativePointer;F)I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_dispatcher
(JNIEnv *env, jobject This, jint opcode, jint index, jint value, jobject ptr, jfloat opt) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	return aEff->dispatcher(
		aEff, 
		opcode, 
		index, 
		value, 
		JLONG_TO_PTR(env->GetLongField(ptr, NativePointer_ptr)), 
		opt
	);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    open
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_open
(JNIEnv * env, jobject This) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->dispatcher(aEff, effOpen, 0, 0, NULL, 0);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_close
(JNIEnv * env, jobject This) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->dispatcher(aEff, effClose, 0, 0, NULL, 0);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getVu
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getVu
(JNIEnv * env, jobject This) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	return (jfloat)aEff->dispatcher(aEff, effGetVu, 0, 0, NULL, 0);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getChunk
 * Signature: (Lorg/tango/jniTools/NativePointer;Z)I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getChunk
(JNIEnv * env, jobject This, jobject ptr_data, jboolean isPreset) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	void * chunk;
	long res = aEff->dispatcher(aEff, effGetChunk, isPreset, 0, &chunk, 0);
	env->SetLongField(ptr_data, NativePointer_ptr, PTR_TO_JLONG(chunk));
	return res;
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setChunk
 * Signature: (Lorg/tango/jniTools/NativePointer;IZ)I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setChunk
(JNIEnv * env, jobject This, jobject ptr_data, jint numBytes, jboolean isPreset) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	return aEff->dispatcher(
		aEff, 
		effSetChunk, 
		isPreset, 
		numBytes,
		JLONG_TO_PTR(env->GetLongField(ptr_data, NativePointer_ptr)),
		0
	);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setSampleRate
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setSampleRate
(JNIEnv * env, jobject This, jfloat rate) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->dispatcher(aEff, effSetSampleRate, 0, 0, NULL, rate);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setBlockSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setBlockSize
(JNIEnv * env, jobject This, jint blockSize) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->dispatcher(aEff, effSetBlockSize, 0, blockSize, NULL, 0);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    suspend
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_suspend
(JNIEnv * env, jobject This) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->dispatcher(aEff, effMainsChanged, 0, (long)false, NULL, 0);
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    resume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_resume
(JNIEnv * env, jobject This) {

	AEffect * aEff = (AEffect*)JLONG_TO_PTR(env->GetLongField(This, JVSTPlugin_aEffectPtr));
	aEff->dispatcher(aEff, effMainsChanged, 0, (long)true, NULL, 0);
}
