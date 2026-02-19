namespace LookAndFeel
{
	
//! UI CONSTANTS	
const var p = { EndCapStyle: "square", JointStyle: "curved", Thickness: 8.0 };
const var ARC = 2.3;
const var DRAG_SIZE = 25;

Engine.loadFontAs("{PROJECT_FOLDER}Fonts/Oswald.ttf", "Oswald");

inline function setupKnob(k)
{
	local mods = k.createModifiers();

	// Turn off fine tune
	k.setModifiers(mods.FineTune, mods.disabled);
	// Turn off mod scaling with CTRL
	k.setModifiers(mods.ScaleModulation, mods.ctrlDown);	
	k.setLocalLookAndFeel(laf);
}

inline function getPointOnArc(valueNormalized, b)
{
	return [(0.5 + 0.5 * Math.cos(valueNormalized * 2.0 * ARC + ARC + 0.1)) * b[2] + b[0],
		    (0.5 + 0.5 * Math.sin(valueNormalized * 2.0 * ARC + ARC + 0.1)) * b[3] + b[1]];
}

inline function getGradientOnArc(v1, v2, b, c1, c2)
{
	local p1 = getPointOnArc(v1, b);
	local p2 = getPointOnArc(v2, b);
	
	return [c1, p1[0], p1[1], c2, p2[0], p2[1], true];
}

inline function drawModulationDragBackground(g, obj)
{
	if(obj.hover)
	{
		local lr = Rectangle(obj.labelArea);

		g.setColour(0xFF282828);
		g.fillRoundedRectangle(lr, 3.0);
		g.setColour(0xFFAAAAAA);
		g.drawRoundedRectangle(lr, 3.0, 1.0);
		g.setColour(0xDDFFFFFF);
		g.setFont("Oswald", 20.0);
		g.drawAlignedText(obj.hoverSourceName + ": " + obj.hoverText, lr.reduced(4, 0), "centred");
	}
}

inline function drawModulationDragger(g, obj)
{
	local area = Rectangle(obj.area);
	local track = Content.createPath();
	local dragPath = Content.createPath();
	local start;
	local end;
	
	switch(obj.mode)
	{
		case 0:
			start = -ARC;
			end = -ARC + 2.0 * ARC * obj.value;
			break;
		case 1:
			start = 0;
			end = ARC * obj.value;
			break;
		case 2:
			end = ARC * obj.value;
			start = - 1.0 * end;
			break;
	}
	
	dragPath.startNewSubPath(area[0], area[1]);
	dragPath.startNewSubPath(area[0] + area[2], area[1] + area[3]);
	
	track.startNewSubPath(area[0], area[1]);
	track.startNewSubPath(area[0] + area[2], area[1] + area[3]);
	
	g.setColour(0xFF282828);
	g.fillEllipse(area);
	g.setColour(0xFFAAAAAA);
	g.drawEllipse(area, 1.0);
		
	dragPath.addArc(area.reduced(4), start, end);
	track.addArc(area.reduced(4), -ARC, ARC);
	g.setColour(obj.hover ? 0x44ffffff : 0x22ffffff);
	p.Thickness = 3;
	g.fillPath(track.createStrokedPath(p, ""), area);
	g.setColour(Colours.withMultipliedAlpha(obj.itemColour, obj.clicked ? 1.0 : 0.8));
	g.fillPath(dragPath.createStrokedPath(p, ""), area);
}

inline function getModulatorDragData(obj)
{
	local sb = Rectangle(obj.sliderBounds).reduced(-DRAG_SIZE * 0.8, 0);
	
	local l = sb.removeFromLeft(DRAG_SIZE);
	local r = sb.removeFromRight(DRAG_SIZE);
	local toUse = l;
	
	for(s in obj.connections)
	{
		local t = toUse.removeFromTop(DRAG_SIZE);
		if(t[3] < DRAG_SIZE)
		{
			toUse = r;
			t = toUse.removeFromTop(DRAG_SIZE);
		}
		
		obj.dragAreas.push(t);
	}

	obj.labelArea = sb.removeFromBottom(20).withSizeKeepingCentre(100, 20).translated(0, 5);


	return obj;

	/*

	local totalWidth = obj.connections.length * DRAG_SIZE;
	totalWidth += 110;
	
	local x = Rectangle(obj.sliderBounds[0], obj.sliderBounds[1] + obj.sliderBounds[3] + 5, totalWidth, DRAG_SIZE);
	x = x.constrainedWithin(obj.parentBounds);
	
	for(s in obj.connections)
		obj.dragAreas.push(x.removeFromLeft(DRAG_SIZE));
	
	x.removeFromLeft(10);
	
	obj.labelArea = x;
	return obj;
	*/
}

inline function drawRotarySlider(g, obj)
{
	local isMatrixKnob = obj.parentType == "ModulationMatrix";
	local paths = [];
	local rc = obj.itemColour1;
	local b = Rectangle(obj.area);
	local w = Math.min(obj.area[2], obj.area[3]);
	b[2] = w;
	b[3] = w;
	
	if(obj.modulationDragState > 0)
	{
		g.setColour(Colours.withAlpha(obj.itemColour1, obj.modulationDragState * 0.3));
		g.fillEllipse(b.reduced(2));
		g.drawEllipse(b.reduced(2), 2.0);
	}
	
	for(i = 0; i < 4; i++)
	{
		paths.push(Content.createPath());
		paths[i].startNewSubPath(0, 0);
		paths[i].startNewSubPath(b[2], b[3]);
	}
	
	paths[0].addArc(b.reduced(0.1*w), -ARC, ARC);
	
	g.setColour(0xFF444444);
	p.Thickness = 4;
	
	g.drawPath(paths[0], b, p);
	
	//g.drawInnerShadowFromPath(paths[0].createStrokedPath(p, 0), b, 0xff000000, 5, [0, 0]);
	
	local r = -ARC + obj.valueNormalized * 2 * ARC;
	
	local sh = Content.createPath();
	sh.addEllipse([0,0, 1.0, 1.0]);
	
	g.setColour(0xFF666666);
	g.fillEllipse(b.reduced(0.17*w));
	g.drawDropShadowFromPath(sh, b.reduced(0.22 * w), 0xBB000000, 0.2 * w, [0, 0.1 * w]);
	g.drawDropShadowFromPath(sh, b.reduced(0.31 * w), 0x55000000, 3, [0, 8]);
	
	g.setGradientFill([0xFF877777, 0.0, 0.0, 0xFFBBBBBB, 0.0, b[3], false]);
	g.fillEllipse(b.reduced(0.25 * w));
	g.setColour(0xFF666666);
	g.rotate(r, [b[2]/2, b[3]/2]);
	g.setColour(0x66FFFFFF);
	g.drawEllipse(b.reduced(0.25 * w), 1.0);
	g.setColour(0x88000000);
	g.fillRoundedRectangle(b.withSizeKeepingCentre(2.0, 13).translated(0, -7), 1);
	g.rotate(-r, [b[2]/2, b[3]/2]);
	g.setColour(obj.textColour);
	
	paths[1].addArc(b.reduced(0.1 * w), -ARC + obj.modMinValue * 2.0 * ARC, -ARC + obj.modMaxValue * 2.0 * ARC);
	
	p.Thickness = 3;
	g.setColour(Colours.withAlpha(rc, 0.6));
	g.drawPath(paths[1], b, p);
	g.drawInnerShadowFromPath(paths[1].createStrokedPath(p, ""), b, Colours.withAlpha(rc, obj.clicked ? 1.0 : 0.5), 3, [0, 0]);
	
	local cb = b.reduced(6);
	local mv;
	
	if(isMatrixKnob)
	{
		if(obj.min == -1.0)
		{
			local isBipolar = obj.valueAsText.contains("+-");
			
			paths[2].addArc(b.reduced(0.1 * w), isBipolar ? -ARC * obj.value : 0, ARC * obj.value);
			
		}
		else
		{
			paths[2].addArc(b.reduced(0.1 * w), -ARC, -ARC + 2.0 * ARC * obj.value);
		}

		g.setColour(0xaaFFFFFF);
		p.Thickness = 3;
		g.drawPath(paths[2], b, p);
		
	}
	else if(obj.modulationActive)
	{
		mv = (obj.scaledValue + obj.addValue);
		local lm = obj.lastModValue;
		paths[2].addArc(b.reduced(0.1 * w), -ARC + lm * 2.0 * ARC, -ARC + mv * 2.0 * ARC);
		local brightness = mv - lm;
		local modRange = obj.modMaxValue - obj.modMinValue;
		local alpha = Math.range(brightness / modRange, 0.0, 1.0);
		
		g.setGradientFill(getGradientOnArc(mv, lm, cb, 0x99FFFFFF, Colours.withAlpha(rc, 0.1)));
		p.Thickness = 4;
		g.drawPath(paths[2], b, p);
		g.setColour(Colours.red);
	}
	else
	{
		mv = obj.valueNormalized;
	}
	
	if(isMatrixKnob)
	{
		g.setFontWithSpacing("Oswald", 20.0, 0.03);
		g.setColour(0x88000000);
		
		local ta = Rectangle(obj.area);
		ta.removeFromLeft(w + 5);
		
		g.drawAlignedText(obj.valueAsText, ta, "left");
	}
	else
	{
		local vp = getPointOnArc(mv, cb.reduced(0));
		local cp = Content.createPath();
		local vb = [vp[0]-1.5, vp[1]-1.5, 3, 3];
		cp.addEllipse(vb);
		g.drawDropShadowFromPath(cp, vb, rc, 5, [0, 0]);
		g.setColour(Colours.withAlpha(Colours.white, 0.9));
		g.fillPath(cp, vb);
		
		g.setColour(obj.textColour);
		g.setFontWithSpacing("Oswald", 18.0, 0.03);
	

		g.drawAlignedText(obj.hover ? obj.valueAsText : obj.text, obj.area, "centredBottom");
	}
}

//! LAF definitions

const var draggerLaf = Content.createLocalLookAndFeel();
draggerLaf.setStyleSheet("dragger.css");

const var laf = Content.createLocalLookAndFeel();

laf.registerFunction("drawModulationDragBackground", drawModulationDragBackground);
laf.registerFunction("drawModulationDragger", drawModulationDragger);
laf.registerFunction("getModulatorDragData", getModulatorDragData);
laf.registerFunction("drawRotarySlider", drawRotarySlider);

laf.setStyleSheet("main.css");

const var headerLaf = Content.createLocalLookAndFeel();
headerLaf.setStyleSheet("header.css");

const var pageLaf = Content.createLocalLookAndFeel();

pageLaf.setStyleSheet("page.css");

const var keyboardLaf = Content.createLocalLookAndFeel();

keyboardLaf.setStyleSheet("keyboard.css");

inline function drawPage(g)
{
	g.fillAll(0xFF181818);
}

inline function drawGreyPanel(g)
{
	local area = Rectangle(this.getLocalBounds(0));

	g.setGradientFill([0xFFAAAAB5, 0.0, 0.0, 0xFFA59999, 0.0, this.get("height"), false]);
	
	g.fillRoundedRectangle(area.reduced(1.0), 3);
	g.setColour(0xFFBFBBBB);
	g.drawRoundedRectangle(area.reduced(2.0), 3.0, 1.0);
	
	g.setColour(0xFF161616);
	
	local header = area.removeFromTop(35).reduced(2);
	
	g.setGradientFill([0xFF303030, 0.0, 0.0, 0xFF161616, 0.0, 35.0, false]);
	
	
	
	
	
	g.fillRoundedRectangle(header, 2.0);
	
	g.addNoise({
			alpha: 0.02,
			monochromatic: true,
			scaleFactor: 1.0,
			
		});
	
	g.setColour(0xFFBBBBBB);
	g.drawRoundedRectangle(header.reduced(0.3), 2.0, 1.0);
	g.setColour(0xFFCCCCCC);
	g.setFontWithSpacing("Oswald Bold", 22.0, 0.0);
	g.drawAlignedText(this.get("text"), header, "centred");
	
	if(isDefined(this.data.screens))
	{
		for(s in this.data.screens)
		{
			g.setColour(0xFF111111);
			g.fillRoundedRectangle(s.reduced(-4), 6.0);
			g.setColour(0xFFBBBBBB);
			g.drawRoundedRectangle(s.reduced(-4), 6.0, 1);
		}
	}
}

const var screenLaf = Content.createLocalLookAndFeel();

screenLaf.registerFunction("drawAnalyserBackground", function(g, obj)
{
	g.fillAll(0xFF161616);
	g.setColour(0xFF262626);
	g.fillRoundedRectangle(obj.area, 3.0);
});

screenLaf.registerFunction("drawAnalyserPath", function(g, obj)
{
	var r = Rectangle(obj.area);

	g.drawDropShadowFromPath(obj.path, r.reduced(-5, 2), Colours.withAlpha(obj.itemColour1,0.2), 10, [0, 0]);
	g.setGradientFill([obj.itemColour1, 0.0, 0.0, obj.itemColour2, 0.0, obj.area[3], false]);
	g.drawPath(obj.path, r.reduced(-3, 2), 1);

	return;
	
	
	g.fillPath(obj.path, r.reduced(3));
});

screenLaf.registerFunction("drawTableBackground", function(g, obj)
{
	g.setColour(0xFF262626);
	g.fillRoundedRectangle(obj.area, 3.0);
});

screenLaf.registerFunction("drawTablePath", function(g, obj)
{
	var r = Rectangle(obj.area);

	g.drawDropShadowFromPath(obj.path, r.reduced(-5, 2), Colours.withAlpha(obj.itemColour,0.2), 10, [0, 0]);
	g.setGradientFill([obj.itemColour, 0.0, 0.0, obj.itemColour2, 0.0, obj.area[3], false]);
	g.drawPath(obj.path, r.reduced(-3, -2), 1);
	

	//Console.print(trace(obj));
});

screenLaf.registerFunction("drawTablePoint", function(g, obj)
{
	g.setColour(0x88FFFFFF);
	g.fillRoundedRectangle(Rectangle(obj.tablePoint).reduced(3), 3);
	
});

screenLaf.registerFunction("drawTableRuler", function(g, obj)
{
	g.setColour(Colours.white);
	g.fillRoundedRectangle([obj.position * obj.area[2], 0, 3, obj.area[3]], 1.5);
});

screenLaf.registerFunction("drawFilterBackground", function(g, obj)
{
	 g.setColour(0xFF262626);
	 g.fillRoundedRectangle(obj.area, 3.0);
});

screenLaf.registerFunction("drawFlexAhdsrBackground", function(g, obj)
{
	g.setColour(0xFF262626);
    g.fillRoundedRectangle(obj.area, 3.0);
});

screenLaf.registerFunction("drawFlexAhdsrFullPath", function(g, obj)
{
	 var r = Rectangle(obj.pathArea);
	 
	 //Console.print(trace(obj));
	 g.drawDropShadowFromPath(obj.path, r, Colours.withAlpha(obj.itemColour,0.2), 10, [0, 0]);
	 	g.setGradientFill([obj.itemColour, 0.0, 0.0, obj.itemColour2, 0.0, obj.area[3], false]);
	 	g.drawPath(obj.path, r, 1);
});

screenLaf.registerFunction("drawFlexAhdsrPosition", function(g, obj)
{
	g.setColour(Colours.white);
	
	g.fillRect([obj.pointOnPath[0], 10, 2, obj.area[3] - 20]);

	//Console.print(trace(obj));
});

screenLaf.registerFunction("drawFilterPath", function(g, obj)
{
	

	 g.setGradientFill([obj.itemColour1, 0.0, 0.0, obj.itemColour2, 0.0, obj.area[3], false]);
	 g.drawPath(obj.path, Rectangle(obj.area).reduced(-10), 2);
});

}
