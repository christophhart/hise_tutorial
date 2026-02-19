namespace Modulation
{
	const var MODULATORS = [];
	
	for(m in Builder.MODS)
	{
		MODULATORS.push(Synth.getModulator(m[1]));
	} 
	
	const var MOD_WIDTHS = [[320, 1.0], //AHDSR
							[320, 1.0], // AHDSR2
							[320, 1.0], // AHDSR3
							[320, 1.0], // AHDSR4
							[220, 1.0], //LFO
							[100, 1.0], // MIDI CC
							 [100, 0.55], // Velocity
							 [100, 0.45], // PitchWheel
							 [260, 0.6], // Table 	
							 [260, 0.4], // Ramp
							 [0, 0.0] // Random
							 
							 ]; 
	
	inline function addComboBox(mod, parameter, items, x, y)
	{
		local k = Content.addComboBox(mod + "_" + parameter, x, y + 40);
		k.set("parentComponent", mod + "_ModPanel");
		k.set("text", parameter);
		k.set("processorId", mod);
		k.set("parameterId", parameter);
		k.set("width", 51);
		k.set("height", 58);
		k.set("items", items.join("\n"));
		k.set("textColour", 0x99000000);
		k.setLocalLookAndFeel(LookAndFeel.laf);
		return k;
	}
	
	inline function addKnob(mod, parameter, mode, x, y)
	{
		local k = Content.addKnob(mod + "_" + parameter, x, y + 40);
		k.set("parentComponent", mod + "_ModPanel");
		k.set("text", parameter);
		k.set("processorId", mod);
		k.set("parameterId", parameter);
		k.set("width", 51);
		k.set("height", 58);
		k.set("mode", mode);
		k.set("textColour", 0x99000000);
		k.setLocalLookAndFeel(LookAndFeel.laf);
		return k;
	}
	
	inline function buildInterface()
	{
		local idx = 0;

		local x = 0;
		local y = 0.0;

		local parent = Content.getComponent("ModPage");

		for(m in MODULATORS)
		{
			local b = MOD_WIDTHS[idx++];
			local n = m.getId();
			local ty = Math.fmod(y, 1.0) * parent.get("height");
			if(ty != 0)
			ty += 1;
			local p = Content.addPanel(n + "_ModPanel", x, ty);
			
			p.set("parentComponent", "ModPage");
			p.set("width", b[0]);
			p.set("height", b[1] * parent.get("height"));
			p.set("text", n);
			
			p.set("allowCallbacks", "Clicks, Hover & Dragging");
			
			p.setMouseCallback(function(event)
			{
				if(event.clicked && event.y < 30)
				{
					Console.print("DRAG");
					this.startInternalDrag({
						"Type": "ModulationDrag",
						"SourceIndex": 2
					});
				}
			});
			
			y += b[1];
			
			if(Math.fmod(y, 1.0) == 0.0)
				x += b[0];
				
			if(["AHDSR", "AHDSR2", "AHDSR3"].contains(n))
				x = 0;
				
			
				
			local mv = Content.addPanel(n + "_ModVAlue", 0, 12);
			mv.set("width", 12);
			mv.set("height", 12);
			mv.set("x", p.get("width")-20);
			mv.set("parentComponent", n + "_ModPanel");
			
			mv.data.mod = Synth.getModulator(n);
			
			mv.data.value = 0.0;
			
			mv.setPaintRoutine(function(g)
			{
				g.fillAll(0xFF222222);
				g.addNoise(0.05);
				g.setColour(0xFFBBBBBB);
				g.fillEllipse(this.getLocalBounds(1));
				g.setColour(0xFF101010);
				g.fillEllipse(this.getLocalBounds(2));
				g.setColour(Colours.mix(0xFF404040, 0xFFFFF600, this.data.value));
				g.fillEllipse(this.getLocalBounds(3));
			});
			
			mv.set("opaque", true);
			
			mv.setTimerCallback(function()
			{
				this.data.value = this.data.mod.getCurrentLevel();
				this.repaint();
			});
			
			mv.startTimer(30);
		}
		
		if(false)
		{
			for(i = 0; i < 4; i++)
			{
				local ahdsrName = "AHDSR";
				
				if(i != 0)
					ahdsrName += (i+1);
		
	
				addKnob(ahdsrName, "Attack", "Time", 7, 190);
				addKnob(ahdsrName, "Hold",   "Time", 69, 190);
				addKnob(ahdsrName, "Decay",  "Time", 132, 190);
				addKnob(ahdsrName, "Sustain", "NormalizedPercentage", 195, 190);
				addKnob(ahdsrName, "Release", "Time", 258, 190);
				
				local ac = addKnob(ahdsrName, "AttackCurve", "NormalizedPercentage", 7, 130);
				ac.set("text", "Curve");
				ac.set("defaultValue", 0.5);
				ac.setValue(0.5);
				
				local dc = addKnob(ahdsrName, "DecayCurve", "NormalizedPercentage", 132, 130).set("text", "Curve");
				local rc = addKnob(ahdsrName, "ReleaseCurve", "NormalizedPercentage", 258, 130).set("text", "Curve");
				local al = addKnob(ahdsrName, "AttackLevel", "NormalizedPercentage", 69, 130).set("text", "Level");
			}
		}
		
		
		if(false)
		{
			//addKnob("LFO", "Frequency", "Frequency", 5, 0 ).set("text", "Speed");
			//addKnob("LFO", "SmoothingTime", "Time", 60, 0 ).set("text", "Smooth");
			//addKnob("LFO", "PhaseOffset", "NormalizedPercentage", 110, 0 ).set("text", "Phase");
			addComboBox("LFO", "WaveFormType", ["Sine","Triangle", "Saw", "Square", "Random"], 0, 0);
		}
		
		if(false)
		{
			addComboBox("MIDI CC", "ControllerNumber", ["Modwheel", "BC", "Volume"],0, 0);
			addKnob("MIDI CC", "SmoothTime", "Time", 60, 0 ).set("text", "Smooth");
			addKnob("Pitch Wheel", "Smoothing", "Time", 25, 5 ).set("text", "Smooth");
		}
		
		if(false)
		{
			addKnob("Table", "Attack", "Time", 60, 0 );
			addKnob("Table", "Release", "Time", 60, 0 );
		}
		
		if(false)
		{
			addKnob("Ramp", "Attack", "Time", 60, 0 );
			addKnob("Ramp", "Release", "Time", 60, 0 );
		}
	}
	
inline function initModPanels()
{
	local l = [];
	
	for(m in MODULATORS)
	{
		local p = Content.getComponent(m.getId() + "_ModPanel");
		p.data.screens = [];
		p.setPaintRoutine(LookAndFeel.drawGreyPanel);
		
		for(c in p.getChildComponents())
		{
			if(c.get("type") == "ScriptSlider" ||
			        c.get("type") == "ScriptComboBox")
			{
				c.setLocalLookAndFeel(LookAndFeel.laf);
				
				if(c.get("type") == "ScriptSlider")
				{
					local targetId = c.get("id").replace("_", " ");
					c.set("matrixTargetId", targetId);
					c.set("itemColour", 0xFFFFC100);
				}
			}
		}
		
		l.push(p);
	}
	
	
	return l;
}
	
	inline function addScreen(panel, screen)
	{
		local p = Content.getComponent(panel + "_ModPanel");
		local s = Content.getComponent(screen);
		s.setLocalLookAndFeel(LookAndFeel.screenLaf);
		p.data.screens.push(Rectangle(s.getLocalBounds(0)).translated(s.get("x"), s.get("y")));
	}
	
	buildInterface();
	
	const var MOD_PANELS = initModPanels();
	
	const var ModulationScreens = [Content.getComponent("AHDSR_Graph"),
	                               Content.getComponent("LFOGraph"),
	                               Content.getComponent("AttackTable"),
	                               Content.getComponent("ReleaseTable")];
	
	const var ModulationTables = [Content.getComponent("VelocityTable"),
	                    Content.getComponent("AttackTable"),
	                    Content.getComponent("ReleaseTable")];
	
	
	for(m in ModulationScreens)
		m.setLocalLookAndFeel(LookAndFeel.screenLaf);
	
	for(m in ModulationTables)
			m.setLocalLookAndFeel(LookAndFeel.screenLaf);
	
	addScreen("AHDSR", "AHDSR_Graph");
	addScreen("AHDSR2", "AHDSR2_Graph");
	addScreen("AHDSR3", "AHDSR3_Graph");
	addScreen("AHDSR4", "AHDSR4_Graph");
	addScreen("LFO", "LFOGraph");
	addScreen("MIDI CC", "CCPlotter");
	addScreen("Velocity", "VelocityTable");
	addScreen("Table", "AttackTable");
	addScreen("Table", "ReleaseTable");
	
	inline function initAHDSRBroadcaster(index)
	{
		local names = ["AHDSR", "AHDSR2", "AHDSR3", "AHDSR4"];
		
		local COMPONENTS = {
			Attack: 	  Content.getComponent(names[index] + "_Attack"),
			Hold: 		  Content.getComponent(names[index] + "_Hold"),
			Decay: 		  Content.getComponent(names[index] + "_Decay"),
			Sustain: 	  Content.getComponent(names[index] + "_Sustain"),
			Release: 	  Content.getComponent(names[index] + "_Release"),
			DecayCurve:   Content.getComponent(names[index] + "_DecayCurve"),
			AttackCurve:  Content.getComponent(names[index] + "_AttackCurve"),
			ReleaseCurve: Content.getComponent(names[index] + "_ReleaseCurve"),
			AttackLevel:  Content.getComponent(names[index] + "_AttackLevel")
		};
		
		// Now we build the list of parameters to pass into the broadcaster
		local PARAMETERS = [];
		local COMPONENT_LIST = [];
		
		for(p in COMPONENTS)
		{
			PARAMETERS.push(p);
			COMPONENT_LIST.push(COMPONENTS[p]);
		}
		
		// this is the broadcaster
		local dragBroadcaster = Engine.createBroadcaster({
			id: "flexDragBroadcaster" + index,
			args: [ "id", "parameter", "value"]
		});
		
		// this broadcaster will suspend the dragBroadcaster when 
		// the knob is dragged (+ 100ms). This prevents the internal modulation
		// from moving the knob to the position (because the parameter modulation will
		// change the internal attribute)
		local dragBypasser = Engine.createBroadcaster({
			id: "dragBypasser" + index,
			args: ["component", "event"]
		});
		
		// listen to all knobs
		dragBypasser.attachToComponentMouseEvents(COMPONENT_LIST, "Clicks Only", "deactivate when dragging sliders");
		
		// suspend the broadcaster when the button is clicked
		dragBypasser.addListener(dragBroadcaster, "bypass", function(component, event)
		{
			if(event.clicked)
				this.setBypassed(true, false, SyncNotification);
		});
		
		// reactivate the drag broadcaster 100ms after the mouse was released
		dragBypasser.addDelayedListener(100, dragBroadcaster, "reactivate", function(component, event)
		{
			if(event.mouseUp)
				this.setBypassed(false, false, SyncNotification);
		});
		
		// attach it to the module parameters of the flex envelope
		dragBroadcaster.attachToModuleParameter(names[index], PARAMETERS, "drag handler");
			
		// and update the component values on parameter change. We pass in the JSON 
		// object with the component references as this object for a extra neat syntax	
		dragBroadcaster.addListener(COMPONENTS, "update UI knobs", function(id, p, value)
		{
			this[p].setValue(value);
		});
		
		return [dragBroadcaster, dragBypasser];
	}
	
	const var AHDSR_BROADCASTERS = [];
	
	AHDSR_BROADCASTERS.push(initAHDSRBroadcaster(0));
	AHDSR_BROADCASTERS.push(initAHDSRBroadcaster(1));
	AHDSR_BROADCASTERS.push(initAHDSRBroadcaster(2));
	AHDSR_BROADCASTERS.push(initAHDSRBroadcaster(3));
	
	const var AHDSR_PAGE_BUTTONS = [];
	const var AHDSR_PAGES = [Content.getComponent("AHDSR_ModPanel"),
	                         Content.getComponent("AHDSR2_ModPanel"),
	                         Content.getComponent("AHDSR3_ModPanel"),
	                         Content.getComponent("AHDSR4_ModPanel")];
	
	
	for( i = 0; i < 4; i++)
	{
		AHDSR_PAGE_BUTTONS.push(Content.addButton("AHDSR" + (i+1) + "_PageButton", 6 + i * 20, 3));
	}
	
	inline function onAhdsrPageButton(component, value)
	{
		if(value)
		{
			local idx = AHDSR_PAGE_BUTTONS.indexOf(component);
			
			for(i = 0; i < AHDSR_PAGES.length; i++)
				AHDSR_PAGES[i].set("visible", i == idx);
		}
	}
	
	for(pb in AHDSR_PAGE_BUTTONS)
	{
		pb.set("parentComponent", "ModPage");
		pb.set("width", 20);
		pb.set("height", 29);
		pb.set("radioGroup", 5125);
		pb.set("saveInPreset", false);
		pb.set("text", AHDSR_PAGE_BUTTONS.indexOf(pb) + 1);
		pb.setLocalLookAndFeel(LookAndFeel.draggerLaf);
		pb.setControlCallback(onAhdsrPageButton);
	}
	
}


