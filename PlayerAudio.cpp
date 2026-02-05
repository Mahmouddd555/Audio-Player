#include "PlayerAudio.h"             // PlayerAudio.cpp

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();

    resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio() {}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    if (resamplingSource)
    {
        resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)////////
{
    if (resamplingSource)
    {
        resamplingSource->getNextAudioBlock(bufferToFill);
    }
    else
    {
        transportSource.getNextAudioBlock(bufferToFill);
    }

    if (abLooping && loopPointA >= 0.0 && loopPointB > loopPointA)
    {
        double currentPos = transportSource.getCurrentPosition();
        if (currentPos >= loopPointB)
        {
            transportSource.setPosition(loopPointA);
        }
    }
    else if (looping && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    if (resamplingSource)
    {
        resamplingSource->releaseResources();
    }
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

            transportSource.start();

            lastLoadedFile = file; // Store the file for later reference
        }
    }
    return true;
}

bool PlayerAudio::addFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.setSource(nullptr);

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

            transportSource.start();

            lastLoadedFile = file; // Store the file for later reference
        }
    }
    return true;
}

void PlayerAudio::play()
{
    transportSource.start();
}
void PlayerAudio::stop()
{
    transportSource.stop();
}
void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}
void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}
double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}
double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::setSpeed(double ratio)
{
    if (ratio <= 0.0)
    {
        return;
    }
    if (resamplingSource)
    {
        resamplingSource->setResamplingRatio(ratio);
    }
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    looping = shouldLoop;

}
bool PlayerAudio::isLooping() const
{
    return looping;
}
bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}

void PlayerAudio::setLoopPoints(double pointA, double pointB)///
{
    loopPointA = pointA;
    loopPointB = pointB;
}

void PlayerAudio::setABLooping(bool shouldLoop)
{
    abLooping = shouldLoop;
}

void PlayerAudio::getCurrentFileName(juce::String& outFileName)
{
    if (readerSource)
    {
        // juce::AudioFormatReader does not have getFile(), so store the file when loading
        if (lastLoadedFile.existsAsFile())
            outFileName = lastLoadedFile.getFileName();
        else
            outFileName = "Unknown file";
    }
    else
    {
        outFileName = "No file loaded";
    }
}

void PlayerAudio::getMetadata(juce::StringPairArray& outMetadata)
{
	outMetadata = metadataValues;

}

void PlayerAudio::getCurrentFileArtist(juce::String& outFileArtist)
{
    if (metadataValues.containsKey("artist"))
    {
        outFileArtist = metadataValues["artist"];
    }
    else
    {
        outFileArtist = "Unknown";
	}
}

void PlayerAudio::getCurrentFileDuration(juce::String& outFileDuration)
{
    if (lastLoadedFile.existsAsFile())
    {
        int duration = transportSource.getLengthInSeconds() / 60;
        durationInSecond = transportSource.getLengthInSeconds();
        int seconds = durationInSecond - duration * 60;
        outFileDuration = juce::String(duration) + ":" + juce::String(seconds);
    }
    else
    {
        outFileDuration = "Unknown";
    }
}

void PlayerAudio::addToPlaylist(const juce::File& file)
{
    playlist.add(file);
}
int PlayerAudio::getPlaylistSize() const
{
    return playlist.size();
}

juce::File PlayerAudio::getPlaylistFile(int index) const
{
    if (index >= 0 && index < playlist.size())
        return playlist.getReference(index);
    return juce::File{};
}

void PlayerAudio::playIndex(int index)
{
    if (index >= 0 && index < playlist.size())
    {
        currentIndex = index;
        loadFile(playlist.getReference(index));
        // loadFile already starts playback; ensure transport is playing
        if (!transportSource.isPlaying())
            transportSource.start();
    }
}

int PlayerAudio::getCurrentIndex() const
{
    return currentIndex;
}

void PlayerAudio::getFilePath(juce::String& outFileName)
{
    if (lastLoadedFile.existsAsFile())
        outFileName = lastLoadedFile.getFullPathName();
}
