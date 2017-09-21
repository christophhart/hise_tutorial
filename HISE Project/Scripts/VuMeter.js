
namespace VuMeter
{
	/** Creates a peak meter.
	*
	*	Usage: Give it a reference to a module (either synth or effect).
	*
	*	It looks best using a width and height with multiple of 4.
	*	Customize the colours using the scriptPanel colour Ids
	*/
	inline function createVuMeter(name, x, y, module)
	{
		local widget = Content.addPanel(name, x, y);
    
		Content.setPropertiesFromJSON(name, {
		"width": 32,
		"height": 100,
		"itemColour": 0x88FFFFFF,
		"itemColour2": 4279505940,
		"bgColour": 4279505940,
		"textColour": 4283782485,
		"saveInPreset": false,
		"opaque": 1
		});
    
		widget.data.module = module;
		
		Console.assertIsObjectOrArray(module);
		
		widget.setPaintRoutine(function(g)
		{
			g.fillAll(this.get("bgColour"));
			
			g.setColour(this.get("itemColour"));
    	
			var lsize = parseInt(this.data.lvalue * (this.getHeight()-4));
			var rsize = parseInt(this.data.rvalue * (this.getHeight()-4));
    	
			g.fillRect([2, this.getHeight() - lsize - 2, (this.getWidth()-4)/2-1, lsize]);
			g.fillRect([2 + this.getWidth() / 2 - 1, this.getHeight() - rsize - 2, (this.getWidth()-4)/2-1, rsize]);
    	
			g.setColour(this.get("itemColour2"));
    	
			for(i = 1; i < this.getHeight()-1; i = i + 3)
			{
				g.fillRect([1, i, this.getWidth()-2, 1]);
			}
		});
    
		widget.setTimerCallback(function()
		{
			var lvalue = getNormalizedPeakValue(this.data.module.getCurrentLevel(false));
			var rvalue = getNormalizedPeakValue(this.data.module.getCurrentLevel(true));
    	
			this.data.lvalue = Math.max(lvalue, this.data.lvalue - 0.04);
			this.data.rvalue = Math.max(rvalue, this.data.rvalue - 0.04);
    	
			this.repaintImmediately();
		});
    
		widget.startTimer(30);
		return widget;
	};

	inline function getNormalizedPeakValue(gain)
	{
		return 0.01 * (100.0 + Engine.getDecibelsForGainFactor(gain));
	}
}