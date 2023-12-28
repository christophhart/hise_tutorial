Content.makeFrontInterface(600, 600);

const var ScriptFX1 = Synth.getEffect("Script FX1");
const var ScriptFX1AP = Synth.getAudioSampleProcessor("Script FX1");

const var recBroadcaster = Engine.createBroadcaster({
	"id": "recordBroadcaster",
	"args": ["processor", "index", "value"],
	"comment": "Deactivates the button when the recording has stopped"
});

recBroadcaster.attachToComplexData("AudioFile.Content", "Script FX1", 0, "");

recBroadcaster.addListener([Content.getComponent("Record"), Content.getComponent("Hold")], "deactivate record button when recording is finished", function(u1, u2, content)
{
	// the content parameter will contain the loaded file
	// (in our case it will use the wildcard {INTERNAL}
	//  but we can use this to distinguish between an empty and
	//  a buffer that has content).
	if(content.length)
	{
		this[0].setValue(false);
		this[1].setValue(true);
		this[1].changed();
	}
});


inline function onRecordControl(component, value)
{
	if(value)
	{
		// Clear the audio file
		ScriptFX1AP.setFile("");
		
		ScriptFX1.setAttribute(2, false);
		
		// Start recording
		ScriptFX1.setAttribute(1, true);
	}
};

Content.getComponent("Record").setControlCallback(onRecordControl);
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
 