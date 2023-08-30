/*
  ==============================================================================

    SliderWithOsc.h
    Created: 30 Aug 2023 9:57:17pm
    Author:  Marek Kabacinski

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "OscSender.h"

class SliderWithOsc : public juce::Component, public juce::Slider::Listener, public juce::TextEditor::Listener
{
public:
    SliderWithOsc();
    SliderWithOsc(juce::String address, juce::AudioProcessor* juceProcessor, int paramId);

    void resized() override;

    // Implement the Slider::Listener methods
    void sliderValueChanged(juce::Slider* slider) override;

    // Implement the TextEditor::Listener methods
    void textEditorTextChanged(juce::TextEditor& editor) override;

private:
    juce::Slider slider;
    juce::TextEditor oscAddressEditor;
    juce::String oscAddress;

    juce::AudioProcessor* processor;
    int id;
    OscSender& oscSender;
};