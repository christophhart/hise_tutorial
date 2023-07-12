Content.makeFrontInterface(1024, 768);

const var WavetableSelector = Content.getComponent("WavetableSelector");

WavetableSelector.set("items", Engine.getWavetableList().join("\n"));

const var WavetableSynthesiser1 = Synth.getChildSynth("Wavetable Synthesiser1");

inline function onWavetableSelectorControl(component, value)
{
	if(value > 0)
	{
		WavetableSynthesiser1.setAttribute(WavetableSynthesiser1.LoadedBankIndex, value);
	}
};

Content.getComponent("WavetableSelector").setControlCallback(onWavetableSelectorControl);




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
 