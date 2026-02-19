namespace WavetableWaterfall
{
	const var Size = {
	  "x": 28.0,
	  "y": 113.0,
	  "width": 560.0,
	  "height": 190.0
	};
	
	const var DATA = 
	
	{
	  "Type": "WavetableWaterfall",

	  "ProcessorId": "WT1",
	  "Index": 0,
	  "FollowWorkspace": false,
	  "Displacement": [
	    0.4,
	    -0.8
	  ],
	  "LineThickness": [
	    2.0,
	    0.4
	  ],
	  "NumDisplayTables": 128,
	  "IsometricFactor": 0.5,
	  "AlphaData": {
	    "NeighbourGlow": 0.0,
	    "Decay3D": 0.8,
	    "FillGain": 0.5,
	    "FillGainCenter": 0.0,
	    "ActiveGlowColour": "0xFFFFFFFF",
	    "ActiveGlowRadius": 3.0,
	    "PeakGain": 2.0,
	    "PrerenderBackground": true,
	    "NumHighlights": 8,
	    "HighlightGain": 1.0,
	    "SmoothRange": [
	      0.0,
	      1.0
	    ]
	  }
	};
}

namespace MainPage
{
	const var EMBEDDED_WAVEFORMS = Engine.loadAudioFilesIntoPool();
	

	const var OSC_HEIGHT = 200;
	

	inline function addBypassButton(target)
	{
		local b = Content.addButton(target + "Enabled", 0, 0);
		local p = Content.getComponent(target + "_Page");
		b.set("parentComponent", target + "_Page");
		b.set("processorId", target);
		b.set("parameterId", "Enabled");
		b.set("width", p.getWidth());
		b.set("height", 35);
		b.setLocalLookAndFeel(LookAndFeel.laf);
		b.setStyleSheetProperty("icon", PathIcons.data.Bypass, "path");
		b.setStyleSheetClass("bypass");
	}

	inline function addKnob(index, suffix, x, y)
	{
		local id = "OSC" + index + suffix;
		local name = "OSC" + index + " " + suffix;
		Console.print("ADD " + id);

		local k = Content.addKnob(id, x, y);
		local p = "OSC" + index + "_Page";
		k.set("parentComponent", p);
		k.setLocalLookAndFeel(LookAndFeel.laf);
		k.set("width", 60);
		k.set("height", 70);
		k.set("text", suffix);
		k.set("processorId", name);
		k.set("parameterId", "Value");
		k.set("textColour", 0xaa000000);
		
		local r = Builder.TARGETS[suffix].InputRange;
		Console.assertIsDefined(r);
		
		for(v in r)
			k.set(v, r[v]);
		
		local mode = Builder.TARGETS[suffix].TextConverter;
		
		if(mode == "Decibels") // HISE...
			mode = "Decibel";
		
		if(isDefined(mode) && mode != "Semitones")
			k.set("mode", mode);
		
		if(suffix == "Pitch")
		{
			k.set("suffix", " st");
			k.set("itemColour", 0xFFA800FF);
		}
		if(suffix == "Fine")
		{
			k.set("suffix", " ct");
			k.set("stepSize", 1);
			k.set("itemColour", 0xFFA800FF);
		}
		if(suffix == "Gain")
		{
			k.set("itemColour", 0xFFFFCA00);
		}
		if(suffix == "Position")
		{
			k.set("itemColour", 0xFF00FF84);
		}
		if(suffix == "Filter Frequency")
		{
			k.set("itemColour", 0xFF39AA9A);
			k.set("parentComponent", "OSC" + index + "_Filter_Page");
			k.set("text", "Frequency");
		}
		if(suffix == "Pan")
		{
			k.set("itemColour", 0xFF39AA9A);

		}
		if(suffix == "Detune")
		{
			k.set("suffix", " st");
			k.set("itemColour", 0xFFFF0061);
		}
		if(suffix == "Filter Q")
		{
			k.set("itemColour", 0xFF39AA9A);
			k.set("parentComponent", "OSC" + index + "_Filter_Page");
		}
		
		return k;
	}

	const var SynthGroups = [ Synth.getChildSynth("OSC1"), Synth.getChildSynth("OSC2") ];
	const var WavetableSynths = [Synth.getChildSynth("WT1"), Synth.getChildSynth("WT2") ];
	const var FileDropTargets = [Synth.getAudioSampleProcessor("WT1").getAudioFile(0),
	                             Synth.getAudioSampleProcessor("WT2").getAudioFile(0)];
	const var Filters = [Synth.getEffect("OSC1_Filter"), Synth.getEffect("OSC2_Filter")];

	const var FilterModeList = Engine.getFilterModeList();

	const var FilterModes = [ FilterModeList.StateVariableLP, 
							  FilterModeList.StateVariableHP, 
							  FilterModeList.StateVariableBandPass ];

