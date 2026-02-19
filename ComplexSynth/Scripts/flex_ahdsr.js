

const var rm = Engine.getGlobalRoutingManager();

const var fe1 = rm.getCable("ahdsr_flex_ui");

const var RELEASE_POINT = 0.66;
const var TIME_SKEW = 0.2;
const var MARGIN = 10;


const var ahdsr = Synth.getModulator("HardcodedEnvelopeModulator1");

namespace ParameterType
{
	const var Curve = 0;
	const var Time = 1;
	const var Level = 2;
}

namespace State
{
	const IDLE = 0;
	const ATTACK = 1;
	const HOLD = 2;
	const DECAY = 3;
	const SUSTAIN = 4;
	const RELEASE = 5;
	const DONE = 6;
}

namespace SpecialParameters
{
	const var OFFSET = 2;

	const Attack = 		 0 + OFFSET;
	const Hold = 		 1 + OFFSET;
	const Decay = 		 2 + OFFSET;
	const Sustain = 	 3 + OFFSET;
	const Release = 	 4 + OFFSET;
	const Mode = 		 5 + OFFSET;
	const AttackLevel =  6 + OFFSET;
	const AttackCurve =  7 + OFFSET;
	const DecayCurve =   8 + OFFSET;
	const ReleaseCurve = 9 + OFFSET;
};

const var Panel1 = Content.getComponent("Panel1");


Panel1.set("allowCallbacks", "All Callbacks");

inline function getAttributeIndex(p)
{
	if(p.data.hoverMode == ParameterType.Curve)
	{
		switch(p.data.hoverIndex)
		{
			case State.ATTACK: return SpecialParameters.AttackCurve;
			case State.DECAY: return SpecialParameters.DecayCurve;
			case State.RELEASE: return SpecialParameters.ReleaseCurve;
		}
	}
	else if (p.data.hoverMode == ParameterType.Time)
	{
		switch(p.data.hoverIndex)
		{
			case State.ATTACK: return SpecialParameters.Attack;
			case State.HOLD: return SpecialParameters.Hold;
			case State.DECAY: return SpecialParameters.Decay;
			case State.RELEASE: return SpecialParameters.Release;
		}
	}
	else if(p.data.hoverMode == ParameterType.Level)
	{
		switch(p.data.hoverIndex)
		{
			case State.ATTACK:
			case State.HOLD:    return SpecialParameters.AttackLevel;
			case State.SUSTAIN: return SpecialParameters.Sustain;
		}
	}
	
	return -1;
}

inline function getLabel(p)
{
	local pIndex = getAttributeIndex(p);

	if(pIndex != -1)
	{
		local names = [
		"Attack Time",
	 	"Hold Time",
	 	"Decay Time",
	 	"Sustain Level",
	 	"Release Time",
	 	"Mode",
	 	"Attack Level",
	 	"Attack Curve",
	 	"Decay Curve",
	 	"Release Curve"
		];
		
		local l = names[pIndex-SpecialParameters.OFFSET];
		
		l += ": ";
		
		local converter = "NormalizedPercentage";
		
		if(p.data.hoverMode == ParameterType.Time)
			converter = "Time";
		
		l += Engine.getTextForValue(ahdsr.getAttribute(pIndex), converter);
		return l;
	}

	return "";
}

inline function toNorm(p, attributeValue)
{
	if(p.data.hoverMode == ParameterType.Time)
		return Math.pow(attributeValue / 30000.0, TIME_SKEW);
		
	if(p.data.hoverMode == ParameterType.Curve && p.data.hoverIndex == State.ATTACK)
		return 1.0 - attributeValue;
		
	return attributeValue;
}

inline function fromNorm(p, attributeValue)
{
	if(p.data.hoverMode == ParameterType.Time)
		return Math.pow(attributeValue, 1.0 / TIME_SKEW) * 30000.0;
		
	if(p.data.hoverMode == ParameterType.Curve && p.data.hoverIndex == State.ATTACK)
		return 1.0 - attributeValue;
		
	return attributeValue;
}

