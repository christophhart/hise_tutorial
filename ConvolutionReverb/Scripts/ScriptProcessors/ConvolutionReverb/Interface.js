Content.makeFrontInterface(520, 500);

inline function attachLabelToKnob(labelName, knobName, text)
{
    local l = Content.getComponent(labelName);
    local k = Content.getComponent(knobName);
    
    l.set("text", text);
    l.set("saveInPreset", false);
    l.set("editable", false);
    
    
    l.set("x", k.get("x"));
    l.set("y", parseInt(k.get("y")) - 25);
    l.set("width", k.get("width"));
};

Engine.loadAudioFilesIntoPool();

const var bgPanel = Content.getComponent("bgPanel");
bgPanel.loadImage("{PROJECT_FOLDER}background.png", "bg");

bgPanel.setImage("bg", 0, 0);


attachLabelToKnob("wetLabel", "wetKnob", "Wet Level");
attachLabelToKnob("dryLabel", "dryKnob", "Dry Level");
attachLabelToKnob("predelayLabel", "predelayKnob", "Predelay");
attachLabelToKnob("hicutLabel", "hicutKnob", "HiCut");
attachLabelToKnob("dampingLabel", "dampingKnob", "Damping");

const var CPULabel = Content.getComponent("CPULabel");


const var cpuTimer = Engine.createTimerObject();

cpuTimer.setTimerCallback(function()
{
	CPULabel.set("text", parseInt(Engine.getCpuUsage()) + "% CPU");
    
});

cpuTimer.startTimer(500);

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
 