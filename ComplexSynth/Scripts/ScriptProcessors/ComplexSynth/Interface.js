Content.makeFrontInterface(1000, 800);

include("UI/LookAndFeel.js");

include("Builder.js");

Engine.setMinimumSampleRate(80000.0);

//! UI DEFINITIONS

const var Keyboard = Content.getComponent("Keyboard");
//Keyboard.setLocalLookAndFeel(LookAndFeel.keyboardLaf);

include("UI/PathIcons.js"); 
include("UI/PageLogic.js");
include("UI/Modulation.js");
include("UI/MainPage.js");
include("WavetableHandling.js");

include("MatrixHandling.js");

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
 