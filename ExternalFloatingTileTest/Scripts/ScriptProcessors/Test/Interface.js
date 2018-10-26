/** Define the size of the interface. */
Content.makeFrontInterface(1024, 768);

/** We grab a reference to the full screen floating tile. */
const var FloatingTile1 = Content.getComponent("FloatingTile1");

/** Just set the ID to the ID we'll use for our C++ component.
*   In HISE it will show a placeholder, but in our compiled plugin
*   we can define the actual component. 
*/
FloatingTile1.setContentData({"Type": "ExternalTestComponent"});
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
