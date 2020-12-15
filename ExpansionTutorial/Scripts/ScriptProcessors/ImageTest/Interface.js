Content.makeFrontInterface(1024, 768);



// Setup the expansion list in the combobox

const var expHandler = Engine.createExpansionHandler();

const var expansionNames = [];

expansionNames.push("No Expansion");

for(e in expHandler.getExpansionList())
    expansionNames.push(e.getProperties().Name);
    

const var ExpansionSelector = Content.getComponent("ExpansionSelector");
ExpansionSelector.set("items", expansionNames.join("\n"));

// Implement the expansion switch

inline function onExpansionSelectorControl(component, value)
{
	local expansionToLoad = component.getItemText();
	
	// We want the first item to reset the current expansion
	// so we need to change it to an empty string
	if(expansionToLoad == expansionNames[0])
        expansionToLoad = "";
    
	expHandler.setCurrentExpansion(expansionToLoad);
};

Content.getComponent("ExpansionSelector").setControlCallback(onExpansionSelectorControl);


// Implement the loading callbac6k that swaps the background image

const var Image1 = Content.getComponent("Image1");

const var Panel2 = Content.getComponent("Panel2");


function refreshImage(newExpansion)
{
    var backgroundImage = "";
    var panelImage = "";
    
    if(isDefined(newExpansion))
    {
        backgroundImage = newExpansion.getWildcardReference("background.png");
        panelImage = newExpansion.getWildcardReference("panel_bg.png");
    }
    else
    {
        backgroundImage = "{PROJECT_FOLDER}background.png";
        panelImage = "{PROJECT_FOLDER}panel_bg.png";
    }
    
    Console.print(backgroundImage);
    
    Image1.set("fileName", backgroundImage);
    
    Panel2.loadImage(panelImage, "bg");
    Panel2.setImage("bg", 0, 0);
    Panel2.repaint();
}

expHandler.setExpansionCallback(refreshImage);

// Call it once with undefined so that it loads the root images
refreshImage(undefined);function onNoteOn()
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
 