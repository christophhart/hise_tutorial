Content.makeFrontInterface(600, 600);

const var Sampler1 = Synth.getSampler("Sampler1");
reg sound;
reg totalSamples = 0;
reg isCustomMap = false;


//! "Page" Logic
// Nothing special, it just shows / hides certain elements...

const var editButtons = [Content.getComponent("EditLoop"),
                         Content.getComponent("EditRange"),
                         Content.getComponent("ShowDropper")];

const var EDIT_LOOP = 0;
const var EDIT_RANGE = 1;
const var SHOW_DROPPER = 2;

const var AudioWaveform1 = Content.getComponent("AudioWaveform1");
const var SampleDropper = Content.getComponent("SampleDropper");
const var LoopPanel = Content.getComponent("LoopPanel");
const var Reverse = Content.getComponent("Reverse");



inline function setEditMode(editMode)
{
	AudioWaveform1.set("enableRange", editMode == EDIT_RANGE);
	LoopPanel.set("visible", editMode == EDIT_LOOP);
	SampleDropper.set("visible", editMode == SHOW_DROPPER);
}

inline function setEditModeCallback(component, value)
{
	if(value)
		setEditMode(editButtons.indexOf(component));
}

for(b in editButtons)
	b.setControlCallback(setEditModeCallback);


inline function onSampleMapLoaderControl(component, value)
{
	if(value > 0)
	{
		local id = Sampler.getSampleMapList()[value - 1];
		Sampler1.loadSampleMap(id);
	}
};

Content.getComponent("SampleMapLoader").setControlCallback(onSampleMapLoaderControl);

const var SampleMapLoader = Content.getComponent("SampleMapLoader");

SampleMapLoader.set("items", Sampler.getSampleMapList().join("\n"));


setEditMode(EDIT_RANGE);

//! Non persistent UI elements
//  These UI controls do not store any data, but will read / write
//  to the sample map directly.


inline function onLoopControl(component, value)
{
	sound.set(Sampler.LoopEnabled, value);
	
	// Make sure the loop end is set to the sample end when you
	// enable the loop the first time
	if(sound.get(Sampler.LoopEnd) == 0)
		sound.set(Sampler.LoopEnd, sound.getRange(Sampler.LoopEnd)[1]);
	
	// update the loop point panel
	LoopPointDragger.updateLoopPoints();
	
	// write the loop points back into the base64 string
	SampleLoadSave.storeSampleMapData();
};

Content.getComponent("Loop").setControlCallback(onLoopControl);

inline function onXFadeControl(component, value)
{
	// the slider is 0...1 so we can use the available crossfade range length
	// to figure out how big of a crossfade we want
	local newValue = parseInt(sound.getRange(Sampler.LoopXFade)[1] * value);
	sound.set(Sampler.LoopXFade, newValue);
	
	LoopPointDragger.updateLoopPoints();
	SampleLoadSave.storeSampleMapData();
};

Content.getComponent("XFade").setControlCallback(onXFadeControl);


include("Migration.js");
include("SampleLoadSave.js");
include("PitchDetector.js");
include("LoopPointDragger.js");



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
 