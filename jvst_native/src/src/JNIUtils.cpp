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
// JNI utility functions
//-
// 2003,2004 Daniel Martin
//-------------------------------------------------------------------------------------------------------


#include "JNIUtils.h"
#include "VSTV20ToPlug.h"
#include "ConfigFileReader.h"

#ifdef MACX
	#include <Carbon/Carbon.h>
	#include <sys/stat.h>
#endif
#ifdef linux
	#include <dlfcn.h>
	#include <stdlib.h>
#endif


//------------------------------------------------------------------------
int IsLogEnabled = 1;
int MessageBoxCount = 0;
extern FILE* log_stream;


//------------------------------------------------------------------------
jvalue JNU_CallJavaMethod(	JNIEnv *env,
							jobject obj,
							jmethodID mid,
							const char *name,
							const char *descriptor,
							...) {
	va_list args;
	jvalue result;


	log("Invoking JNU_CallJavaMethod method=%s descriptor=%s", name, descriptor);

	if (mid) {
		const char *p = descriptor;
		//skip over argument types to find out the
		//return type

		while (*p != ')') p++; // skip till ')'
		p++;
		va_start(args, descriptor);

		switch (*p) {
			case 'V':
				env->CallVoidMethodV(obj, mid, args);
				break;
			case '[':
			case 'L':
				result.l = env->CallObjectMethodV( obj, mid, args);
				break;
			case 'Z':
				result.z = env->CallBooleanMethodV(obj, mid, args);
				break;
			case 'B':
				result.b = env->CallByteMethodV(obj, mid, args);
				break;
			case 'C':
				result.c = env->CallCharMethodV(obj, mid, args);
				break;
			case 'S':
				result.s = env->CallShortMethodV(obj, mid, args);
				break;
			case 'I':
				result.i = env->CallIntMethodV(obj, mid, args);
				break;
			case 'J':
				result.j = env->CallLongMethodV(obj, mid, args);
				break;
			case 'F':
				result.f = env->CallFloatMethodV(obj, mid, args);
				break;
			case 'D':
				result.d = env->CallDoubleMethodV(obj, mid, args);
				break;
			default:
				env->FatalError("** JNU_CallJavaMethod: illegal descriptor");
		}

		va_end(args);
	}

	checkException(env);

	return result;
}


//------------------------------------------------------------------------
int log(const char* data, ...) {
    int retval = -1;
	int isWarningOrError = 1;
	char message[9999];


	if (data==NULL) return retval;

	va_list marker;
	va_start (marker, data);
	vsprintf (message, data, marker);
	message[9998]='\0'; //cut string at the end, if its longer...

	isWarningOrError = (*message == '*') && (*(message+1)=='*');

	//write to log file
	if (IsLogEnabled || isWarningOrError) {
		if(log_stream!=NULL) {
#ifdef WIN32
			fprintf(log_stream, "\nThread=%i: %s", GetCurrentThreadId(), message);
#endif
#if defined(MACX) || defined(linux)
			fprintf(log_stream, "\nThread=%i: %s", pthread_self(), message);
#endif
			fflush(log_stream);
			retval = 0;
		}
		else {
#ifdef WIN32
			fprintf(stderr, "\nThread=%i: %s", GetCurrentThreadId(), message);
#endif
#if defined(MACX) || defined(linux)
			fprintf(stderr, "\nThread=%i: %s", pthread_self(), message);
#endif
			fflush(stderr);
			retval = 0;
		}
	}
	else retval = 0;


	//show MessageBox on Error or warning!
	//restrict it to 5 messages per session.
	//more would just be annoying...
	if (isWarningOrError) {
		MessageBoxCount++;
		if (MessageBoxCount<=5) {
			if (MessageBoxCount==5) {
				sprintf(message, "Max number of error messages reached.\n Will now supress error messages and only log them to the log file.");
			}

#ifdef WIN32
			MessageBoxA(0, message, "jVSTwRapper", 0);
#endif
#ifdef MACX
			CFStringRef msg = CFStringCreateWithCString(NULL, message, kCFStringEncodingASCII);
			CFOptionFlags response;
			CFUserNotificationDisplayAlert(0,
				kCFUserNotificationCautionAlertLevel,
				NULL, NULL, NULL,
				CFSTR("jVSTwRapper"), msg,
				NULL, NULL, NULL, &response);
#endif
#ifdef linux
			//if anybody knows a simple solution to show a messagebox using Xlib, please let me know...
			//For now, the linux version has to live with log file output only
#endif
		}
	}

    return retval;
}


