/*
  ==============================================================================

    SliderWithOsc.cpp
    Created: 30 Aug 2023 9:57:17pm
    Author:  Marek Kabacinski

  ==============================================================================
*/

#include "SliderWithOsc.h"

SliderWithOsc::SliderWithOsc() : oscSender(OscSender::getInstance())
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setRange(0.0, 1.0, 0.01);
    slider.addListener(this);

    addAndMakeVisible(oscAddressEditor);
    oscAddressEditor.setText("/default/osc");
    oscAddressEditor.addListener(this);

    oscAddress = oscAddressEditor.getText();
}

SliderWithOsc::SliderWithOsc(juce::String address, juce::AudioProcessor* juceProcessor, int paramId) : oscSender(OscSender::getInstance()), processor(juceProcessor), id(paramId)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setRange(0.0, 1.0, 0.01);
    slider.addListener(this);

    addAndMakeVisible(oscAddressEditor);
    oscAddressEditor.setText(address);
    oscAddressEditor.addListener(this);

    oscAddress = oscAddressEditor.getText();
}

void SliderWithOsc::resized()
{
    auto area = getLocalBounds();
    slider.setBounds(area.removeFromTop(2 * area.getHeight() / 3));
    oscAddressEditor.setBounds(area);
}

void SliderWithOsc::sliderValueChanged(juce::Slider* slider)
{
    oscSender.sendCustomMessageWithValue(oscAddress, slider->getValue());
    processor->setParameterNotifyingHost(id, slider->getValue());
}

void SliderWithOsc::textEditorTextChanged(juce::TextEditor& editor)
{
    oscAddress = editor.getText();
}