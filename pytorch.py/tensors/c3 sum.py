import torch
data = [[1, 2],[3, 4]]

sum = torch.tensor(data).sum()
sum = sum.item()
print(sum, type(sum))