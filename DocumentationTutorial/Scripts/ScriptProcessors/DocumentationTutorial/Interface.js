Content.makeFrontInterface(800, 668);

// Load Roboto standard to be used in the Markdown Panel
Engine.loadFontAs("{PROJECT_FOLDER}fonts/Roboto-Regular.ttf", "Roboto");

// Don't call it "Roboto Bold", because a "Bold" suffix will search for the "original" bold version
// which might not work with custom fonts.
Engine.loadFontAs("{PROJECT_FOLDER}fonts/Roboto-Black.ttf", "Roboto Fat");


include("HelpOverlay.js");

// This is the entire "client code" for the doc overlay system:

// init the doc panel
HelpOverlay.initDocPanel("HelpPanel");

// init the help button
HelpOverlay.initHelpButton("HelpButton");

// now add a link for each UI element
HelpOverlay.add("SomeButton", "/setup");
HelpOverlay.add("SomeButton1", "/embed#images");function onNoteOn()
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
 