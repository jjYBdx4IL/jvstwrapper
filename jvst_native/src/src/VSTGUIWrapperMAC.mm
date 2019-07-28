//
//  VSTGUIWrapperMAC.m
//  jvstwrapper
//
//  Created by Daniel Martin on 29.12.06.
//  Copyright 2006 http://www.jvstwrapper.sf.net All rights reserved.
//

#import <JavaVM/jni.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <jawt_md.h>
#import <pthread.h>
#include <unistd.h>

#import "VSTGUIWrapperMAC.h"
#import "VSTGUIWrapper.h"
#import "JNIUtils.h"



/* Plain C stuff */

int initializeCocoa() {
	//create anonymous autorelease pool
	//this prevents the problem when loading NapkinGUI stuff
	//this generally fixes the bug that a program blocks 
	//when an image is loaded using awt methods.
	//this is a known bug in apples jvm implementation
	//comment this line out and load a gui that uses images... (e.g. napkinlaf)
	[[NSAutoreleasePool alloc] init];
	
	//init cocoa, so that we can interop with it in carbon
	//also, without this, we wouldnt be able to create any java window (awt/swing) 
	BOOL ret = NSApplicationLoad();
	log("NSApplicationLoad returned=%i", ret);
	
	//Put cocoa in multithreaded mode!
	//see apple docs for details
	//since we are using pthreads later, and cocoa doesnt out itself in multithread mode
	//when using pthreads, we need to do this ourselves...
	[NSThread detachNewThreadSelector:@selector(self) toTarget:[NSString string] withObject:nil];

	BOOL multi = [NSThread isMultiThreaded];
	log("NSThread isMultithreaded returned=%i", multi);

	log("Appkit Thread ID=%i, stay off this thread for AWT calls!", pthread_self());

	return (ret==YES && multi==YES) ? 0 : -1;
}

int objectiveCwrapperTemplate() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	//do stuff
	[pool release];
	return 0;
}

OSStatus testme123() {
	printf("Hello from ObjC++!");
	return noErr;
}

/*
NSLock *globalLock = [[NSLock alloc] init]; //Global Mutex
void aquireGlobalLock() {
	log("aquiring GLOBAL lock in thread %i", pthread_self());
	[globalLock lock];
	log("GLOBAL lock AQUIRED!");
}
void releaseGlobalLock() {
	log("releasing GLOBAL lock in thread %i", pthread_self());
	[globalLock unlock];
	log("GLOBAL lock RELEASED!");
}

NSLock *initializerLock = [[NSLock alloc] init]; //initializing Mutex
void aquireInitializerLock() {
	log("aquiring INITIALIZER lock in thread %i", pthread_self());
	[initializerLock lock];
	log("INITIALIZER lock AQUIRED!");
}
void releaseInitializerLock() {
	log("releasing INITIALIZER lock in thread %i", pthread_self());
	[initializerLock unlock];
	log("INITIALIZER lock RELEASED!");
}
*/


// objc stuff
@interface VSTGUIWrapperMAC : NSObject {
@protected
	void*	object;
	void*	parameter;
	void*	parameter2;	
    int		method;
}
 
-(void)setObject: (void*)obj;
-(void)setParameter: (void*)param;
-(void)setParameter2: (void*)param2;
-(void)setMethod: (int)method;

-(void)detachNewThread;

-(void)callback;
-(void)callback2;
@end



void printAppKitThreadID() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	VSTGUIWrapperMAC *instance = [[[VSTGUIWrapperMAC alloc] init] retain];

	[instance performSelectorOnMainThread:@selector(callback2) withObject:nil waitUntilDone:NO];

	[pool  release];
}

void* performOnAnotherThread(void* obj, void* param, void* param2, int method, bool appKitThread) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	VSTGUIWrapperMAC *instance = [[[VSTGUIWrapperMAC alloc] init] retain];
	
	[instance setObject: obj];
	[instance setParameter: param];
	[instance setParameter2: param2];
	[instance setMethod: method];
	
	if (appKitThread) {
		if (false) { //ATTENTION: *** change to be ALWAYS sync! ***
			log("perfoming ASYNC call on Appkit main thread");
			[instance performSelectorOnMainThread:@selector(callback) withObject:nil waitUntilDone:NO];
		}
		else {
			log("perfoming SYNC call on Appkit main thread");
			[instance performSelectorOnMainThread:@selector(callback) withObject:nil waitUntilDone:YES];
		}
	}
	else {
		[instance detachNewThread];
	}
	
	[pool release];
	return instance; //retain called before --> instance count should be 1, so instance* wont be garbage collected...
					 //may need a cast to void* here... (see below for cast back to typed object, damn objc-c conversion stuff)
}



/* Obj C implementation */

@implementation VSTGUIWrapperMAC
-(id)init {
	self = [super init];
	return self;
}

-(void)setObject: (void*)obj {
	object = obj;
}

-(void)setParameter: (void*)p {
	parameter = p;
}

-(void)setParameter2: (void*)p {
	parameter2 = p;
}

-(void)setMethod: (int)m {
	method = m;
}

-(void*)getObject {
	return object;
}

-(void*)getParameter {
	return parameter;
}

-(void*)getParameter2 {
	return parameter2;
}

-(int)getMethod {
	return method;
}

-(void)detachNewThread {
	log("Spawning NEW thread, current thread=%i", pthread_self());		
	[NSThread detachNewThreadSelector:@selector(callback) toTarget:self withObject:nil];
}

-(void)callback2 {
	log("### APP-KIT threadID=%i ### - Stay off this one by any chance !!!", pthread_self());
}

-(void)callback {
	log("Running method %i on thread=%i", method, pthread_self());
	
	switch(method) {  
		case GuiWrapperInitJavaSide: {
			VSTGUIWrapper* wrap = (VSTGUIWrapper*)object;
			
			//aquireInitializerLock(); //lock
			wrap->initJavaSide();
			//releaseInitializerLock();
			break;
		}
		case GuiWrapperOpen:  {
			VSTGUIWrapper* wrap = (VSTGUIWrapper*)object;

			//aquireInitializerLock(); //lock
			//wrap->wrappedOpen(parameter);
			//releaseInitializerLock();
			
			break;
		}
		case GuiWrapperClose:  {
			VSTGUIWrapper* wrap = (VSTGUIWrapper*)object;
			
			//aquireInitializerLock(); //lock
			//wrap->wrappedClose();
			//releaseInitializerLock();
			
			break;
		}
		case GuiWrapperDestroy:  {
			VSTGUIWrapper* wrap = (VSTGUIWrapper*)object;
			
			//aquireInitializerLock(); //actually, no need - just to be save!
			//wrap->wrappedDestroy(); 
			//releaseInitializerLock();
			
			break;
		}
		case GuiWrapperInitCocoa:  {
		
			//aquireInitializerLock();
			//this moves the awt-appkit thread off to another thread!
			//init cocoa to be able to interop with it in carbon
			if (initializeCocoa()==0) log("Cocoa initialized successfully on thread=%i", pthread_self());
			else {
				log("** Error while initilizing Cocoa");
			}
			//releaseInitializerLock();
			
			break;
		}
	}
	
	log("Thread %i finishes", pthread_self());
}

@end

