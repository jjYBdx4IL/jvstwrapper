/* 
 * jVSTwRapper - The Java way into VST world!
 * 
 * jVSTwRapper is an easy and reliable Java Wrapper for the Steinberg VST interface. 
 * It enables you to develop VST 2.3 compatible audio plugins and virtual instruments 
 * plus user interfaces with the Java Programming Language. 3 Demo Plugins(+src) are included!
 * 
 * Copyright (C) 2006  Daniel Martin [daniel309@users.sourceforge.net] 
 * 					   and many others, see CREDITS.txt
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

//-------------------------------------------------------------------------------------------------------
// VSTi Java Wrapper
//-
// Dll WinAPI entry
// VSTi API entry
//-
// 2003,2004,2007,2008,2011 Daniel Martin, Gerard Roma
//-------------------------------------------------------------------------------------------------------

#include "jVSTwRapperMAIN.h"
#include "JNIUtils.h"
#include "VSTV24ToPlug.h"
#include "ConfigFileReader.h"

#include <time.h>

// *** VERSION *** //
#define JVSTWRAPPER_VERSION "1.0beta2"


#ifdef WIN32
	#include <windows.h>
#endif

#ifdef MACX
	#include <Carbon/Carbon.h>
	#include <sys/stat.h>
	#include <sys/resource.h>
	#include <pthread.h>
	#include <unistd.h>

	extern "C" {
		#include <mach-o/dyld.h>
		#include <mach-o/ldsyms.h>
	}
	#include <CoreFoundation/CFBundle.h>
	#include <CoreFoundation/CoreFoundation.h>
	
	//cocoa init stuff
	#include "VSTGUIWrapperMAC.h"
#endif

#ifdef linux
	#include <dlfcn.h>
#endif

#if defined(WIN32) || defined(linux)
	extern jint (JNICALL *PTR_CreateJavaVM)(JavaVM **, void **, void *); 
	extern jint (JNICALL *PTR_GetCreatedJavaVMs)(JavaVM **, jsize, jsize *);
#endif


//------------------------------------------------------------------------
extern int IsLogEnabled;

//------------------------------------------------------------------------
VSTV24ToPlug* WrapperInstance = NULL;  //TODO: this var should be passed around as argument rather than hang around as global var
									   // might cause problems with concurrent starts of the wrapper!
JavaVM *GlobalJVM = NULL;

//------------------------------------------------------------------------
void calculatePaths();
int loadPlugin();

#ifdef MACX
	CFTypeRef runLoop = NULL;
	pthread_t JavaVMThreadID = NULL;

	void sourceCallBack(void *info);
	int loadCFApp();
	int startJavaThread();

	void* startJava(void *nix); 

	int checkJVMVersionRequest(char* requestedJVMVersion);
#endif

#if defined(WIN32) || defined(linux)
	int startJava(void *nix);
#endif

//------------------------------------------------------------------------
char DllLocation[JVST_PATH_MAX];
char DllPath[JVST_PATH_MAX];

bool IsLADSPALoaded = false;
char ConfigFileName[JVST_FILE_MAX];
char LogFileName[JVST_FILE_MAX];
audioMasterCallback audioMaster = NULL;
FILE *log_stream = NULL;



//main entry points for different platforms
extern "C" {
	//vst2.4 generic entry point
	VST_EXPORT AEffect* VSTPluginMain (audioMasterCallback pAudioMaster) {return jvst_main(pAudioMaster);}
	
	//alternative entry points
	#ifdef WIN32
		VST_EXPORT AEffect *MAIN (audioMasterCallback pAudioMaster) {return jvst_main(pAudioMaster);}
	#endif
	#ifdef MACX
		VST_EXPORT AEffect *main_macho (audioMasterCallback pAudioMaster) {return jvst_main(pAudioMaster);}
	#endif
	#ifdef linux
		VST_EXPORT AEffect* main_plugin (audioMasterCallback pAudioMaster) asm ("main");
		#define main main_plugin
	
		VST_EXPORT AEffect *main (audioMasterCallback pAudioMaster) {return jvst_main(pAudioMaster);}
	#endif
} // extern "C"


//-----------------------------------------------------------------------
//main entry point! --> The real thing
//-----------------------------------------------------------------------
AEffect* jvst_main(audioMasterCallback pAudioMaster) {
	log("\n***** Bootstrapping jVSTwRapper v%s *****", JVSTWRAPPER_VERSION);
	calculatePaths();
	
	audioMaster=pAudioMaster;

	ConfigFileReader *cfg = new ConfigFileReader();
	IsLogEnabled = cfg->IsLoggingEnabled;

	char log_location[JVST_PATH_MAX];
	vst_strncpy(log_location, DllPath, JVST_PATH_MAX-1);
	vst_strncat(log_location, LogFileName, JVST_FILE_MAX-1);
	log("log_location=%s", log_location);

	//creating log stream...
	log_stream = fopen(log_location, "a");
	if (log_stream==NULL) log("** ERROR: cant create log stream at '%s'", log_location);

	log("\n\n***** jVSTwRapper v%s *****", JVSTWRAPPER_VERSION);

	//print the current time/date
	time_t now = time(NULL);
	log("---- %s", ctime(&now));

	// Get VST Version
	if (!pAudioMaster(0, audioMasterVersion, 0, 0, 0, 0)) return 0;  // old version
	log("Get VST Version OK!");

	WrapperInstance = NULL;

#ifdef WIN32
	//see if there is a custom JVM we want to load (from the .ini file)!
	char* jvmLibLocation;
	jvmLibLocation = cfg->CustomJVMLocation;
	
	if (jvmLibLocation==NULL) {
		//no custom jvm specified, see if there is a request for a specific version in the .ini
		
		if (cfg->CustomJVMRegistryKey!=NULL) {
			//see if we can read the jvm location from a custom reg key
			jvmLibLocation = readJVMLibLocation(NULL, cfg->CustomJVMRegistryKey);
			log("Custom Registry Key for JVM location query returned=%s", jvmLibLocation);
		}		
		else if (cfg->RequestedJVMVersion!=NULL) {
			log("Looking for a JVM version=%s", cfg->RequestedJVMVersion);
			jvmLibLocation = readJVMLibLocation(cfg->RequestedJVMVersion, NULL);

			if (jvmLibLocation!=NULL) log("desired JVM version found in registry at %s", jvmLibLocation);
			else log("Could not find desired JVM version, seems not to be installed. Falling back to latest JVM installed on this system.");
		}
		
		//FALLBACK to default JVM on System
		if (jvmLibLocation==NULL) {
			log("querying registry for location of DEFAULT jvm.dll");
			//no custom JVM configured in the .ini. Look in the windows registry
			//get jvm location from registry and load jvm interface pointers
			jvmLibLocation = readJVMLibLocation(NULL, NULL);

			if (jvmLibLocation==NULL) {
				log("* WARNING: Could not find jvm.dll location in registry! \n using the one from the PATH environment variable!");
				//try loading jvm.dll from PATH
				jvmLibLocation = "jvm.dll";
			}
			else log("found jvm.lib in registry at %s", jvmLibLocation);
		}
	}
	else {
		jvmLibLocation = replace(jvmLibLocation, "{WrapperPath}", DllPath);
		log("Loading custom JVM !!! at %s", jvmLibLocation);
	}
	
	
	if (initJVMFunctionPointers(jvmLibLocation) != 0) {
		log("** ERROR: cant init jvm interface pointers!");
		return NULL;
	}
	
	//start the jvm
	if (startJava(NULL) != 0)  {
		log("**ERROR in startJava()");
		return NULL;
	}
#endif

#ifdef linux
	//see if there is a custom JVM we want to load (from the .ini file)!
	char* jvmLibLocation;
	jvmLibLocation = cfg->CustomJVMLocation;
	
	if (jvmLibLocation==NULL) {
		log("Checking if there is a JAVA_HOME defined");
		
		jvmLibLocation = readJVMLibLocation(NULL);

		if (jvmLibLocation==NULL) {
			log("* WARNING: $JAVA_HOME not defined! Unable to load from $JAVA_HOME/jre/lib/i386/client/libjvm.so!\n Trying to load libjvm.so from $LD_LIBRARY_PATH!");
			//try to load jvm from LD_LIBRARY_PATH
			jvmLibLocation = "libjvm.so";
		}
		else log("found JAVA_HOME, libjvm.so=%s", jvmLibLocation);
	}
	else {
		jvmLibLocation = replace(jvmLibLocation, "{WrapperPath}", DllPath);
		log("Loading custom JVM !!! at %s", jvmLibLocation);
	}
	
	
	if (initJVMFunctionPointers(jvmLibLocation) != 0) {
		log("** ERROR: cant init jvm interface pointers!");
		return NULL;
	}
	
	//start the jvm
	if (startJava(NULL) != 0)  {
		log("**ERROR in startJava()");
		return NULL;
	}
#endif

#ifdef MACX
	//mac doesnt need all this custom jvm loading mess
	//it comes with a preinstalled jvm ;-), but we need to start he jvm in a separate thread
	
	//and we have the feature of requesting a specific jvm in the .ini file
	if (cfg->RequestedJVMVersion!=NULL) {
		int ret = -1;
		ret = checkJVMVersionRequest(cfg->RequestedJVMVersion);
		
		//make sure that we use any jvm
		if (ret!=0) {
			log("Warning: Could NOT find requested JVM version from .ini, using default JVM instead");
			unsetenv("JAVA_JVM_VERSION");
		} 
	}
	
	if (startJavaThread() != 0) {
		log("**ERROR in startJavaThread()");
		return NULL;
	}
#endif

	//load plugin
	log("starting to load the java plugin");
	if (loadPlugin() != 0) {
		log("** ERROR loading java plugin, see log for details");
		return NULL;
	}

	if (cfg) delete cfg;
	log("ALLES OK!"); 
	return WrapperInstance->getAeffect();
}

//------------------------------------------------------------------------
#if defined(WIN32) || defined(linux) 
int startJava(void *nix) { 
#endif
#ifdef MACX
void* startJava(void *nix) {
#endif
	//Create JVM
	//**********************************************
	JNIEnv *env;
	JavaVM *jvm;
	jint res;
	int result;
	JavaVMInitArgs vm_args;
	JavaVMOption options[6]; //assume the max number of options...
	char java_path[JVST_PATH_MAX]; //we need to add jVSTsYstem_bin.jar to the ClassPath of the Bootstrap ClassLoader!

	
	result = -1;
	
	ConfigFileReader *cfg = new ConfigFileReader();

	strcpy(java_path, "-Djava.class.path=");
	vst_strncat(java_path, replace(cfg->SystemClassPath, "{WrapperPath}", DllPath), JVST_PATH_MAX - strlen("-Djava.class.path=") - 2);

	log("SystemPath=%s", java_path);
	options[0].optionString = java_path;
	
	//If the mac still causes problems even when not using a java swing gui
	//try running the jvm in "headless" mode (so that cocoa doesnt even get initialised)
	//-Djava.awt.headless=true
	
	//apply the options from the .ini file
	int optionNum = 0;	
	if (cfg->JVMOption1!=NULL) {
		optionNum++;
		log("Using additional JVM option=%s", cfg->JVMOption1);
		options[optionNum].optionString=cfg->JVMOption1;
	}
	if (cfg->JVMOption2!=NULL) {
		optionNum++;
		log("Using additional JVM option=%s", cfg->JVMOption2);
		options[optionNum].optionString=cfg->JVMOption2;
	}
	if (cfg->JVMOption3!=NULL) {
		optionNum++;
		log("Using additional JVM option=%s", cfg->JVMOption3);
		options[optionNum].optionString=cfg->JVMOption3;
	}
	if (cfg->JVMOption4!=NULL) {
		optionNum++;
		log("Using additional JVM option=%s", cfg->JVMOption4);
		options[optionNum].optionString=cfg->JVMOption4;
	}
	if (cfg->JVMOption5!=NULL) {
		optionNum++;
		log("Using additional JVM option=%s", cfg->JVMOption5);
		options[optionNum].optionString=cfg->JVMOption5;
	}

	optionNum++;

#if defined(WIN32) || defined(linux)
	vm_args.version = JNI_VERSION_1_2;
#endif
#ifdef MACX
	vm_args.version = JNI_VERSION_1_4; //we want at least 1.4 on the mac!
#endif
	vm_args.options = options;
	vm_args.nOptions = optionNum;
	vm_args.ignoreUnrecognized = JNI_FALSE; //use -X options as well!

	//erst schauen ob schon ne jvm aktiv ist:
	JavaVM *vmBuffer;
	jsize nVMs;


#if defined(WIN32) || defined(linux)
	res = PTR_GetCreatedJavaVMs(&vmBuffer, 1, &nVMs);
#endif
#ifdef MACX
	res = JNI_GetCreatedJavaVMs(&vmBuffer, 1, &nVMs);
#endif
	if (res < 0) {
		log("** ERROR: Can't get created Java VMs");
		goto leave;
	}

	if (nVMs>0) {
		log("before reusing JavaVM");
		jvm = &vmBuffer[nVMs-1];
#if defined(WIN32) || defined(linux)
		res = jvm->GetEnv((void**)&env, JNI_VERSION_1_2);
#endif
#ifdef MACX
		res = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
#endif		
		log("after reusing JavaVM");
		
		log("* Warning: Since we are reusing a Java VM which is already in memory, \n\
			all VM startup options will be ignored! You need to restart your audio app in order to make them effective!");

		if (res==JNI_EDETACHED) { 
			log("attaching current thread to jvm!");
			res = jvm->AttachCurrentThread((void**)&env, NULL);
			if (res<0) log ("** ERROR: Attaching JVM to native thread!");
		}
		
		if (res < 0) {
			log("** ERROR: getting Java env!");
			if (res==JNI_EVERSION) log("GetEnv Error because of different JNI Version!");
			goto leave;
		}
	} 
	else {
		/* Create the Java VM */
		
		log("Before JNI_CreateJavaVM");
