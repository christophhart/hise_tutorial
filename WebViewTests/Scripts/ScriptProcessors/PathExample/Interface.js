Content.makeFrontInterface(600, 600);

const var WebView1 = Content.getComponent("WebView1");

var p = FileSystem.getFolder(FileSystem.AudioFiles).getParentDirectory().getChildFile("Images/path-example/index.html");

WebView1.set("enableCache", false);
WebView1.reset();
WebView1.setIndexFile(p);

const var dp = Synth.getDisplayBufferSource("Script FX1");

// We'll use the oscilloscope display buffer,
// use 0 for the core.peak module...
const var peakBuffer = dp.getDisplayBuffer(1);

const var repainter = Engine.createTimerObject();

repainter.setTimerCallback(function()
{
	// this function will encode a buffer into a string with
	// two characters for each value. This resolution should be
	// enough for visualisation purposes (check out the Javascript
	// side for a decode function).
	var s = peakBuffer.getReadBuffer().toCharString(500, [-1,1]);
	
	// we just call a function called repaint in JS with the
	// given string...
	WebView1.callFunction("repaint", s);
});

repainter.startTimer(30);
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
 