#include "StringBuffer.h"

namespace java_lang {

	jclass class_StringBuffer;
	jmethodID StringBuffer_append__C;
	jmethodID StringBuffer_append__AC;
	jmethodID StringBuffer_delete_II;
	jmethodID StringBuffer_append__Ljava_lang_String;

	void jni_init_StringBuffer(JNIEnv * env) {

		class_StringBuffer = env->FindClass("java/lang/StringBuffer");
		if (class_StringBuffer == 0) {
			/* HANDLE THIS ERROR ... */
		}
		class_StringBuffer = (jclass)env->NewGlobalRef(class_StringBuffer);
		StringBuffer_append__C = env->GetMethodID(class_StringBuffer, "append", "(C)Ljava/lang/StringBuffer;");
		StringBuffer_append__AC = env->GetMethodID(class_StringBuffer, "append", "([C)Ljava/lang/StringBuffer;");
		StringBuffer_append__Ljava_lang_String = env->GetMethodID(class_StringBuffer, "append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");
		StringBuffer_delete_II = env->GetMethodID(class_StringBuffer, "delete", "(II)Ljava/lang/StringBuffer;");
	}
};