#include "org_tango_JaVaSTyle_JVSTLibrary.h"

#include "dynlib_support.h"
#include "globals.h"

#include <cstdlib>

using namespace jvst;


/*
 * Class:     org_tango_JaVaSTyle_JVSTLibrary
 * Method:    nativeLoadVST
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_tango_JaVaSTyle_JVSTLibrary_nativeLoadVST
	(JNIEnv * env, jobject This, jstring libName) {

  const char *str = env->GetStringUTFChars(libName, 0);
  jlong ret;

  #if WIN32
  HMODULE hModule = LoadLibrary((LPCWSTR)str);
  ret = (jlong)hModule;

  #elif POSIX
  void * hModule = dlopen(str, RTLD_NOW, RTLD_GLOBAL);
  if (!hModule) {
  	/* error ... */
  }
  ret = PTR_TO_JLONG(hModule);

  // TO DO : #elif MAC ... 

  #endif

  env->ReleaseStringUTFChars(libName, str);
  return ret;
}

/*
 * Class:     org_tango_JaVaSTyle_JVSTLibrary
 * Method:    nativeUnloadVST
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_org_tango_JaVaSTyle_JVSTLibrary_nativeUnloadVST
  (JNIEnv * env, jobject This, jlong libH) {
  
  #if defined(WIN32) || defined(_WIN32)
  return (jboolean)FreeLibrary((HMODULE)libH);
  
  #elif defined(POSIX)
  return (jboolean)!dlclose(JLONG_TO_PTR(libH));
  
  // TO DO : elif MAC ...
  #endif
  
}


