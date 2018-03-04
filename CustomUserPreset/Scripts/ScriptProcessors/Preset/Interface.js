Content.makeFrontInterface(600, 500);

// This gets a list of all available user presets
const var list = Engine.getUserPresetList();

/** removes the directory names from the path. */
inline function getPresetName(relativePath)
{
    local l = relativePath.split("/");
    
    if(l.length == 3)
        return l[2];
        
    return "";
}

const var prettyList = [];
prettyList.reserve(list.length);

// Copy the preset names without the paths into a new array
for(i in list) prettyList.push(getPresetName(i));


// Load the pretty names into a viewport list
const var PresetListSelector = Content.getComponent("PresetListSelector");
PresetListSelector.set("items", prettyList.join("\n"));


inline function onPresetListSelectorControl(component, value)
{
    // Grab the preset name from the array and load it.
	Engine.loadUserPreset(list[value]);
    
};

PresetListSelector.setControlCallback(onPresetListSelectorControl);




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
