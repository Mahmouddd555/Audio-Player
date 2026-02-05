#pragma once                           // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"

class WaveformDisplay : public juce::Component,
    public juce::ChangeListener
{
public:
    WaveformDisplay()
        : thumbnailCache(5),
        thumbnail(512, formatManager, thumbnailCache)
    {
        formatManager.registerBasicFormats(); 
        thumbnail.addChangeListener(this);
    }

    void loadURL(const juce::File& file)
    {
        thumbnail.clear();
        thumbnail.setSource(new juce::FileInputSource(file));
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat();
        float cornerSize = 15.0f;

        g.fillAll(juce::Colours::transparentBlack);

        juce::Path clipPath;
        clipPath.addRoundedRectangle(area, cornerSize);
        g.reduceClipRegion(clipPath);

        if (thumbnail.getTotalLength() > 0.0)
        {
            juce::ColourGradient waveGrad(juce::Colours::mediumpurple, 0, 0,
                juce::Colours::purple.withAlpha(0.8f), 0, (float)getHeight(), false);
            g.setGradientFill(waveGrad);
            thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
        }
        else
        {
            g.setColour(juce::Colours::white.withAlpha(0.4f));
            g.setFont(juce::Font(18.0f, juce::Font::italic));
            g.drawFittedText("No file loaded", getLocalBounds(), juce::Justification::centred, 1);
        }

        g.setColour(juce::Colours::deeppink);
        auto x = (float)(getWidth() * position);
        g.drawLine(x, 0.0f, x, (float)getHeight(), 2.0f);
    }

    void changeListenerCallback(juce::ChangeBroadcaster*) override
    {
        repaint();
    }

    void setPosition(double pos) { position = pos; repaint(); }

private:
    juce::AudioFormatManager formatManager;      
    juce::AudioThumbnailCache thumbnailCache;     
    juce::AudioThumbnail thumbnail;               
    double position = 0.0;
};


class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel // <-- Add this base class
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void paint(juce::Graphics& g) override;
    bool isCurrentlyPlaying() const;

    // ListBoxModel overrides
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent& e) override;
    void selectedRowsChanged(int lastRowSelected) override {}

    juce::Slider& getSpeedSlider() { return speedSlider; }

private:
    PlayerAudio playerAudio;
    juce::ListBox playlistBox;
    WaveformDisplay wavedis;

    juce::DrawableButton loadButton{ "Load Files", juce::DrawableButton::ImageFitted };
    juce::DrawableButton restartButton{ "Restart", juce::DrawableButton::ImageFitted };
    juce::DrawableButton stopButton{ "Stop", juce::DrawableButton::ImageFitted };
    juce::DrawableButton muteButton{ "Mute", juce::DrawableButton::ImageFitted };
    juce::DrawableButton playButton{ "Play", juce::DrawableButton::ImageFitted };
    juce::DrawableButton pauseButton{ "Pause", juce::DrawableButton::ImageFitted };
    juce::DrawableButton tostartButton{ "Tostart",juce::DrawableButton::ImageFitted };
    juce::DrawableButton toendButton{ "Toend", juce::DrawableButton::ImageFitted };
    juce::DrawableButton loopButton{ "Loop", juce::DrawableButton::ImageFitted };
    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::ProgressBar progressBar{ progress };
    juce::Label name;
    juce::String fileName;
    juce::Label artist;
    juce::String fileArtist;
    juce::Slider positionSlider;
    juce::Label positionLabel;
    juce::Label volumeLabel;
    juce::Label volume;
	juce::Label speedLabel;
    juce::Label speed;
	juce::Label durationSong;
	juce::Label abLoopLabel;
	juce::String fileDuration;
    juce::DrawableButton setAButton{ "Set A", juce::DrawableButton::ImageFitted };
    juce::DrawableButton setBButton{ "Set B", juce::DrawableButton::ImageFitted };
    juce::DrawableButton abLoopButton{ "A-B Loop", juce::DrawableButton::ImageFitted };
    juce::DrawableButton addTolist{ "Add Files", juce::DrawableButton::ImageFitted };
    juce::SystemTrayIconComponent pp;

    double pointA = -1.0;
    double pointB = -1.0;
    bool abLoopingEnabled = false;

    std::unique_ptr<juce::FileChooser> fileChooser;

    float savedVolume{ 5.0f };
    bool isMuted{ false };
    double lastPosition{ 0.0f };
    float currentspeed{ 1.0f };
    double progress = 0.0;


    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