#define WHITESPACE_STR  " \f\n\r\t\v"
/**
 * Remove whitespace characters from both ends of a copy of
 *  '\0' terminated STRING and return the result.
 **/
char *trim (char *string) {
  char *result = 0;

  /* Ignore NULL pointers.  */
  if (string) {
      char *ptr = string;

      /* Skip leading whitespace.  */
      while (strchr (WHITESPACE_STR, *ptr))
        ++ptr;

      /* Make a copy of the remainder.  */
      result = strdup (ptr);

      /* Move to the last character of the copy.  */
      for (ptr = result; *ptr; ++ptr);
      --ptr;

      /* Remove trailing whitespace.  */
      for (--ptr; strchr (WHITESPACE_STR, *ptr); --ptr)
          *ptr = '\0';
   }

  return result;
}


char *replace(char *string, char *oldpiece, char *newpiece) {
   int str_index, newstr_index, oldpiece_index, end, new_len, old_len, cpy_len;
   char *c;
   char newstring[JVST_PATH_MAX];


   newstring[JVST_PATH_MAX-1]='\0';

   if ((c = (char *) strstr(string, oldpiece)) == NULL)
      return strdup(string);


   new_len = strlen(newpiece);
   old_len = strlen(oldpiece);
   end = strlen(string) - old_len;
   oldpiece_index = c - string;


   newstr_index = 0;
   str_index = 0;
   while(str_index <= end && c != NULL) {
      /* Copy characters from the left of matched pattern occurence */
      cpy_len = oldpiece_index-str_index;
      strncpy(newstring+newstr_index, string+str_index, cpy_len);
      newstr_index += cpy_len;
      str_index += cpy_len;


      /* Copy replacement characters instead of matched pattern */
      strcpy(newstring+newstr_index, newpiece);
      newstr_index += new_len;
      str_index += old_len;


      /* Check for another pattern match */
      if((c = (char *) strstr(string+str_index, oldpiece)) != NULL)
         oldpiece_index = c - string;
   }
   /* Copy remaining characters from the right of last matched pattern */
   strcpy(newstring+newstr_index, string+str_index);


   return strdup(newstring);
}


bool checkException(JNIEnv *env) {
	if (env->ExceptionCheck()==0) return false;

	jthrowable exc = env->ExceptionOccurred();
	if (exc==NULL) return false;

	//this clears the exception, but we want it to be trown in java again.
	//this will happen when the native method returns.

	//but if we called a java method, the exception remains pending in the
	//thread until someone clears it (either a java native method returns or we do it!)
	env->ExceptionDescribe();
	env->ExceptionClear();


	//get the stack trace
	jclass c = env->FindClass("jvst/wrapper/system/ExceptionUtililities");
	if (c!=NULL) {
		jmethodID mid = env->GetStaticMethodID(c, "getStackTrace", "(Ljava/lang/Throwable;)Ljava/lang/String;");

		if (mid!=NULL) {
			jstring ret = (jstring)env->CallStaticObjectMethod(c, mid, exc);
			if (ret==NULL) {
				log("** ERROR: getStackTrace returned NULL --> aborting!");
				return true;
			}
			const char* jstr = env->GetStringUTFChars(ret, NULL);
			char buffer[10000]; //stack trace could be very big!
			sprintf(buffer, "** %s", jstr); //add the error indicator ('**')
			log(buffer);
		}
		else {
			log("** Error: cannot find MethodID getStackTrace((Ljava/lang/Throwable;)Ljava/lang/String;)");
			env->ExceptionClear();
			env->Throw(exc); //because we want this error to be reported when its possible.
		}
	}
	else {
		log("** Error: Cannot find class jvst/wrapper/system/ExceptionUtililities");
		env->ExceptionClear();
		env->Throw(exc); //because we want this error to be reported when its possible.
	}

	//since we had an exception, return true!
	return true;
}



