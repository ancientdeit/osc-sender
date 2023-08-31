/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OSC_SenderAudioProcessor::OSC_SenderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
, parameters(*this, nullptr), oscSender(OscSender::getInstance())
{
    // Init the m_Parameters here
    for (int i = 0; i < 16; ++i)
    {
        auto param = parameters.createAndAddParameter(std::make_unique<juce::AudioParameterFloat>("track" + juce::String(i + 1), "Track " + juce::String(i + 1), 0.0f, 1.0f, 0.5f));
        parameters.addParameterListener("track" + juce::String(i + 1), this);
    }
    parameters.state = juce::ValueTree("savedParams");
}

OSC_SenderAudioProcessor::~OSC_SenderAudioProcessor()
{
    for (int i = 0; i < 16; ++i)
        parameters.removeParameterListener("track" + juce::String(i + 1), this);
}

//==============================================================================
const juce::String OSC_SenderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OSC_SenderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OSC_SenderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OSC_SenderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OSC_SenderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OSC_SenderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OSC_SenderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OSC_SenderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OSC_SenderAudioProcessor::getProgramName (int index)
{
    return {};
}

void OSC_SenderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OSC_SenderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OSC_SenderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OSC_SenderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void OSC_SenderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool OSC_SenderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OSC_SenderAudioProcessor::createEditor()
{
    return new OSC_SenderAudioProcessorEditor (*this);
}

//==============================================================================
void OSC_SenderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OSC_SenderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void OSC_SenderAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    for (int i = 0; i < 16; ++i) {
        if(parameterID == "track" + juce::String(i + 1)) {
            oscSender.sendMessageWithValue(newValue, i);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OSC_SenderAudioProcessor();
}