#if defined(WIN32) || defined(linux)
		res = PTR_CreateJavaVM(&jvm, (void**)&env, &vm_args);
#endif
#ifdef MACX
		JavaVMThreadID = pthread_self();
		log("creating Java VM in thread=%i", JavaVMThreadID);
		res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
#endif		
		log("AFTER JNI_CreateJavaVM");
		
		if (res < 0) {
			log("** ERROR: Can't create Java VM (are your VM options in the .ini file correct?)");
			goto leave;
		}
		if (checkException(env)) goto leave;
	}
	
	if (checkException(env)) goto leave;


	GlobalJVM = jvm;
	delete cfg;
	result = 0; //everythign is fine when we came to this point

leave:
#ifdef MACX
	return NULL; //NULL //man, this makes the whole result var unecessary...
#endif
#if defined(WIN32) || defined(linux)
	return result;
#endif
}


int loadPlugin() { 
	int result = -1;
	bool hasGUI = false;
	jclass guiRunnerClass = NULL;
	ConfigFileReader *cfg = new ConfigFileReader();
	char class_path[JVST_PATH_MAX];

	//try to get an jni env from the loaded jvm
	//on the mac, we might not be attached to the thread (the jvm was started)
	//in another one. 
	JNIEnv *env = ensureJavaThreadAttachment(GlobalJVM);

	log("DllPath (implicitly added to the classpath)=%s", DllPath);	
	vst_strncpy(class_path, DllPath, JVST_PATH_MAX-1);
	
#if defined(MACX) || defined(linux)
	strcat(class_path, ":\0");
#endif
#ifdef WIN32
	strcat(class_path, ";\0");
#endif

	strcat(class_path, replace(cfg->JVMClassPath, "{WrapperPath}", DllPath));

	log("VSTiClassLoader Path=%s", class_path);
	
	
	jclass manager = NULL;
	jmethodID loadcl_mid = NULL;
	jstring dllloc = NULL;
	jstring plug = NULL;
	jstring cp = NULL;
	jclass clazz = NULL;
	jmethodID init_mid = NULL;
	jstring dll = NULL;
	jboolean doLogging = JNI_TRUE;
	
	
	//IMPORTANT: clear possible pending exceptions...
	if (checkException(env)) goto leave;


	//USE THE VSTiClassLoaderManager here!!!
	//load this ClassLoader with the bootstrap ClassLoader
	manager = env->FindClass("jvst/wrapper/system/VSTiClassLoaderManager");
	if (manager == NULL) {
		log("** ERROR: cannot find jvst/wrapper/system/VSTiClassLoaderManager");
		checkException(env); //print statck trace...
		goto leave;
	}

	loadcl_mid = env->GetStaticMethodID(manager, "loadVSTiClass", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Class;");
	if (loadcl_mid == NULL)  {
		log("** ERROR: cannot find static method loadVSTiClass(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Class;");
		checkException(env); //print statck trace...
		goto leave;
	}

	dllloc = env->NewStringUTF(DllLocation);
	plug = env->NewStringUTF(cfg->PluginClass);
	cp = env->NewStringUTF(class_path);
	clazz = (jclass)env->CallStaticObjectMethod(manager, loadcl_mid, dllloc, plug, cp);
	if(clazz == NULL) {
		log("** ERROR: Could not load Plugin Class");
		checkException(env);
		goto leave;
	}
	
	if (checkException(env)) goto leave;


	//calling static _initPlugFromNative
	//passing config data
	init_mid = env->GetStaticMethodID(clazz, "_initPlugFromNative", "(Ljava/lang/String;Z)V");
	if (init_mid == NULL)  {
		log("** ERROR: cannot find static method _initPlugFromNative(Ljava/lang/String;Z)V");
		checkException(env); //print statck trace...
		goto leave;
	}
	dll = env->NewStringUTF(DllLocation);
	doLogging = cfg->IsLoggingEnabled;
	env->CallStaticVoidMethod(clazz, init_mid, dll, doLogging);

	if (checkException(env)) goto leave;


	// Create the AudioEffect
	log("before Wrapper Constructor");
	WrapperInstance = new VSTV24ToPlug(audioMaster, 1, 1, GlobalJVM);
	if (!WrapperInstance) {
		log("** ERROR: Error Creating VST Wrapper instance");
		delete WrapperInstance;
		checkException(env); //print statck trace...
		goto leave;
	}
	
	//calling Java side constructors
	log("calling effects java construtor!");
	if (WrapperInstance->initJavaSide(clazz)) goto leave;

	if (checkException(env)) goto leave;



	//test if we can load the GUI class
	if (cfg->PluginUIClass!=NULL && IsLADSPALoaded==false) {
		log("classloding gui class using the GUI Runner, GUI=%s", cfg->PluginUIClass);

		//load guirunner using our own ClassLoader...
		jstring gui = env->NewStringUTF("jvst/wrapper/gui/VSTPluginGUIRunner");

		guiRunnerClass = (jclass)env->CallStaticObjectMethod(manager, loadcl_mid, dllloc, gui, cp);
		if (checkException(env)) {
			guiRunnerClass=NULL;
			hasGUI=false;
			log("* WARNING: Could not load GUIRunner. Using Plug-ins default UI!");
		}
		else hasGUI = true;
	}

	if (checkException(env)) goto leave;

	if(hasGUI) {
		log("calling java gui wrapper constructor");
		//init gui wrapper
		jstring guiclazz = env->NewStringUTF(cfg->PluginUIClass);
		VSTGUIWrapper* guiWrapper = new VSTGUIWrapper(WrapperInstance, guiRunnerClass, guiclazz);
		WrapperInstance->setEditor(guiWrapper);	
	}
	else {
		WrapperInstance->setEditor(NULL);
		log("Plugin is NOT using a custom UI!");
	}
	
	if (cfg) delete cfg;
	if (checkException(env)) goto leave;
	
	result=0;
	
leave:
	return result;
}



//------------------------------------------------------------------------
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
#if defined(WIN32) || defined(linux)
	return JNI_VERSION_1_2; //we use JNI 1.2 functions, tell that the jvm that loads us. 
							//we dont need jni 1.4 functionality, so 1.2 is enough
#endif
#ifdef MACX
	return JNI_VERSION_1_4;
#endif
}

