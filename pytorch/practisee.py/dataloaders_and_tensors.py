import torch
from torch.utils.data import Dataset
from torch.utils.data import DataLoader

data = (5,5)

tensor_plus = torch.zeros(data)
tensor_plus[0,2] = 255
tensor_plus[1,2] = 255
tensor_plus[2,2] = 255
tensor_plus[3,2] = 255
tensor_plus[4,2] = 255
tensor_plus[2,0] = 255
tensor_plus[2,1] = 255
tensor_plus[2,3] = 255
tensor_plus[2,4] = 255
normalised = tensor_plus / 255

tensor_diagnol = torch.zeros(data)
tensor_diagnol[0,0] = 255
tensor_diagnol[1,1] = 255
tensor_diagnol[2,2] = 255
tensor_diagnol[3,3] = 255
tensor_diagnol[4,4] = 255

normalised1 = tensor_diagnol / 255

tensor_border = torch.zeros(data)
tensor_border[0,0] = 255
tensor_border[0,1] = 255
tensor_border[0,2] = 255
tensor_border[0,3] = 255
tensor_border[0,4] = 255
tensor_border[1,0] = 255
tensor_border[1,4] = 255
tensor_border[2,0] = 255  
tensor_border[2,4] = 255
tensor_border[3,0] = 255
tensor_border[3,4] = 255
tensor_border[4,0] = 255
tensor_border[4,1] = 255
tensor_border[4,2] = 255
tensor_border[4,3] = 255
tensor_border[4,4] = 255

normalised2 = tensor_border / 255
shapes_list = ["plus"] * 10 + ["diagnol"] * 10 + ["border"] * 10
class NameDataset(Dataset):
    def __init__(self,shapes):
        self.shapes = shapes

    def __len__(self):
        return len(self.shapes)
    
    def __getitem__(self,idx):
        if self.shapes[idx] == "plus":
            tensor_normal = normalised
            label = 0
        elif self.shapes[idx] == "diagnol":
            tensor_normal = normalised1
            label = 1
        elif self.shapes[idx] == "border":
            tensor_normal = normalised2
            label = 2
            return tensor_normal , label
        return tensor_normal , label
    
ds = NameDataset(shapes_list)
dl = DataLoader(ds, batch_size=10,shuffle=True)
for batch in dl:
    print(batch)

        
        