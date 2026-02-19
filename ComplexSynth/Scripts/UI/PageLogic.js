namespace PageLogic
{
	const var MAIN_PAGE = 0;
	const var MATRIX_PAGE = 1;
	const var FX_PAGE = 2
	const var PRESET_PAGE = 3;
	
	const var PAGE_NAMES = ["Main", "Matrix", "FX", "Presets"];
	
	const var FULL_WIDTH = 1000;
	const var FULL_HEIGHT = 800;
	
	const var HEADER_HEIGHT = 40;
	const var KEYBOARD_HEIGHT = 80;
	const var PRESET_WIDTH = 400;
	const var CONTENT_HEIGHT = 350;
	const var DRAGGER_HEIGHT = 40;
	const var MOD_PAGE_HEIGHT = 290;
	
	inline function buildInterface()
	{
		local bg = Content.getComponent("BG");
		local h = Content.addPanel("Header", 0, 0);
		h.set("width", bg.get("width"));
		h.set("height", HEADER_HEIGHT);
		h.set("parentComponent", "BG");
		
		local p = Content.addPanel("PresetPanel", (bg.get("width") - PRESET_WIDTH) / 2, 0);
		p.set("parentComponent", "BG");
		p.set("width", PRESET_WIDTH);
		p.set("height", HEADER_HEIGHT);
		p.set("allowCallbacks", "Clicks & Hover");
		
		local pp = Content.addButton("PrevPreset", 0, 0);
		local np = Content.addButton("NextPreset", PRESET_WIDTH - HEADER_HEIGHT, 0);
		
		pp.set("width", HEADER_HEIGHT);
		pp.set("height", HEADER_HEIGHT);
		pp.set("parentComponent", "PresetPanel");
		pp.set("saveInPreset", false);
		pp.set("isMomentary", true);
		
		np.set("width", HEADER_HEIGHT);
		np.set("height", HEADER_HEIGHT);
		np.set("parentComponent", "PresetPanel");
		np.set("saveInPreset", false);
		np.set("isMomentary", true);
		
		local c = Content.addPanel("Content", 0, HEADER_HEIGHT);
		c.set("width", bg.get("width"));
		c.set("height", CONTENT_HEIGHT);
		c.set("parentComponent", "BG");
		
		local widthForPageButtons = p.get("x") / PAGE_NAMES.length;
		
		local x = 0;
		
		for(pageName in PAGE_NAMES)
		{
			local b = Content.addButton(pageName + "_PageButton", x, 0);
			b.set("width", widthForPageButtons);
			b.set("height", HEADER_HEIGHT);
			b.set("text", pageName);
			b.set("saveInPreset", false);
			b.set("parentComponent", "Header");
			b.set("radioGroup", 95210);
			x += widthForPageButtons;
			
			local pn = Content.addPanel(pageName + "_Page", 0, 0);
			pn.set("width", c.get("width"));
			pn.set("height", c.get("height"));
			pn.set("parentComponent", "Content");
			pn.set("text", pageName);
		}
		
		local dragger = Content.addFloatingTile("Draggers", 0, c.get("y") + c.get("height"));
		dragger.set("height", DRAGGER_HEIGHT);
		dragger.set("width", c.get("width"));
		
		dragger.setContentData({
		  "Type": "ModulationMatrixController",	
		  "ProcessorId": "Global Modulator Container1",
		  "Index": 0
		});
		
		local modPage = Content.addPanel("ModPage", 0, dragger.get("y") + DRAGGER_HEIGHT);
		modPage.set("width", c.get("width"));
		modPage.set("height", MOD_PAGE_HEIGHT);
		modPage.set("text", "Modulation");
	}
	
	inline function onPageCallback(component, value)
	{
		local idx = PAGE_BUTTONS.indexOf(component);
		
		for(i = 0; i < PAGES.length; i++)
			PAGES[i].set("visible", idx == i);

		Console.print(value);
	}
	
	inline function initPageButtons()
	{
		local l = [Content.getComponent("Main_PageButton"),
	               Content.getComponent("Matrix_PageButton"),
	               Content.getComponent("FX_PageButton"),
	               Content.getComponent("Presets_PageButton")];
		
		for(b in l)
		{
			b.setControlCallback(onPageCallback);
			b.setStyleSheetClass(".page-button");
			
		}
		
		return l;
	}
	
	inline function onPresetButton(component, value)
	{
		
	}
	
	inline function initPresetButtons()
	{
		local l = [Content.getComponent("PrevPreset"),
             	   Content.getComponent("NextPreset")];
		
		for(b in l)
		{
			b.setControlCallback(onPresetButton);
			b.setStyleSheetClass(".presetbuttons");
			b.setStyleSheetProperty("icon", PathIcons.data[b.get("id")], "path");
		}
		
		return l;
	}
	
	inline function initPresetPanel()
	{
		local pp = Content.getComponent("PresetPanel");
		pp.setMouseCallback(function(event)
		{
			this.repaint();
		});
		
		pp.setLocalLookAndFeel(LookAndFeel.headerLaf);
		return pp;
	}
	
	inline function initPages()
	{
		local l = [];

		for(pn in PAGE_NAMES)
		{
			local p = Content.getComponent(pn + "_Page");
			p.setPaintRoutine(pn == "Presets" ? LookAndFeel.drawGreyPanel : LookAndFeel.drawPage);
			l.push(p);
		}
		
		
		
		return l;
	}
	
	inline function initModPage()
	{
		local p = Content.getComponent("ModPage");
		//p.setLocalLookAndFeel(LookAndFeel.laf);
		p.setPaintRoutine(LookAndFeel.drawPage);
		p.setStyleSheetProperty("title", p.get("text"), "text");
		
		return p;
	}
	
	inline function initDraggers()
	{
		local p = Content.getComponent("Draggers");
		p.setLocalLookAndFeel(LookAndFeel.draggerLaf);
		return p;
	}
	
	//buildInterface();
	
	const var PAGES = initPages();
	const var PAGE_BUTTONS = initPageButtons();
	
	const var PRESET_BUTTONS = initPresetButtons();
	const var header_p = Content.getComponent("Header");
	header_p.setLocalLookAndFeel(LookAndFeel.headerLaf);
	const var preset_p = initPresetPanel();
	const var draggers = initDraggers();
	
	const var modpage_p = initModPage();
	
	const var keyboard = Content.getComponent("Keyboard");
	keyboard.setLocalLookAndFeel(LookAndFeel.keyboardLaf);
}