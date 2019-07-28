package jvst.test;

import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.valueobjects.MidiKeyName;
import jvst.wrapper.valueobjects.MidiProgramCategory;
import jvst.wrapper.valueobjects.MidiProgramName;
import jvst.wrapper.valueobjects.VSTEvents;
import jvst.wrapper.valueobjects.VSTParameterProperties;
import jvst.wrapper.valueobjects.VSTPatchChunkInfo;
import jvst.wrapper.valueobjects.VSTPinProperties;
import jvst.wrapper.valueobjects.VSTSpeakerArrangement;
import jvst.wrapper.valueobjects.VSTSpeakerProperties;
import jvst.wrapper.valueobjects.VSTTimeInfo;
import jvst.wrapper.valueobjects.VSTVariableIO;

public class TestPlugin extends VSTPluginAdapter {

	public TestPlugin(long Wrapper) {
		super(Wrapper);
		
		this.setNumInputs(1);
		this.setNumOutputs(1);
		
	}

	public int canDo(String feature) {
		log("Cando: " + feature);
		return 0;
	}

	
	//test calls to HOST
	public void resume() {
		  setUniqueID(32434);
		  hasVu(true);
		  hasClip(true);
		  canMono(true);
		  canProcessReplacing(true);
		  setRealtimeQualities(234);
		  setOfflineQualities(235);
		  setInitialDelay(22);
		  programsAreChunks(false);
		  getSampleRate();
		  getBlockSize();
		  setParameterAutomated(2, 22.22f);
		  getMasterVersion();
		  getCurrentUniqueId();
		  masterIdle();
		  isInputConnected(1);
		  isOutputConnected(2);
		
		  
		  canHostDo("weiss nich");
		  sizeWindow(222, 333);
		  ioChanged();
		  getTimeInfo(21332);
		  sendVstEventsToHost(new VSTEvents());
		  needIdle();
		  updateSampleRate();
		  updateBlockSize();
		  getAutomationState();
		  getCurrentProcessLevel();
		  getDirectory();
		  getHostProductString();
		  getHostVendorVersion();
		  getHostVendorString();
		  getHostLanguage();
		  getInputLatency();
		  getOutputLatency();
		  getNumAutomatableParameters();

		  getParameterQuantization();
		  hasExternalBuffer(false);
		  isSynth(true);
		  noTail(true);

		  setOutputSamplerate(44100.f);
		  tempoAt(12);
		  updateDisplay();
		  wantAsyncOperation(false);
		  wantEvents(2);
		  willProcessReplacing();
		  beginEdit(333);
		  endEdit(222);
		  setPanLaw (1, 0.9988f);
		  beginLoadBank (new VSTPatchChunkInfo());
		  beginLoadProgram (new VSTPatchChunkInfo());
		  copySpeaker(new VSTSpeakerProperties(), new VSTSpeakerProperties());
		  matchArrangement(new VSTSpeakerArrangement(), new VSTSpeakerArrangement());
		  canDoubleReplacing (true);
	}
		
	//those here mix toPlug and toHost calls. 
	//was just easier to write and doesnt matter since all overwritten 
	//methods call their super class...
	
	public boolean setBypass(boolean value) {return false;}
	public boolean string2Parameter(int index, String value) {return false;}
	public String getProgramNameIndexed(int category, int index) {return "getProgramNameIndexed";}
	public String getProductString() {return "getProductString";}
	public String getVendorString() {return "getVendorString";}
	public int getPlugCategory() {return TestPlugin.PLUG_CATEG_EFFECT;}
	public void setParameter(int index, float value) {}
	public float getParameter(int index) {return 0.99f;}
	public void processReplacing(float[][] inputs, float[][] outputs, int sampleFrames) {}
	public int getProgram() {return 1;}
	public void setProgram(int index) {}
	public void setProgramName(String name) {}
	public String getProgramName() {return "getProgramName";}
	public String getParameterName(int index) {return "getParameterName";}
	public String getParameterDisplay(int index) {return "getParameterDisplay";}
	public String getParameterLabel(int index) {return "getParameterLabel";}
	public int getNumPrograms() {return 1;}
	public int getNumParams() {return 1;}