//check for exception, print stack trace, and throw it again,
//so it can be catched in java code!
bool checkAndThrowException(JNIEnv *env) {
	jthrowable exc = env->ExceptionOccurred();
	if (exc==NULL) return false;

	bool ret = checkException(env);

	env->Throw(exc);

	return ret;
}


//-----------------------------------------------------------------------------
JNIEnv* ensureJavaThreadAttachment(JavaVM* vm) {
	JNIEnv* ret = NULL;
	jint stat = vm->AttachCurrentThread((void**)&ret, NULL);
	if (stat<0) log("** ERROR: attaching to native THREAD in VSTV10ToPlug Wrapper!");

	//ultra important, if a pending exception isnt cleared, all following calls will fail...
	checkException(ret);
	return ret;
}








//----------------------------------------------------------
//registry + dynamic lib loading stuff for windows...

#ifdef WIN32

#define JVM_REG_18 "Software\\JavaSoft\\Java Runtime Environment\\1.8"
#define JVM_REG_17 "Software\\JavaSoft\\Java Runtime Environment\\1.7"
#define JVM_REG_16 "Software\\JavaSoft\\Java Runtime Environment\\1.6"
#define JVM_REG_15 "Software\\JavaSoft\\Java Runtime Environment\\1.5"
#define JVM_REG_14 "Software\\JavaSoft\\Java Runtime Environment\\1.4"
#define JVM_REG_13 "Software\\JavaSoft\\Java Runtime Environment\\1.3"
#define JVM_REG_12 "Software\\JavaSoft\\Java Runtime Environment\\1.2"

char* readJVMLibLocation(char* requestedJVMVersion, char* customRegKey) {
	DWORD	rc;
	HKEY	regKey;
	DWORD	len;
	DWORD	dwType;
	char	javaLibLocation[JVST_PATH_MAX]; //value stored here


	if (customRegKey!=NULL) {
		log("Trying to locate custom jvm location from registry with key=%s", customRegKey);

		//check if this key is available
		rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, customRegKey, 0, KEY_ALL_ACCESS, &regKey);
		if (rc != ERROR_SUCCESS) return NULL;
	}
	else if (requestedJVMVersion!=NULL) {
		//there is a specific jvm version mentioned in the .ini, try to locate this one...
		char jvmRegKey[512] = {'\0'};
		strcat(jvmRegKey, "Software\\JavaSoft\\Java Runtime Environment\\\0");
		strcat(jvmRegKey, requestedJVMVersion);

		log("Trying to locate specific jvm version with regkey=%s", jvmRegKey);

		//check if this key is available
		rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, jvmRegKey, 0, KEY_ALL_ACCESS, &regKey);
		if (rc != ERROR_SUCCESS) return NULL;
	}
	else {
		//Auto check for installed JVMs
		rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\JavaSoft\\Java Runtime Environment", 0, KEY_ALL_ACCESS, &regKey);
		if (rc!=ERROR_SUCCESS) return NULL;

		char currentVersion[64];
		len = sizeof(currentVersion);
		rc = RegQueryValueEx(regKey, "CurrentVersion", 0, &dwType, (unsigned char*)currentVersion, &len);
		if (rc!=ERROR_SUCCESS) return NULL;

		char jvmRegKey[512] = {'\0'};
		strcat(jvmRegKey, "Software\\JavaSoft\\Java Runtime Environment\\\0");
		strcat(jvmRegKey, currentVersion);

		log("default jvm is at regkey=%s", jvmRegKey);

		rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, jvmRegKey, 0, KEY_ALL_ACCESS, &regKey);
		if (rc!=ERROR_SUCCESS) return NULL;
	}

	len = sizeof(javaLibLocation);
	rc = RegQueryValueEx(regKey, "RuntimeLib", 0, &dwType, (unsigned char*)javaLibLocation, &len);

	if (rc==ERROR_SUCCESS) return strdup(javaLibLocation);
	else return NULL;
}
#endif

