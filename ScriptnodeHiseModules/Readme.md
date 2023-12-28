# Scriptnode Module Repository

This HISE project contains scriptnode implementations of some of the stock HISE modules. There are always two networks for each module:

1. a simple one that outlines the most important concept of each module (eg. `lfo_simple.xml`)
2. a full implementation that deals with some of the pitfalls and details (eg. `lfo.xml`)

This makes this a good resource for learning the capabilities of scriptnode in varying complexity. They are documented pretty thoroughly and can be considered as good-practice examples for using scriptnode. 

### Compiling the networks

You should also be able to compile the networks, however they can also be used without compilation (no SNEX or Faust nodes are used).

> Be aware that the modulation nodes have a compile channel count of 1, so only put them into a modchain container if you reuse them in a FX network.