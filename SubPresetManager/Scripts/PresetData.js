
/*  Here we define different states for each control in the group
*   We need an array of JSON objects with the following requirements:
*     - the array must match the items in the combobox
*     - each array item must be a JSON object and will contain the values
*       of each "sub-preset"
*     - each JSON object must contain a key/value pair for each control
*     - the key must match the control ID
*     - the value must be the value of the control for the given preset
*/

const var sinePresetData = [
{
    "SineVolume": 1.0,
    "SineBalance": 0.0
},
{
    "SineVolume": 0.0,
    "SineBalance": -100
},
{
    "SineVolume": 0.5,
    "SineBalance": 100
}
];

const var sawPresetData = [
{
    "SawVolume": 1.0,
    "SawBalance": -75
},
{
    "SawVolume": 0.0,
    "SawBalance": 40
},
{
    "SawVolume": 0.5,
    "SawBalance": 0
}
];