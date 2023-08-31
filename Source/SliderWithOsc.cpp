/*
  ==============================================================================

    SliderWithOsc.cpp
    Created: 30 Aug 2023 9:57:17pm
    Author:  Marek Kabacinski

  ==============================================================================
*/

#include "SliderWithOsc.h"

SliderWithOsc::SliderWithOsc(OSC_SenderAudioProcessor* juceProcessor, int paramId) : oscSender(OscSender::getInstance()), processor(juceProcessor), id(paramId)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setRange(0.0, 1.0, 0.01);
    slider.addListener(this);

    addAndMakeVisible(oscAddressEditor);
    oscAddressEditor.setText(oscSender.getAddress(id));
    oscAddressEditor.addListener(this);

    oscSender.setAddress(oscAddressEditor.getText(), id);

    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->parameters, "track" + juce::String(id + 1), slider);
}

void SliderWithOsc::setValue(float value)
{
    slider.setValue(value);
}

float SliderWithOsc::getValue()
{
    return slider.getValue();
}

juce::Slider& SliderWithOsc::getSlider()
{
    return slider;
}

void SliderWithOsc::resized()
{
    auto area = getLocalBounds();
    slider.setBounds(area.removeFromTop(2 * area.getHeight() / 3));
    oscAddressEditor.setBounds(area);
}

void SliderWithOsc::sliderValueChanged(juce::Slider* slider)
{
    processor->setParameterNotifyingHost(id, slider->getValue());
}

void SliderWithOsc::textEditorTextChanged(juce::TextEditor& editor)
{
    oscSender.setAddress(editor.getText(), id);
}