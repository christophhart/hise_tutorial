#include "RawEditor.h"

RawEditor::RawEditor(FrontendProcessor::RawDataHolder* data) :
	ControlledObject(data->getMainController()),
	masterContainer(getMainController()->getMainSynthChain())
{
	auto dataHolder = dynamic_cast<ExampleRawDataHolder*>(data);

	// listen to changes to update the slider
	masterContainer->addChangeListener(this);

    // add the slider (standard JUCE stuff)
	addAndMakeVisible(s = new Slider());
	s->setRange(0.0, 1.0);
	s->addListener(this);

	addAndMakeVisible(keyboard = new MidiKeyboardComponent(getMainController()->getKeyboardState(), MidiKeyboardComponent::horizontalKeyboard));

	addAndMakeVisible(sliderPack = new SliderPack(dataHolder->dataPack));
	sliderPack->addListener(this);

    // this just spits out an array of all modules with the given type.
	sines = ProcessorHelpers::getListOfAllProcessors<SineSynth>(masterContainer);

	setSize(700, 400);
}

RawEditor::~RawEditor()
{
	masterContainer->removeChangeListener(this);
	s->removeListener(this);
	sliderPack->removeListener(this);

	s = nullptr;
	sliderPack = nullptr;
}

void RawEditor::sliderPackChanged(SliderPack */*s*/, int index)
{
	sines[index]->setAttribute(ModulatorSynth::Gain,
                               sliderPack->getValue(index),
                               sendNotification);
}

void RawEditor::changeListenerCallback(SafeChangeBroadcaster* b)
{
	s->setValue(masterContainer->getAttribute(ModulatorSynth::Gain), dontSendNotification);
}

void RawEditor::paint(Graphics& g)
{
	g.fillAll(Colours::darkblue);
}

void RawEditor::resized()
{
	s->setBounds(getLocalBounds().withSizeKeepingCentre(400, 80));
	keyboard->setBounds(getLocalBounds().removeFromBottom(80));
	sliderPack->setBounds(getLocalBounds().removeFromTop(100));
}

void RawEditor::sliderValueChanged(Slider*)
{
	masterContainer->setAttribute(ModulatorSynth::Gain, s->getValue(), sendNotification);
}

Component* FrontendProcessorEditor::createRawEditor(hise::FrontendProcessor::RawDataHolder* data)
{
	// This method will be called to create the custom interface for your plugin.
	// It must set the default size correctly. The scale factor and the overlay
	// for critical messages will added automatically.
	return new RawEditor(data);
}
