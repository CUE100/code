import torch
from torch.utils.data import Dataset, DataLoader

class NumberDataset(DataLoader):
    def __init__(self, n):
        self.n = n
        