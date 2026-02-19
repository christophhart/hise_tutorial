namespace WavetableHandling
{
	inline function setResampleMode(wt, enabled)
	{
		local op = wt.getResynthesisOptions();
		
		op.PhaseMode = "StaticPhase";
		op.CycleMultiplier = 4;
		op.RemoveNoise = false;
		op.UseLoris = false;
		
		if(enabled)
		{
			op.ForceResynthesis = true;
			op.RootNote = 17;
		}
		else
		{
			op.ForceResynthesis = false;
			op.RootNote = -1;
		}
		
		wt.setResynthesisOptions(op);
	}
	
	const var WAVETABLE_OBJECTS = [];
	
	inline function getWavetableSynthFromWaterfall(component, key)
	{
		for(w in WAVETABLE_OBJECTS)
		{
			if(w.Waterfall == component.get("id"))
				return w[key];
		}
		
		return undefined;
	}
	
	reg detune = 0.0;
	reg noteNumber = 17;
	reg count = 0;
	
	inline function resample(wt, target)
	{
		local list = [];
		local on = Engine.createMessageHolder();
		local off = Engine.createMessageHolder();
	
		local sr = Engine.getSampleRate();
		
		local numbers = [];
		for(i = 0; i < 128; i++)
			numbers.push(i);
		
		numbers = numbers.map(x => Engine.getSampleRate() / Engine.getFrequencyForMidiNoteNumber(x));
		local min = 900000;
		local minIndex = -1;
		local idx = 0;
		
		for(n in numbers)
		{
			local dist = Math.abs(n - 2048);
	
			if(dist < min)
			{
				minIndex = idx;
				min = dist;
			}
				
			idx++;
		}
		
		noteNumber = minIndex;
		//detune = Engine.getSemitonesFromPitchRatio(numbers[minIndex] / 2048.0);
		count = 0;
		
		on.setType(Message.NoteOn);
		off.setType(Message.NoteOff);
		on.setChannel(1);
		off.setChannel(1);
		on.setVelocity(127);
		
		on.setNoteNumber(minIndex);
		off.setNoteNumber(minIndex);
		off.setTimestamp(2048 * 64);
		
		list.push(on);
		list.push(off);
		
		setResampleMode(wt, true);
		setResampleMode(target, true);
		
		Engine.renderAudio(list, function[wt, target](data, funky)
		{
			if(count++ == 1)
			{
				detune = 0;
				target.loadData(data.channels, Engine.getSampleRate(), []);
			}
		});
	}
	
	inline function onWavetableContextMenu(component, index)
	{
		local wt_ = getWavetableSynthFromWaterfall(component, "WavetableSynth");
		local af_ = getWavetableSynthFromWaterfall(component, "AudioFileSlot");
		local tg = getWavetableSynthFromWaterfall(component, "Target");

		setResampleMode(wt_, false);

		if(index == 0) // clear
			wt_.loadData([], 0, []);
			
		if(index == 1)
		{
			FileSystem.browse(FileSystem.AudioFiles, false, "*.wav", function[af_](x)
			{
				af_.loadFile(x.toString(0));
			});
		}
			
			
		if(index == 2)
		{
			FileSystem.browse(FileSystem.AudioFiles, true, "*.wav", function[wt_](x)
			{
				wt_.saveAsAudioFile(x);
			});
		}
			
		if(index == 3)
			resample(wt_, tg);
			
		if(index == 4 || index == 5 || index == 6) // Generate
		{
			local bf = Buffer.create(2048);
			
			if(index == 4) // Sine
			{
				for(i = 0; i < bf.length; i++)
					bf[i] = Math.sin(Math.PI * 2.0 * i / 2048.0);
			}
			if(index == 5) // Saw
			{
				for(i = 0; i < bf.length; i++)
					bf[i] = 2.0 * Math.fmod((i + bf.length/2) / 2048.0, 1.0) - 1.0;
			}
			if(index == 6) // Square
			{
				for(i = 0; i < bf.length; i++)
					bf[i] = i < bf.length / 2 ? 1.0 : -1.0;
			}
			
			wt_.loadData(bf, 48000.0, [0, 2048]);
		}
	};

	inline function init(wavetableId, floatingTileId)
	{
		local obj = {};

		local wt = Synth.getWavetableController(wavetableId);
		local wtAudioFile = Synth.getAudioSampleProcessor(wavetableId).getAudioFile(0);
		
		obj.WavetableSynth = wt;
		obj.AudioFileSlot = wtAudioFile;
		obj.Waterfall = floatingTileId;
		
		local wtContextBroadcaster = Engine.createBroadcaster({
		  "id": floatingTileId + "ContextBroadcaster",
		  "args": ["component", "index"],
		  "tags": []
		});
		
		Content.getComponent(floatingTileId).setLocalLookAndFeel(LookAndFeel.laf);
		
		wtContextBroadcaster.attachToContextMenu([floatingTileId], x => x == "enabled", 
		[
		  "Clear Wavetable", 
		  "Load wavetable from audio file", 
		  "Save wavetable",
		  "Resample oscillator",
		  "Initialise::Sine",
		  "Initialise::Saw",
		  "Initialise::Square"
		], "", 0);
		
	
		wtContextBroadcaster.addListener("", "wavetable context menu", onWavetableContextMenu);
		
		obj.Broadcaster = wtContextBroadcaster;
		
		setResampleMode(wt, true);
		return obj;
	}

	WAVETABLE_OBJECTS.push(init("WT1", "WT_Panel1"));
	WAVETABLE_OBJECTS.push(init("WT2", "WT_Panel2"));
	
	WAVETABLE_OBJECTS[0].Target = WAVETABLE_OBJECTS[1].WavetableSynth;
	WAVETABLE_OBJECTS[1].Target = WAVETABLE_OBJECTS[0].WavetableSynth;
	
	inline function initLoadingPanel(panelId)
	{
		local LoadPanel = Content.getComponent("LoadPanel");
				
		LoadPanel.setLoadingCallback(function(isPreloading)
		{
			this.set("visible", isPreloading);
			
			if(isPreloading)
				this.startTimer(60);
			else
				this.stopTimer();
		});
		
		LoadPanel.setTimerCallback(function()
		{
			this.data.progress = Engine.getPreloadProgress();
			this.repaint();
		});
		
		LoadPanel.setPaintRoutine(function(g)
		{
			g.fillAll(0x22FFFFFF);
		
			var r = Rectangle(this.getLocalBounds(0));
			
			r = r.removeFromLeft(r[2] * this.data.progress);
			g.setColour(Colours.white);
			g.fillRect(r);
		});
		
		LoadPanel.set("visible", false);
		
		return LoadPanel;
	}
	
}
