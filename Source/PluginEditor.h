/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SliderWithOsc.h"
#include "OscSender.h"

#include <vector>

//==============================================================================
/**
*/
class OSC_SenderAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::TextEditor::Listener
{
public:
    OSC_SenderAudioProcessorEditor (OSC_SenderAudioProcessor&);
    ~OSC_SenderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // Implement the TextEditor::Listener methods
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorFocusLost(juce::TextEditor& editor) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OSC_SenderAudioProcessor& audioProcessor;

    juce::TextEditor m_AddressEditor;
    juce::TextEditor m_PortEditor;
    std::vector<std::unique_ptr<SliderWithOsc>> m_Sliders;

    OscSender& oscSender;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC_SenderAudioProcessorEditor)
};