Panel1.setMouseCallback(function(event)
{
	if(event.clicked)
	{
		var pIndex = getAttributeIndex(this);

		if(pIndex != -1)
			this.data.downValue = toNorm(this, ahdsr.getAttribute(pIndex));
	}

	if(event.drag)
	{
		var isX = this.data.hoverMode == ParameterType.Time;
		var delta = isX ? (0.4 * event.dragX / (this.get("width") * 0.25)) : (-1.0 * event.dragY / this.get("height"));
		var newValue = Math.range(this.data.downValue + delta, 0.0, 1.0);
		var pIndex = getAttributeIndex(this);
		
		if(pIndex != -1)
			ahdsr.setAttribute(pIndex, fromNorm(this, newValue));
	}
	else
	{
		this.data.hoverIndex = -1;
		this.data.hoverMode = -1;
		
		Console.assertEqual(this.data.boxes.length, this.data.curvePoints.length);
		
		for(i = 0; i < this.data.boxes.length; i++)
		{
			var b = this.data.boxes[i];

			if(isDefined(b) && b.contains([event.x, event.y]))
			{
				this.data.hoverIndex = i;
				
				var xNorm = (event.x - this.data.boxes[i][0]) / this.data.boxes[i][2];
				var yNorm =  (event.y - this.data.boxes[i][1]) / this.data.boxes[i][3];
				
				var isTime = yNorm > 0.2;
				
				if(i == State.RELEASE || i == State.DECAY) 
					isTime = true;
				else if(i == State.SUSTAIN)
					isTime = false;
					
				var dx = this.data.curvePoints[i][0] - event.x;
				var dy = this.data.curvePoints[i][1] - event.y;
				var cd = Math.sqrt(dx*dx + dy*dy);
				var curveHover = cd < 20.0;
					
				if(curveHover)
					this.data.hoverMode = ParameterType.Curve;
				else
				{
					if(isTime)
						this.data.hoverMode = ParameterType.Time;
					else
						this.data.hoverMode = ParameterType.Level;
				}
				
				break;
			}
		}
		
		updateMouseCursor(this);
	}
	
	this.repaint();
});

inline function updateMouseCursor(p)
{
	local mc = "NormalCursor";

	switch(p.data.hoverMode)
	{
		case ParameterType.Time: 
			mc = "LeftRightResizeCursor"; 
			break;
		case ParameterType.Curve:
			mc = "DraggingHandCursor";
			break;
		case ParameterType.Level:
			mc = "UpDownResizeCursor";
			break;
	}

	p.setMouseCursor(mc, Colours.white, [0, 0]);
}


Panel1.setPaintRoutine(function(g)
{
	g.fillAll(0x22FFFFFF);
	
	var idx = 0;
	
	if(isDefined(this.data.segments))
	{
		for(p in this.data.segments)
		{	
			if(idx == this.data.CurrentState)
			{
				g.setColour(0x22FFFFFF);
				g.fillPath(p, this.getLocalBounds(10));
			}
			
			if(idx == this.data.hoverIndex)
			{
				g.setColour(0x05FFFFFF);
				g.fillPath(p, this.getLocalBounds(10));
			}
			
			idx++;
		}
	}
	
	if(false && isDefined(this.data.boxes))
	{
		idx = 0;

		for(b in this.data.boxes)
		{
			if(isDefined(b))
			{
				g.setColour(idx == this.data.hoverIndex ? (this.data.ok ? Colours.red : Colours.yellow) : Colours.grey);
				g.fillRect(b.reduced(2));
			}

			idx++;	
		}
	}
	
	if(isDefined(this.data.path))
	{
		g.setColour(Colours.white);
		g.drawPath(this.data.path, this.getLocalBounds(MARGIN), 2.0);
	}
	
	if(isDefined(this.data.curvePoints))
	{
		idx = 0;

		for(cp in this.data.curvePoints)
		{
			if(isDefined(cp))
			{
				var active = this.data.hoverIndex == idx && this.data.hoverMode == ParameterType.Curve;
				
				g.setColour(Colours.white);
				
				var margin = active ? 10 : 7;
				
				g.fillEllipse([cp[0]-margin/2, cp[1]-margin/2, margin, margin]);
			}
			
			idx++;
		}
	}
	
	g.setFont("Lato", 14);
	
	g.drawAlignedText(getLabel(this), this.getLocalBounds(10), "topRight");
	
});

