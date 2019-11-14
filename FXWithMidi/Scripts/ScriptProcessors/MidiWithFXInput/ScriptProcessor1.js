const var PhaseFX1 = Synth.getEffect("Phase FX1");
const var PhaseFX2 = Synth.getEffect("Phase FX2");

Content.makeFrontInterface(600, 600);
const var FrequencyDisplay = Content.getComponent("FrequencyDisplay");

function onNoteOn()
{
	local freq = Engine.getFrequencyForMidiNoteNumber(Message.getNoteNumber());
    Console.print(freq);
    
    FrequencyDisplay.setValue(freq);
    
    
    PhaseFX1.setAttribute(PhaseFX1.Frequency2, freq);
    PhaseFX2.setAttribute(PhaseFX2.Frequency2, freq*2);
    
	
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
 