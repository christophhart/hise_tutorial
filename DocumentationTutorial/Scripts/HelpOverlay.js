/** HelpOverlay.js

    A system that shows overlay panels with clickable links to a 
    markdown documentation. 
    
    Author:  Christoph Hart
    License: Don't care Licene
*/
namespace HelpOverlay
{
    const var docData = {
      "Type": "MarkdownPanel",
      "ShowBack": false,
      "ShowSearch": false,
      "ShowToc": true,
      "Font": "Roboto",
      "FontSize": 18,
      "BoldFontName": "Roboto Fat",
      "FixTocWidth": 230,
      "StartURL": "/link#the-overlay-system",
      
      "ColourData":
      {
        "bgColour": 0xFF222222,
        "itemColour1": 0xFF885588, // the headline colour
        "itemColour2": 0xFF8877EE, // the link colour
        "itemColour3": 0x05FFFFFF, // the TOC bg colour
        "textColour": 0xFFAAAAAA
      }
    };

    //"ServerUpdateURL": "https://docs.hise.audio/cache/tutorial",
    
    reg helpButton;
    reg docPanel;
    const var panels = [];
    
    
    /** Call this in your onInit callback with the name of your markdown panel. */
    inline function initDocPanel(name)
    {
        docPanel = Content.getComponent(name);
        
        // This will fire if it can't find the markdown floating tile
        Console.assertIsObjectOrArray(docPanel);
        
        docPanel.setContentData(docData);
    }
    
    /** Call this at startup. */
    inline function initHelpButton(name)
    {
        helpButton = Content.getComponent(name);
        helpButton.set("saveInPreset", false);
        helpButton.setControlCallback(helpButtonCallback);
    }

    
    /** Call this whenever you want to show a help for a certain UI element. */
    inline function add(originalId, url)
    {
        // first grab the original component
        local oc = Content.getComponent(originalId);
        
        // Now we create a panel and set it to overlay the component
        local p = Content.addPanel(originalId + "_help", 0, 0);
        p.set("parentComponent", oc.get("parentComponent"));
        p.set("x", oc.get("x"));
        p.set("y", oc.get("y"));
        p.set("width", oc.get("width"));
        p.set("height", oc.get("height"));
        
        // hide it by default
        p.set("visible", false);
        
        // Make it respond to clicks and hover events
        p.set("allowCallbacks", "Clicks & Hover");
        
        // set the paint routine for the overlay rendering
        p.setPaintRoutine(__helpPaintRoutine);
        
        p.setMouseCallback(__helpMouseCallback);
        
        // save the data into the panel's data object
        // it will be used by the mouse callback later
        p.data.url = url;
        
        panels.push(p);
    }
    
    //! ==================================================== PRIVATE FUNCTIONS
    
    inline function helpButtonCallback(component, value)
    {
        if(value)
        {
            __showOverlays(true);
        }
        else
        {
            __showOverlays(false);
            docPanel.set("visible", false);
        }
    }

    
    inline function __showOverlays(value)
    {
        for(p in panels)
            p.set("visible", value);
    }
    
    inline function __showLink(url)
    {
    
        docData.StartURL = url;

        // Rebuild the markdown panel with the given URL
        docPanel.setContentData(docData);

        // We'll also make it visible in case that it isn't...
        docPanel.set("visible", true);
    }
    
    /** This function will be the paint routine for all panel overlays. */
    function __helpPaintRoutine(g)
    {
        // this grabs the colour from the actual headline colour
        g.fillAll(Colours.withAlpha(docData.ColourData.itemColour1, 0.8));
        
        if(this.data.hover)
            g.fillAll(0x11000000);
        
        g.setColour(Colours.black);
        g.setFont("Roboto Fat", 13.0);
        g.drawAlignedText("HELP", [0, 0, this.getWidth(), this.getHeight()], "centred");
    }
    
    function __helpMouseCallback(event)
    {
        this.data.hover = event.hover;
        this.repaint();
        
        if(event.clicked)
        {
            __showLink(this.data.url);
        }
    }
}