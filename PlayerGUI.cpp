#include "PlayerGUI.h"                    // PlayerGUI.cpp
#include "PlayerAudio.h"                  // PlayerAudio class

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);

    if (playerAudio.isLooping() && !playerAudio.isPlaying())
    {
        playerAudio.setPosition(0.0);
        playerAudio.play();
    }
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{

    juce::ColourGradient gradient(juce::Colours::darkblue.darker(2.0f),getWidth() / 2, getHeight() / 2,juce::Colours::black,0, 0, true);
    gradient.addColour(0.5, juce::Colours::darkblue.darker(1.9f));
    g.setGradientFill(gradient);
    g.fillAll();

    juce::Graphics::ScopedSaveState state(g);

    auto playlistArea = playlistBox.getBounds().toFloat();


    g.setColour(juce::Colours::transparentBlack);
    g.fillRoundedRectangle(playlistBox.getBounds().toFloat(), 15.0f);
    g.fillRoundedRectangle(wavedis.getBounds().toFloat(), 15.0f);

    g.setColour(juce::Colours::transparentBlack);
    g.drawRoundedRectangle(playlistBox.getBounds().toFloat(), 15.0f, 1.0f);
    g.drawRoundedRectangle(wavedis.getBounds().toFloat(), 15.0f, 1.0f);

    if (playerAudio.getPlaylistSize() == 0)
    {
        g.setColour(juce::Colours::white.withAlpha(0.4f));
        g.setFont(juce::Font(18.0f, juce::Font::italic));

        g.drawFittedText("Add Queue",playlistBox.getBounds(),juce::Justification::centred,1);
    }
  

    // Translate to where you want the vertical bar to appear
    g.addTransform(juce::AffineTransform::translation(50, getHeight() - 50));

    // Rotate counterclockwise 90 degrees (PI / -2)
    g.addTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi));
}

