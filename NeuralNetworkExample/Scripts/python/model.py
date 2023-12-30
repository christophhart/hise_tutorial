import torch
import torch.nn as nn
import torch.nn.functional as F

from dataset import MyDataset

# the model for the tanh waveshaper
class TanhModel(nn.Module):
    def __init__(self):
        super().__init__()
        numHidden = 5
        self.network = nn.Sequential(
            nn.Linear(1, numHidden*2),
            nn.Tanh(),
            nn.Linear(numHidden*2, numHidden),
            nn.Sigmoid(),
            nn.Linear(numHidden, 1)
            )
        
    def forward(self, x):
        x = self.network(x)
        return x

# the model for the sine function approximator
class SineModel(nn.Module):
    def __init__(self):
        super().__init__()
        numHidden = 4
        self.network = nn.Sequential(
            nn.Linear(1, numHidden*2),
            nn.Tanh(),
            nn.Linear(numHidden*2, numHidden),
            nn.Tanh(),
            nn.Linear(numHidden, 1)
            )
        
    def forward(self, x):
        x = self.network(x)
        return x
