Content.makeFrontInterface(600, 500);

include("VuMeter.js");

const var v1 = VuMeter.createVuMeter("VuMeter", 0, 0);
const var v2 = VuMeter.createVuMeter("VuMeter1", 0, 0);

VuMeter.setModule(v1, Synth.getChildSynth("Sine Wave Generator"));
VuMeter.setModule(v2, Synth.getChildSynth("Noise Generator"));


// Get a reference to the master container
const var MultiChannelTest = Synth.getChildSynth("MultiChannelTest");

// Get a reference to its routing matrix
const var matrix = MultiChannelTest.getRoutingMatrix();

const var SucessLabel = Content.getComponent("SucessLabel");
const var SucessLabel1 = Content.getComponent("SucessLabel1");



inline function onOutputSelectorControl(component, value)
{
	
	// this variable checks if the output channel exists.
	local success = true;
	
	switch(value)
    {
        case 1: // Routes the first two input channels (= sine wave);
                // to the first to output channels
            matrix.addConnection(0, 0);
            matrix.addConnection(1, 1);
            break;
        case 2: // Routes the first two input channels
                // to the second stereo output
            matrix.addConnection(0, 2);
            
            // addConnection returns true if the connection could be added
            // if the host doesn't support multichannels, this returns false
            // and you can reset the connections to default later (see below)
            success = matrix.addConnection(1, 3);
            break;
        case 3:
            matrix.addConnection(0, 4);
            success = matrix.addConnection(1, 5);
            break;
        case 4:
            matrix.addConnection(0, 6);
            success = matrix.addConnection(1, 7);
            break;
    }
    
    if(!success)
    {
        // Reset to Channel 1+2 in case of an error
        matrix.addConnection(0, 0);
        matrix.addConnection(1, 1);
    }
    
    SucessLabel.set("text", success ? "OK" : "Error");
};

Content.getComponent("OutputSelector").setControlCallback(onOutputSelectorControl);


inline function onOutputSelector1Control(component, value)
{
	// this variable checks if the output channel exists.
	local success = true;
	
	switch(value)
    {
        case 1:
            matrix.addConnection(2, 0);
            matrix.addConnection(3, 1);
            break;
        case 2:
            matrix.addConnection(2, 2);
            success = matrix.addConnection(3, 3);
            break;
        case 3:
            matrix.addConnection(2, 4);
            success = matrix.addConnection(3, 5);
            break;
        case 4:
            matrix.addConnection(2, 6);
            success = matrix.addConnection(3, 7);
            break;
    }
    
    if(!success)
    {
        matrix.addConnection(2, 0);
        matrix.addConnection(3, 1);
    }
    
    SucessLabel1.set("text", success ? "OK" : "Error");
};

Content.getComponent("OutputSelector1").setControlCallback(onOutputSelector1Control);


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
