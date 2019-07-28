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
// A JNI Wrapper 
// for Java as the language to write VSTi Plugins
//-------------------------------------------------------------------------------------------------------

	
#include "VSTV21ToPlug.h"
#include "JNIUtils.h"


//Impl of Host -> Plug calls
//Native -> Java 
//-----------------------------------------------------------------------------
VSTV21ToPlug::VSTV21ToPlug (audioMasterCallback audioMaster, int progs, int parms, JavaVM *jvm) 
	: VSTV20ToPlug (audioMaster, progs, parms, jvm) {

}
	
//-----------------------------------------------------------------------------------------
VSTV21ToPlug::~VSTV21ToPlug () {

}

//-----------------------------------------------------------------------------------------
VstInt32 VSTV21ToPlug::getMidiProgramName (VstInt32 channel, MidiProgramName* midiProgramName) { 
	if (midiProgramName==NULL) return 0;

	JNIEnv* env = this->ensureJavaThreadAttachment();

	jclass jMidiProgramClass = env->FindClass("jvst/wrapper/valueobjects/MidiProgramName");
	if (jMidiProgramClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiProgramName");
		return 0;
	}

	jmethodID mid = env->GetMethodID(jMidiProgramClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiProgramName!");
		return 0;
	}
	jobject jMidiProgramObject = env->NewObject(jMidiProgramClass, mid);
	if (jMidiProgramObject == NULL) {
		log("** ERROR: cannot create MidiProgramName Object!");
		return 0;
	}

	jfieldID thisProgramIndexField = env->GetFieldID(jMidiProgramClass, "thisProgramIndex", "I");
	if (thisProgramIndexField == NULL) {
		log("** ERROR: cannot find field-id thisProgramIndex (I)");
		return 0;
	}
	jfieldID parentCategoryIndexField = env->GetFieldID(jMidiProgramClass, "parentCategoryIndex", "I");
	if (parentCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id parentCategoryIndex (I)");
		return 0;
	}
	jfieldID flagsField = env->GetFieldID(jMidiProgramClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return 0;
	}
	jfieldID nameField = env->GetFieldID(jMidiProgramClass, "name", "Ljava/lang/String;");
	if (nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return 0;
	}
	jfieldID midiProgramField = env->GetFieldID(jMidiProgramClass, "midiProgram", "B");
	if (midiProgramField == NULL) {
		log("** ERROR: cannot find field-id midiProgram (B)");
		return 0;
	}
	jfieldID midiBankMsbField = env->GetFieldID(jMidiProgramClass, "midiBankMsb", "B");
	if (midiBankMsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankMsb (B)");
		return 0;
	}
	jfieldID midiBankLsbField = env->GetFieldID(jMidiProgramClass, "midiBankLsb", "B");
	if (midiBankLsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankLsb (B)");
		return 0;
	}
	jfieldID reservedField = env->GetFieldID(jMidiProgramClass, "reserved", "B");
	if (reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (B)");
		return 0;
	}
	
	//in parameter!
	env->SetIntField(jMidiProgramObject, thisProgramIndexField, midiProgramName->thisProgramIndex);
	
	mid = env->GetMethodID(this->JavaPlugClass, "getMidiProgramName", "(ILjvst/wrapper/valueobjects/MidiProgramName;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getMidiProgramName(ILjvst/wrapper/valueobjects/MidiProgramName;)I"); return 0;}
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid, channel, jMidiProgramObject);
	
	//read out parameters
	midiProgramName->flags = env->GetIntField(jMidiProgramObject, flagsField);
	midiProgramName->parentCategoryIndex = env->GetIntField(jMidiProgramObject, parentCategoryIndexField);
	
	jstring jstr = (jstring)env->GetObjectField(jMidiProgramObject, nameField);
	if (jstr!=NULL) {
		const char* str = env->GetStringUTFChars(jstr, NULL);
		vst_strncpy (midiProgramName->name, str, kVstMaxNameLen-1);
		env->ReleaseStringUTFChars(jstr, str);
		env->DeleteLocalRef(jstr);
	}

	midiProgramName->midiBankLsb = env->GetByteField(jMidiProgramObject, midiBankLsbField);
	midiProgramName->midiBankMsb = env->GetByteField(jMidiProgramObject, midiBankMsbField);
	midiProgramName->midiProgram = env->GetByteField(jMidiProgramObject, midiProgramField);
	midiProgramName->reserved = env->GetByteField(jMidiProgramObject, reservedField);

	this->checkException(env);

	return ret;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTV21ToPlug::getCurrentMidiProgram (VstInt32 channel, MidiProgramName* midiProgramName) { 
	if (midiProgramName==NULL) return 0;

	JNIEnv* env = this->ensureJavaThreadAttachment();

	jclass jMidiProgramClass = env->FindClass("jvst/wrapper/valueobjects/MidiProgramName");
	if (jMidiProgramClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiProgramName");
		return 0;
	}

	jmethodID mid = env->GetMethodID(jMidiProgramClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiProgramName!");
		return 0;
	}
	jobject jMidiProgramObject = env->NewObject(jMidiProgramClass, mid);
	if (jMidiProgramObject == NULL) {
		log("** ERROR: cannot create MidiProgramName Object!");
		return 0;
	}

	jfieldID thisProgramIndexField = env->GetFieldID(jMidiProgramClass, "thisProgramIndex", "I");
	if (thisProgramIndexField == NULL) {
		log("** ERROR: cannot find field-id thisProgramIndex (I)");
		return 0;
	}
	jfieldID parentCategoryIndexField = env->GetFieldID(jMidiProgramClass, "parentCategoryIndex", "I");
	if (parentCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id parentCategoryIndex (I)");
		return 0;
	}
	jfieldID flagsField = env->GetFieldID(jMidiProgramClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return 0;
	}
	jfieldID nameField = env->GetFieldID(jMidiProgramClass, "name", "Ljava/lang/String;");
	if (nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return 0;
	}
	jfieldID midiProgramField = env->GetFieldID(jMidiProgramClass, "midiProgram", "B");
	if (midiProgramField == NULL) {
		log("** ERROR: cannot find field-id flags (B)");
		return 0;
	}
	jfieldID midiBankMsbField = env->GetFieldID(jMidiProgramClass, "midiBankMsb", "B");
	if (midiBankMsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankMsb (B)");
		return 0;
	}
	jfieldID midiBankLsbField = env->GetFieldID(jMidiProgramClass, "midiBankLsb", "B");
	if (midiBankLsbField == NULL) {
		log("** ERROR: cannot find field-id midiBankLsb (B)");
		return 0;
	}
	jfieldID reservedField = env->GetFieldID(jMidiProgramClass, "reserved", "B");
	if (reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (B)");
		return 0;
	}

	mid = env->GetMethodID(this->JavaPlugClass, "getCurrentMidiProgram", "(ILjvst/wrapper/valueobjects/MidiProgramName;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getCurrentMidiProgram(ILjvst/wrapper/valueobjects/MidiProgramName;)I"); return 0;}
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid, channel, jMidiProgramObject);
	
	//read out parameters
	midiProgramName->thisProgramIndex = env->GetIntField(jMidiProgramObject, thisProgramIndexField);
	midiProgramName->flags = env->GetIntField(jMidiProgramObject, flagsField);
	midiProgramName->parentCategoryIndex = env->GetIntField(jMidiProgramObject, parentCategoryIndexField);
	
	jstring jstr = (jstring)env->GetObjectField(jMidiProgramObject, nameField);
	if (jstr!=NULL) {
		const char* str = env->GetStringUTFChars(jstr, NULL);
		vst_strncpy (midiProgramName->name, str, kVstMaxNameLen-1);
		env->ReleaseStringUTFChars(jstr, str);
		env->DeleteLocalRef(jstr);
	}

	midiProgramName->midiBankLsb = env->GetByteField(jMidiProgramObject, midiBankLsbField);
	midiProgramName->midiBankMsb = env->GetByteField(jMidiProgramObject, midiBankMsbField);
	midiProgramName->midiProgram = env->GetByteField(jMidiProgramObject, midiProgramField);
	midiProgramName->reserved = env->GetByteField(jMidiProgramObject, reservedField);

	this->checkException(env);

	return ret;
}

