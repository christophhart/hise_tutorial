/** RawEditor.h - The interface example. */

#pragma once

#include "JuceHeader.h"
#include "RawProcessor.h"

using namespace juce;
using namespace hise;

/** This is the interface of our custom C++ plugin. */
class RawEditor : public Component,
				  public ControlledObject,
                  public Slider::Listener,
                  public hise::SafeChangeListener,
				  public hise::SliderPack::Listener
{
public:
    
	RawEditor(FrontendProcessor::RawDataHolder* data);
    ~RawEditor();
    
	void sliderPackChanged(SliderPack */*s*/, int index) override;
    void changeListenerCallback(SafeChangeBroadcaster* b) override;
	void sliderValueChanged(Slider*) override;

	void paint(Graphics& g) override;
	void resized() override;
    
private:

	Array<WeakReference<SineSynth>> sines;

	ScopedPointer<hise::SliderPack> sliderPack;
    WeakReference<hise::Processor> masterContainer;
    
	ScopedPointer<juce::MidiKeyboardComponent> keyboard;
	ScopedPointer<Slider> s;
};
