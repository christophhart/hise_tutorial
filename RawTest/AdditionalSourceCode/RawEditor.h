/** RawEditor.h - The interface example. */

#pragma once

#include "JuceHeader.h"
#include "RawProcessor.h"

using namespace juce;
using namespace hise;

/** This is the interface of our custom C++ plugin. */
class RawEditor : public Component,
				  public ControlledObject,
				  public hise::SliderPack::Listener
{
public:
    
	RawEditor(FrontendProcessor::RawDataBase* data);
    ~RawEditor();
    
	void sliderPackChanged(SliderPack */*s*/, int index) override;

	void paint(Graphics& g) override;
	void resized() override;
    
private:

	Array<WeakReference<SineSynth>> sines;

	ScopedPointer<hise::SliderPack> sliderPack;
    WeakReference<hise::Processor> masterContainer;
    
	ScopedPointer<juce::MidiKeyboardComponent> keyboard;

	ScopedPointer<Slider> s;

	raw::UIConnection::Ptr sliderConnection;
};
