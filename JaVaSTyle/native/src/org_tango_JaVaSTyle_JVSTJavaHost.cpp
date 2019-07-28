#include "org_tango_JaVaSTyle_JVSTJavaHost.h"
#include "AEffect.h"

#include "PluginLibrary.h"
#include "globals.h"
#include "dynlib_support.h"

using namespace jvst;

char* callbackMethodSignatures[][2] = {
	{ "setParameterAutomated", "(Lorg/tango/JaVaSTyle/JVSTPlugin;II)V" },
	{ "getMasterVersion", "(Lorg/tango/JaVaSTyle/JVSTPlugin;)I" },
	{ "getCurrentUniqueId", "(Lorg/tango/JaVaSTyle/JVSTPlugin;)I" },
	{ "masterIdle", "(Lorg/tango/JaVaSTyle/JVSTPlugin;)V" },
	{ "isInputConnected", "(Lorg/tango/JaVaSTyle/JVSTPlugin;I)Z" }
};

const int numCallbacks = 
	sizeof(callbackMethodSignatures)/sizeof(*(callbackMethodSignatures));

long VSTCALLBACK masterCallback (AEffect *effect, long opcode, long index,
long value, void *ptr, float opt);

jmethodID callbackMethodIDs[numCallbacks];

/*
 * Class:     org_tango_JaVaSTyle_JVSTJavaHost
 * Method:    initJNI
 * Signature: ()J
 */
JNIEXPORT void JNICALL Java_org_tango_JaVaSTyle_JVSTJavaHost_initJNI
  (JNIEnv * env, jclass cls) {

	for (int i=0; i<numCallbacks; i++) {
  		callbackMethodIDs[i] = env->GetMethodID(
   			cls, 
     		callbackMethodSignatures[i][0],
     		callbackMethodSignatures[i][1]
  		);
	}
}


/*
 * Class:     org_tango_JaVaSTyle_JVSTJavaHost
 * Method:    declareHost
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_tango_JaVaSTyle_JVSTJavaHost_declareHost
	(JNIEnv *, jobject) {
	return PTR_TO_JLONG(masterCallback);
}

long VSTCALLBACK masterCallback (AEffect *effect, long opcode, long index,
	long value, void *ptr, float opt) {
/*	FROM AEffect.h :
	----------------
	audioMasterAutomate = 0,		// index, value, returns 0
	audioMasterVersion,				// VST Version supported (for example 2200 for VST 2.2)
	audioMasterCurrentId,			// Returns the unique id of a plug that's currently
									// loading
	audioMasterIdle,				// Call application idle routine (this will
									// call effEditIdle for all open editors too) 
	audioMasterPinConnected			// Inquire if an input or output is beeing connected;

*/

	JNIEnv *env;
	jvst::jvm->AttachCurrentThread((void**)&env, NULL);

	jobject thePlugin = JVST::jvstLibrary[effect].plugin; // TEMP !! check for presence of element
	jobject theHost = JVST::jvstLibrary[effect].host; // TEMP !! check for presence of element
	
	switch (opcode) {
		case audioMasterAutomate:
			env->CallIntMethod(
				theHost, 
				callbackMethodIDs[opcode],
				thePlugin,
				(jint)index,
				(jint)value);
			return (long)0;
		case audioMasterVersion:
			return (long)env->CallIntMethod(
				theHost,
				callbackMethodIDs[opcode],
				thePlugin);
		case audioMasterCurrentId:
			return (long)env->CallIntMethod(
				theHost,
				callbackMethodIDs[opcode],
				thePlugin);
		case audioMasterIdle:
			env->CallVoidMethod(
				theHost,
				callbackMethodIDs[opcode],
				thePlugin);
			return (long)0;
		case audioMasterPinConnected:
			return (long)env->CallBooleanMethod(
					theHost,
					callbackMethodIDs[opcode],
					thePlugin,
					(jint)index
				);
		default:
			return 0;
	}
}