fe1.registerDataCallback(function(data)
{
	for(s in data)
		Panel1.data[s] = data[s];

	Panel1.data.Attack = 0.25 * Math.pow(data.Attack / 30000.0, TIME_SKEW);
	Panel1.data.Hold = Math.max(0.01, 0.25 * Math.pow(data.Hold / 30000.0, TIME_SKEW));
	Panel1.data.Decay = 0.25 * Math.pow(data.Decay / 30000.0, TIME_SKEW);
	Panel1.data.Release = 0.25 * Math.pow(data.Release / 30000.0, TIME_SKEW);
	
	var area = Rectangle(Panel1.getLocalBounds(10));
	var w = area[2];
	var h = area[3];
	
	Panel1.data.boxes = [];
	Panel1.data.boxes.push(undefined);
	Panel1.data.boxes.push(area.removeFromLeft(w * Panel1.data.Attack).removeFromBottom(h * Panel1.data.AttackLevel));
	Panel1.data.boxes.push(area.removeFromLeft(w * Panel1.data.Hold).removeFromBottom(h * Panel1.data.AttackLevel));
	Panel1.data.boxes.push(area.removeFromLeft(w * Panel1.data.Decay).removeFromBottom(h * Panel1.data.AttackLevel));
	
	var state1 = Panel1.data.Attack + Panel1.data.Hold + Panel1.data.Decay;
	
	if(state1 < RELEASE_POINT)
		Panel1.data.boxes.push(area.removeFromLeft((RELEASE_POINT - state1) * w));
	else
		Panel1.data.boxes.push(area.removeFromLeft(0));
	
	Panel1.data.boxes.push(area.removeFromLeft(w * Panel1.data.Release));
	
	Panel1.data.boxes[State.ATTACK][0] -= MARGIN;
	Panel1.data.boxes[State.ATTACK][2] += MARGIN;
	
	Panel1.data.boxes[State.DECAY][2] += MARGIN;
	
	Panel1.data.boxes[State.SUSTAIN][3] += MARGIN;
	
	Panel1.data.boxes[State.RELEASE][2] += MARGIN;
	
	Panel1.data.segments = [];
	
	for(b in Panel1.data.boxes)
	{
		var np = Content.createPath();
		
		np.startNewSubPath(1.0, 0.0); 
		np.startNewSubPath(0.0, 1.0); 
		Panel1.data.segments.push(np);
	}
		
	var path = Content.createPath();
	var x = 0.0;
	var y = 1.0;
	var cx = 0.0;
	var cy = 1.0;
	var alpha = 0.5;
	
	path.startNewSubPath(1.0, 0.0); 
	
	path.startNewSubPath(x, y); 
	
	x += Panel1.data.Attack;
	y = 1.0 - Panel1.data.AttackLevel;
	
	alpha = Panel1.data.AttackCurve;
	
	cy = alpha * cy + (1.0 - alpha) * y;
	cx = (1.0 - alpha) * cx + (alpha) * x;
	
	path.quadraticTo(cx, cy, x, y);
	
	Panel1.data.segments[State.ATTACK].startNewSubPath(0.0, 1.0);
	Panel1.data.segments[State.ATTACK].quadraticTo(cx, cy, x, y);
	Panel1.data.segments[State.ATTACK].lineTo(x, 1.0);
	Panel1.data.segments[State.ATTACK].closeSubPath();
	
	Panel1.data.segments[State.HOLD].startNewSubPath(x, 1.0);
	Panel1.data.segments[State.HOLD].lineTo(x, y);
	x += Panel1.data.Hold;
	Panel1.data.segments[State.HOLD].lineTo(x, y);
	Panel1.data.segments[State.HOLD].lineTo(x, 1.0);
	Panel1.data.segments[State.HOLD].closeSubPath();
	
	path.lineTo(x, y);
	
	cx = x;
	cy = y;
	
	Panel1.data.segments[State.DECAY].startNewSubPath(x, 1.0);
	Panel1.data.segments[State.DECAY].lineTo(x, y);
	
	x += Panel1.data.Decay;
	y = 1.0 - Panel1.data.Sustain;
	
	alpha = Panel1.data.DecayCurve;
	
	cy = alpha * cy + (1.0 - alpha) * y;
	cx = (1.0 - alpha) * cx + (alpha) * x;
	
	Panel1.data.segments[State.DECAY].quadraticTo(cx, cy, x, y);
	Panel1.data.segments[State.DECAY].lineTo(x, 1.0);
	Panel1.data.segments[State.DECAY].closeSubPath();
	
	path.quadraticTo(cx, cy, x, y);
	
	if(x < RELEASE_POINT)
	{
		Panel1.data.segments[State.SUSTAIN].startNewSubPath(x, 1.0);
		Panel1.data.segments[State.SUSTAIN].lineTo(x, y);
	

		x = RELEASE_POINT;
		
		Panel1.data.segments[State.SUSTAIN].lineTo(x, y);
		Panel1.data.segments[State.SUSTAIN].lineTo(x, 1.0);
		Panel1.data.segments[State.SUSTAIN].closeSubPath();
		
		
		path.lineTo(x, y);
	}
	
	cx = x;
	cy = y;
	
	Panel1.data.segments[State.RELEASE].startNewSubPath(x, 1.0);
	Panel1.data.segments[State.RELEASE].lineTo(x, y);
	
	x = Math.min(1.0, x + Panel1.data.Release);
	y = 1.0;
	
	alpha = Panel1.data.ReleaseCurve;
		
	cy = alpha * cy + (1.0 - alpha) * y;
	cx = (1.0 - alpha) * cx + (alpha) * x;
	
	path.quadraticTo(cx, cy, x, y);
	Panel1.data.segments[State.RELEASE].quadraticTo(cx, cy, x, y);
	Panel1.data.segments[State.RELEASE].closeSubPath();
	
	path.lineTo(1.0, 1.0);
	
	path.scaleToFit(MARGIN, MARGIN, Panel1.get("width") - 2*MARGIN, Panel1.get("height") - 2*MARGIN, false);
	
	Panel1.data.curvePoints = [];
	
	
	var idx = 0;
	
	for(b in Panel1.data.boxes)
	{
		if(isDefined(b))
		{
			var midX = b[0] + b[2] * 0.5 - 5; 
					
			if(idx == State.HOLD || idx == State.SUSTAIN || b[2] < 12)
			{
				Panel1.data.curvePoints.push(undefined);
			}
			else
			{
				var y = path.getYAt(midX);
				
				if(isDefined(y))
				{
					Panel1.data.curvePoints.push([midX, y]);
				}
				else
				{
					Panel1.data.curvePoints.push(undefined);
				}
			}
		}
		else
		{
			Panel1.data.curvePoints.push(undefined);
		}
		
		idx++;
	}
	
	Panel1.data.path = path;
	Panel1.repaint();
	
	for(np in Panel1.data.segments)
	{
		np.scaleToFit(MARGIN, MARGIN, Panel1.get("width") - 2*MARGIN, Panel1.get("height") - 2*MARGIN, false);
	}
});