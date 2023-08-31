/*
  ==============================================================================

    OscSender.h
    Created: 30 Aug 2023 9:46:28pm
    Author:  Marek Kabacinski

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscSender
{
public:
    static OscSender& getInstance()
    {
        static OscSender instance;
        return instance;
    }

    OscSender(OscSender const&) = delete;
    void operator=(OscSender const&) = delete;

    void setupSender(const juce::String& ipAddress, int portNumber);

    void sendCustomMessage(const juce::String& address, const juce::String& message);

    void sendCustomMessageWithValue(const juce::String& address, float value);

    void sendMessageWithValue(float value, int id);

    void setAddress(juce::String address, int id);

    juce::String getAddress(int id);

private:
    OscSender();
    ~OscSender() {}

    std::vector<juce::String> addressToSend;
    juce::OSCSender sender;
};