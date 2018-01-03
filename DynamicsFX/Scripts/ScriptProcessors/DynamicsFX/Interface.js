Content.makeFrontInterface(600, 500);

include("VuMeter.js");

const var InputMeter = VuMeter.createVuMeter("InputMeter", 130, 20);
InputMeter.set("height", 230);

VuMeter.setModule(InputMeter, Synth.getEffect("Input"));

const var OutputMeter = VuMeter.createVuMeter("OutputMeter", 440, 20);
OutputMeter.set("height", 230);

VuMeter.setModule(OutputMeter, Synth.getEffect("Dynamics"));

include("DynamicsMeter.js");

const var compressorPanel = DynamicsMeter.createDynamicsMeter("compressorPanel", 170, 20, "Dynamics");
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