PlayerGUI::PlayerGUI()
{
    // Add Buttons

    std::unique_ptr<juce::Drawable> loadIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8insertbutton100_svg, BinaryData::icons8insertbutton100_svgSize);

    std::unique_ptr<juce::Drawable> muteIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8mute100_svg, BinaryData::icons8mute100_svgSize);

    std::unique_ptr<juce::Drawable> pauseIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8pausebutton100_svg, BinaryData::icons8pausebutton100_svgSize);

    std::unique_ptr<juce::Drawable> playIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8circledplaybutton100_svg, BinaryData::icons8circledplaybutton100_svgSize);

    std::unique_ptr<juce::Drawable> loopIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8repeat100_svg, BinaryData::icons8repeat100_svgSize);

    std::unique_ptr<juce::Drawable> restartIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8replay100_svg, BinaryData::icons8replay100_svgSize);

    std::unique_ptr<juce::Drawable> stopIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8stopcircled100_svg, BinaryData::icons8stopcircled100_svgSize);

    std::unique_ptr<juce::Drawable> tostartIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8skiptostart100_svg, BinaryData::icons8skiptostart100_svgSize);

    std::unique_ptr<juce::Drawable> toendIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8end100_svg, BinaryData::icons8end100_svgSize);

    std::unique_ptr<juce::Drawable> setAIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8markera100_svg, BinaryData::icons8markera100_svgSize);

    std::unique_ptr<juce::Drawable> setBIcon = juce::Drawable::createFromImageData(
        BinaryData::icons8markerb100_svg, BinaryData::icons8markerb100_svgSize);

    std::unique_ptr<juce::Drawable> abLoopIcon = juce::Drawable::createFromImageData(
		BinaryData::icons8refresh100_svg, BinaryData::icons8refresh100_svgSize);

    std::unique_ptr<juce::Drawable> addFilesIcon = juce::Drawable::createFromImageData(
        BinaryData::pluscirclesvgrepocom_svg, BinaryData::pluscirclesvgrepocom_svgSize);

    loadButton.setImages(loadIcon.get(), loadIcon.get(), loadIcon.get());
    muteButton.setImages(muteIcon.get(), muteIcon.get(), muteIcon.get());
    pauseButton.setImages(pauseIcon.get(), pauseIcon.get(), pauseIcon.get());
    playButton.setImages(playIcon.get(), playIcon.get(), playIcon.get());
    loopButton.setImages(loopIcon.get(), loopIcon.get(), loopIcon.get());
    restartButton.setImages(restartIcon.get(), restartIcon.get(), restartIcon.get());
    stopButton.setImages(stopIcon.get(), stopIcon.get(), stopIcon.get());
    tostartButton.setImages(tostartIcon.get(), tostartIcon.get(), tostartIcon.get());
    toendButton.setImages(toendIcon.get(), toendIcon.get(), toendIcon.get());
    setAButton.setImages(setAIcon.get(), setAIcon.get(), setAIcon.get());
    setBButton.setImages(setBIcon.get(), setBIcon.get(), setBIcon.get());
	abLoopButton.setImages(abLoopIcon.get(), abLoopIcon.get(), abLoopIcon.get());
	addTolist.setImages(addFilesIcon.get(), addFilesIcon.get(), addFilesIcon.get());

    juce::Button* buttons[] = { &loadButton, &restartButton, &stopButton, &muteButton,
        &playButton, &pauseButton, &tostartButton, &toendButton, &loopButton };
    for (juce::Button* btn : buttons)
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // A-B Looping Buttons
    juce::Button* abButtons[] = { &setAButton, &setBButton, &abLoopButton };
    for (juce::Button* btn : abButtons)
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    addTolist.addListener(this);
    addAndMakeVisible(addTolist);

    for (auto* btn : { &loadButton, &restartButton, &stopButton, &muteButton, &playButton, &pauseButton, &loopButton, &setAButton, &setBButton })
    {
        btn->setColour(juce::DrawableButton::backgroundColourId, juce::Colours::transparentBlack);
        btn->setColour(juce::DrawableButton::backgroundOnColourId, juce::Colours::transparentBlack);
        btn->setEdgeIndent(0);
    }

    //Wave form
    addAndMakeVisible(wavedis);


    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);            // this object provides rows
    playlistBox.setRowHeight(24);

    addAndMakeVisible(name);
    playerAudio.getCurrentFileName(fileName);
    name.setText(fileName, juce::dontSendNotification);
    name.setFont(juce::Font(20.0f));
    name.setColour(juce::Label::textColourId, juce::Colours::aquamarine);

    addAndMakeVisible(artist);
    playerAudio.getCurrentFileArtist(fileArtist);
    artist.setText("Artist:" + fileArtist, juce::dontSendNotification);
    artist.setFont(juce::Font(16.0f));
    artist.setColour(juce::Label::textColourId, juce::Colours::aquamarine.withAlpha(0.7f));

    addAndMakeVisible(durationSong);
    durationSong.setText("Duration:" + fileDuration, juce::dontSendNotification);
    durationSong.setFont(juce::Font(16.0f));
    durationSong.setColour(juce::Label::textColourId, juce::Colours::lightgrey);

    // Speed Slider
    speedSlider.setRange(0.01, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(speedSlider);

    // Volume Slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(volumeSlider);

    // Volume Label
    volumeLabel.setText("50%", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(volumeLabel);

	volume.setText("volume", juce::dontSendNotification);
	volume.setJustificationType(juce::Justification::centredLeft);
	addAndMakeVisible(volume);

    //ab Loop Label
	abLoopLabel.setText("Off", juce::dontSendNotification);
	abLoopLabel.setJustificationType(juce::Justification::centredLeft);
	addAndMakeVisible(abLoopLabel);

	// Speed Label
	speedLabel.setText("1.00x", juce::dontSendNotification);
	speedLabel.setJustificationType(juce::Justification::centredLeft);
	addAndMakeVisible(speedLabel);

	speed.setText("speed", juce::dontSendNotification);
	speed.setJustificationType(juce::Justification::centredLeft);
	addAndMakeVisible(speed);

    // Position Slider
    positionSlider.setRange(0.0, 1.0, 0.01);
    positionSlider.addListener(this);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(positionSlider);
    positionLabel.setText("00:00", juce::dontSendNotification);
    addAndMakeVisible(positionLabel);
    startTimer(200);

    //Progress Bar

    addAndMakeVisible(progressBar);

    // Set colours:

    // Slider Colours
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::deeppink);
    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colours::deeppink.darker(1.0f));
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colours::cyan.darker(1.0f));
    positionSlider.setColour(juce::Slider::thumbColourId, juce::Colours::yellow);
    positionSlider.setColour(juce::Slider::trackColourId, juce::Colours::gold);
    progressBar.setColour(juce::ProgressBar::foregroundColourId, juce::Colours::cyan);
    progressBar.setColour(juce::ProgressBar::backgroundColourId, juce::Colours::black);
	progressBar.setColour(juce::Label::textColourId, juce::Colours::white);
    // Volume Label
    volumeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	volume.setColour(juce::Label::textColourId, juce::Colours::white);

	// Ab Loop Label
	abLoopLabel.setColour(juce::Label::textColourId, juce::Colours::white);

	// Speed Label
	speedLabel.setColour(juce::Label::textColourId, juce::Colours::white);
	speed.setColour(juce::Label::textColourId, juce::Colours::white);

    savedVolume = (float)volumeSlider.getValue();

    // Playlist Box
    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::black.withAlpha(0.2f));
    playlistBox.setColour(juce::ListBox::outlineColourId, juce::Colours::transparentBlack);
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        juce::ColourGradient selectGrad(juce::Colours::deeppink.withAlpha(0.4f), 0, 0,
            juce::Colours::transparentBlack, width, 0, false);
        g.setGradientFill(selectGrad);
        g.fillRoundedRectangle(2, 2, width - 4, height - 4, 5.0f);
        g.setColour(juce::Colours::white);
    }
    else
    {
        g.setColour(juce::Colours::white);
    }

    auto playlistArea = playlistBox.getBounds().toFloat();
    g.fillRoundedRectangle(playlistArea, 15.0f);

    g.drawRoundedRectangle(playlistArea, 15.0f, 1.5f);

    g.setFont(juce::Font(14.0f));
    juce::File f = playerAudio.getPlaylistFile(rowNumber);
    juce::String text = f.existsAsFile() ? f.getFileName() : "Unknown";
    g.drawText(text, 10, 0, width - 20, height, juce::Justification::centredLeft, true);
}

