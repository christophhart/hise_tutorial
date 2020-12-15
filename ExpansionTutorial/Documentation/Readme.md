---
keywords: Expansion Tutorial
summary:  New fil
author:   Christoph Hart
index:    01
modified: 15.12.2020
---



This tutorial covers the full feature set of the `FileBased` expansion type.

- Loading background images based on the current Expansion: ImageTest.xml
- Loading audio files from different expansions into AudioLoopers: AudioTest.xml
- Loading sample maps from different expansions into Samplers: SamplerTest.xml TODO
- Loading UserPresets from different expansions: UserPresetTest.xml TODO
- Loading MidiFiles from different expansions: MidiTest.xml TODO

In order to build these, just load one of the XML projects and export the plugin.

1. Go into the settings and tick the box `LinkExpansionToProject`. This will create a local link file 
   so that the compiled plugin will use the Expansion directory. 
2. Make sure to rebuild the Pool files at each export, because the projects might contain different data and
   you will end up with a non-working product if you eg. export the AudioTest with the ImageTest pool.
   (If you haven't explicitely disabled Rebuild Pool files)
3. Compile the plugin and check how it works. Try removing one of the expansions **AFTER** compilation
   and you will see that the next time you open the compiled project, it will not contain the contents of 
   the removed expansion.





