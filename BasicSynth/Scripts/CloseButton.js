
namespace CloseButton
{
	const var expHandler = Engine.createExpansionHandler();
	

    const var CLOSE_BUTTON_WIDTH = 20;
    const var p = Content.addPanel("closeButton", 0, 0);
    p.set("x", 1024 - CLOSE_BUTTON_WIDTH - 10);
    p.set("y", 10);
    p.set("width", CLOSE_BUTTON_WIDTH);
    p.set("height", CLOSE_BUTTON_WIDTH);
    
    p.set("allowCallbacks", "Clicks & Hover");
    p.setMouseCallback(function(event)
    {
        this.data.hover = event.hover;
        this.repaint();

        if(event.clicked)
            expHandler.setCurrentExpansion("");
    });
    
    p.setPaintRoutine(function(g)
    {
        g.setColour(Colours.withAlpha(Colours.white, this.data.hover ? 1.0 : 0.4));
        var icon = Content.createPath();
        
        icon.startNewSubPath(0.0, 0.0);
        icon.lineTo(1.0, 1.0);
        icon.startNewSubPath(1.0, 0.0);
        icon.lineTo(0.0, 1.0);
        
        g.drawPath(icon, [3, 3, this.getWidth() - 6, this.getHeight() - 6], 3);
        
    });
}

