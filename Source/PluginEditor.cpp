/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OSC_SenderAudioProcessorEditor::OSC_SenderAudioProcessorEditor (OSC_SenderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), oscSender(OscSender::getInstance())
{
    // Implementation of m_PortEditor
    m_PortEditor.setInputFilter(new juce::TextEditor::LengthAndCharacterRestriction(5, "0123456789"), true);
    m_PortEditor.setText("8001");
    addAndMakeVisible(&m_PortEditor);

    // Implementation of m_AddressEditor
    m_AddressEditor.setInputFilter(new juce::TextEditor::LengthAndCharacterRestriction(15, "0123456789."), true);
    m_AddressEditor.setText("127.0.0.1");
    addAndMakeVisible(&m_AddressEditor);

    oscSender.setupSender(m_AddressEditor.getText(), m_PortEditor.getText().getIntValue());

    // Implementation of m_Sliders
    for (int i = 0; i < 16; ++i)
    {
        m_Sliders.push_back(std::make_unique<SliderWithOsc>(&audioProcessor, i));
        addAndMakeVisible(m_Sliders.back().get());
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 800);
}

OSC_SenderAudioProcessorEditor::~OSC_SenderAudioProcessorEditor()
{
}

//==============================================================================
void OSC_SenderAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void OSC_SenderAudioProcessorEditor::resized()
{
    // Position AddressEditor and PortEditor at the top of the screen
    m_AddressEditor.setBounds(10, 10, getWidth() - 20, 20);
    m_PortEditor.setBounds(10, 40, getWidth() - 20, 20);

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    // Define grid properties
    grid.templateRows    = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };

    // Add items to the grid
    for (auto& slider : m_Sliders)
        grid.items.add(juce::GridItem (*slider));

    // Perform the layout
    grid.performLayout (getLocalBounds().withTrimmedTop(70));
}

void OSC_SenderAudioProcessorEditor::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    if (&editor == &m_AddressEditor || &editor == &m_PortEditor)
    {
        oscSender.setupSender(m_AddressEditor.getText(), m_PortEditor.getText().getIntValue());
    }
}
void OSC_SenderAudioProcessorEditor::textEditorFocusLost(juce::TextEditor& editor)
{
    if (&editor == &m_AddressEditor || &editor == &m_PortEditor)
    {
        oscSender.setupSender(m_AddressEditor.getText(), m_PortEditor.getText().getIntValue());
    }
}