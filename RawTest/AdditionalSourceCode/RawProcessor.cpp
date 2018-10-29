#include "RawProcessor.h"

using namespace juce;
using namespace hise;

/** A example module with custom MIDI behaviour. */
class PitchRandomizer: public hise::HardcodedScriptProcessor
{
public:
    
    // This line sets the ID / type names
    SET_PROCESSOR_NAME("PitchRandomizer", "Pitch Randomizer");
    
    PitchRandomizer(ModulatorSynth* parentSynth):
    HardcodedScriptProcessor(parentSynth->getMainController(), "Randomizer", parentSynth)
    {};
    
    /** Override the callbacks known from the scripting API. */
    void onNoteOn() override
    {
        // Use the same API as in the scripting environment.
        Message.setTransposeAmount(r.nextInt({0, 12}));
    }
    
private:
    
    juce::Random r;
};



FrontendProcessor::RawDataHolder* FrontendProcessor::createPresetRaw()
{
    // Here we are creating our module architecture using HISE modules as well
    // as a custom made module defined in the class above.
    
    // This helper class is offering a few convenient functions in order to
    // build the module architecture.
    hise::raw::Builder builder(this);

    // Programatically create a bunch of sine wave generators for additive synthesis.
    const float gainFactors[NUM_HARMONICS] = { 1.0f, 0.75f, 0.5f, 0.25f };
    
    for(int i = 0; i < NUM_HARMONICS; i++)
    {
        auto sine = builder.create<SineSynth>(getMainSynthChain());
        
        // Use the frequency mode for setting the harmonic transposing
        sine->setAttribute(SineSynth::UseFreqRatio, true, dontSendNotification);
        sine->setAttribute(SineSynth::CoarseFreqRatio, (float)(i + 1), dontSendNotification);
        
        // Set the harmonic volume. The gain is a property of the base class, hence the other enum
        sine->setAttribute(ModulatorSynth::Gain, gainFactors[i], dontSendNotification);
    }
    
    
    
    // Create an instance of our custom MIDI processor class.
    auto r = new PitchRandomizer(getMainSynthChain());
    builder.add(r, getMainSynthChain(), raw::ChainIndexes::Midi);
    
    // Add a stock reverb to the master container
    auto reverb = builder.create<SimpleReverbEffect>(getMainSynthChain(), raw::ChainIndexes::FX);
	
    // If we want to set multiple attributes (or want to set the same attributes for multiple
    // modules, we can use a raw::AttributeCollection
    raw::AttributeCollection reverbSettings =
    {
        { SimpleReverbEffect::WetLevel, 0.5f },
        { SimpleReverbEffect::Damping, 0.2f },
        { SimpleReverbEffect::Width, 0.8f }
    };
    
    builder.setAttributes(reverb, reverbSettings);
    
    // If we want to add modules created in HISE, we can export them as base64 encoded state
    // (Select module, Edit -> Create Base64 encoded state).
    // and load them directly. This is a Detuned Saw with a LFO on the filter frequency.
    const static String encodedSaw = "829.3oc0W97aSCCEG2YsVhIX7qUgFbpGfCHjlVKSHwoFZWKphVZ2R0FSvESxqsV3ZGZb1V4u.9yZ+ArCHw+.vcNv+AfcRaiS01TUYCskCQ48ri8m26801IsGJbgf.wPjUtNi7Aj0Mw6QN.bFwk8Q02BYcK7VfLjCdNjCQkG4SBB.OjkUlWSnbj0xYQQWeqTYBivcgDWHztBpKzfNfJS75a+FJiUi3AcnCL58l10cE7JBlHTASF7FnVtREIcFR3A9h.nf4HOSaEMaKl2oc+oeX8iickpW5nrlX3.ig8F1SbVzzYaB2nS6erx1n88saROJw7GkpxIejANfJZ7ZE3pBVhTmemzieWpcHK.1i5I6Wv7ESbWzzsUlpdT0H3HIRH.YgKK7F4zWbHWmlJf1kFPUSXrgiJ+Em8zkmlBuP0rSEbi9WSv7zUP8ynJ8oLu1SDAAH0v2NQRjIVRjC2j5Qm5uReckWqMtaTC4SdCS8g0Ro4N6Yy8FFbe93gLnKaLcJHhCyzjkN9SQVctD3AT4Hyhxr3lY9v8TxfVFLt7DFcnC7YPU9A.Sn7nY7dpUVcIgL4TulP1TvE98EbpqopcGPNj1qGjROcpwyqjRh6mR7jydGfAj.ikbO1tAkCjgpzDrfohByep37JWqgiwMudOg7+Kks+qpr6gaSkt8medUojKadGulcEb0tcAWYBrYw0d2ht.svohx7p9uOtsfMJVLWixjJ0qFnUMbmer+y9.FDp1P3yg.2MIe90uausoXHsPdM6sCIrYx+ko9BFY3bUaVbce14T.8.7zf5xR0ufktUhoFiaTqUDp2Q+T9owv4xn8O+xuJMa453SN4Q15y8qym8b3tpibimujPrAzSMOld5.C7EpOLI0FhNCDBYeJuW5ulHmcCgvO9nXOyAoRXfTLXxjhrdBt3lqm95fVpacel9wWVIxF4HA+sHRBx583WD0ei1edqKF6KP82Ryo9a0nh5zxmt7dEeK2XhSsp4J+gD5Mxt9bn1Cwi2iLJKe8g6ai295Cr5ub9nq.3tiHTp17rIQ8Ykp+hA+1vANpe.yETnx4.S++FVKgzuZj8FZ6n8cAtWjweTWiarf11ZbiElzH5uK8zs6C";
    
    
    
    // This is commented out so we have only the additive synthesiser in our example.
    //builder.createFromBase64String(encodedSaw, getMainSynthChain());
    
	// Just create an instance of the class defined above.
	return new ExampleRawDataHolder(this);
}

void FrontendProcessor::loadStateRaw(const ValueTree& v)
{
    // This method will be used for restoring the state of your plugin.
    // It's called from the host when you load a project with your plugin
    // or if you save a user preset using the UserPresetBrowser
    
    ignoreUnused(v);
}

ValueTree FrontendProcessor::saveStateRaw()
{
    // This method will be used when you need to save the current state of your plugin
    // either when the host needs to save the project or the user created a user preset.
    // Be aware that for user presets the ValueTree will get converted to a XML file
    // so you need to make sure it can do it.
    
	return {};
}
