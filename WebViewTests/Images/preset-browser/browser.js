
var currentPresets;

window.updatePresetList = function(presetList)
{
    currentPresets = presetList;
    var index = 0;

    var presets=document.getElementById('presets');

    presets.innerHTML = "";

    for(element in presetList)
    {
       
       
       var opt = document.createElement("option");
       opt.value = presetList[element].file;

       opt.innerHTML = presetList[element].name; // whatever property it has

       // then append it to the select element
       presets.appendChild(opt);
       index++;
       
    }
}

window.setSelectedPreset = function(fullPath)
{
  console.log(fullPath);

  for(s in currentPresets)
  {
    if(currentPresets[s].file == fullPath)
    {
      console.log("FOUND");
      document.getElementById("presets").selectedIndex = s;
      break;
    }
  }
}

window.updateValue = function(value)
{
  document.getElementById("valueDisplay").innerHTML = value;
}

/*
updatePresetList([{"name":"Preset 1", "file": "preset1.preset"},
                  {"name":"Preset 2", "file": "preset2.preset"},
                  {"name":"Preset 3", "file": "preset3.preset"},
                  {"name":"Preset 4", "file": "preset4.preset"},
                  {"name":"Preset 5", "file": "preset5.preset"},
                  {"name":"Preset 6", "file": "preset6.preset"}]);
*/

