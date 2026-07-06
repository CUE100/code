import torch
data = (8,8)

grid = torch.zeros(data)
grid[:, 4] = 255
grid[4, :] = 255
new = grid/255
print(new.int())
print(grid.dtype)
print(new.dtype)