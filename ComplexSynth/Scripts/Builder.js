namespace Builder
{
	const var builder = Synth.createBuilder();
	
	const var TARGETS = {
		"Gain": {
			InputRange: { min: -100.0, max: 6.0, middlePosition: 0.0 },
			OutputRange: {  min: 0.0, max: 2.0 },
			TextConverter: "Decibels",
			Index: builder.ChainIndexes.Gain
		},
		"Pitch": {
			InputRange: { min: -24.0, max: 24.0, stepSize: 1.0 },
			OutputRange: { min: -2.0, max: 2.0 },
			TextConverter: "Semitones",
			UseMidPositionAsZero: true,
			Index: builder.ChainIndexes.Pitch
		},
		"Fine": {
			InputRange: { min: -100.0, max: 100.0, stepSize: 1 },
			OutputRange: { min: -1.0, max: 1.0 },
			UseMidPositionAsZero: true,
			Index: builder.ChainIndexes.Pitch
		},
		"Position": {
			InputRange: { min: 0.0, max: 1.0, },
			OutputRange: { min: 0.0, max: 1.0 },
			TextConverter: "NormalizedPercentage",
			Index: 4
		},
		"Detune": {
			InputRange: { min: 0.0, max: 6.0, middlePosition: 1.0 },
			OutputRange: { min: 0.0, max: 1.0, middlePosition: 1.0 / 6.0 },
			TextConverter: "Semitones",
			Index: 4
		},
		"Filter Frequency": {
			InputRange: { min: 20.0, max: 20000.0, middlePosition: 2000.0 },
			OutputRange: { min: 0.0, max: 1.0, middlePosition: 0.1 },
			TextConverter: "Frequency",
			Index: 0
		},
		"Filter Q": {
			InputRange: { min: 0.3, max: 8.0, stepSize: 0.01, middlePosition: 1.0 },
			OutputRange: { min: 0.0, max: 1.0, middlePosition: 0.125 },
			UseMidPositionAsZero: true,
			Index: 3
		},
		"Pan": {
			InputRange: { min: -100, max: 100, stepSize: 1 },
			OutputRange: { min: 0.0, max: 1.0 },
			TextConverter: "Pan",
			UseMidPositionAsZero: true,
			Index: 0
		}
	};
	
	
	const var MODS = [ 
	  ["FlexAHDSR", "AHDSR"],
	  ["FlexAHDSR", "AHDSR2"],
	  ["FlexAHDSR", "AHDSR3"],
	  ["FlexAHDSR", "AHDSR4"],
	  ["LFO", "LFO"],
	  ["MidiController", "MIDI CC"],
	  ["Velocity", "Velocity"],
	  ["PitchWheel", "Pitch Wheel"],
	  ["TableEnvelope", "Table"],
	  ["SimpleEnvelope", "Ramp"],
	  ["Random", "Random"]
	  ];
	
	const var rm = Engine.getGlobalRoutingManager();
	
	for(m in MODS)
	{
		rm.getCable(m[1]).connectToGlobalModulator(m[1], true);
	}
	
	inline function buildGlobalModulators()
	{
		local gm = builder.create("GlobalModulatorContainer", "Global Modulator Container1", 0, -1);
		
		for(m in MODS)
		{
			local idx = builder.create(m[0], m[1], gm, builder.ChainIndexes.Gain);
			
			if(m[0] == "Velocity")
			{
				builder.setAttributes(idx, {
					UseTable: 1.0
				});
			}
			if(m[0] == "FlexAHDSR")
			{
				builder.setAttributes(idx, {
					Attack: 10,
					Hold: 0,
					Decay: 300,
					Sustain: 0.5,
					Release: 300,
					AttackLevel: 1.0,
					AttackCurve: 0.5,
					DecayCurve: 0.5,
					ReleaseCurve: 0.5
				});
			}
			if(m[0] == "LFO")
			{
				builder.setAttributes(idx, {
					FadeIn: 10.0
				});
			}
		}
		
	}
	
	inline function addMatrix(index, suffix, target)
	{
		local obj = TARGETS[suffix];
		Console.assertIsDefined(obj.Index);

		local matrix = builder.create(builder.Modulators.MatrixModulator, "OSC" + index + " " + suffix, target, obj.Index);
		local mod = builder.get(matrix, builder.InterfaceTypes.Modulator);
		
		mod.setMatrixProperties(obj);
		
		if(suffix == "Pitch" || suffix == "Fine")
		{
			mod.setIsBipolar(false);
			mod.setIntensity(suffix == "Pitch" ? 12.0 : 1.0);
	
			builder.setAttributes(matrix, {
				Value: 0.0
			});
		}
	}
	
	inline function buildOSC(index)
	{
		local root = builder.create("SynthGroup", "OSC" + index, 0, -1);
		
		builder.clearChildren(root, builder.ChainIndexes.Gain);
		
		local wt = builder.create("WavetableSynth", "WT" + index, root, -1);
		
		builder.setAttributes(root, {
			UnisonoDetune: 6.0
		});
		
		builder.setAttributes(wt, {
			TableIndexValue: 1.0,
			RefreshMipmap: 1.0
		});

		local filter = builder.create("PolyphonicFilter", "OSC" + index + "_Filter", root, builder.ChainIndexes.FX);
		
		builder.setAttributes(filter, {
			Q: 8.0
		});
		
		local stereoFX = builder.create(builder.Effects.StereoFX, "OSC"+index+"_Stereo", root, builder.ChainIndexes.FX);
		
		local indexes = {};
		
		indexes["Gain"] = root;
		indexes["Pitch"] = root;
		indexes["Fine"] = root;
		indexes["Position"] = wt;
		indexes["Detune"] = root;
		indexes["Filter Frequency"] = filter;
		indexes["Filter Q"] = filter;
		indexes["Pan"] = stereoFX;
		
		for(t in TARGETS)
		{
			Console.assertIsDefined(indexes[t]);
			addMatrix(index, t, indexes[t]);
		}
		
		builder.flush();
	}
	
	inline function buildModuleTree()
	{
		builder.clear();
		buildGlobalModulators();
		buildOSC(1);
		buildOSC(2);
		builder.flush();
	}
	
	//buildModuleTree();
}