	inline function onComboBox(component, value)
	{
		local id = component.get("id");
		Console.print(id);
		local idx = id.getTrailingIntValue() - 1;
		Console.print(idx);
		
		if(id.contains("Unisono"))
		{
			SynthGroups[idx].setAttribute(7, value);
		}
		else if (id.contains("FilterType"))
		{
			Filters[idx].setAttribute(3, FilterModes[value-1]);
		}
		else if (id.contains("Waveform"))
		{
			local fn = EMBEDDED_WAVEFORMS[value-1];
			Console.print("load" + fn);
			FileDropTargets[idx].loadFile(fn);
		}
	}

	inline function addComboBox(index, id, x, y)
	{
		

		local cb = Content.addComboBox(id + index, x, y);
		cb.set("parentComponent", "OSC" + index + "_Page");
		cb.set("width", 100);
		cb.set("height", 24);
		
		if(id == "Unisono")
		{
			local items = [];
			for(i = 0; i < 8; i++)
				items.push((i+1) + " Voices");

			cb.set("items", items.join("\n"));
		}
		else if (id == "FilterType")
		{
			cb.set("items", ["LP", "HP", "BP"].join("\n"));
			cb.set("width", 156);
			cb.set("parentComponent", "OSC" + index + "_Filter_Page");
		}
		else if (id == "Waveform")
		{
			cb.set("width", 128);
			
			local n = [];
			
			for(w in EMBEDDED_WAVEFORMS)
				n.push(w.replace("{PROJECT_FOLDER}", "").replace(".wav", ""));
			
			cb.set("items", n.join("\n"));
		}
		
		
		cb.setLocalLookAndFeel(LookAndFeel.laf);
		cb.setControlCallback(onComboBox);
		return cb;
	}

	inline function buildOsc(index)
	{
		local oscId = "OSC" + index + "_Page";

		local op = Content.addPanel(oscId, (index-1) * 500, 0);
		op.data.screens = [];
		op.set("parentComponent", "Main_Page");
		op.set("width", PageLogic.FULL_WIDTH/2);
		op.set("height", OSC_HEIGHT);
		op.set("text", "OSC " + index);
		op.setPaintRoutine(LookAndFeel.drawGreyPanel);
		
		local fp = Content.addPanel("OSC" + index + "_Filter_Page", (index-1) * 500, OSC_HEIGHT);
		fp.set("parentComponent", "Main_Page");
		fp.set("text", "OSC " + index + " Filter");
		fp.data.screens = [];
		fp.setPaintRoutine(LookAndFeel.drawGreyPanel);
		fp.set("height", 150);
		fp.set("width", 350);
		
		local fd = Content.addFloatingTile("OSC"+index + "_FilterDisplay", 100, 50);
		fd.set("height", 45);
		fd.set("width", 150);
		fd.set("parentComponent", fp.get("id"));
		fd.setContentData({
			Type: "FilterDisplay",
			ProcessorId: "OSC"+index + "_Filter",
			Index:0
		});
		
		fp.data.screens.push(Rectangle(100, 50, 150, 45));
		
		fd.setLocalLookAndFeel(LookAndFeel.screenLaf);
		
		
		local sp = Content.addPanel("OSC" + index + "_Shaper_Page", (index-1) * 500 + 350, OSC_HEIGHT);
		sp.set("parentComponent", "Main_Page");
		sp.set("text", "OSC " + index + " Shaper");
		sp.setPaintRoutine(LookAndFeel.drawGreyPanel);
		sp.set("height", 150);
		sp.set("width", 150);
		
		local wt = Content.addFloatingTile("WT_Panel" + index, 263, 44);
		
		wt.set("parentComponent", oscId);
		wt.set("width", 221);
		wt.set("height", 110);
				
		local wtData = WavetableWaterfall.DATA.clone();
		
		wtData.ProcessorId = "WT" + index;
				
		wt.setContentData(wtData);
		wt.set("bgColour", 0xFF222222);
		wt.set("itemColour2", 0x66FFFFFF);
		wt.set("itemColour", 0xFFFFFFFF);
		wt.set("itemColour3", 0xA768E0FF);
		
		op.data.screens.push(Rectangle(263, 44, 221, 110));
		
		if(true)
		{
			addKnob(index, "Position", 20, 115);
			addKnob(index, "Gain", 100, 115);
			addKnob(index, "Pitch", 180, 115);
			addKnob(index, "Detune", 20, 40);
			addKnob(index, "Pan", 100, 40);
			addKnob(index, "Fine", 180, 40);
			addKnob(index, "Filter Frequency", 20, 50);
			addKnob(index, "Filter Q", 270, 50);
			
			
			addComboBox(index, "Unisono", 258, 163);
			addComboBox(index, "Waveform", 360, 163);
			addComboBox(index, "FilterType", 96, 105);
		}
	}
	
	
	
	buildOsc(1);
	buildOsc(2);
	
	addBypassButton("OSC1");
	addBypassButton("OSC2");
	addBypassButton("OSC1_Filter");
	addBypassButton("OSC2_Filter");
	
	
}