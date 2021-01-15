#include "MainComponent.h"
#include <corecrt_math_defines.h>
//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    frequency = 330;
    phase = 0; 
    wtSize = 1024;
    amplitude = 0.15;
    increment = frequency * wtSize / sampleRate;
    for (int i = 0; i < wtSize; i++) {
        WaveTable.insert(i, sin(2.0 * M_PI * i / wtSize));
    };
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); channel++) {

      for (int sample = 0; sample < bufferToFill.numSamples; sample++) {
         leftSpeaker[sample] = WaveTable[(int)phase]*amplitude;
        //rightSpeaker[sample] = WaveTable[(int)phase] * amplitude;
        phase =fmod( (phase + increment),wtSize);
       }
    }
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
 
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
