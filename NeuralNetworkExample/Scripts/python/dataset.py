import math
import torch
from torch.utils.data import Dataset
import random

class MyDataset(Dataset):
    def __init__(self):
        super().__init__()
        self.x = []
        self.y = []

        numValues = 800

        for x in range(numValues):
            i = random.random() * 2.0 - 1.0 
            self.x.append(i)
            self.y.append(math.tanh(15.0 * i))

    def __len__(self):
        return len(self.x)

    def __getitem__(self, idx):
        return self.x[idx], self.y[idx]

