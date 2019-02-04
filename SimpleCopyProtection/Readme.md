# A simple copy protection system for HISE

There is a rather advanced license key copy protection system available for commercial projects which prevents the authorisation on multiple systems. However if you just want to embed a simple serial number check into your plugin, you can follow this route:

All you need to do is to follow this guide and distribute one serial number for each purchase. On the first load, the end user will have to enter the serial and if it's a valid one, the plugin will be unlocked (otherwise you can think about ways to deactivate the plugin as you wish)...

It has no strong protection and people will be able to run your plugin on as many systems as they want, but they might be hesitant to share their serial number which can be associated to their purchase (although if cracking crews use stolen credit cards you won't have too much legal possibilities...)

## Create the serial numbers

You can use HISE's advanced, industrial strength (!) cryptographic functions to generate the serials directly in the script (check the [Scripts/SerialGenerator.js](Scripts/SerialGenerator.js) file)

This code will generate a file called `Serials.js` in the root folder which contains a list of 1000 serials in the form `ABCD-EFGH-IJKL-MNOP`. Use this list and supply one serial per purchase to your end users (most web shops should offer the possibility to paste valid serial numbers there). Then you copy the file into the `Scripts` subfolder, make it a valid Javascript file by pretending `const var serials = ` before the JSON object and `;` after it, then include the file as object with `include("Serials.js");` Since all script files will be embedded in the plugin, the serial list will not be accessible to end users.

## The authorisation procedure

You most certainly don't want the user to enter the serial number each time, so we need to make a persistent check whether it is already authorized. For this purpose, we use the `Engine.dumpAsJSON()` method to just store their entered serial to a file which is loaded every time the plugin is instanciated.

The example project contains a authorisation dialogue and the necessary logic to get you started. Please take a look at the [Scripts/Authorisation.js](Scripts/Authorisation.js) file to understand how it works.

