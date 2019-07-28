#include "org_tango_jniTools_NativeFloatArray.h"


jclass NativeFloatArray_class;
jfieldID NativeFloatArray_ptr;
jfieldID NativeFloatArray_size;

inline float * getPtr(JNIEnv * env, jobject This) {
	return (float*)env->GetLongField(This, NativeFloatArray_ptr);
}

inline int getSize(JNIEnv * env, jobject This) {
	return env->GetIntField(This, NativeFloatArray_size);
}

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    initJNI
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_tango_jniTools_NativeFloatArray_initJNI
  (JNIEnv * env, jclass cls) {
	NativeFloatArray_class = (jclass)env->NewGlobalRef(cls);
	NativeFloatArray_ptr = env->GetFieldID(NativeFloatArray_class, "ptr", "J");
	NativeFloatArray_size = env->GetFieldID(NativeFloatArray_class, "size", "I");
}


/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    get
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_org_tango_jniTools_NativeFloatArray_get
  (JNIEnv * env, jobject This, jint index) {
  	return getPtr(env, This)[index];
}

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyToJava
 * Signature: ([F)Z
 */
JNIEXPORT jboolean JNICALL Java_org_tango_jniTools_NativeFloatArray_copyToJava___3F
	(JNIEnv * env, jobject This, jfloatArray the_jArray) {
	env->SetFloatArrayRegion(
		the_jArray, 
		0, 
		getSize(env, This), 
		getPtr(env, This)
	);
	return true;
}

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyFromJava
 * Signature: ([F)Z
 */
JNIEXPORT jboolean JNICALL Java_org_tango_jniTools_NativeFloatArray_copyFromJava___3F
	(JNIEnv * env, jobject This, jfloatArray the_jArray) {
	env->GetFloatArrayRegion(
		the_jArray, 
		0, 
		getSize(env, This), 
		getPtr(env, This)
	);
	return true;
}

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyToJava
 * Signature: ([FIII)Z
 */
JNIEXPORT jboolean 
JNICALL Java_org_tango_jniTools_NativeFloatArray_copyToJava___3FIII(
	JNIEnv * env, 
	jobject This, 
	jfloatArray the_jArray, 
	jint startN, 
	jint startJ, 
	jint len) {
	env->SetFloatArrayRegion(
		the_jArray, 
		startJ, 
		len, 
		getPtr(env, This)+startN
	);
	return true;
}

/*
 * Class:     org_tango_jniTools_NativeFloatArray
 * Method:    copyFromJava
 * Signature: ([FIII)Z
 */
JNIEXPORT jboolean JNICALL 
Java_org_tango_jniTools_NativeFloatArray_copyFromJava___3FIII(
	JNIEnv * env, 
	jobject This, 
	jfloatArray the_jArray, 
	jint startN, 
	jint startJ, 
	jint len) {
	env->GetFloatArrayRegion(
		the_jArray, 
		startJ, 
		len, 
		getPtr(env, This)+startN
	);
	return true;
}