//------------------------------------------------------------------------
void calculatePaths() {
#ifdef linux
	//find out where we are:
	char* soPath = find_exe_for_symbol ((const void*)"");
	if (soPath == NULL) {
		log("** ERROR: could not locate my own location! (/proc/self/maps missing?)");
		return;
	}	
	strncpy(DllLocation, soPath, JVST_PATH_MAX-1);
	log("DllLocation=%s", DllLocation);
#endif

#if defined(WIN32) || defined(linux)
#ifdef WIN32
	char* lastSlash = strrchr(DllLocation, '\\');
#endif
#ifdef linux
	char* lastSlash = strrchr(DllLocation, '/');
#endif

	//config file name
	strcpy(ConfigFileName, lastSlash);
	char* lastPoint = strrchr(ConfigFileName, '.');
	int len = lastPoint - ConfigFileName;
	strncpy(ConfigFileName + len + 1, "ini\0", 4);
	log("ConfigFileName=%s", ConfigFileName);
	
	//log file name
	strcpy(LogFileName, lastSlash);
	lastPoint = strrchr(LogFileName, '.');
	len = lastPoint - LogFileName;
	strncpy(LogFileName + len, "_log.txt\0", 9);
	log("LogFileName=%s", LogFileName);
	
	//DllPath
	len = lastSlash - DllLocation;
	strncpy(DllPath, DllLocation, len);
	DllPath[len]='\0';
	log("DllPath=%s", DllPath);
#endif

#ifdef MACX
	const mach_header* header = &_mh_bundle_header;
	const char* imagename = 0;
	int cnt = _dyld_image_count();
	for (int idx1 = 1; idx1 < cnt; idx1++) {
		if (_dyld_get_image_header((unsigned long)idx1) == header) {			
			imagename = _dyld_get_image_name(idx1);
			break;
		}
	}
	if (imagename == 0) {
		log("** ERROR: Cant read image name from bundle!");
		return;
	}
	
	char* lastSlash = strrchr(imagename, '/') + 1;
	
	int len = strlen(lastSlash);
	strcpy(DllLocation, imagename);
	log("DllLocation=%s", DllLocation);
	
	//config file name
	strcpy(ConfigFileName, lastSlash);
	strncpy(ConfigFileName + len , ".ini\0", 5);
	log("ConfigFileName=%s", ConfigFileName);
	
	//log file name
	strcpy(LogFileName, lastSlash);
	strncpy(LogFileName + len, "_log.txt\0", 9);
	log("LogFileName=%s", LogFileName);

	//DllPath
	len = lastSlash - imagename - 1;
	char tmp[JVST_PATH_MAX];
	strncpy(tmp, imagename, len);
	tmp[len]='\0';
	lastSlash = strrchr(tmp, '/');
	len = lastSlash - tmp;
	strncpy(DllPath, imagename, len);
	DllPath[len]='\0';
	strcat(DllPath,"/Resources/\0");
	log("DllPath=%s", DllPath);
#endif
	
}


