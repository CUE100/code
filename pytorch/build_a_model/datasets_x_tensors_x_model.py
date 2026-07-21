import torch 
from torch import nn
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
from torchvision import datasets

labels_map = {
    0: "T-shirt/top",
    1: "Trouser",
    2: "Pullover",
    3: "Dress",
    4: "Coat",
    5: "Sandal",
    6: "Shirt",
    7: "Sneaker",
    8: "Bag",
    9: "Ankle boot",
}

class NameDataset(Dataset):
    def __init__(self):
        self.image = datasets.FashionMNIST(
            root="data", download=True , train=False
        )
    def __len__(self):
        return len(self.image)
    
    def __getitem__(self,idx):
        image , label = self.image[idx]
        return image,label
    
    def labelname(self,label,labels_map):
        label_name = labels_map[label]
        return label_name,label
    


    

