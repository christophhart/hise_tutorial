Content.makeFrontInterface(600, 500);

include("VuMeter.js");

const var InputMeter = VuMeter.createVuMeter("InputMeter", 130, 20);
InputMeter.set("height", 230);

VuMeter.setModule(InputMeter, Synth.getEffect("Input"));

const var OutputMeter = VuMeter.createVuMeter("OutputMeter", 440, 20);
OutputMeter.set("height", 230);

VuMeter.setModule(OutputMeter, Synth.getEffect("Dynamics"));

include("DynamicsMeter.js");

const var compressorPanel = DynamicsMeter.createDynamicsMeter("compressorPanel", 170, 20, "Dynamics");

//! =============================================================== Gate Controls

const var GateEnabled = Content.addButton("GateEnabled", 50, 270);
// [JSON GateEnabled]
Content.setPropertiesFromJSON("GateEnabled", {
  "text": "Gate On",
  "processorId": "Dynamics",
  "parameterId": "GateEnabled"
});
// [/JSON GateEnabled]


const var gateThreshold = Content.addKnob("gateThreshold", 20, 310);
// [JSON gateThreshold]
Content.setPropertiesFromJSON("gateThreshold", {
  "text": "G Threshold",
  "min": -100,
  "max": 0,
  "processorId": "Dynamics",
  "parameterId": "GateThreshold",
  "mode": "Decibel",
  "stepSize": 0.1,
  "middlePosition": -50,
  "suffix": " dB"
});
// [/JSON gateThreshold]


const var GateAttack = Content.addKnob("GateAttack", 40, 370);
// [JSON GateAttack]
Content.setPropertiesFromJSON("GateAttack", {
  "text": "G Attack",
  "min": 0.1,
  "max": 100,
  "processorId": "Dynamics",
  "parameterId": "GateAttack",
  "mode": "Time",
  "stepSize": 0.1,
  "middlePosition": 10,
  "defaultValue": "10",
  "suffix": " ms"
});
// [/JSON GateAttack]
const var GateRelease = Content.addKnob("GateRelease", 10, 430);
// [JSON GateRelease]
Content.setPropertiesFromJSON("GateRelease", {
  "text": "G Release",
  "max": 800,
  "processorId": "Dynamics",
  "parameterId": "GateRelease",
  "mode": "Time",
  "stepSize": "0.1",
  "middlePosition": 50,
  "defaultValue": "50",
  "suffix": " ms"
});
// [/JSON GateRelease]

//! ========================================================= Compressor Controls


const var CompEnabled = Content.addButton("CompEnabled", 240, 270);
// [JSON CompEnabled]
Content.setPropertiesFromJSON("CompEnabled", {
  "text": "Comp On",
  "processorId": "Dynamics",
  "parameterId": "CompressorEnabled"
});
// [/JSON CompEnabled]

const var CompThreshold = Content.addKnob("CompThreshold", 240, 300);
// [JSON CompThreshold]
Content.setPropertiesFromJSON("CompThreshold", {
  "text": "C Treshold",
  "min": -60,
  "max": 0,
  "processorId": "Dynamics",
  "parameterId": "CompressorThreshold",
  "mode": "Decibel",
  "stepSize": 0.1,
  "middlePosition": -24,
  "suffix": " dB"
});
// [/JSON CompThreshold]


const var CompRatio = Content.addKnob("CompRatio", 240, 350);
// [JSON CompRatio]
Content.setPropertiesFromJSON("CompRatio", {
  "text": "C Ratio",
  "min": 1,
  "max": 32,
  "processorId": "Dynamics",
  "parameterId": "CompressorRatio",
  "middlePosition": 4
});
// [/JSON CompRatio]


const var CompressorAttack = Content.addKnob("CompressorAttack", 240, 400);
// [JSON CompressorAttack]
Content.setPropertiesFromJSON("CompressorAttack", {
  "text": "C Attack",
  "max": 200,
  "processorId": "Dynamics",
  "parameterId": "CompressorAttack",
  "mode": "Time",
  "stepSize": 1,
  "middlePosition": 30,
  "defaultValue": "30",
  "suffix": " ms"
});
// [/JSON CompressorAttack]

const var CompressorRelease = Content.addKnob("CompressorRelease", 240, 452);
// [JSON CompressorRelease]
Content.setPropertiesFromJSON("CompressorRelease", {
  "text": "C Release",
  "min": 1,
  "max": 20000,
  "mode": "Time",
  "stepSize": "0.1",
  "middlePosition": 10,
  "defaultValue": "10",
  "suffix": " ms"
});
// [/JSON CompressorRelease]

//! ============================================================ Limiter Controls


const var LimiterEnabled = Content.addButton("LimiterEnabled", 420, 270);
// [JSON LimiterEnabled]
Content.setPropertiesFromJSON("LimiterEnabled", {
  "text": "Limiter On",
  "processorId": "Dynamics",
  "parameterId": "LimiterEnabled"
});
// [/JSON LimiterEnabled]

const var LimiterThreshold = Content.addKnob("LimiterThreshold", 450, 310);
// [JSON LimiterThreshold]
Content.setPropertiesFromJSON("LimiterThreshold", {
  "text": "L Threshold",
  "min": -36,
  "max": 0,
  "processorId": "Dynamics",
  "parameterId": "LimiterThreshold",
  "mode": "Decibel",
  "stepSize": 0.1,
  "middlePosition": -12,
  "suffix": " dB"
});
// [/JSON LimiterThreshold]

const var LimiterAttack = Content.addKnob("LimiterAttack", 440, 370);
// [JSON LimiterAttack]
Content.setPropertiesFromJSON("LimiterAttack", {
  "text": "L Attack",
  "min": 2,
  "max": 80,
  "processorId": "Dynamics",
  "parameterId": "LimiterAttack",
  "mode": "Time",
  "stepSize": "0.1",
  "middlePosition": 10,
  "defaultValue": "10",
  "suffix": " ms"
});
// [/JSON LimiterAttack]
const var LimiterRelease = Content.addKnob("LimiterRelease", 460, 430);
// [JSON LimiterRelease]
Content.setPropertiesFromJSON("LimiterRelease", {
  "text": "L Release",
  "min": 1,
  "max": 200,
  "processorId": "Dynamics",
  "parameterId": "LimiterRelease",
  "mode": "Time",
  "stepSize": 1,
  "middlePosition": 10,
  "defaultValue": "10",
  "suffix": " ms"
});
// [/JSON LimiterRelease]function onNoteOn()
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