#ifdef WIN32
//------------------------------------------------------------------------
// WINAPI Dll Entry
//------------------------------------------------------------------------
void* hInstance;

BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved) {
	hInstance = hInst;

	GetModuleFileName(hInst, DllLocation, JVST_PATH_MAX);
	//Workaround for the "DllMain Bug" in VST Unit Tester by Christian Budde (great Tool btw!)
	//sprintf(DllLocation, "C:\\Programme\\Steinberg\\VstPlugins\\jvstwrapper2\\jvstwrapper.dll");
	return 1;
}
#endif


#ifdef MACX
//void sourceCallBack (  void *info  ) {}

int startJavaThread(){
	//If there is a Java GUI class configured, then we need to initialize cocoa by hand
	ConfigFileReader *cfg = new ConfigFileReader();
	if (cfg->PluginUIClass!=NULL) {
		//init cocoa to be able to interop with it in carbon
		if (initializeCocoa()==0) log("Cocoa initialized successfully!");
		else {
			log("** Error while initilizing Cocoa");
			return -1;
		}
		//performOnAnotherThread(NULL,NULL,GuiWrapperInitCocoa,false);
	}
	else log("NOT using Cocoa!");
	if(cfg) delete(cfg);

	log("starting java thread");

	/* Start the thread that runs the VM. */
	pthread_t vmthread;
	
	/* create a new pthread copying the stack size of the primordial pthread */ 
	struct rlimit limit;
    size_t stack_size = 0;
    int rc = getrlimit(RLIMIT_STACK, &limit);
    if (rc == 0) {
        if (limit.rlim_cur != 0LL) {
            stack_size = (size_t)limit.rlim_cur;
        }
    }
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    if (stack_size > 0) {
        pthread_attr_setstacksize(&thread_attr, stack_size);
    }	

    /* Start the thread that we will start the JVM on. */
    rc = pthread_create(&vmthread, &thread_attr, &startJava, NULL);
	if (rc != 0) {
		log("** Error creating thread via pthread_create() !");
		return -1;
	}
	pthread_attr_destroy(&thread_attr);
	
	pthread_join(vmthread,NULL); //wait for jvm init thread to return

	return 0;
}
#endif