	public boolean beginSetProgram() {return super.beginSetProgram();}
	public boolean canParameterBeAutomated(int index) {return super.canParameterBeAutomated(index);}
	public void close() {super.close();	}
	public boolean copyProgram(int destination) {return super.copyProgram(destination);}
	public boolean endSetProgram() {return super.endSetProgram();}
	public int fxIdle() {return super.fxIdle();}
	public float getChannelParameter(int channel, int index) {return super.getChannelParameter(channel, index);}
	public int getChunk(byte[][] data, boolean isPreset) {return super.getChunk(data, isPreset);}
	public int getCurrentMidiProgram(int channel, MidiProgramName currentProgram) {return super.getCurrentMidiProgram(channel, currentProgram);}
	public String getEffectName() {return "TestPlugin";}
	public String getErrorText() {return "ERROR text";}
	public int getGetTailSize() {return super.getGetTailSize();}
	public VSTPinProperties getInputProperties(int index) {return new VSTPinProperties();}
	public boolean getMidiKeyName(int channel, MidiKeyName keyName) {return super.getMidiKeyName(channel, keyName);}
	public int getMidiProgramCategory(int channel, MidiProgramCategory category) {return super.getMidiProgramCategory(channel, category);}
	public int getMidiProgramName(int channel, MidiProgramName midiProgramName) {return super.getMidiProgramName(channel, midiProgramName);}
	public int getNextShellPlugin(String name) {return super.getNextShellPlugin(name);}
	public int getNumCategories() {return super.getNumCategories();}
	public int getNumMidiInputChannels() {return super.getNumMidiInputChannels();}
	public int getNumMidiOutputChannels() {return super.getNumMidiOutputChannels();}

	public VSTPinProperties getOutputProperties(int index) {return new VSTPinProperties();}
	public VSTParameterProperties getParameterProperties(int index) {return new VSTParameterProperties();}
	public int getVendorVersion() {return super.getVendorVersion();}
	public int getVstVersion() {return super.getVstVersion();}
	public float getVu() {return super.getVu();}
	public boolean hasMidiProgramsChanged(int channel) {return super.hasMidiProgramsChanged(channel);}
	public void inputConnected(int index, boolean state) {super.inputConnected(index, state);}
	public boolean keysRequired() {return super.keysRequired();}
	public void open() {super.open();}
	public void outputConnected(int index, boolean state) {super.outputConnected(index, state);}

	public void process(float[][] inputs, float[][] outputs, int sampleFrames) {super.process(inputs, outputs, sampleFrames);}
	public void processDoubleReplacing(double[][] inputs, double[][] outputs, int sampleFrames) {super.processDoubleReplacing(inputs, outputs, sampleFrames);}
	public int processEvents(VSTEvents e) {	return super.processEvents(e);}
	public boolean processVariableIo(VSTVariableIO vario) {return super.processVariableIo(vario);}
	public int reportCurrentPosition() {return super.reportCurrentPosition();}
	public float[] reportDestinationBuffer() {return new float[2];}
	
	public void setBlockSize(int blockSize) {super.setBlockSize(blockSize);}
	public void setBlockSizeAndSampleRate(int blockSize, float sampleRate) {super.setBlockSizeAndSampleRate(blockSize, sampleRate);}
	public int setChunk(byte[] data, int byteSize, boolean isPreset) {		return super.setChunk(data, byteSize, isPreset);}
	public boolean setProcessPrecision(int precision) {return super.setProcessPrecision(precision);}

	public boolean setSpeakerArrangement(VSTSpeakerArrangement pluginInput, VSTSpeakerArrangement pluginOutput) {return super.setSpeakerArrangement(pluginInput, pluginOutput);}
	public boolean getSpeakerArrangement(VSTSpeakerArrangement pluginInput, VSTSpeakerArrangement pluginOutput) {
		pluginInput.setType(999);
		pluginInput.setNumChannels(1);
		pluginInput.setSpeakers(new VSTSpeakerProperties[]{new VSTSpeakerProperties()});
		pluginOutput.setSpeakers(new VSTSpeakerProperties[]{new VSTSpeakerProperties()});
		pluginOutput.setType(888); 
		pluginOutput.setNumChannels(1);
		return true;
	}
	public int setTotalSampleToProcess(int value) {return super.setTotalSampleToProcess(value);}
	public int startProcess() {return super.startProcess();}
	public int stopProcess() {return super.stopProcess();}
	public void suspend() {	super.suspend();}
}
