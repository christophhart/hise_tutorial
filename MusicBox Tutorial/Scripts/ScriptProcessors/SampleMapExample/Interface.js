Content.makeFrontInterface(600, 500);
const var ComboBox = Content.addComboBox("ComboBox", 25, 37);

const var MySampler = Synth.getSampler("MySampler");


const var sampleMapList = Sampler.getSampleMapList();

ComboBox.set("items", sampleMapList.join("\n"));

inline function onComboBoxControl(component, value)
{
	MySampler.loadSampleMap(sampleMapList[value-1]);	
};

ComboBox.setControlCallback(onComboBoxControl);
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
