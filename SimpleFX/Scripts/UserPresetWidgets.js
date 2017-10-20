
namespace UserPresetWidgets
{
	/** Creates a arrow button that cycles through the current category. */
	inline function createPresetButton(name, x, y, up)
	{
		local widget = Content.addPanel(name, x, y);
    
		Content.setPropertiesFromJSON(name, {
		"width": 20,
		"height": 20,
		"saveInPreset": false,
		"tooltip": up ? "Load next user preset" : "Load previous user preset",
		"allowCallbacks": "Clicks & Hover"
		});
    
		widget.data.up = up;
    
		widget.setPaintRoutine(function(g)
		{
			g.setColour(this.data.hover ? 0xFFFFFFFF : 0x88FFFFFF);
			g.fillTriangle([0, 0, this.getWidth(), this.getHeight()], this.data.up ? Math.PI/2 : 1.5 * Math.PI);
		});
    
		widget.setMouseCallback(function(event)
		{
			this.data.hover = event.hover;
    	
			if(event.clicked)
			{
				if(this.data.up)
					Engine.loadNextUserPreset(true);
				else
					Engine.loadPreviousUserPreset(true);		
			}
    	
			this.repaint();
		});
		return widget;
	};

	/** Creates a Label that shows the current User Preset.
	*
	*	You can click on it and it will open up a popup with the preset browser.
	*
	*	Customization: Use the itemColour property of the Panel to set the
	*	Preset Browser Colour.
	*/
	inline function createPresetDisplay(name, x, y)
	{
		local widget = Content.addPanel(name, x, y);
    
		Content.setPropertiesFromJSON(name, {
		"width": 163,
		"height": 25,
		"tooltip": "Click to show the Preset browser",
		"allowCallbacks": "Clicks & Hover"
		});
    
    
		widget.setPaintRoutine(function(g)
		{
			g.fillAll(this.data.hover ? 0xFF333333 : 0xFF222222);
			g.setColour(0x44FFFFFF);
			g.drawRect([0, 0, this.getWidth(), this.getHeight()], 1);
			g.setFont("Oxygen Bold", 15.0);
			g.setColour(Colours.white);
    	
			g.drawAlignedText(Engine.getCurrentUserPresetName(), [0, 0, this.getWidth(), this.getHeight()], "centred");
		});
    
		widget.setTimerCallback(function()
		{
			this.repaint();
		});
    
		widget.startTimer(300);
		
		widget.setPopupData({"Type": "PresetBrowser"}, [80, 30, 500, 350]);
    
    
		widget.setMouseCallback(function(event)
		{
			this.data.hover = event.hover;
			this.repaint();
		});
		return widget;
	};

	/** Change the appearance of the preset browser using this method. */
	inline function setPresetBrowserLookAndFeel(display, font, fontSize, colour)
	{
		display.setPopupData({"Type": "PresetBrowser", "Font": font,  "ColourData": {"itemColour1": colour}, "FontSize": fontSize}, [ display.getWidth()/2, display.getHeight(), 480, 300]);
	}
}