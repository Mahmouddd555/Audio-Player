#pragma once                           // MainComponent.h
#include <JuceHeader.h>
#include "PlayerGUI.h"
#include "PlayerAudio.h"

class MainComponent : public juce::AudioAppComponent,
    public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;


    // Implement the pure virtual function from juce::Slider::Listener;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    PlayerGUI player1;
    PlayerGUI player2;

    bool Mix = false;
    std::unique_ptr<juce::Drawable> mixIcon;

    juce::Slider mainspeedslider;
    juce::Label mainspeedlabel{ {}, "Main Speed" };
    juce::Label mainspeed{ {}, "Global Speed" };
    juce::Label mixlabel{ {}, "Off" };
    juce::DrawableButton mixButton{ "Mix", juce::DrawableButton::ImageFitted };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