#ifdef linux
//on linux, we check if there is a $JAVA_HOME
char* readJVMLibLocation(char* requestedJVMVersion) {
	char *pPath = getenv("JAVA_HOME");

  	if(pPath==NULL) return NULL;
  	else {
  		char* tmp = (char *)malloc(1024);
  		strncpy(tmp, pPath, 1023);
  		strcat(tmp,"/lib/i386/client/libjvm.so\0");
  		return strdup(tmp);
  	}
}
#endif



#if defined(WIN32) || defined(linux)

//globals
jint (JNICALL *PTR_CreateJavaVM)(JavaVM **, void **, void *) = NULL;
jint (JNICALL *PTR_GetCreatedJavaVMs)(JavaVM **, jsize, jsize *) = NULL;


int initJVMFunctionPointers(char *vmlibpath) {
#ifdef WIN32
	//load the library
	//*******************
	//do what http://www.duckware.com/tech/java6msvcr71.html suggests:
	//Call SetCurrentDirectory() and SetDllDirectory() to the Java bin folder
	//Call LoadLibrary() on the full path to the jvm.dll
	//Call SetCurrentDirectory(old-dir) and SetDllDirectory(NULL)

	//clear errors
	SetLastError(0);

	char dir1[5000] = {'\0'}, dir2[5000] = {'\0'}, oldDir[5000] = {'\0'};
	DWORD ret = GetFullPathName(strcat(strcat(dir1,vmlibpath),"\\..\\.."), sizeof(dir2), dir2, NULL);
	log("jvm bin dir=%s ret=%i", dir2, ret);
	
	GetCurrentDirectory(sizeof(oldDir), oldDir);
	log ("old current dir=%s", oldDir);

	SetCurrentDirectory(dir2);
	SetDllDirectory(dir2);
	HINSTANCE hVM = LoadLibrary(vmlibpath);
	SetCurrentDirectory(oldDir);
	SetDllDirectory(NULL);
#endif
#ifdef linux
	void* hVM = dlopen(vmlibpath, RTLD_LAZY); //RTLD_NOW);
#endif

    if (hVM == NULL) {
		log("**ERROR: Could not locate jvm.dll. \n\
			It seems that you do not have a properly installed Java Virtual Machine (JVM).\n\
			If so, please download (its free!) and install one from \n\n\
			http://www.java.com/ \n\n\
			If you already did that and see this message again, please add the path \n\
			where the jvm.dll is located to your PATH environment variable.\n\n\
			By default, the jvm.dll is located at 'C:\\j2sdk1.4.2_05\\jre\\bin\\client'. But this \n\
			may differ depending on your choice during installation. \n\
			You can modify your PATH variable if you click \n\n\
			Start/Settings/Control Panel/System/Advanced (Tab)/Environment Variables (Button)\n\
			/System Variables (the textbox at the bottom).\n\
			Double-click on the variable called Path, and add a ';the-path-where-jvm.dll-is-located' \n\
			to the end of its value.\n\
			Note the ';' at the beginning of the string. close all windows with ok, and you \n\
			should be all set.\n\
			If you still cant use the plugin, contact the forum at \n\nhttp://sourceforge.net/forum/forum.php?forum_id=318265");
#ifdef linux
		log(dlerror());
#endif
#ifdef WIN32
		char buffer[5000] = {'\0'};
		DWORD err = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, LANG_NEUTRAL, buffer, sizeof(buffer), NULL);
		log("**ERROR: LoadLibrary error %X: %s", err, buffer);
#endif
		return -1;
	}


#ifdef WIN32
	PTR_CreateJavaVM = (jint (JNICALL *)(JavaVM **, void **, void *)) GetProcAddress(hVM, "JNI_CreateJavaVM");
    PTR_GetCreatedJavaVMs = (jint (JNICALL *)(JavaVM **, jsize, jsize *)) GetProcAddress(hVM, "JNI_GetCreatedJavaVMs");
#endif
#ifdef linux
	PTR_CreateJavaVM = (jint (JNICALL *)(JavaVM **, void **, void *)) dlsym(hVM, "JNI_CreateJavaVM");
   	PTR_GetCreatedJavaVMs = (jint (JNICALL *)(JavaVM **, jsize, jsize *)) dlsym(hVM,  "JNI_GetCreatedJavaVMs");
