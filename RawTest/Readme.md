# HISE Raw - Using HISE as pure C++ framework

This example shows how you can bypass the entire IDE side of HISE and directly create your plugin / interface using C++.

This is the most flexible approach to the HISE ecosystem as it leaves you with the full flexibility of what C++ / JUCE offers without being limited to the things available through scripting.

If you just want to create a custom user interface with C++ / JUCE, but still want to use the instrument you've designed with HISE, check out the `ExternalFloatingTile` example project in this repository.

## How to use this example

1. Build the latest version of HISE from the `development` branch and load this project.
2. In order to tell HISE to provide you with a barebone project that you can use for your C++ adventures, add the compiler switch `USE_RAW_FRONTEND=1` to all target configurations (this step is already done, but you will need to do it for your own projects).
3. Create a dummy interface. HISE will complain if you don't have a interface to catch noobs doing weird stuff, but in our case, it will do nothing on the final plugin.
4. Export your plugin as usual, cancel the build process and open up the solution files in your favorite IDE.
5. Write your plugin entirely in C++. (Checkout the code example in this repository). 
**Important: Do not add any files in the `Binaries` subfolder as it will be merciless deleted. Use the AdditionalSourceCode folder for this.**
When exporting, HISE will copy a reference to all .cpp and .h files in the AdditionalSourceCode folder.
6. During development, just use your IDE to build and test it like a normal C++ application.
7. Whenever you need to do something in HISE (eg. add resource files, map samples, change the export target), just open HISE and repeat steps 3-4.

## Topics covered in this example

In the AdditionalSourceCode directory, you will see 4 C++ files that define the example project. In these files you can see how to:

- create a simple additive synthesiser using C++ API calls via the new `hise::raw::Builder` object.
- create a user interface with a slider that changes the master volume.
- create and use a custom MIDI processor as C++ class (which uses the same API as the Javascript MIDI Processors).
- hook up a custom HISE UI element (the SliderPack) to control the harmonic structure of the additive synthesiser. This also shows how to handle separated data and interface objects as required for plugins.

## Where to go from here

This example should explain everything you need to do in order to get started. From here on, the things you can do are just limited by your imagination and your knowledge of the HISE API, which, unfortunately, is at the time not 100% intended to be used in this way (although I will try to clear things up along the way).

Trying to turn a 500k+ C++ codebase into a fully documented API overnight is not an option (also there are much things that you don't ever need to use), so I intend to solve this by offering properly documented building tools that act like some sort of gateway to the full HISE codebase in the `raw` namespace (just like the `raw::Builder` object takes care of adding modules and setting their attributes in a pretty simple and elegant way.) I can think of these objects that I will implement over the time:

- `raw::Parameter` - a data structure that acts as plugin parameter and can be hooked to a collection of internal HISE module attributes, along with a few wrappers for default UI elements (buttons, sliders, etc)
- `raw::Widget` - a wrapper around the more complex HISE widgets (Table, SliderPack and AudioWaveform)
- `raw::FloatingTileWrapper`: a wrapper around all components available as floating tile.
- `raw::UserPreset` - a wrapper around user presets, either stored on disk or as embedded data in a DAW project.

This along with improving the documentation of the parts of the codebase which you will actually use (like the documentation for the module base types so you can subclass them to create eg. new modulators) should be the way do go. Until then, there is a pretty random doxygen generated API doc available [here](http://hise.audio/hise_api/index.html). However I wouldn't rely on it to find the right information (just cross-read it and maybe something catches your eye).
