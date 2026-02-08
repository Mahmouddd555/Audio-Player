#pragma once            // PlayerAudio.h
#include <JuceHeader.h>


class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    bool loadFile(const juce::File& file);
    bool addFile(const juce::File& file);
    void play();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
    void setLooping(bool shouldLoop);
    bool isLooping() const;
    bool isPlaying() const;
    void setSpeed(double ratio);
    void setLoopPoints(double pointA, double pointB);    //
    void setABLooping(bool shouldLoop);
    void getCurrentFileName(juce::String& outFileName);
    void getMetadata(juce::StringPairArray& outMetadata);
    void getCurrentFileDuration(juce::String& outFile);
    void addToPlaylist(const juce::File& file);
    int getPlaylistSize() const;
    juce::File getPlaylistFile(int index) const;
    void playIndex(int index);
    int getCurrentIndex() const;
    void getFilePath(juce::String& outFileName);

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::StringPairArray metadataValues;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;
    juce::Array<juce::File> playlist;
    int duration;
    int durationInSecond;
    int currentIndex = 0;
    bool looping = false;
    double currentSpeed = 1.0;
    double loopPointA = -1.0;
    double loopPointB = -1.0;
    bool abLooping = false;
    juce::File lastLoadedFile;
};
