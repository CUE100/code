import torch
import os
from torchvision import datasets, transforms
from torch.utils.data import DataLoader
from torch import nn
device = torch.accelerator.current_accelerator().type if torch.accelerator.is_available() else "cpu"

class Neuralnetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        
        self.sequential_relu_stack = nn.Sequential(
            nn.Linear(28*28,512),
            nn.ReLU(),
            nn.Linear(512,512),
            nn.ReLU(),
            nn.Linear(512,10),
            
        )
    def forward(self,x):
        x = self.flatten(x)
        logits = self.sequential_relu_stack(x)
        return logits
    

model = Neuralnetwork().to(device)
print(f"Model structure: {model}\n\n")

for name, param in model.named_parameters():
    print(f"Layer: {name} | Size: {param.size()} | Values : {param[:2]} \n")