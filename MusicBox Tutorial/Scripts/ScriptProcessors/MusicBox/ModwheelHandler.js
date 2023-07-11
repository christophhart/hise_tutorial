// This adds a knob to the script interface
const var amount = Content.addKnob("amount");

// This will store the current normalized mod value.
reg modWheelGain = 1.0;

// This will store the current velocity
reg currentVelocity = 127.0;

const var LO_KEY = 50;
const var HI_KEY = 76;function onNoteOn()
{
	currentVelocity = amount.getValue() * Message.getVelocity() * modWheelGain + 
                      (1.0 - amount.getValue()) * Message.getVelocity();

    Message.setVelocity(currentVelocity);
    
    if(Message.getNoteNumber() < LO_KEY || 
       Message.getNoteNumber() > HI_KEY)
	{
		Message.ignoreEvent(true);
	}
}
function onNoteOff()
{
	if(Message.getNoteNumber() < LO_KEY || 
       Message.getNoteNumber() > HI_KEY)
	{
		Message.ignoreEvent(true);
	}
}
function onController()
{
	if(Message.getControllerNumber() == 1)
    {
        modWheelGain = Message.getControllerValue() / 127.0;
    }
}
function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 