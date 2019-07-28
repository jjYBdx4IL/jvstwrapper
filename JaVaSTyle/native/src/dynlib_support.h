#ifndef __DYNLIB_SUPPORT_H__
#define __DYNLIB_SUPPORT_H__

#if WIN32
#	include <windows.h>
#elif POSIX
#	include <dlfcn.h>
#	include <link.h>
// TO DO : #elif MAC ... 
#endif

#define PTR_TO_JLONG(ptr) ((jlong)((int*)(ptr)-(int*)0))
#define JLONG_TO_PTR(off) (((int*)0)+(off))

#endif
