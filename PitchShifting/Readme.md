# PitchShifting Examples

This project contains a simple pitch shifting algorithm that is implemented as scriptnode patch and SNEX code as well as a few "higher level" examples:

- a real time pitch shifter
- a pseudo stereo effect
- a "timestretching" loop player

The sound quality of the pitch shifting algorithm is orders of magnitudes worse than the current industry standard, so this is not intended to be used in a real world project, but rather to demonstrate the workflow of scriptnode.

## How to build it

### Compile the PitchShift node

The first thing you should do is to compile the `PitchShifter.xml` network into a C++ node. Open the Network in the **SNEX workbench**, then go to **Tools / Compile All nodes**.

The performance of the uncompiled node is around 5% CPU, and goes down to 0.9% CPU in Release mode. 

### Build the examples

When you compile the nodes in a project, they will also be copied to the **AdditionalSourceCode** directory of the project root and when you export a plugin from the project that uses one of the compiled nodes, it will compile them again and embed them into the binary of the plugin (so you don't need to ship that .dll that you've used in the SNEX workbench with your plugin). This is the recommended workflow for all non-trivial custom nodes in a project.