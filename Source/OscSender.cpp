/*
  ==============================================================================

    OscSender.cpp
    Created: 30 Aug 2023 9:46:28pm
    Author:  Marek Kabacinski

  ==============================================================================
*/

#include "OscSender.h"

OscSender::OscSender()
{
    for (int i = 0; i < 16; ++i)
        addressToSend.push_back(juce::String("/track/" + juce::String(i + 1) + "/pan"));
}

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

void OscSender::sendMessageWithValue(float value, int id)
{
    if(addressToSend[id] == "")
        return;
    juce::OSCMessage oscMessage(addressToSend[id]);
    oscMessage.addFloat32(value);
    sender.send(oscMessage);
}

void OscSender::setAddress(juce::String address, int id)
{
    addressToSend[id] = address;
}

juce::String OscSender::getAddress(int id)
{
    return addressToSend[id];
}