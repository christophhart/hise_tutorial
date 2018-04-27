# Sub-Preset Manager 

This is a simple patch containing a sine wave generator and a saw wave generator.

There are different presets for each single sound generator as well as global presets which
can be recalled independently.

## How it works:
This example uses script-defined data objects to define multiple presets for a subset of the GUI controls.
You can load "sub-presets" by restoring the state using the values defined in the data objects.


If you recall "global" user presets, it will restore the sub-preset and
then restore the custom values.
> In order to make this work correctly, make sure that the controls that load
the sub presets are defined **before** the other controls (otherwise the 
custom control states of the controls will be overwritten).

