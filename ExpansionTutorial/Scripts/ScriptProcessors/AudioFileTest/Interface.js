Content.makeFrontInterface(700, 600);

const var SlotSelectors = [Content.getComponent("FirstSlotSelector"),
                           Content.getComponent("SecondSlotSelector")];

const var AudioLoopPlayers = [Synth.getAudioSampleProcessor("Audio Loop Player1"),
                              Synth.getAudioSampleProcessor("Audio Loop Player2")]

const var PlayButtons = [Content.getComponent("PlayButton1"),
                         Content.getComponent("PlayButton2")];


const var expHandler = Engine.createExpansionHandler();
const var expansionList = expHandler.getExpansionList();


const var allList = [];
const var allIds = [];

// This loads all files from the project folder and returns a list of references
const var rootList = Engine.loadAudioFilesIntoPool();

// We'll keep a list of "nice names" and a list of valid references
allList.push("no file");
allIds.push("");

for(r in rootList)
{
    allList.push(r.split("}")[1]);
    allIds.push(r);
}


for(e in expansionList)
{
    for(af in e.getAudioFileList())
    {
        allList.push(af.split("}")[1]);
        allIds.push(af);
    }
}

for(s in SlotSelectors)
    s.set("items", allList.join("\n"));



inline function onSlotSelectorControl(component, value)
{
    local index = SlotSelectors.indexOf(component);
    
    if(value != 0)
    {
	AudioLoopPlayers[index].setFile(allIds[value-1]);
    }
};

const var numbers = [64, 48];
const var ids = [-1, -1];

inline function onPlayButtonControl(component, value)
{
    local index = PlayButtons.indexOf(component);
    
    if(value )
    {
        ids[index] = Synth.playNote(numbers[index], 127);
    }   
    else
    {
        Synth.noteOffByEventId(ids[index]);
        ids[index] = -1;
    }
};

for(pb in PlayButtons)
    pb.setControlCallback(onPlayButtonControl);

for(s in SlotSelectors)
    s.setControlCallback(onSlotSelectorControl);
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
 