
#include "JuceHeader.h"

//using namespace juce; // Normally I would use these in order to save typing 
//using namespace hise; // but for the sake of this example, we comment these out

/** This will be our custom interface for the HISE project. 

	You have to subclass it from FloatingTileContent and Component.

*/
class ExternalTestComponent : public hise::FloatingTileContent,
							  public juce::Component,
							  public juce::Slider::Listener
{
public:

	// This line will define the ID of the component and has to be the EXACT same String that
	// you've used in the Javascript code as `Type` property...
	SET_PANEL_NAME("ExternalTestComponent");

	ExternalTestComponent(juce::FloatingTile* parent) :
		FloatingTileContent(parent)
	{
		// The first thing we do is grabbing a reference to the MainController, which is the
		// object used for the entire HISE instance. It can be used to access pretty much everything
		// you need.
		// Check the C++ API for more information about this class.
		hise::MainController* mc = getMainController();

		// Get the data model that handles MIDI messages from our main controller
		auto& state = mc->getKeyboardState();

		// Add a stock JUCE MIDI keyboard and connect it to the main controller's MIDI state
		addAndMakeVisible(stockKeyboard = new juce::MidiKeyboardComponent(state, MidiKeyboardComponent::horizontalKeyboard));
		stockKeyboard->setKeyWidth(40);

		// Add a settings window that can be used to change the audio settings
		addAndMakeVisible(settingsWindow = new hise::CustomSettingsWindow(mc, true));
		
		// Deactivate some controls
		settingsWindow->setProperty(hise::CustomSettingsWindow::Properties::GlobalBPM, false);
		settingsWindow->setProperty(hise::CustomSettingsWindow::Properties::ClearMidiCC, false);
		settingsWindow->setProperty(hise::CustomSettingsWindow::Properties::GraphicRendering, false);

		// Get a reference to the sine wave generator
		sine = hise::ProcessorHelpers::getFirstProcessorWithName(mc->getMainSynthChain(), "Sine Wave Generator");

		jassert(sine != nullptr);
		
		addAndMakeVisible(gainSlider = new Slider("Gain"));
		gainSlider->addListener(this);
		gainSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
		gainSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 80, 40);
		gainSlider->setRange(0.0, 1.0);
	}

	~ExternalTestComponent()
	{
		// Be a good boy and clean up properly...

		stockKeyboard = nullptr;
		settingsWindow = nullptr;
	}

	void resized() override
	{
		// Position the elements
		auto area = getLocalBounds();

		stockKeyboard->setBounds(area.removeFromBottom(200));
		settingsWindow->setBounds(area.removeFromLeft(400));

		gainSlider->setBounds(area.withSizeKeepingCentre(80, 120));
	}

	void paint(Graphics& g) override
	{
		// Use the full JUCE power to draw your interface
		g.fillAll(Colours::darkblue);
	}

	void sliderValueChanged(juce::Slider* slider) override
	{
		sine->setAttribute(hise::ModulatorSynth::Parameters::Gain, slider->getValue(), dontSendNotification);
	}

	juce::ScopedPointer<hise::CustomSettingsWindow> settingsWindow;
	juce::ScopedPointer<juce::MidiKeyboardComponent> stockKeyboard;
	juce::WeakReference<hise::Processor> sine;
	juce::ScopedPointer<juce::Slider> gainSlider;
};


// This method needs to be defined and used for registering all external panels we define in our project scoped C++ code
void hise::FloatingTileContent::Factory::registerExternalPanelTypes()
{
	// Just use this handy macro for registering
	REGISTER_EXTERNAL_FLOATING_TILE(ExternalTestComponent);
}