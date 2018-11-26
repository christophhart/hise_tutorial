#include "RawEditor.h"

RawEditor::RawEditor(FrontendProcessor::RawDataBase* data) :
	ControlledObject(data->getMainController()),
	masterContainer(getMainController()->getMainSynthChain())
{
	auto dataHolder = dynamic_cast<ExampleRawDataHolder*>(data);

    // add the slider (standard JUCE stuff)
	addAndMakeVisible(s = new Slider());
	s->setRange(0.0, 1.0);

	sliderConnection = new raw::UIConnection::Slider<hise::ModulatorSynth::Gain>(s, getMainController(), "Master Chain");

	addAndMakeVisible(keyboard = new MidiKeyboardComponent(getMainController()->getKeyboardState(), MidiKeyboardComponent::horizontalKeyboard));

	addAndMakeVisible(sliderPack = new SliderPack(dataHolder->dataPack));
	sliderPack->addListener(this);

    // this just spits out an array of all modules with the given type.
	sines = ProcessorHelpers::getListOfAllProcessors<SineSynth>(masterContainer);

	setSize(700, 400);
}

RawEditor::~RawEditor()
{
	sliderPack->removeListener(this);
	sliderPack = nullptr;

	s = nullptr;
	sliderConnection.release();
}

void RawEditor::sliderPackChanged(SliderPack */*s*/, int index)
{
	sines[index]->setAttribute(ModulatorSynth::Gain,
                               sliderPack->getValue(index),
                               sendNotification);
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

