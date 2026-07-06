import torch

data = (24)
final = torch.reshape(torch.arange(data), (4, 6))
print(torch.zeros_like(final))
grid_4x6 = final.reshape(4, 6)
grid_6x4 = final.reshape(6, 4)

print(grid_4x6)
print(grid_6x4)