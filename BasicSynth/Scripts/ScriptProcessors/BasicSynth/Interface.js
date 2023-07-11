/** HISE Basic Synth
*
*   An example project that shows the usage of HISE for a basic synth.
*
*/

// iPad full screen resolution (uses retina automatically)
Content.makeFrontInterface(1024, 768);



//! =============================================================== Page Handling

// Get these lines by selecting all controls and choose "Create Script Definition for selection"
const var SynthPageButton = Content.getComponent("SynthPageButton");
const var PresetButton = Content.getComponent("PresetButton");
const var SettingsButton = Content.getComponent("SettingsButton");
const var FXPageButton = Content.getComponent("FXPageButton");

const var SynthPage = Content.getComponent("SynthPage");
const var FXPage = Content.getComponent("FXPage");
const var PresetBrowser = Content.getComponent("PresetBrowser");
const var SettingsPage = Content.getComponent("SettingsPage");

// Put them all in an array for better iterating.
const var pages = [SynthPage, FXPage, PresetBrowser, SettingsPage];

/** This function iterates the array, hides all pages and only shows the given page.
*
*   pageToShow is a reference to the control. Check the onControl callback to see what's going on.
*/
inline function handlePages(pageToShow)
{
    for(p in pages)
        p.set("visible", pageToShow == p);
}

// This page will be shown as default
// If you're working on another page, just pass this is temporarily
// so it doesn't switch back when compiling.
handlePages(SynthPage);

// Adds the close button for the expansion
include("CloseButton.js");
include("CustomCallbacks.js");



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
    /*  This callback just calls the page handling function with the respective page.
    *   The other custom callbacks are implemented in the onInit script.
    */
	switch(number)
    {
        case SynthPageButton: handlePages(SynthPage); break;
        case FXPageButton: handlePages(FXPage); break;
        case SettingsButton: handlePages(SettingsPage); break;
        case PresetButton: handlePages(PresetBrowser); break;
    }
}
