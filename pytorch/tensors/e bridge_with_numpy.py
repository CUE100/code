import torch
import numpy 

t = torch.zeros(5)
t.add_(1)
n = t.numpy()
numpy.add(t.numpy(), 1, out=n)
tensor = torch.from_numpy(n)
print(tensor)