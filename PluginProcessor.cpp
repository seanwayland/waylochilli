/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
WaylochilliAudioProcessor::WaylochilliAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

WaylochilliAudioProcessor::~WaylochilliAudioProcessor()
{
}

int waylotrans = 0;
int waylochili = 60;
int playing[127];

//==============================================================================
const String WaylochilliAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaylochilliAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaylochilliAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaylochilliAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaylochilliAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaylochilliAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaylochilliAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaylochilliAudioProcessor::setCurrentProgram (int index)
{
}

const String WaylochilliAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaylochilliAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WaylochilliAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WaylochilliAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaylochilliAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaylochilliAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    MidiMessage m;
    
    {
        buffer.clear();

        
        
        MidiBuffer processedMidi;
        int time;
        MidiMessage m;
        
        for (MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
        {
            if (waylochili > 126)
            {
                waylochili = 48;
            }
            if (m.isNoteOn() && m.getNoteNumber() < 37 )
                
                
            {
                waylochili = m.getNoteNumber() + 24 ;
                m = MidiMessage::noteOff(m.getChannel(), m.getNoteNumber(), m.getVelocity());
                
            }
            
            
            else if (m.isNoteOff() && m.getNoteNumber() < 37  )
                
                
            {
                waylotrans = 0;
                m = MidiMessage::noteOff(m.getChannel(), m.getNoteNumber(), m.getVelocity());
            }
            
            else if (m.isNoteOn() && m.getNoteNumber() >= 37)
                
            {
                waylochili = waylochili + m.getNoteNumber() - 60;
                int NewNote = waylochili + waylotrans ;
                playing[m.getNoteNumber()]= NewNote;
                m = MidiMessage::noteOn(m.getChannel(), NewNote , m.getVelocity());
                
            }
            else if (m.isNoteOff() && m.getNoteNumber() >= 37)
                
            {
                int NewNote = playing[m.getNoteNumber()];
                playing[m.getNoteNumber()] = NULL;
                m = MidiMessage::noteOff(m.getChannel(), NewNote , m.getVelocity());
                
            }
            
            
            else if (m.isAftertouch())
            {
            }
            else if (m.isPitchWheel())
            {
            }
            
            processedMidi.addEvent (m, time);
        }
        
        midiMessages.swapWith (processedMidi);
    }
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool WaylochilliAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WaylochilliAudioProcessor::createEditor()
{
    return new WaylochilliAudioProcessorEditor (*this);
}

//==============================================================================
void WaylochilliAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaylochilliAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaylochilliAudioProcessor();
}
