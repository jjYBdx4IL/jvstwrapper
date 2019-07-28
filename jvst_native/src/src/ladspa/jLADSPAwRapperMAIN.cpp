
#include "ladspa.h"
#include "JNIUtils.h"
#include "jVSTwRapperMAIN.h"

#include <aeffect.h>
#include <aeffectx.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


extern bool IsLADSPALoaded;  //this is only used to disable custom VST GUIs, no need to load them in LADSPA!


//***********************************************************************************
// VST AudioMaster callback
//

typedef AEffect *( *vstPluginMain )( audioMasterCallback audioMaster );

extern "C" {
   static VstIntPtr audioMaster( AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
      switch (opcode) {
         case audioMasterVersion:
            return 2;

         case audioMasterCurrentId:
            return 1;
      }
      return 0;
   }
}


//***********************************************************************************
// This class adapts VST effects to LADSPA effects
//

class LadspaPluginAdapter {
 public:
   LadspaPluginAdapter();
   ~LadspaPluginAdapter();

   bool canBeReused;

   bool Init( unsigned long id ) ;
   bool InitVSTEffect( unsigned long rate );

   char *GetStr( VstInt32 opcode, char *dflt );
   LadspaPluginAdapter *GetPrev();
   void SetPrev( LadspaPluginAdapter *prev );

   /*LADSPA methods*/
   LADSPA_Descriptor *GetDescriptor();
   void Cleanup();
   void Connect_Port( unsigned long port, LADSPA_Data *data );
   void Run( unsigned long count );
   void Run_Adding( unsigned long count );
   void Set_Run_Adding_Gain( LADSPA_Data gain );
   void Set_Parameters();


 private:
   LadspaPluginAdapter *mPrev;
   AEffect *mThunk;
   AEffect *mEffect;
   void *mLib;
   LADSPA_Descriptor mDesc;
   LADSPA_Data **mPorts;
   LADSPA_Data mGain;
   unsigned long mBlockSize;
   int mNumPorts;
   int mFirstParam;
};


//***********************************************************************************
// LADSPA methods
//

// This is the ladspa MAIN entry point
VST_EXPORT const LADSPA_Descriptor *ladspa_descriptor( unsigned long index ) {
	if (index!=0) return NULL; //Only one plugin per .so file!

	IsLADSPALoaded = true;
	LadspaPluginAdapter *plugin = new LadspaPluginAdapter();

	//TODO: fix this so that a random id is generated every time
   	//random() here generates always the same num (check with analysepluing)
	bool success = plugin->Init( 100000 + (rand() % 100000) );
	if (success==false) {
		log("** LADSPA: Error wrapping VST Plugin");
		//delete plugin;
		//plugin = NULL;
		IsLADSPALoaded = false;
		return NULL;
	}

	plugin->canBeReused=true; //reuse this instance in vst_instantiate
   	return plugin->GetDescriptor();
}


static LADSPA_Handle vst_instantiate(const LADSPA_Descriptor *desc, unsigned long rate) {
   //printf("LADSPA instantiate\n");

   LadspaPluginAdapter *plug = (LadspaPluginAdapter *) desc->ImplementationData;
   bool success = false;

   if (plug->canBeReused) {
   		log("reusing LADSPA wrapper instance\n");
   		success = plug->InitVSTEffect(rate);
   		plug->canBeReused=false;
   }
   else {
   		log("creating new LADSPA wrapper instance\n");
   		plug = new LadspaPluginAdapter();

   		success = plug->Init( 100000 + (rand() % 100000) );
   		if (success==false) {
			log("** LADSPA: Error initializing VST Plugin");
			delete plug;
			plug = NULL;
			return NULL;
		}

   		success = plug->InitVSTEffect(rate);
   }

   if (success==false) {
		log("** LADSPA: Error init VST Effect");
		delete plug;
		plug = NULL;
		return NULL;
   }

   return (LADSPA_Handle)plug;
}

static void vst_activate(LADSPA_Handle handle) {
	//show GUI here???
	//printf("LADSPA activate\n");
}