void PlayerGUI::resized()
{


    // wave 

    wavedis.setBounds(80, 90, getWidth() - 140, getHeight() / 6);

    // Main control buttons
    loadButton.setBounds(55, getHeight() - 85, 80, 50);
    restartButton.setBounds((getWidth() / 9), (getHeight() / 2) - 75, 80, 40);
    stopButton.setBounds((getWidth() / 2) + 20, (getHeight() / 2) - 100, 80, 40);
    muteButton.setBounds((getWidth() / 2) - 90, (getHeight() / 2) - 100, 80, 40);
    loopButton.setBounds((3 * (getWidth()) / 4), (getHeight() / 2) - 75, 80, 40);

    // Position control buttons
    tostartButton.setBounds(20, (getHeight() / 2) - 5, 50, 40);
    toendButton.setBounds(getWidth() - 70, (getHeight() / 2) - 5, 50, 40);
    playButton.setBounds((getWidth() / 2) + 20, (getHeight() / 2) - 50, 80, 40);
    pauseButton.setBounds((getWidth() / 2) - 90, (getHeight() / 2) - 50, 80, 40);

    // Sliders and Labels
    positionSlider.setBounds(80, getHeight() / 2, getWidth() - 200, 30);
    volumeSlider.setBounds(80, (getHeight() / 2) + 55, 35, (getHeight() / 4) - 10);
    speedSlider.setBounds(120, (getHeight() / 2) + 55, 80, (getHeight() / 4) - 10);
    positionLabel.setBounds(getWidth() - 120, getHeight() / 2, 60, 30);
    volumeLabel.setBounds(80, (getHeight() / 2) + 35 + ((getHeight() / 4)), 60, 30);
	volume.setBounds(70, (getHeight() / 2) + 35, 60, 30);
	speedLabel.setBounds(138, (getHeight() / 2) + 35 + ((getHeight() / 4)), 60, 30);
	speed.setBounds(135, (getHeight() / 2) + 35, 60, 30);

    // Progress Bar
    progressBar.setBounds((getHeight() / 30), 150, (getHeight() / 4), 20);
    progressBar.setTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi)
        .translated(50, getHeight() - 50));

    // A-B Looping buttons
    setAButton.setBounds(170, (getHeight() / 2) + 30, 80, 30);
    setBButton.setBounds(250, (getHeight() / 2) + 30, 80, 30);
    abLoopButton.setBounds(350, (getHeight() / 2) + 30, 100, 30);
	abLoopLabel.setBounds(460, (getHeight() / 2) + 30, 60, 30);

    // File details label
    name.setBounds(20, 20, (getWidth() / 2) - 30, 30);
    artist.setBounds(20, 40, (getWidth() / 2) - 30, 30);
    durationSong.setBounds(20, 60, (getWidth() / 2) - 30, 30);

    // Playlist Box
    auto playlistWidth = getWidth() / 2 - 30;
    auto playlistHeight = getHeight() / 4;
    auto playlistX = (getWidth() / 2) - 50;
    auto playlistY = (getHeight() / 2) + 80;

    playlistBox.setBounds(playlistX, playlistY, playlistWidth, playlistHeight);

    addTolist.setBounds(450, 530, 120, 60);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");;

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                    playerAudio.addToPlaylist(file);
                    juce::String fileName;
                    playerAudio.getCurrentFileName(fileName);
                    name.setText(fileName, juce::dontSendNotification);
                    juce::String fileArtist;
                    playerAudio.getCurrentFileArtist(fileArtist);
                    artist.setText("Artist:" + fileArtist, juce::dontSendNotification);
                    juce::String fileDuration;
                    playerAudio.getCurrentFileDuration(fileDuration);
                    durationSong.setText("Duration:" + fileDuration, juce::dontSendNotification);

                    playlistBox.updateContent();
                    int last = playerAudio.getPlaylistSize() - 1;
                    if (last >= 0)
                        playlistBox.selectRow(last);

                    wavedis.loadURL(file);
                }
            });
    }

    if (button == &addTolist)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync
        (
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                auto files = fc.getResults();
                for (int i = 0; i < files.size(); i++) 
                {
                    if (files[i].existsAsFile())
                    {
                        playerAudio.addToPlaylist(files[i]);

                        playlistBox.updateContent();
                        int last = playerAudio.getPlaylistSize() - 1;
                        if (last >= 0)
                        {
                            playlistBox.selectRow(last);
                        }
                    }
                }
            }
        );
    }

    if (button == &restartButton)
    {
        playerAudio.setPosition(0.0);
        playerAudio.play();

    }
    if (button == &stopButton)
    {
        playerAudio.stop(); 
        playerAudio.setPosition(0.0);
    }
    if (button == &playButton)
    {
        playerAudio.play();
    }
    if (button == &pauseButton)
    {
        lastPosition = playerAudio.getPosition();
        playerAudio.stop();
    }
    if (button == &tostartButton)
    {
        playerAudio.setPosition(0.0);
        playerAudio.play();
    }
    if (button == &toendButton)
    {
        playerAudio.setPosition(playerAudio.getLength() - 0.1);
        playerAudio.play();
    }

    if (button == &loopButton)
    {
        bool newLoopState = !playerAudio.isLooping();
        playerAudio.setLooping(newLoopState);
        loopButton.setButtonText(newLoopState ? "Loop: On" : "Loop: Off");
    }

    if (button == &muteButton)
    {
        //Mute
        if (!isMuted)
        {
            savedVolume = (float)volumeSlider.getValue();
            volumeSlider.setValue(0.0);
            isMuted = true;
            muteButton.setButtonText("Unmute");
        }
        // Unmute
        else
        {
            volumeSlider.setValue(savedVolume);
            isMuted = false;
            muteButton.setButtonText("Mute");
        }
    }

    // A-B Looping buttons
    if (button == &setAButton)
    {
        pointA = playerAudio.getPosition();
        setAButton.setButtonText("A: " + juce::String::formatted("%02d:%02d", (int)(pointA / 60), (int)((int)pointA % 60)));
    }

    if (button == &setBButton)
    {
        pointB = playerAudio.getPosition();
        setBButton.setButtonText("B: " + juce::String::formatted("%02d:%02d", (int)(pointB / 60), (int)((int)pointB % 60)));
    }

    if (button == &abLoopButton)
    {
        abLoopingEnabled = !abLoopingEnabled;
        abLoopLabel.setText(abLoopingEnabled ? "On" : "Off", juce::dontSendNotification);

        if (abLoopingEnabled && pointA >= 0.0 && pointB > pointA)
        {
            playerAudio.setLoopPoints(pointA, pointB);
        }

        playerAudio.setABLooping(abLoopingEnabled);
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    // VolumeSlider
    if (slider == &volumeSlider)
    {
        float v = (float)slider->getValue();
        playerAudio.setGain(v);

        // Update volume label
        int volumePercent = v * 100;
        volumeLabel.setText(juce::String::formatted("%d%%", volumePercent), juce::dontSendNotification);

        if (v == 0.0f)
        {
            isMuted = true;
            muteButton.setButtonText("Unmute");
        }
        else
        {
            isMuted = false;
            savedVolume = v;
            muteButton.setButtonText("Mute");
        }
    }

    // Position Slider
    if (slider == &positionSlider)
    {
        double length = playerAudio.getLength();
        double pos = positionSlider.getValue() * length;
        playerAudio.setPosition(pos);
    }

    // Speed Slider
    if (slider == &speedSlider)
    {
        double ratio = speedSlider.getValue();
        playerAudio.setSpeed(ratio);
		speedLabel.setText(juce::String::formatted("%.2fx", ratio), juce::dontSendNotification);
    }
}

