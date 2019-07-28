#ifndef __JNI_java_lang_StringBuffer_H__
#define __JNI_java_lang_StringBuffer_H__

#include "jni.h"

namespace java_lang {

	extern jclass class_StringBuffer;
	extern jmethodID StringBuffer_append__C;
	extern jmethodID StringBuffer_append__AC;
	extern jmethodID StringBuffer_append__Ljava_lang_String;
	extern jmethodID StringBuffer_delete_II;

	void jni_init_StringBuffer(JNIEnv * env);
};
#endif