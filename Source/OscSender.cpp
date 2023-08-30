/*
  ==============================================================================

    OscSender.cpp
    Created: 30 Aug 2023 9:46:28pm
    Author:  Marek Kabacinski

  ==============================================================================
*/

#include "OscSender.h"

void OscSender::setupSender(const juce::String& ipAddress, int portNumber)
{
    if (!sender.connect(ipAddress, portNumber))
    {
        throw std::runtime_error("Failed to connect to the OSC server at " + ipAddress.toStdString() + ":" + std::to_string(portNumber));
    }
}

void OscSender::sendCustomMessage(const juce::String& address, const juce::String& message)
{
    juce::OSCMessage oscMessage(address);
    oscMessage.addString(message);
    sender.send(oscMessage);
}

void OscSender::sendCustomMessageWithValue(const juce::String& address, float value)
{
    juce::OSCMessage oscMessage(address);
    oscMessage.addFloat32(value);
    sender.send(oscMessage);
}