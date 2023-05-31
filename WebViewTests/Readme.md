# WebView Test Project

This project contains multiple patches that demonstrate the usage of the new WebView UI element.

Just load any of the XML files in the XmlPresetBackups directory to check out different things. This collection will be extended over time. Currently available:

1. A [three.js](https://threejs.org/) demo using the WebGL renderer to draw a 3D cube that is pulsating using a HISE LFO output
2. A custom user preset browser that allows you to write your own preset browser in HTML. It supports bidirectional communication to update and load the currently selected browser
3. A audio oscilloscope using a 60fps HTML5 canvas with blur and fast communication between HISE and the Webview.
4. A game demo using the PlayCanvas web game engine

The scripts are verbosely documented so the best way to learn about this is to just load a preset and skim the code...