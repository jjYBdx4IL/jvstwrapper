#include "globals.h"
#include <iostream>

namespace jvst {

	JavaVM * jvm;

	jclass JVSTLibrary_class;

	jclass JVSTPlugin_class;
	jfieldID JVSTPlugin_libHandle;
	jfieldID JVSTPlugin_aEffectPtr;
	jfieldID JVSTPlugin_host;

	jclass JVSTHost_class;
	jmethodID JVSTHost_getCallback;

	jclass JVSTJavaHost_class;

	jclass NativePointer_class;
	jfieldID NativePointer_ptr;

	jclass NativeFloatArray_class;
	jfieldID NativeFloatArray_ptr;
	jfieldID NativeFloatArray_size;


	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

		std::cout << "Loaded JVSTLib.dll ..." << std::endl;

		jvm = vm;

		JNIEnv * env;
		vm->AttachCurrentThread((void**)&env, NULL);

		jclass JVSTPlugin_cls = env->FindClass("org/tango/JaVaSTyle/JVSTPlugin");
		if (JVSTPlugin_cls == 0) {
			/* HANDLE THIS ERROR ... */
		}
		JVSTPlugin_class = (jclass)env->NewGlobalRef(JVSTPlugin_cls);
		JVSTPlugin_libHandle = env->GetFieldID(JVSTPlugin_class, "libHandle", "J");
		JVSTPlugin_aEffectPtr = env->GetFieldID(JVSTPlugin_class, "aEffectPtr", "J");
		JVSTPlugin_host = env->GetFieldID(JVSTPlugin_class, "host", "Lorg/tango/JaVaSTyle/JVSTHost;");
/*		  
		jclass JVSTHost_cls = env->FindClass(
  			"org/tango/jniTools/JVSTHost"
		);
		if (JVSTHost_cls == 0) {
  			// HANDLE THIS ERROR ...
		}
		JVSTHost_class = (jclass)env->NewGlobalRef(JVSTHost_cls);
		JVSTHost_getCallback = env->GetMethodID(JVSTHost_class, "getCallback", "()J");
*/
		jclass JVSTJavaHost_cls = env->FindClass("org/tango/JaVaSTyle/JVSTJavaHost");
		if (JVSTJavaHost_cls == 0) {
			/* HANDLE THIS ERROR ... */
		}
		JVSTJavaHost_class = (jclass)env->NewGlobalRef(JVSTJavaHost_cls);
		JVSTHost_class = JVSTJavaHost_class;
		JVSTHost_getCallback = env->GetMethodID(JVSTHost_class, "getCallback", "()J");

		jclass NativePointer_cls = env->FindClass(
  			"org/tango/jniTools/NativePointer"
		);
		if (NativePointer_cls == 0) {
  			/* HANDLE THIS ERROR ... */
		}
		NativePointer_class = (jclass)env->NewGlobalRef(NativePointer_cls);
		NativePointer_ptr = env->GetFieldID(NativePointer_cls, "ptr", "J");


		jclass JVSTLibrary_cls = env->FindClass("org/tango/JaVaSTyle/JVSTLibrary");
		if (JVSTLibrary_cls == 0) {
  			/* HANDLE THIS ERROR ... */
		}
		JVSTLibrary_class = (jclass)env->NewGlobalRef(JVSTLibrary_cls);

		jclass NativeFloatArray_cls = env->FindClass("org/tango/jniTools/NativeFloatArray");
		if (NativeFloatArray_cls == 0) {
			/* HANDLE THIS ERROR ... */
		}
		NativeFloatArray_class = (jclass)env->NewGlobalRef(NativeFloatArray_cls);
		NativeFloatArray_ptr = env->GetFieldID(NativeFloatArray_class, "ptr", "J");
		NativeFloatArray_size = env->GetFieldID(NativeFloatArray_class, "size", "I");

		java_lang::jni_init_StringBuffer(env);

		return JNI_VERSION_1_2;
	}

	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
	}
};
