import torch

tensor = torch.tensor([3, 4, 5], dtype=torch.float32)

y1 = tensor @ tensor.T
y2 = tensor.matmul(tensor.T)

y3 = torch.ones_like(y1)
torch.matmul(tensor, tensor.T, out=y3)

z1 = tensor * tensor
z2 = tensor.mul(tensor)

z3 = torch.ones_like(tensor)
torch.mul(tensor, tensor, out=z3)

print(f"y1: {y1}, y2: {y2}, y3: {y3}")
print(f"z1: {z1}, z2: {z2}, z3: {z3}")
