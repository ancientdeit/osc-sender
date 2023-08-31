/*
  ==============================================================================

    SliderWithOsc.h
    Created: 30 Aug 2023 9:57:17pm
    Author:  Marek Kabacinski

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "OscSender.h"

class SliderWithOsc : public juce::Component,
                      public juce::Slider::Listener,
                      public juce::TextEditor::Listener
{
public:
    SliderWithOsc(OSC_SenderAudioProcessor* juceProcessor, int paramId);

    void setValue(float value);
    float getValue();

    juce::Slider& getSlider();

    void resized() override;

    // Implement the Slider::Listener methods
    void sliderValueChanged(juce::Slider* slider) override;

    // Implement the TextEditor::Listener methods
    void textEditorTextChanged(juce::TextEditor& editor) override;

private:
    juce::Slider slider;
    juce::TextEditor oscAddressEditor;

    OSC_SenderAudioProcessor* processor;
    int id;
    OscSender& oscSender;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
};