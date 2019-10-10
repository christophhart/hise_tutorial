---
keywords: How to link
summary:  Open links in the markdown panel
author:   Christoph Hart
modified: 08.10.2019
---
  
One of the main [benefits](/link#the-overlay-system) of an inbuilt documentation is that you can add in-place documentation of UI elements and open the page (and anchor) to directly show the documentation of the UI element.

Since there are many ways to do this, HISE just offers a minimal API for this and leaves the implementation up to the developer.  
This project will show one possible approach:  
A button that toggles "help" mode, which shows a view panels that overlay UI elements. If you click on of of these elements, it will show the markdown panel with the link to the documentation of the UI element.
The classes will be self-contained and should be easily transferable to other projects.

## The API

First of all, we'll describe the API. If you want to implement your own help system, you can stop after reading this chapter.

It all comes down to a single property in the MarkdownPanel: `StartURL`.  
This URL will just tell which markdown panel to show at initialisation. Now what looks like a static property gives us all the features we need: if we want to link to another page, we'll just recreate the MarkdownPanel by changing the object and rebuilding the floating tile.

However this requires us to **not** use the **ContentType** property from the interface designer, but use a JSON object from the scripting world and the `ScriptFloatingTile.setContentData(var data)` scripting call, so in before we can proceed, we have to switch to that approach.

Most of the stuff can just be copied from the object defined in the Interface designer:

```javascript
const var docData = {
  "Type": "MarkdownPanel",
  "ShowBack": false,
  "ShowSearch": false,
  "ShowToc": true,
  "Font": "Roboto",
  "FontSize": 18,
  "BoldFontName": "Roboto Fat",
  "FixTocWidth": 300,
  "StartURL": "/",
  "ColourData":
  {
    "bgColour": 0xFF222222,
    "itemColour1": 0xFF880088, // the headline colour
    "itemColour2": 0xFF110188, // the link colour
    "textColour": 0xFFAAAAAA
  }
};
```

> Note: if you can't see the code above in the compiled version don't worry, it's a known issue that I'll remove soon...

with these modifications:

- the `Type` has to be specified
- the `Font` and `FontSize` properties have to be added
- the `ColourData` needs to be an object within the object

from then on its business as usual: grab a reference to the panel and set the content data:

```javascript
const var HelpPanel = Content.getComponent("HelpPanel");
HelpPanel.setContentData(docData);
```

After we've made that transition, linking to a page is pretty straightforward: we just change the `StartURL` property and call the method again.  
We'll use a function that does that so we save some typing later:

```javascript
inline function showLink(url)
{
    
    docData.StartURL = url;

    // Rebuild the markdown panel with the given URL
    HelpPanel.setContentData(docData);

    // We'll also make it visible in case that it isn't...
    HelpPanel.set("visible", true);
}
```

So whenever we want to show a page in the docs, we just call this function and give it the URL to this page.

> A quick way to get valid links that are guaranteed to work is to right click in the TOC and choose "Copy Link".

This concludes the basic setup and we can proceeed with a overlay system that uses panels.

## The overlay system

For our system we need 2 things:

1. The toggle button
2. The overlay templates.

We'll keep it nice and tidy and put everything in a separate namespace called `HelpOverlay` in a dedicated file.

> If you want to transfer this method to one of your projects, just grab that file and use it like described here.

Before we proceed, we'll clean up and move all the code above into this namespace. We'll also add a function `initDocPanel()` which you need to call with the ID of your MarkdownPanel floating tile.

```javascript
// Call this somewhere in your onInit callback
HelpOverlay.initDocPanel("myPanelName");
```

### The help button

First of all we need a button that toggles the help mode. We'll add another function `HelpOverlay.initHelpButton(buttonId)` which initialises the button and sets its properties accordingly.  
We'll also define the callback for the button that toggles the overlays (don't bother about the `showOverlays` function, we'll add that later).


```javascript
reg helpMode = 0;
reg helpButton;

inline function helpButtonCallback(component, value)
{
    if(value)
    {
        showOverlays(true);
    }
    else
    {
        showOverlays(false);
        docPanel.set("visible", false);
    }
}

inline function initHelpButton(name)
{
    helpButton = Content.getComponent(name);
    helpButton.set("saveInPreset", false");
    helpButton.setControlCallback(helpButtonCallback);
}
```

Then we need to call this from our onInit script:

```javascript
HelpOverlay.initHelpButton("HelpButton");
```

### The overlay panels

The second part of this system are the overlay panels. It is important that adding new overlays is as easy as possible (otherwise chances are high that you won't add these).

The most simplified workflow for this is:

1. Add a panel and use the same ID but with a `_help` suffix
2. Call HelpOverlay.add(originalId) with **the ID of the original widget**.

The rest will be taken care of automatically:

- setting the position of the panel: parent hierarchy and position
- setting the paint routine and mouse logic
- storing it in an array that will be toggled when you click the help button

Take a look at the implementation in HelpOverlay.js. You can of course customize the appearance by changing the paint routine that's been given to those panels.

