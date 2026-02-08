#include "MainComponent.h"            //MainComponent.cpp

MainComponent::MainComponent()
{
    // Added Multiple Player GUIs (TASK 1 bonus part)

    addAndMakeVisible(player1);
    addAndMakeVisible(player2);

    // Mix Button Setup
    mixIcon = std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(BinaryData::icons8synchronize100_svg, BinaryData::icons8synchronize100_svgSize));
    mixButton.setImages(mixIcon.get(), mixIcon.get(), mixIcon.get());
    mixButton.setColour(juce::DrawableButton::backgroundColourId, juce::Colours::transparentBlack);
    mixButton.setColour(juce::DrawableButton::backgroundOnColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(mixButton);

    mixlabel.setText("Off", juce::dontSendNotification);
    mixlabel.setJustificationType(juce::Justification::centred);
    mixlabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(mixlabel);


    // Main speed slider

    mainspeedslider.setRange(0.01, 2.0, 0.01);
    mainspeedslider.setValue(1.0);
    mainspeedslider.addListener(this);
    addAndMakeVisible(mainspeedslider);

    mainspeedslider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mainspeedslider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    mainspeedslider.setColour(juce::Slider::trackColourId, juce::Colours::cyan.darker(1.0f));
    mainspeedlabel.setText("1.00x", juce::dontSendNotification);
    mainspeedlabel.setJustificationType(juce::Justification::centredLeft);
    mainspeed.setText("Global Speed", juce::dontSendNotification);
    mainspeed.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(mainspeed);
    addAndMakeVisible(mainspeedlabel);


    // Mix Button Functionality

    mixButton.onClick = [this]()
        {
            Mix = !Mix;
            if (Mix)
            {
                mixlabel.setText("On", juce::dontSendNotification);
            }
            else
            {
                mixlabel.setText("Off", juce::dontSendNotification);
            }
        };

    // Set initial window size and audio channels
    setSize(1400, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Added second player prepareToPlay for task 1 in the bonus part

    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    //Check if Mix is enabled

    if (!Mix)
    {
        if (player1.isCurrentlyPlaying())
        {
            player1.getNextAudioBlock(bufferToFill);
        }
        else
        {
            player2.getNextAudioBlock(bufferToFill);
        }
    }
    else
    {
        // Create temporary buffer for second player's output

        juce::AudioBuffer<float> tempBuffer(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
        juce::AudioSourceChannelInfo tempInfo(&tempBuffer, 0, bufferToFill.numSamples);
        tempBuffer.clear();

        // Get audio from both players

        player1.getNextAudioBlock(bufferToFill);
        player2.getNextAudioBlock(tempInfo);

        // Mix the two by adding player2’s buffer into player1’s buffer

        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++)
        {
            bufferToFill.buffer->addFrom(channel, bufferToFill.startSample, tempBuffer, channel, 0, bufferToFill.numSamples);
        }
    }
}

void MainComponent::releaseResources()
{
    // Added second player releaseResources for task 1 in the bonus part

    player1.releaseResources();
    player2.releaseResources();
}


void MainComponent::resized()
{
    auto area = getLocalBounds();
    auto halfWidth = area.getWidth() / 2;

    player1.setBounds(0, 0, halfWidth, area.getHeight());
    player2.setBounds(halfWidth, 0, halfWidth, area.getHeight());

    const int mixButtonSize = 60;
    const int mixLabelHeight = 25;
    const int bottomMargin = 30;
    int centerX = area.getCentreX();

    mixButton.setBounds(centerX - mixButtonSize / 2, area.getBottom() - mixButtonSize - bottomMargin, mixButtonSize, mixButtonSize);
    mixlabel.setBounds(centerX - 20, mixButton.getBottom() + 5, 40, mixLabelHeight);

    const int mainsliderWidth = 200;
    const int mainsliderHeight = 20;

    mainspeedslider.setBounds((mainsliderWidth / 2) + 230, 25, mainsliderWidth, mainsliderHeight);
    mainspeed.setBounds(centerX - (mainsliderWidth / 2), 5, mainsliderWidth, 20);
    mainspeed.setJustificationType(juce::Justification::centred);
    mainspeedlabel.setBounds(mainspeedslider.getRight() + 5, 25, 50, mainsliderHeight);
    mainspeed.setBounds((area.getWidth() / 4), 0, (area.getWidth() / 4), 20);
}


void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    // Use comparison (==). If mainspeedslider changed, push its value to both player speed sliders.
    if (slider == &mainspeedslider)
    {
        double ratio = mainspeedslider.getValue();
        mainspeedlabel.setText(juce::String::formatted("%.2fx", ratio), juce::dontSendNotification);

        // Update player UI sliders so each PlayerGUI receives the change and updates playback.

        player1.getSpeedSlider().setValue(ratio, juce::sendNotification);
        player2.getSpeedSlider().setValue(ratio, juce::sendNotification);
    }
}

