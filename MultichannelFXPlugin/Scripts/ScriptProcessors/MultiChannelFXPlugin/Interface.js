Content.makeFrontInterface(600, 250);

const var rb = Synth.getDisplayBufferSource("Compressor").getDisplayBuffer(0);

const var Panel1 = Content.getComponent("Panel1");

reg p;

Panel1.setPaintRoutine(function(g)
{
	g.fillAll(Colours.black);
	

	g.setColour(Colours.white);
	
	
	g.drawAlignedText("Gain reduction", this.getLocalBounds(5), "topRight");
	
	g.setColour(0x66FFFFFF);
	
	g.fillPath(p, this.getLocalBounds(0));
	
});

Panel1.setTimerCallback(function()
{
	p = rb.createPath(this.getLocalBounds(0), [0.0, 1.0, 0, -1], 0.0);

	this.repaint();
});

Panel1.startTimer(30);

const var masterMatrix = Synth.getRoutingMatrix("MultiChannelFXPlugin");

inline function onStereoButtonControl(component, value)
{
	masterMatrix.clear();

	for(i = 0; i < masterMatrix.NumInputs; i++)
	{
		masterMatrix.addConnection(i, value ? (i % 2) : i);
	}
};

Content.getComponent("StereoButton").setControlCallback(onStereoButtonControl);
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
 