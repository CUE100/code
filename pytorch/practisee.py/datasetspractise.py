import torch
from torch.utils.data import Dataset
from torch.utils.data import DataLoader


class NameDataset(Dataset):
    def __init__(self,names):
        self.names = names

    def __len__(self):
        return len(self.names)
    
    def __getitem__(self,idx):
        ascii_num = ord(self.names[idx])
        tensor = torch.tensor(ascii_num)
        label = idx
        return tensor , label
ds = NameDataset("kaithal")
dl = DataLoader(ds, batch_size=3, shuffle=True)
for batch in dl:
    print(batch)
print(len(ds))        
print(ds[0])           
print(ds[2])           