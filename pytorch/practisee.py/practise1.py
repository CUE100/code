import torch
from torch.utils.data import Dataset, DataLoader

class NumberDataset():
    def __init__(self, n):
        self.n = n

    def __len__(self):
        return self.n

    def __getitem__(self, idx):
        if idx % 2 == 0:
            label = 0   
        else:
            label = 1
        
        return idx , label
    
dataset = NumberDataset(20)
print(len(dataset))
print(dataset[7])