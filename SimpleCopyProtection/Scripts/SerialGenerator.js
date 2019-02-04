/** This namespace contains the logic to create a new list of serials. 
    
    Feel free to remove this when not needed. */
namespace SerialGenerator
{
    inline function getNewRandomChar()
    {
        // ASCII, fuck yeah!
        local a = String.fromCharCode(Math.randInt(65, 90));
        local b = String.fromCharCode(Math.randInt(48, 57));
    
        return Math.random() > 0.5 ? a : b;
    }

    inline function createNewSerial()
    {
        local s = "";
    
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += "-";
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += "-";
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += "-";
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
        s += getNewRandomChar();
    
        return s;
    }

    const var NUM_SERIALS = 1000;

    inline function createNewSerials()
    {
        local d = [];
        d.reserve(NUM_SERIALS);
    
        for(i = 0; i < NUM_SERIALS; i++)
        {
            d.push(createNewSerial());
        }
    
        local obj = { "Data": d };
    
        Engine.dumpAsJSON(obj, "../Serials.js");
    }

    // Uncomment this line to regenerate serials.
    //createNewSerials();
}