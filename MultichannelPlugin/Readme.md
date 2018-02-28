# Multichannel plugins in HISE

Required version: `1.5.2`

You can now export multichannnel plugins in HISE (instruments only, FX plugins are stereo).  In order to do this, you need to do the following things:

### Increase the channel amount of the master container

Click on the routing button and change the channel amount to the desired number. You don't need to bother about the routing now.

### Setup the internal routing

For every sound source that you want to control the output, route it to a dedicated channel in the master container.

### Implement the routing logic via scripting

There are a few new scripting functions that allow dynamic control of the routing. Take a look at this example how to use them.

### Export the plugin

If you export the plugin, it will automatically create as many channels as you defined in your master container's routing matrix.