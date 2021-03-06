/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class org_tango_JaVaSTyle_JVSTPlugin */

#ifndef _Included_org_tango_JaVaSTyle_JVSTPlugin
#define _Included_org_tango_JaVaSTyle_JVSTPlugin
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    instanciate
 * Signature: (Lorg/tango/JaVaSTyle/JVSTHost;J)J
 */
JNIEXPORT jlong JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_instanciate
  (JNIEnv *, jobject, jobject, jlong);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    process
 * Signature: (Lorg/tango/jniTools/NativePointer;Lorg/tango/jniTools/NativePointer;I)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_process
  (JNIEnv *, jobject, jobject, jobject, jint);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    processReplacing
 * Signature: (Lorg/tango/jniTools/NativePointer;Lorg/tango/jniTools/NativePointer;I)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_processReplacing
  (JNIEnv *, jobject, jobject, jobject, jint);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setParameter
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setParameter
  (JNIEnv *, jobject, jint, jfloat);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameter
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameter
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameterDisplay
 * Signature: (ILjava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameterDisplay
  (JNIEnv *, jobject, jint, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameterName
 * Signature: (ILjava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameterName
  (JNIEnv *, jobject, jint, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getParameterLabel
 * Signature: (ILjava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getParameterLabel
  (JNIEnv *, jobject, jint, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getProgram
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getProgram
  (JNIEnv *, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setProgram
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setProgram
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setProgramName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setProgramName
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getProgramName
 * Signature: (Ljava/lang/StringBuffer;)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getProgramName
  (JNIEnv *, jobject, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    dispatcher
 * Signature: (IIILorg/tango/jniTools/NativePointer;F)I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_dispatcher
  (JNIEnv *, jobject, jint, jint, jint, jobject, jfloat);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    open
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_open
  (JNIEnv *, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_close
  (JNIEnv *, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getVu
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getVu
  (JNIEnv *, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    getChunk
 * Signature: (Lorg/tango/jniTools/NativePointer;Z)I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_getChunk
  (JNIEnv *, jobject, jobject, jboolean);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setChunk
 * Signature: (Lorg/tango/jniTools/NativePointer;IZ)I
 */
JNIEXPORT jint JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setChunk
  (JNIEnv *, jobject, jobject, jint, jboolean);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setSampleRate
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setSampleRate
  (JNIEnv *, jobject, jfloat);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    setBlockSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_setBlockSize
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    suspend
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_suspend
  (JNIEnv *, jobject);

/*
 * Class:     org_tango_JaVaSTyle_JVSTPlugin
 * Method:    resume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTPlugin_resume
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
