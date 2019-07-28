#ifndef __JaVaSTyle_PLUGIN_LIBRARY_H__
#define __JaVaSTyle_PLUGIN_LIBRARY_H__

#include "jni.h"
#include "AEffect.h"

#include <map>

/** The JaVaSTyle plugin library is a global map from 
 * AEffect pointers to java plugin objects.
 */

namespace JVST {

	typedef struct {
		jobject plugin;
		jobject host;
	} jvst_plugin_info;

	extern std::map<AEffect*, jvst_plugin_info> jvstLibrary;
};

#endif