static void vst_connect_port(LADSPA_Handle handle, unsigned long port, LADSPA_Data *data) {
   //printf("LADSPA connectPort\n");
   LadspaPluginAdapter *inst = (LadspaPluginAdapter *) handle;
   inst->Connect_Port( port, data );
}

static void vst_run(LADSPA_Handle handle, unsigned long count) {
   LadspaPluginAdapter *inst = (LadspaPluginAdapter *) handle;
   inst->Run( count );
}

static void vst_run_adding(LADSPA_Handle handle, unsigned long count) {
   LadspaPluginAdapter *inst = (LadspaPluginAdapter *) handle;
   inst->Run_Adding( count );
}

static void vst_set_run_adding_gain(LADSPA_Handle handle, LADSPA_Data gain) {
	//printf("LADSPA setRunAddingAgain\n");
   	LadspaPluginAdapter *inst = (LadspaPluginAdapter *) handle;
   	inst->Set_Run_Adding_Gain( gain );
}

static void vst_deactivate(LADSPA_Handle handle) {
	//hide GUI here???
	//printf("LADSPA DEactivate\n");
}

static void vst_cleanup(LADSPA_Handle handle) {
	//printf("LADSPA cleanup\n");
   	LadspaPluginAdapter *inst = (LadspaPluginAdapter *) handle;
   	inst->Cleanup();
   	//delete inst; //this causes all hosts to coredump !!!
}



//***********************************************************************************
// This is the VST LADSPA plugin adapter
//

LadspaPluginAdapter::LadspaPluginAdapter() {
   canBeReused=false;
   mLib = NULL;
   mPrev = NULL;
   mEffect = NULL;
   memset( &mDesc, 0, sizeof( mDesc ) );
}

LadspaPluginAdapter::~LadspaPluginAdapter() {
   unsigned long i;

   if( mDesc.Label ) delete mDesc.Label;
   if( mDesc.Name ) delete mDesc.Name;
   if( mDesc.Maker ) delete mDesc.Maker;
   if( mDesc.Copyright ) delete mDesc.Copyright;

   for( i = 0; i < mDesc.PortCount; i++ ) {
      if( mDesc.PortNames ) {
         if( mDesc.PortNames[ i ] ) {
            free( (void *) mDesc.PortNames[ i ] );
         }
      }
   }

   if( mDesc.PortDescriptors ) delete [] mDesc.PortDescriptors;
   if( mDesc.PortNames ) delete [] mDesc.PortNames;
   if( mDesc.PortRangeHints ) delete [] mDesc.PortRangeHints;

   if( mPorts ) delete [] mPorts;
}

