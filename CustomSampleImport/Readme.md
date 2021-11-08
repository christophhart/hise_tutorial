# Custom Sample Import example

This example project shows how to use the new JSON samplemap functions to load a custom sample into the sampler. Until now every project that wanted to allow the user to import her own audio files required a Audio Loop Player which often came with a significant overhead, bugs and limited capabilities.

Now you can just load any arbitrary sample into a sampler module and edit its properties. This example project includes:

- the logic of loading / saving custom samplemaps using a Base64 encoded sample map in a panel. This project also includes some dummy internal samplemaps as
  most real world projects come with their own samples (`Scripts/SampleLoadSave.js`)
- some UI components that change the sample properties (including loop points and crossfade)
- a pitch detection button that analyses the loaded sample and moves the root note to match the frequency (`Scripts/PitchDetection.js`)
- a panel that acts as loop point editor. It overlays the audio waveform and can be dragged around to update the loop points (`Scripts/LoopPointDragger.js`).

> Be aware that this is a rather complex topic and incorporating this example into your project requires a profound knowledge of the HiseScript language as well as the data management system of user presets.

Just load the XML preset `CustomImport` and play around with your own samples (or use the samples in the `Samples` subfolder to check the relative path parsing).

## Backwards compatibility

If you already have user presets that connect to an audio waveform and want to move to this system, you need to enable the user preset preprocessing callback, extract the information from the audio waveform values and convert it to a Base64 version. You can load `XmlPresetBackups/OldStyle.xml` which contains a simple patch with a audiowaveform connected to a looper to create a few user presets. If you then try to load these presets with the new version, it will automatically convert the information from the user preset into a sample map (`Scripts/Migration.js`);