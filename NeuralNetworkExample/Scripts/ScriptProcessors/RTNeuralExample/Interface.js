Content.makeFrontInterface(600, 600);

const var pythonRoot = FileSystem.getFolder(FileSystem.UserPresets).getParentDirectory().getChildFile("Scripts/Python");

//! PYTORCH MODEL ============================================================================

// Create a neural network
const var pt = Engine.createNeuralNetwork("PytorchNetwork");;

// Load the model layout & weights that were exported as JSON
const var modelJSON = pythonRoot.getChildFile("tanh_model.json").loadAsObject();

// Load the model & weights:
pt.loadPytorchModel(modelJSON);

// Print the model layout:

// Ready for inference
const var b = Buffer.create(512);

// Create a ramp from 0 ... 1
for(i = 0; i < b.length; i++)
	b[i] = 0.5 * Math.sin(i / b.length * Math.PI * 2.0);
	
// Our Pytorch toy model is approximating a simple tanh function with a gain of 20.0
// so let's look how it transforms a sine wave:
for(s in b)
	s = pt.process(s);

//! TENSORFLOW MODEL =========================================================================

const var tf = Engine.createNeuralNetwork("TensorFlowNetwork");;

// Load the weight & model JSON
const var weights_tf = pythonRoot.getChildFile("model_tf.json").loadAsObject();

// Tensorflow exports both model and weights as one JSON so we use this API call
// instead of the build() and loadWeights() methods
tf.loadTensorFlowModel(weights_tf);

// Ready for inference
const var b2 = Buffer.create(512);

for(i = 0; i < b2.length; i++)
	b2[i] = 40.0 * Math.sin(i / b2.length * Math.PI * 2.0);
	
// The Tensorflow model is apparently simulating some distortion, so
// let's take a look how it transforms the sine wave:	
for(s in b2)
	s = tf.process(s);function onNoteOn()
{
	
}
 function onNoteOff()
{
	
}
 function onController()
{
	
}
 function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 