bool LadspaPluginAdapter::Init( unsigned long id ) {
   LADSPA_PortDescriptor *portdescriptors;
   LADSPA_PortRangeHint *hints;
   char **portnames;
   char temp[ 500 ];
   int port;
   int i;


   AEffect* effect = jvst_main(audioMaster);

   if( effect && effect->magic == kEffectMagic );
   else {
		log("** LADSPA: Error during jvst_main");
		return false;
   }

   mThunk = effect;
   mEffect = effect;
   mDesc.UniqueID = id;

   //check vst plugin type
   //we only accept types of effect (ladspa doesnt know midi, vst instruments, ...)
   VstInt32 cat = mEffect->dispatcher(effect, effGetPlugCategory , 0, 0, NULL, 0);
   log("LADSPA: vst plug is of category=%i", cat);
   if (cat==kPlugCategSynth || cat==kPlugCategOfflineProcess || cat==kPlugCategShell) {
   		log("** LADSPA Error: VST plug is of type Synth, OfflineProcess or Shell --> LADSPA doesnt support this!");
   		return false;
   }

   mDesc.Label = GetStr( effGetEffectName, "jVSTwRapper" );
   mDesc.Name = GetStr( effGetProductString, "jVSTwRapper" );
   mDesc.Maker = GetStr( effGetVendorString, "None" );
   mDesc.Copyright = GetStr( effVendorSpecific, "None" );

   if( !mDesc.Label || !mDesc.Name || !mDesc.Maker || !mDesc.Copyright ) return false;

   mDesc.PortCount = mEffect->numInputs + mEffect->numOutputs + mEffect->numParams;
   mDesc.PortDescriptors = portdescriptors = new LADSPA_PortDescriptor[ mDesc.PortCount ];
   mDesc.PortNames = portnames = new char *[ mDesc.PortCount ];
   mDesc.PortRangeHints = hints = new LADSPA_PortRangeHint[ mDesc.PortCount ];

   if( !mDesc.PortDescriptors || !mDesc.PortNames || !mDesc.PortRangeHints ) return false;

   mDesc.ImplementationData = (void *) this;

   mDesc.instantiate = vst_instantiate;
   mDesc.connect_port = vst_connect_port;
   mDesc.run = vst_run;
   mDesc.run_adding = vst_run_adding;
   mDesc.set_run_adding_gain = vst_set_run_adding_gain;
   mDesc.cleanup = vst_cleanup;
   mDesc.activate = vst_activate;
   mDesc.deactivate = vst_deactivate;

   log( "LADSPA name '%s'\n", mDesc.Name );
   log( "LADSPA maker '%s'\n", mDesc.Maker );
   log( "LADSPA label '%s'\n", mDesc.Label );
   log( "LADSPA crite '%s'\n", mDesc.Copyright );

   port = 0;

   for( i = 0; i < mEffect->numInputs; i++, port++ ) {
      portdescriptors[ port ] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;

      sprintf( temp, "Audio In %d", i );
      portnames[ port ] = strdup( temp );

      if( !portnames[ port ] ) return false;

      hints[ port ].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
   }

   for( i = 0; i < mEffect->numOutputs; i++, port++ ) {
      portdescriptors[ port ] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

      sprintf( temp, "Audio Out %d", i );

      portnames[ port ] = strdup( temp );
      if( !portnames[ port ] ) return false;

      hints[ port ].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
   }

   for( i = 0; i < mEffect->numParams; i++, port++ ) {
      float val;

      portdescriptors[ port ] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;

      memset( temp, 0, sizeof( temp ) );
      sprintf( temp, "_paramname_%d", i );

      mThunk->dispatcher( mEffect, effGetParamName, i, 0, temp, 0 );
      temp[ sizeof( temp ) - 1 ] = '\0';

      portnames[ port ] = strdup( temp );
      if( !portnames[ port ] ) return false;

      hints[ port ].HintDescriptor = LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      hints[ port ].LowerBound = 0.0f;
      hints[ port ].UpperBound = 1.0f;

      val = mThunk->getParameter( mEffect, i );

      if( val < 0.4f ) {
         if( val < 0.1f ) {
            if( val == 0.0f ) {
               hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_0;
            }
            else {
               hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_MINIMUM;
            }
         }
         else {
            hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_LOW;
         }
      }
      else {
         if( val < 0.6f ) {
            hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_MIDDLE;
         }
         else {
            if( val > 0.9f ) {
               if( val == 1.0f ) {
                  hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_1;
               }
               else {
                  hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_MAXIMUM;
               }
            }
            else {
               hints[ port ].HintDescriptor |= LADSPA_HINT_DEFAULT_HIGH;
            }
         }
      }
   }

   return true;
}

bool LadspaPluginAdapter::InitVSTEffect( unsigned long rate ) {
   mBlockSize = 0;
   mFirstParam = mEffect->numInputs + mEffect->numOutputs;
   mNumPorts = mFirstParam + mEffect->numParams;
   mGain = 1.0f;

   mPorts = new LADSPA_Data *[ mNumPorts ];
   if( mPorts == NULL ) return false;

   memset( mPorts, 0, mNumPorts * sizeof(LADSPA_Data *));

   mThunk->dispatcher( mEffect, effOpen, 0, 0, NULL, 0 );
   mThunk->dispatcher( mEffect, effMainsChanged, 0, 0, NULL, 0 );
   mThunk->dispatcher( mEffect, effSetSampleRate, 0, 0, NULL, (float) rate );
   mThunk->dispatcher( mEffect, effSetBlockSize, 0, 64, NULL, 0 );
   mThunk->dispatcher( mEffect, effMainsChanged, 0, 1, NULL, 0 );

   return true;
}

