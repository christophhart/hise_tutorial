/** RawProcessor.h - example how to use HISE as C++ framework.
 
 Covered in this example:
 
- Create a barebone HISE project and create an additive synthesiser with 4 sine-waves
  in C++ using the hise::raw::Builder toolclass.
- Create and use a custom MIDI processor module using the same API as the Javascript processor
  in HISE.
- Create an interface and hook it up to control a parameter of the module structure
  defined in C++
- Create a object that will contain the data used by our project.
- Create a special HISE UI element (the slider pack) and use it to control the 
  harmonic gain structure of the additive synthesis. Connect the slider pack to the
  data object for encapsulation between interface and data model as required for plugin
  usage.
 
 */

#pragma once

#include "JuceHeader.h"

constexpr int NUM_HARMONICS = 4;

// We want to store the slider pack data as persistent data in the processor.
// So we need to subclass the RawDataHolder class
class ExampleRawDataHolder : public hise::FrontendProcessor::RawDataBase
{
public:

	ExampleRawDataHolder(hise::MainController* mc):
		RawDataBase(mc)
	{
		createModules();

		// Create the data holder for the harmonics slider pack.

		// the undo manager used for control changes
		auto undoManager = mc->getControlUndoManager(); 

		// the pooled updater used for coallescated updates
		auto pooledUpdater = mc->getGlobalUIUpdater();

		// Create a SliderPackData object that will hold the values
		// for the slider pack
		dataPack = new hise::SliderPackData(undoManager, pooledUpdater);

		// Set the slider amount
		dataPack->setNumSliders(NUM_HARMONICS);
	};

	void createModules();

	Component* createEditor() override;

	ValueTree exportAsValueTree() const override
	{
		return {};
	}

	void restoreFromValueTree(const ValueTree &previouslyExportedState) override
	{
		
	}

	juce::ScopedPointer<hise::SliderPackData> dataPack;
};
