
namespace MatrixHandling
{
	
}



const var Matrix = Content.getComponent("Matrix");
Matrix.setLocalLookAndFeel(LookAndFeel.laf);

const var MatrixOptions = Content.addButton("MatrixOptions", 950, 6);
MatrixOptions.set("width", 32);
MatrixOptions.set("height", 28);
MatrixOptions.set("saveInPreset", false);
MatrixOptions.set("isMomentary", true);

MatrixOptions.set("parentComponent", "Matrix");

MatrixOptions.setLocalLookAndFeel(LookAndFeel.laf);
MatrixOptions.setStyleSheetProperty("icon", PathIcons.data.Dropdown, "path");
MatrixOptions.setStyleSheetClass(".more-button");

const var matrixMoreContext = Engine.createBroadcaster({
  "id": "matrixMoreContext",
  "args": ["component", "index"],
  "tags": []
});

matrixMoreContext.attachToContextMenu(["MatrixOptions"], x => x == "enabled", ["Clear all connections"], "", true);

inline function onMatrixContextMenu(component, index)
{
	if(index == 0)
		sm.clearAllConnections("");
}

// attach first listener
matrixMoreContext.addListener(0, "matrix option context menu", onMatrixContextMenu);


const var sm = Engine.createModulationMatrix("Global Modulator Container1");