LadspaPluginAdapter *LadspaPluginAdapter::GetPrev() {
   return mPrev;
}

void LadspaPluginAdapter::SetPrev( LadspaPluginAdapter *prev ) {
   mPrev = prev;
}

LADSPA_Descriptor *LadspaPluginAdapter::GetDescriptor() {
   return &mDesc;
}

char *LadspaPluginAdapter::GetStr( VstInt32 opcode, char *dflt ) {
   char str[ 256 ];
   int len;
   int i;
   int dot;
   int slash;

   str[255]='\0';
   if( mThunk->dispatcher( mEffect, opcode, 0, 0, str, 0 ) )  {
      if( strlen( str ) > 0 ) return strdup( str );
   }

   len = (int) strlen( dflt );
   dot = -1;
   slash = -1;

   for( i = len - 1; i >= 0; i-- ) {
      if( dot < 0 && dflt[ i ] == '.' ) {
         dot = i;
         continue;
      }

      if( dflt[ i ] != '/' && dflt[ i ] != '\\' ) continue;

      slash = i;
      break;
   }

   if( slash > 0 ) {
      strncpy( str, &dflt[ slash + 1 ], len - slash - 1 );
      str[ len - slash - 1 ] = '\0';
   }
   else {
      strcpy( str, dflt );
   }

   if( dot > 0 ) str[ dot - slash - 1 ] = '\0';

   return strdup( str );
}

void LadspaPluginAdapter::Cleanup() {
   mThunk->dispatcher( mEffect, effClose, 0, 0, NULL, 0 );
}

void LadspaPluginAdapter::Connect_Port( unsigned long port, LADSPA_Data *data ) {
   mPorts[ port ] = data;
}

void LadspaPluginAdapter::Run( unsigned long count ) {
   Set_Parameters();

   if( mBlockSize != count ) {
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 0, NULL, 0);
      mThunk->dispatcher( mEffect, effSetBlockSize, 0, count, NULL, 0);
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 1, NULL, 0);

      mBlockSize = count;
   }

   mThunk->processReplacing( mEffect, &mPorts[ 0 ], &mPorts[ mEffect->numInputs ], count );
}

void LadspaPluginAdapter::Run_Adding( unsigned long count ) {
   LADSPA_Data *out[ 2 ];
   int index;
   int ich;
   int och;

   Set_Parameters();

   if( mBlockSize != count ) {
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 0, NULL, 0);
      mThunk->dispatcher( mEffect, effSetBlockSize, 0, count, NULL, 0);
      mThunk->dispatcher( mEffect, effMainsChanged, 0, 1, NULL, 0);

      mBlockSize = count;
   }

   out[ 0 ] = new LADSPA_Data[ count ];
   if( !out[ 0 ] ) return;

   out[ 1 ] = new LADSPA_Data[ count ];
   if( !out[ 0 ] || !out[ 1 ] ) return;

   mThunk->processReplacing( mEffect, &mPorts[ 0 ], &out[ 0 ], count );

   for( och = mEffect->numInputs, ich = 0; och < mEffect->numInputs + mEffect->numOutputs ; och++, ich++ ) {
      for( index = 0; index < (int) count; index++ ) {
         mPorts[ och ][ index ] += out[ ich ][ index ] * mGain;
      }
   }

   delete [] out[ 0 ];
   delete [] out[ 1 ];
}

void LadspaPluginAdapter::Set_Run_Adding_Gain( LADSPA_Data gain ) {
   mGain = gain;
}

void LadspaPluginAdapter::Set_Parameters() {
   int index;

   for( index = mFirstParam; index < mNumPorts; index++ ) {
      if( mPorts[index] ) mThunk->setParameter( mEffect, index - mFirstParam, *mPorts[ index ] );
   }
}

