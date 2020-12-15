Content.makeFrontInterface(1024, 768);

const var expHandler = Engine.createExpansionHandler();
const var background = Content.getComponent("background");


inline function refreshExpansion(e)
{
    Console.print("TUT");
    
    
    if(!isDefined(e))
    {
        background.set("fileName", "{PROJECT_FOLDER}background.png");
    }
    else
    {
        background.set("fileName", e.getImageList()[0]);
    }
}

refreshExpansion(undefined);

expHandler.setExpansionCallback(refreshExpansion);function onNoteOn()
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
 