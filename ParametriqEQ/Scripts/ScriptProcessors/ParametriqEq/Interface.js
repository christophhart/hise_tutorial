Content.makeFrontInterface(700, 600);

const var ParametriqEQ = Synth.getEffect("Parametriq EQ");
const var dp = Synth.getDisplayBufferSource("Analyser");
const var b = dp.getDisplayBuffer(0);

// You can specify the properties of the FFT analyser here
b.setRingBufferProperties({
  "BufferLength": 16384,
  "WindowType": "Blackman Harris",
  "DecibelRange": [
    -100.0,
    -12.0
  ],
  "UsePeakDecay": false,
  "UseDecibelScale": true,
  "YGamma": 1.0,
  "Decay": 0.699999988079071,
  "UseLogarithmicFreqAxis": true
});

// This will change the appearance of the value popup when you drag sliders
Content.setValuePopupData({
    "fontName":"Lato",
    "fontSize": 17,
    "borderSize": 1,
    "borderRadius": 3,
    "margin": 10,
    "bgColour": 0xFF636363,
    "itemColour": 0xFF000000,
    "itemColour2": 0xFF000000,
     "textColour": 0xFFAAAAAA 
});

//! ===================================================================== Enabled


inline function onBand1EnabledControl(component, value)
{
	// Since the parameter index of the parametric EQ is not fixed,
	// we need to apply some math to get the correct index.
	// The idea is to use the formula BAND_INDEX * BANDOFFSET + MODE
	// to get an integer that controls the value we want
	local index = ParametriqEQ.Enabled + 0 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band1Enabled").setControlCallback(onBand1EnabledControl);

inline function onBand1Enabled1Control(component, value)
{
	local index = ParametriqEQ.Enabled + 1 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band2Enabled").setControlCallback(onBand1Enabled1Control);

inline function onBand1Enabled2Control(component, value)
{
	local index = ParametriqEQ.Enabled + 2 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band3Enabled").setControlCallback(onBand1Enabled2Control);

inline function onBand1Enabled3Control(component, value)
{
	local index = ParametriqEQ.Enabled + 3 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band4Enabled").setControlCallback(onBand1Enabled3Control);


inline function onBand1SelectorControl(component, value)
{
	local index = ParametriqEQ.Type + 0 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value-1);
};

//! ======================================================================== Type


Content.getComponent("Band1Selector").setControlCallback(onBand1SelectorControl);

inline function onBand2SelectorControl(component, value)
{
	local index = ParametriqEQ.Type + 1 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value-1);
};

Content.getComponent("Band2Selector").setControlCallback(onBand2SelectorControl);

inline function onBand3Selector3Control(component, value)
{
	local index = ParametriqEQ.Type + 2 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value-1);
};

Content.getComponent("Band3Selector3").setControlCallback(onBand3Selector3Control);

inline function onBand4SelectorControl(component, value)
{
	local index = ParametriqEQ.Type + 3 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value-1);
};

Content.getComponent("Band4Selector").setControlCallback(onBand4SelectorControl);


//! =================================================================== Frequency



inline function onBand1FrequencyControl(component, value)
{
	local index = ParametriqEQ.Freq  + 0 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band1Frequency").setControlCallback(onBand1FrequencyControl);

inline function onBand2FrequencyControl(component, value)
{
	local index = ParametriqEQ.Freq + 1 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band2Frequency").setControlCallback(onBand2FrequencyControl);

inline function onBand3FrequencyControl(component, value)
{
	local index = ParametriqEQ.Freq + 2 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band3Frequency").setControlCallback(onBand3FrequencyControl);

inline function onBand4FrequencyControl(component, value)
{
	local index = ParametriqEQ.Freq + 3 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band4Frequency").setControlCallback(onBand4FrequencyControl);


//! ======================================================================== Gain


inline function onBand1GainControl(component, value)
{
	local index = ParametriqEQ.Gain + 0 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band1Gain").setControlCallback(onBand1GainControl);


inline function onBand2GainControl(component, value)
{
	local index = ParametriqEQ.Gain + 1 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band2Gain").setControlCallback(onBand2GainControl);




inline function onBand3GainControl(component, value)
{
	local index = ParametriqEQ.Gain + 2 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band3Gain").setControlCallback(onBand3GainControl);



inline function onBand4GainControl(component, value)
{
	local index = ParametriqEQ.Gain + 3 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band4Gain").setControlCallback(onBand4GainControl);

//! =========================================================================== Q


inline function onBand1QControl(component, value)
{
	local index = ParametriqEQ.Q + 0 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band1Q").setControlCallback(onBand1QControl);

inline function onBand2QControl(component, value)
{
	local index = ParametriqEQ.Q + 1 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band2Q").setControlCallback(onBand2QControl);

inline function onBand3QControl(component, value)
{
	local index = ParametriqEQ.Q + 2 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band3Q").setControlCallback(onBand3QControl);

inline function onBand4QControl(component, value)
{
	local index = ParametriqEQ.Q + 3 * ParametriqEQ.BandOffset;
	ParametriqEQ.setAttribute(index, value);
};

Content.getComponent("Band4Q").setControlCallback(onBand4QControl);
function onNoteOn()
{
	
}
 function onNoteOff()
{
	
}
 function onController()
{
	
}
 function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 