#endif

	if (PTR_CreateJavaVM==NULL || PTR_GetCreatedJavaVMs==NULL) {
		log("**ERROR: Cant find jvm interface pointers!");
#ifdef linux
		log(dlerror());
#endif
#ifdef WIN32
		char buffer[5000] = {'\0'};
		DWORD err = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, LANG_NEUTRAL, buffer, sizeof(buffer), NULL);
		log("**ERROR: CreateJavaVM error %X: %s", err, buffer);
#endif
		return -1;
	}

	return 0;
}
#endif




#ifdef MACX

void printCurrentThreadID() {
	log("Current Thread ID=%i", pthread_self());
}

//**************************************************************************************************
// Mac feature for requesting a specific JVM


/*
To invoke Java 1.4.1 or the currently preferred JDK as defined by the operating system
(1.4.2 as of the release of this sample and the release of Mac OS X 10.4) nothing changes in 10.4 vs 10.3
in that when a JNI_VERSION_1_4 is passed into JNI_CreateJavaVM as the vm_args.version it returns
the current preferred JDK.

To specify the current preferred JDK in a family of JVM's, say the 1.5.x family, applications should set
the environment variable JAVA_JVM_VERSION to 1.5, and then pass JNI_VERSION_1_4 into JNI_CreateJavaVM
as the vm_args.version. To get a specific Java 1.5 JVM, say Java 1.5.0, set the environment variable
JAVA_JVM_VERSION to 1.5.0. For Java 1.6 it will be the same in that applications will need to set
the environment variable JAVA_JVM_VERSION to 1.6 to specify the current preferred 1.6 Java VM, and
to get a specific Java 1.6 JVM, say Java 1.6.1, set the environment variable JAVA_JVM_VERSION to 1.6.1.

To make this sample bring up the current preferred 1.5 JVM, set the environment variable
JAVA_JVM_VERSION to 1.5 before calling JNI_CreateJavaVM as shown below.  Applications must currently
check for availability of JDK 1.5 before requesting it.  If your application requires JDK 1.5 and it is not
found, it is your responsibility to report an error to the user. To verify if a JVM is installed, check
to see if the symlink, or directory exists for the JVM in /System/Library/Frameworks/JavaVM.framework/Versions/
before setting the environment variable JAVA_JVM_VERSION.

If the environment variable JAVA_JVM_VERSION is not set, and JNI_VERSION_1_4 is passed into
JNI_CreateJavaVM as the vm_args.version, JNI_CreateJavaVM will return the current preferred JDK.
Java 1.4.2 is the preferred JDK as of the release of this sample and the release of Mac OS X 10.4.
*/


int checkJVMVersionRequest(char* requestedJVMVersion) {
	if (requestedJVMVersion==NULL) return -1;

	log("checking for a jvm version");
	log(requestedJVMVersion);

	CFStringRef targetJVM = CFStringCreateWithCString(NULL, requestedJVMVersion, kCFStringEncodingASCII);
	CFBundleRef JavaVMBundle;
	CFURLRef    JavaVMBundleURL;
	CFURLRef    JavaVMBundlerVersionsDirURL;
	CFURLRef    TargetJavaVM;
	UInt8 pathToTargetJVM [PATH_MAX] = "\0";
	struct stat sbuf;
	int retval = -1;

	// Look for the JavaVM bundle using its identifier
	JavaVMBundle = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.JavaVM") );

	if(JavaVMBundle != NULL) {
		// Get a path for the JavaVM bundle
		JavaVMBundleURL = CFBundleCopyBundleURL(JavaVMBundle);
		CFRelease(JavaVMBundle);

		if(JavaVMBundleURL != NULL) {
			// Append to the path the Versions Component
			JavaVMBundlerVersionsDirURL = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,JavaVMBundleURL,CFSTR("Versions"),true);
			CFRelease(JavaVMBundleURL);

			if(JavaVMBundlerVersionsDirURL != NULL) {
				// Append to the path the target JVM's Version
				TargetJavaVM = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,JavaVMBundlerVersionsDirURL,targetJVM,true);
				CFRelease(JavaVMBundlerVersionsDirURL);

				if(TargetJavaVM != NULL) {
					if(CFURLGetFileSystemRepresentation (TargetJavaVM,true,pathToTargetJVM,PATH_MAX )) {
						// Check to see if the directory, or a sym link for the target JVM directory exists, and if so set the
						// environment variable JAVA_JVM_VERSION to the target JVM.
						if(stat((char*)pathToTargetJVM,&sbuf) == 0) {
							// Ok, the directory exists, so now we need to set the environment var JAVA_JVM_VERSION to the CFSTR targetJVM
							// We can reuse the pathToTargetJVM buffer to set the environement var.
							if(CFStringGetCString(targetJVM, (char*)pathToTargetJVM, PATH_MAX, kCFStringEncodingUTF8)) {
								setenv("JAVA_JVM_VERSION", (char*)pathToTargetJVM, 1);
								retval = 0;
								log("Found a JVM that matches the one requested, loading that one!!!");
								log((char*)pathToTargetJVM);
							} else log("Error getting path to target jvm");
						} else log("Error checking symlink for target jvm");
					} else log("Error getting file system representation for bundle url");
					CFRelease(TargetJavaVM);
				} else log("Error appending version component to bundle url");
			} else log("Error appending path component to bundle url");
		} else log("Error copying bulde url");
	} else log("ERROR: cant find bundle: com.apple.JavaVM");


	return retval;
}
#endif