void PlayerGUI::timerCallback()
{
    double pos = playerAudio.getPosition();
    double length = playerAudio.getLength();
    if (length > 0.0)
    {
        progress = pos / length;
        progressBar.setPercentageDisplay(true);

        positionSlider.setValue(pos / length, juce::dontSendNotification);

        int seconds = (int)pos;
        int mins = seconds / 60;
        seconds %= 60;

        positionLabel.setText(juce::String::formatted("%02d:%02d", mins, seconds), juce::dontSendNotification);

        wavedis.setPosition(progress);
    }
}

bool PlayerGUI::isCurrentlyPlaying() const
{
    return playerAudio.isPlaying();
}

int PlayerGUI::getNumRows()
{
    return playerAudio.getPlaylistSize();
}

void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
    // Play the selected index
    playerAudio.playIndex(row);

    // Update displayed file info
    juce::String filePath;
    playerAudio.getFilePath(filePath);
    playerAudio.getCurrentFileName(fileName);
    name.setText(fileName, juce::dontSendNotification);
    playerAudio.getCurrentFileArtist(fileArtist);
    artist.setText("Artist:" + fileArtist, juce::dontSendNotification);
    playerAudio.getCurrentFileDuration(fileDuration);
    durationSong.setText("Duration:  " + fileDuration, juce::dontSendNotification);
    wavedis.loadURL(filePath);

    playlistBox.selectRow(row);
}
