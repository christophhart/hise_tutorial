
Content.makeFrontInterface(600, 500);

//! ========================================================= Reference Variables

const var SineWaveGenerator = Synth.getChildSynth("Sine Wave Generator");
const var WaveformGenerator = Synth.getChildSynth("Waveform Generator");

const var SinePresets = Content.getComponent("SinePresets");
const var SawPresets = Content.getComponent("SawPresets");
const var GlobalUserPresets = Content.getComponent("GlobalUserPresets");


//! ================================================================= Preset Data

// This file contains the preset data
include("PresetData.js");

//! =============================================================== Control Logic


inline function loadPreset(component, value)
{
    if(value == 0) // If nothing is selected, don't load anything
        return;
    
    // Fetch the associated preset data object from the array
    // this is using the component parameter to check which control
    // was changed.
    local presetData = (component == SinePresets) ? sinePresetData[value-1] : sawPresetData[value-1];
  
    // Iterate over the array, get the component by the key (must be a matching ID)
    // and set it's value
    for(component in presetData)
    {
        // Fetch the component
        // This is not the most efficient way
        // but for preset loading it should be fine...
        local control = Content.getComponent(component);
        
        // Set the value from the data JSON object
        control.setValue(presetData[component]);
        
        // New API call: causes the onControl callback of the
        // control to be executed asynchronously.
        control.changed();
    }
        
};

// Set the same inline function as callback for both comboboxes
// (They will be distinguished within the function)
SinePresets.setControlCallback(loadPreset);
SawPresets.setControlCallback(loadPreset);

const var unneededPresetPrefix = "Bank/Category/";

const var presetList = Engine.getUserPresetList();

// Remove the Bank and category from the pretty preset list
for(presetName in presetList)
    presetName = presetName.substring(unneededPresetPrefix.length, 1000);

GlobalUserPresets.set("items", presetList.join("\n"));

inline function onGlobalUserPresetsControl(component, value)
{
    // Join the unneeded prefix with the combobox text to get
    // the full preset name
	Engine.loadUserPreset(unneededPresetPrefix + component.getItemText());
    
};

Content.getComponent("GlobalUserPresets").setControlCallback(onGlobalUserPresetsControl);
function onNoteOn()
{
	
}
function onNoteOff()
{
	
}
function onController()
{
	
}
function onTimer()
{
	
}
function onControl(number, value)
{
	
}
