/** Custom User Preset Browser using a webview!

	This example project shows how to use a webview to implement a 
	custom preset browser using a HTML webview.
	
	It supports bidirectional synchronisation, so if you load a user preset,
	it will update the selection in the webview and vice versa!
	
	The state here is just a single knob that will send it's value to the webview to
	be displayed.
	
	The HTML side only consists of a javascript file that defines the
	callbacks and populates a standard HTML select element. You probably want to use something
	more fancy in the end, but the communication will be pretty similar.
*/

Content.makeFrontInterface(600, 600);

// This is required because JS will mess up the "\\" path on Windows
inline function getEscapedFilePath(file)
{
	return file.toString(0).replace("\\", "/");
}

// Let's build up the preset database. For our example we'll use an array of
// JSON objects with a `name` and a `file` key that will be consumed by the Webview and
// populate the select element, but you're free to attach whatever other things you might need
const var userPresetObject = [];

for(userPreset in FileSystem.findFiles(FileSystem.getFolder(FileSystem.UserPresets), "*.preset", true))
{
	userPresetObject.push({
		"name": userPreset.toString(1),
		"file": getEscapedFilePath(userPreset)
	});
}
	
// Grab a reference to our little budddy
const var wv = Content.getComponent("WebView1")

// That's a bit ugly, but we grab the image folder by getting the HISE project AudioFiles folder
// and then doing some next level-hacking to get a subdirectory of the Images folder which I think is a nice
// place for web stuff...
const var webroot = FileSystem.getFolder(FileSystem.AudioFiles).getParentDirectory().getChildFile("Images/preset-browser");

// Call this to make sure that there is no intermediate temp stuff being carried around
wv.reset();

// set the index file (this is already the default, but I'll call it here so that you know it exists)
wv.set("indexFile", "/index.html");

// set the root directory using the 
wv.set("rootDirectory", webroot.toString(0));

// 
wv.set("enableCache", true);

wv.set("enablePersistence", true);

// Now we call the updatePresetList function that is defined in the Javascript of the Webview 
// (and tucked onto the global `window` object).
//
// Be aware that if the webview is not existent (because this script will be exeucuted before the plugin interface appears)
// It will store all function calls with their most recent value and then call all of them (asynchronously) when a new webview
// is created. This yields a somewhat persistent state that is similar to the usual audio plugin data model
wv.callFunction("updatePresetList", userPresetObject);

inline function onKnob1Control(component, value)
{
	// This just simulates the communication from HISE -> WebView
	// (the function window.updateValue() is defined in browser.js)
	wv.callFunction("updateValue", value);
};

Content.getComponent("Knob1").setControlCallback(onKnob1Control);

// This binding will call the given function when the Webview is
// calling the loadUserPreset function (which happens in the onselection callback)
wv.bindCallback("loadUserPreset", function(args)
{
	Console.print("LOAD FROM WEBVIEW");

	Engine.loadUserPreset(FileSystem.fromAbsolutePath(args[0]));
});

// This will attach a WebView function call to anytime a user preset is loaded
// and will update the selection (window.setSelectedPreset is defined in browser.js)
const var uph = Engine.createUserPresetHandler();

uph.setPostCallback(function(presetFile)
{
	wv.callFunction("setSelectedPreset", getEscapedFilePath(presetFile));
});

// This is just a zoom factor combobox that demonstrates the correct scaling
// of the WebView
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
 