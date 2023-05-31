Content.makeFrontInterface(1024, 800);

const var WebView1 = Content.getComponent("WebView1");


const var f = FileSystem.getFolder(FileSystem.AudioFiles).getParentDirectory().getChildFile("Images/playcanvas/index.html");

Console.assertTrue(f.isFile());

WebView1.set("enableCache", true);

WebView1.setIndexFile(f);

// We've hooked something into the teleport function that calls this method whenever
// the ball is teleported.
WebView1.bindCallback("onFinish", function(args)
{
	Synth.playNote(64, 127);
});


inline function onButton1Control(component, value)
{
	WebView1.callFunction("setKeyPressed", value);
};

Content.getComponent("Button1").setControlCallback(onButton1Control);


inline function onComboBox1Control(component, value)
{
	if(value)
		Settings.setZoomLevel(component.getItemText());
};

Content.getComponent("ComboBox1").setControlCallback(onComboBox1Control);
function onNoteOn()
{
	// We've added a manual function to the window object 
	// that changes a variable which is queried for each rendered frame
	WebView1.callFunction("setKeyPressed", true);
}
 function onNoteOff()
{
	WebView1.callFunction("setKeyPressed", false);
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
 