import torch
import numpy as np

data = (5,3)
data1 = [(5,3)]
data2 = (((((((((5,3)))))))))
data3 = ((((5,3))))

tensor = torch.tensor(data)
tensor1 = torch.tensor(data1)
tensor2 = torch.tensor(data2)
tensor3 = torch.tensor(data3)
print(tensor.shape)
print(tensor1.shape)
print(tensor2.shape)
print(tensor3.shape)



