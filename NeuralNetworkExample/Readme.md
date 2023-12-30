# Neural Network Demo Project

This project contains an example of how to use the Neural Network API to run inference on trained networks. 

- `RTNeuralExample.xml` shows how to load an neural network in HiseScript
- `NeuralSynthesiser.xml` shows how to load a neural network in a scriptnode patch and use it in a polyphonic multichannel context. The network that is used is an approximation of a sine wave with -45dB accuracy so you basically end up with a worse sounding sine wave generator that uses 10x the CPU resources of a normal one, but now it's AI powered.
- `Scripts/Python` contains some helper scripts in Python as well as a tutorial how to use TorchStudio for creating & training the models.