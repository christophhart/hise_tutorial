Content.makeFrontInterface(600, 500);

const var MIDIPlayer1 = Synth.getMidiPlayer("MIDI Player1");

inline function onPlayButtonControl(component, value)
{
	if(value)
	    MIDIPlayer1.play(0);
	else
	    MIDIPlayer1.stop(0);
};


const var MidiFileSelector = Content.getComponent("MidiFileSelector");


const var list = MIDIPlayer1.getMidiFileList();

list.insert(0, "Nothing");

const var expHandler = Engine.createExpansionHandler();

for(e in expHandler.getExpansionList())
{
    for(c in e.getMidiFileList())
        list.push(c);
}

MidiFileSelector.set("items", list.join("\n"));



inline function onMidiFileSelectorControl(component, value)
{
    var refToLoad = component.getItemText();
    
    if(refToLoad == "Nothing")
        refToLoad = "";
    
    MIDIPlayer1.setFile(refToLoad, true, true);
};

Content.getComponent("MidiFileSelector").setControlCallback(onMidiFileSelectorControl);


Content.getComponent("PlayButton").setControlCallback(onPlayButtonControl);
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
 