# Using HISE as pure C++ framework

This example shows how you can bypass the entire IDE side of HISE and directly create your plugin / interface using C++ API calls.

## Advantages of this approach

HISE started out as IDE with the aim of making it possible for people without C++ knowledge to create virtual instruments. Over the time, it got more and more complex features until a point where the overhead of working inside a GUI environment becomes a severe limitation.

You are not completely on your own though - even in this use case, HISE will take care of many things for you:

### Project Management / Resources

All the file management, export configurations as well as audio settings dialogues can be used from HISE. You can even use the user preset system to load / save your plugin's state with any arbitrary kind of data.

### Reuse Stock HISE modules

All HISE modules can be reused and assembled together using simple helper classes. However if you want to add custom modules into the signal chain, it will be very easy to do so by creating your own modules from the HISE API.

## How to do it

1. Create a project with HISE. You can also add images, audio files, create samplemaps as all these resources will be available in your C++ project.
2. In order to tell HISE to provide you with a barebone project that you can use for your C++ adventures, add the compiler switch `USE_RAW_FRONTEND=1` to all target configurations. 
3. Create a dummy interface. HISE will complain if you don't have a interface to catch noobs doing weird stuff, but in our case, it will do nothing on the final plugin.
4. Export your plugin, cancel the build process and open up the solution files in your favorite IDE.
5. Write your plugin entirely in C++. (Checkout the code example in this repository). 
**Important: Do not add any files in the `Binaries` subfolder as it will be merciless deleted. Use the AdditionalSourceCode folder for this.**
6. If you're done, you can use the command line tools of HISE to build your project automatically.

## TODO

