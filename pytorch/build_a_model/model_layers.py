import torch
import os
from torchvision import datasets, transforms
from torch.utils.data import DataLoader
from torch import nn

device = torch.accelerator.current_accelerator().type if torch.accelerator.is_available() else "cpu"
image = torch.rand(3,28,28)
flattened = nn.Flatten()
flat_image = flattened(image)

linear = nn.Linear(28*28 ,20)
linear_image = linear(flat_image)

relu = nn.ReLU()
functioned_image = relu(linear_image)

linear2 = nn.Linear(20,10)
linear_image2 = linear2(functioned_image)

softmax_wrong = nn.Softmax(dim=0)
softmax_right = nn.Softmax(dim=1)

print(softmax_wrong(linear_image2).sum(dim=1))  
print(softmax_right(linear_image2).sum(dim=1))  