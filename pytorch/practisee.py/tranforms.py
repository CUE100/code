import torch
import torch.nn.functional as F
from torchvision import datasets
from torchvision.transforms import v2

ds = datasets.FashionMNIST(root="/Users/rohinboora/code/pytorch" ,download=True, train=True , transform=v2.Compose([v2.ToImage(), v2.ToDtype(scale=True, dtype=torch.float32)]), target_transform=v2.Lambda(lambda y: F.one_hot(torch.tensor(y), num_class=10).float()))