//-----------------------------------------------------------------------------------------
VstInt32 VSTV21ToPlug::getMidiProgramCategory (VstInt32 channel, MidiProgramCategory* category) { 
	if (category==NULL) return 0;
	
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jclass jMidiProgramCategoryClass = env->FindClass("jvst/wrapper/valueobjects/MidiProgramCategory");
	if (jMidiProgramCategoryClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiProgramCategory");
		return 0;
	}

	jmethodID mid = env->GetMethodID(jMidiProgramCategoryClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiProgramCategory!");
		return 0;
	}
	jobject jMidiProgramCategoryObject = env->NewObject(jMidiProgramCategoryClass, mid);
	if (jMidiProgramCategoryObject == NULL) {
		log("** ERROR: cannot create MidiProgramCategory Object!");
		return 0;
	}

	jfieldID thisCategoryIndexField = env->GetFieldID(jMidiProgramCategoryClass, "thisCategoryIndex", "I");
	if (thisCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id thisCategoryIndex (I)");
		return 0;
	}
	jfieldID parentCategoryIndexField = env->GetFieldID(jMidiProgramCategoryClass, "parentCategoryIndex", "I");
	if (parentCategoryIndexField == NULL) {
		log("** ERROR: cannot find field-id parentCategoryIndex (I)");
		return 0;
	}
	jfieldID flagsField = env->GetFieldID(jMidiProgramCategoryClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return 0;
	}
	jfieldID nameField = env->GetFieldID(jMidiProgramCategoryClass, "name", "Ljava/lang/String;");
	if (nameField == NULL) {
		log("** ERROR: cannot find field-id name (Ljava/lang/String;)");
		return 0;
	}



	//in parameter!
	env->SetIntField(jMidiProgramCategoryObject, thisCategoryIndexField, category->thisCategoryIndex);
	
	mid = env->GetMethodID(this->JavaPlugClass, "getMidiProgramCategory", "(ILjvst/wrapper/valueobjects/MidiProgramCategory;)I");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getMidiProgramCategory(ILjvst/wrapper/valueobjects/MidiProgramCategory;)I"); return 0;}
	
	jint ret = env->CallIntMethod(this->JavaPlugObj, mid, channel, jMidiProgramCategoryObject);
	
	//read out parameters
	category->flags = env->GetIntField(jMidiProgramCategoryObject, flagsField);
	category->parentCategoryIndex = env->GetIntField(jMidiProgramCategoryObject, parentCategoryIndexField);
	
	jstring jstr = (jstring)env->GetObjectField(jMidiProgramCategoryObject, nameField);
	if (jstr!=NULL) {
		const char* str = env->GetStringUTFChars(jstr, NULL);
		vst_strncpy (category->name, str, kVstMaxNameLen-1);
		env->ReleaseStringUTFChars(jstr, str);
		env->DeleteLocalRef(jstr);
	}

	this->checkException(env);

	return ret;
}

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::hasMidiProgramsChanged (VstInt32 channel) { 
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "hasMidiProgramsChanged", "(I)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method hasMidiProgramsChanged(I)Z"); return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, channel);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::getMidiKeyName (VstInt32 channel, MidiKeyName* keyName) { 
	if (keyName==NULL) return false;

	JNIEnv* env = this->ensureJavaThreadAttachment();

	jclass jMidiKeyNameClass = env->FindClass("jvst/wrapper/valueobjects/MidiKeyName");
	if (jMidiKeyNameClass == NULL) {
		log("** ERROR: cannot find Class jvst.wrapper.valueobjects.MidiKeyName");
		return false;
	}

	jmethodID mid = env->GetMethodID(jMidiKeyNameClass, "<init>", "()V");
	if (mid == NULL) {
		log("** ERROR: cannot find default contructor for MidiKeyName!");
		return false;
	}
	jobject jMidikeyNameObject = env->NewObject(jMidiKeyNameClass, mid);
	if (jMidikeyNameObject == NULL) {
		log("** ERROR: cannot create MidiKeyName Object!");
		return false;
	}

	jfieldID thisProgramIndexField = env->GetFieldID(jMidiKeyNameClass, "thisProgramIndex", "I");
	if (thisProgramIndexField == NULL) {
		log("** ERROR: cannot find field-id thisProgramIndex (I)");
		return false;
	}
	jfieldID thisKeyNumberField = env->GetFieldID(jMidiKeyNameClass, "thisKeyNumber", "I");
	if (thisKeyNumberField == NULL) {
		log("** ERROR: cannot find field-id thisKeyNumber (I)");
		return false;
	}
	jfieldID flagsField = env->GetFieldID(jMidiKeyNameClass, "flags", "I");
	if (flagsField == NULL) {
		log("** ERROR: cannot find field-id flags (I)");
		return false;
	}
	jfieldID reservedField = env->GetFieldID(jMidiKeyNameClass, "reserved", "I");
	if (reservedField == NULL) {
		log("** ERROR: cannot find field-id reserved (I)");
		return false;
	}
	jfieldID keyNameField = env->GetFieldID(jMidiKeyNameClass, "keyName", "Ljava/lang/String;");
	if (keyNameField == NULL) {
		log("** ERROR: cannot find field-id keyName (Ljava/lang/String;)");
		return false;
	}



	//in parameter!
	env->SetIntField(jMidikeyNameObject, thisKeyNumberField, keyName->thisKeyNumber);
	env->SetIntField(jMidikeyNameObject, thisProgramIndexField, keyName->thisProgramIndex);
	
	mid = env->GetMethodID(this->JavaPlugClass, "getMidiKeyName", "(ILjvst/wrapper/valueobjects/MidiKeyName;)Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method getMidiKeyName(ILjvst/wrapper/valueobjects/MidiKeyName;)Z"); return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid, channel, jMidikeyNameObject);
	
	//read out parameters
	jstring jstr = (jstring)env->GetObjectField(jMidikeyNameObject, keyNameField);
	if (jstr!=NULL) {
		const char* str = env->GetStringUTFChars(jstr, NULL);
		vst_strncpy (keyName->keyName, str, kVstMaxNameLen-1);
		env->ReleaseStringUTFChars(jstr, str);
		env->DeleteLocalRef(jstr);
	}
	
	if (this->checkException(env)) return false;

	return ret!=0;
}

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::beginSetProgram () { 
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "beginSetProgram", "()Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method beginSetProgram()Z"); return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid);
	
	if (this->checkException(env)) return false;

	return ret!=0;
} 

//-----------------------------------------------------------------------------------------
bool VSTV21ToPlug::endSetProgram () { 
	JNIEnv* env = this->ensureJavaThreadAttachment();

	jmethodID mid = env->GetMethodID(this->JavaPlugClass, "endSetProgram", "()Z");
	if (mid == NULL) {log("** ERROR: cannot find instance-method endSetProgram()Z"); return false;}
	
	jboolean ret = env->CallBooleanMethod(this->JavaPlugObj, mid);
	
	if (this->checkException(env)) return false;

	return ret!=0;
}   


