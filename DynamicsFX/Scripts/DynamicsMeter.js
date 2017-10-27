

namespace DynamicsMeter
{
	inline function closePath(p, targetLevel)
	{
		p.lineTo(1.0, 1.0 - targetLevel);
		p.lineTo(1.0, 0.0);
		p.lineTo(1.0, 1.0);
		p.closeSubPath();
	}


	inline function refreshPath(panel)
	{
		local p = panel.data.p;
		local l = panel.data.l;
		local fx = panel.data.fx;
		local lvl = panel.data.lastLevel;
	
		local gt = fx.getAttribute(fx.GateThreshold);
		local ct = fx.getAttribute(fx.CompressorThreshold);
		local cr = fx.getAttribute(fx.CompressorRatio);
		local lt = fx.getAttribute(fx.LimiterThreshold);
	
		local thisLevel = (100 + Engine.getDecibelsForGainFactor(fx.getCurrentLevel(true))) / 100.0;
	
		if(thisLevel > panel.data.lastLevel)
			panel.data.lastLevel = thisLevel;
		else
			panel.data.lastLevel = panel.data.lastLevel * 0.96;
	
		// Normalize them
		gt = (100.0 + gt) / 100.0;
	
		if(!fx.getAttribute(fx.GateEnabled))
			gt = 0.0;
	
		ct = Math.max(gt, (100.0 + ct) / 100.0);
	
		if(!fx.getAttribute(fx.CompressorEnabled))
			ct = -1.0;
	
		lt = Math.max(ct, (100.0 + lt) / 100.0);
	
		if(!fx.getAttribute(fx.LimiterEnabled))
			lt = 1.0;
	
		// Draw the background path
		p.clear();
		p.startNewSubPath(0.0, 1.0);
		p.lineTo(gt, 1.0);
		p.lineTo(gt, 1.0 - gt);
	
		if(ct != -1.0)
		{
			p.lineTo(ct, 1.0 - ct);
	
			// this is pretty trivial...
			local nx = ct + (1.0-ct)*1.0/cr;
			local a = (1.0 - ct) * (lt - ct) / (nx - ct);
	
			if(nx > lt)
			{
				p.lineTo(ct + a, 1.0 - lt);
				p.lineTo(1.0, 1.0 - lt);
			}
			else
			{
				p.lineTo(1.0, 1.0 - nx);
			}
		}
		else
		{
			p.lineTo(lt, 1.0 - lt);
			p.lineTo(1.0, 1.0 - lt);
		}
	
		p.lineTo(1.0, 0.0);
		p.lineTo(1.0, 1.0);
		p.closeSubPath();

	
		// Now draw the level path
		l.clear();
		l.startNewSubPath(0.0, 1.0);
	
		if(gt > lvl)
		{
			closePath(l, 0.0);
			return;
		}
	
		l.lineTo(gt, 1.0);
		l.lineTo(gt, 1.0 - gt);
	
		if(ct != -1.0)
		{
			if(ct > lvl)
			{
				l.lineTo(lvl, 1.0-lvl);
				closePath(l, lvl);
				return;
			}
		
			if(lvl > lt)
			{
				l.lineTo(ct, 1.0 - ct);
	
				local nx = ct + (1.0-ct)*1.0/cr;
				local a = (1.0 - ct) * (lt - ct) / (nx - ct);
	
				if(nx > lt)
				{
					l.lineTo(ct + a, 1.0 - lt);
					l.lineTo(1.0, 1.0 - lt);
				}
				else
				{
					l.lineTo(1.0, 1.0 - nx);
				}
			}
			else
			{
				l.lineTo(ct, 1.0 - ct);
	
				local nx = ct + (1.0-ct)*1.0/cr;
				local a = (1.0 - ct) * (lt - ct) / (nx - ct);
			
				if(lvl > nx)
				{
					if(nx > lt)
					{
						l.lineTo(ct + a, 1.0 - lt);
						l.lineTo(1.0, 1.0 - lt);
					}
					else
						l.lineTo(1.0, 1.0 - nx);
				}
				else
				{
					local b = (1.0 - ct) * (lvl - ct) / (nx - ct);
					l.lineTo(ct + b, 1.0 - lvl);
					closePath(l, lvl);
					return;
				}	
			}
		}
		else
		{
			l.lineTo(lvl, 1.0 - lvl);
			closePath(l, lvl);
			return;
		}
	
		l.lineTo(1.0, 0.0);
		l.lineTo(1.0, 1.0);
		l.closeSubPath();
	};

	inline function createDynamicsMeter(name, x, y, effectId)
	{
		local widget = Content.addPanel(name, x, y);
    
		Content.setPropertiesFromJSON(name, {
		"width": 260,
		"height": 230,
		"opaque": 1
		});
    
		widget.data.p = Content.createPath();
		widget.data.l = Content.createPath();
		widget.data.lastLevel = -1.0;
		widget.data.values = [];
		widget.data.values.reserve(5);
    
		widget.setPaintRoutine(function(g)
		{
			g.fillAll(0xff222222);
			g.setGradientFill([0xFF444444, 0.0, 0.0, 0xFF333333, 0.0, this.getHeight()]);
    	
			g.fillPath(this.data.p, [0, 0, this.getWidth(), this.getHeight()]);
			g.setColour(0x44FFFFFF);
    	
			g.fillPath(this.data.l, [0, 0, this.getWidth()+1, this.getHeight()+1]);
			g.drawPath(this.data.l, [0, 0, this.getWidth()+1, this.getHeight()+1], 2.0);
			g.drawRect([0, 0, this.getWidth(), this.getHeight()], 1.0);
		});
    
		widget.setTimerCallback(function()
		{
			refreshPath(this);
    	
			this.repaintImmediately();
		});
    
		widget.startTimer(30);
    
    
		widget.data.fx = Synth.getEffect(effectId);
		return widget;
	};
};