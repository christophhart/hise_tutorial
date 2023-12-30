import torch
import torch.nn as nn
from torch.utils.data import Dataset
import numpy as np
import json
from json import JSONEncoder

# Import the model
from model import TanhModel
from model import SineModel

# Helper class to encode the JSON (stolen from the RTNeural repository)
class EncodeTensor(JSONEncoder,Dataset):
    def default(self, obj):
        if isinstance(obj, torch.Tensor):
            return obj.cpu().detach().numpy().tolist()
        return super(json.NpEncoder, self).default(obj)

# The export function
def export(model, weightfile, outputFile):
    model.load_state_dict(torch.load(weightfile))
    model.eval()

    print("Export model")
    print(model)

    # Create a dict with the model string representation and
    # the model weights. This will be picked up by the
    # loadPytorchModel() function and parses the string (hacky!!!)
    # to a model layout
    j = { "layers": str(model),
          "weights": model.state_dict() }

    #with open('model.txt', 'w') as f:
    #    print(model, file=f) 

    with open(outputFile, 'w') as json_file:
        json.dump(j, json_file,cls=EncodeTensor)

    print("Wrote model to " + outputFile)

    #with open('parameters.json', 'w') as json_file:
    #    json.dump(model.state_dict(), json_file,cls=EncodeTensor)


export(TanhModel(), "tanh_weights.pth", 'tanh_model.json')
export(SineModel(), "sine_weights.pth", "sine_model.json")





