/** Three JS Funky 3D Animation Demo

	This demo utilizes the three.js library to render a 3D WebGL object.
	
	It's attached to some signals from HISE and other stuff that shows the communication possibilities...
*/

Content.makeFrontInterface(600, 600);

// Grab a reference to our little budddy
const var wv = Content.getComponent("WebView1")

// That's a bit ugly, but we grab the image folder by getting the HISE project AudioFiles folder
// and then doing some next level-hacking to get a subdirectory of the Images folder which I think is a nice
// place for web stuff...
const var webroot = FileSystem.getFolder(FileSystem.AudioFiles).getParentDirectory().getChildFile("Images/three-js");

// Call this to make sure that there is no intermediate temp stuff being carried around
wv.reset();

// set the index file (this is already the default, but I'll call it here so that you know it exists)
wv.set("indexFile", "/index.html");

// set the root directory using the 
wv.set("rootDirectory", webroot.toString(0));

// This is set to true so that when you export it, the files will be embedded...
wv.set("enableCache", true);

wv.set("enablePersistence", true);

// Let's use a cable to route the LFO signal to the Cube's size
const var rm = Engine.getGlobalRoutingManager();

const var cable = rm.getCable("funkyslot");

// Register an callback that passes the function asynchronously to the webview
cable.registerCallback(function(value)
{
	wv.callFunction("updateZ", 3.0 + 1.0 * value);
}, AsyncNotification);

// Let's add another knob that will control the rotation speed
// this time we'll not calling the function from HISE script to update the position
// but bind a getX() function to the WebView that will be called in the render method
// to query the value it should use. This is definitely not the best solution and
// most likely super inefficient, but it demonstrates how to use JS promises to query
// stuff from HISE...
const var Knob1 = Content.getComponent("Knob1");

wv.bindCallback("getX", function(args)
{
	return Knob1.getValue() * 0.05;
});

// Our beloved zoom level combobox...
inline function onZoomFactorControl(component, value)
{
	if(value)
		Settings.setZoomLevel(component.getItemText());
};

Content.getComponent("ZoomFactor").setControlCallback(onZoomFactorControl);

	function onNoteOn()
{
	
}
 function onNoteOff()
{
	
}
 function onController()
{
	wv.callFunction("updateSpeed", Message.getControllerValue() / 127.0);
}
 function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 