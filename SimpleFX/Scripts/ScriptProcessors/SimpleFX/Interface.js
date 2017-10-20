Content.makeFrontInterface(600, 500);

include("UserPresetWidgets.js");

const var BGPanel = Content.addPanel("BGPanel", 10, 60);
// [JSON BGPanel]
Content.setPropertiesFromJSON("BGPanel", {
  "width": 580,
  "height": 430,
  "itemColour": 436207615,
  "itemColour2": 891166238,
  "textColour": 536870911
});
// [/JSON BGPanel]

const var WetSlider = Content.addKnob("WetSlider", 100, 250);
// [JSON WetSlider]
Content.setPropertiesFromJSON("WetSlider", {
  "text": "Wet Amount",
  "processorId": "Simple Reverb",
  "parameterId": "WetLevel",
  "mode": "NormalizedPercentage",
  "middlePosition": 0.5,
  "suffix": "%"
});
// [/JSON WetSlider]
const var SizeSlider = Content.addKnob("SizeSlider", 370, 250);
// [JSON SizeSlider]
Content.setPropertiesFromJSON("SizeSlider", {
  "text": "Room Size",
  "processorId": "Simple Reverb",
  "parameterId": "RoomSize",
  "mode": "NormalizedPercentage",
  "middlePosition": 0.5,
  "suffix": "%"
});
// [/JSON SizeSlider]
const var Label = Content.addLabel("Label", 100, 170);
// [JSON Label]
Content.setPropertiesFromJSON("Label", {
  "text": "Simple FX",
  "width": 400,
  "height": 30,
  "fontName": "Oxygen",
  "fontSize": 24,
  "fontStyle": "Bold",
  "multiline": false
});
// [/JSON Label]

const var UpButton = UserPresetWidgets.createPresetButton("UpButton", 390, 23, true);
const var DownButton = UserPresetWidgets.createPresetButton("DownButton", 194, 23, false);

const var PresetDisplay = UserPresetWidgets.createPresetDisplay("PresetDisplay", 220, 20);
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
