namespace Authorisation
{
    const var SerialInput = Content.getComponent("SerialInput");
    const var Description = Content.getComponent("Description");
    const var SerialStateLabel = Content.getComponent("SerialStateLabel");
    const var AuthorisationDialogue = Content.getComponent("AuthorisationDialogue");
    const var GlobalMute = Synth.getMidiProcessor("GlobalMute");
    
    /** Checks if the serial input is valid and stores the result if successful. */
    inline function onSubmitButtonControl(component, value)
    {
        if(!value) // Just execute once
            return;
    
        local v = SerialInput.getValue();
        Console.print(v);
	
        // Checks if it's in the input
        if(serials.Data.contains(v))
        {
            Console.print("Serial number found");
        
            local data = 
            {
                "Serial": v
            };
        
            // Stores the file to the hard drive. In HISE it will be the project folder
            // but in the compiled plugin it will use the parent directory to the 
            // user preset directory (which is usually the app data folder).
            Engine.dumpAsJSON(data, "../RegistrationInfo.js");
            
            setValidLicense(true);
        }
        else
        {
            Console.print("Invalid serial number");
            Description.set("text", "Invalid serial number. The number you supplied does not match");
            
            setValidLicense(false);
        }
    };

    Content.getComponent("SubmitButton").setControlCallback(onSubmitButtonControl);


    inline function setValidLicense(isValid)
    {
        // Do whatever you want to do here. I suggest a MIDI muter...
        GlobalMute.setAttribute(0, 1 - isValid);
    
        if(isValid)
        {
            // Change this to any other visual indication...
            SerialStateLabel.set("bgColour", Colours.greenyellow);
            AuthorisationDialogue.set("visible", false);
        }
        else
        {
            SerialStateLabel.set("bgColour", Colours.red);
            AuthorisationDialogue.set("visible", true);
        }
    }

    inline function checkOnLoad()
    {
        // Clear the input
        SerialInput.set("text", "");
        
        // Load the serial from the stored file
        local pData = Engine.loadFromJSON("../RegistrationInfo.js");
        Console.print("Checking serial");
    
        if(pData)    
        {
            local v = pData.Serial;
            Console.print("Restored serial: " + v);
        
            if(serials.Data.contains(v))
            {
                setValidLicense(true);
                return;
            }
        }
    
        setValidLicense(false);
    }

    // Call this on startup
    checkOnLoad();

}