//stupid shared library lookup stuff
//dont even look at it. unstable, platform dependent (most likely only vanilla linux) and ugly!!!
#ifdef linux

#include <limits.h>

//kindly provided by autotools
char* find_exe_for_symbol (const void *symbol) {
	#define SIZE PATH_MAX + 100
	FILE *f;
	size_t address_string_len;
	char *address_string, line[SIZE], *found;

	if (symbol == NULL)
		return (char *) NULL;

	f = fopen ("/proc/self/maps", "r");
	if (f == NULL)
		return (char *) NULL;

	address_string_len = 4;
	address_string = (char *) malloc (address_string_len);
	found = (char *) NULL;

	while (!feof (f)) {
		char *start_addr, *end_addr, *end_addr_end, *file;
		void *start_addr_p, *end_addr_p;
		size_t len;

		if (fgets (line, SIZE, f) == NULL)
			break;

		/* Sanity check. */
		if (strstr (line, " r-xp ") == NULL || strchr (line, '/') == NULL)
			continue;

		/* Parse line. */
		start_addr = line;
		end_addr = strchr (line, '-');
		file = strchr (line, '/');

		/* More sanity check. */
		if (!(file > end_addr && end_addr != NULL && end_addr[0] == '-'))
			continue;

		end_addr[0] = '\0';
		end_addr++;
		end_addr_end = strchr (end_addr, ' ');
		if (end_addr_end == NULL)
			continue;

		end_addr_end[0] = '\0';
		len = strlen (file);
		if (len == 0)
			continue;
		if (file[len - 1] == '\n')
			file[len - 1] = '\0';

		/* Get rid of "(deleted)" from the filename. */
		len = strlen (file);
		if (len > 10 && strcmp (file + len - 10, " (deleted)") == 0)
			file[len - 10] = '\0';

		/* I don't know whether this can happen but better safe than sorry. */
		len = strlen (start_addr);
		if (len != strlen (end_addr))
			continue;


		/* Transform the addresses into a string in the form of 0xdeadbeef,
		 * then transform that into a pointer. */
		if (address_string_len < len + 3) {
			address_string_len = len + 3;
			address_string = (char *) realloc (address_string, address_string_len);
		}

		memcpy (address_string, "0x", 2);
		memcpy (address_string + 2, start_addr, len);
		address_string[2 + len] = '\0';
		sscanf (address_string, "%p", &start_addr_p);

		memcpy (address_string, "0x", 2);
		memcpy (address_string + 2, end_addr, len);
		address_string[2 + len] = '\0';
		sscanf (address_string, "%p", &end_addr_p);


		if (symbol >= start_addr_p && symbol < end_addr_p) {
			found = file;
			break;
		}
	}

	free (address_string);
	fclose (f);

	if (found == NULL)
		return (char *) NULL;
	else
		return strdup (found);
}

#endif
