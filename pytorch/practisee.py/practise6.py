import torch

data = (4,4)
tensor = torch.zeros(data) 
tensor[1,2] = 255
tensor[2,1] = 100
tensor[1,1] = 255
tensor[3,2] = 100
tensor[2,2] = 255
tensor[2,3] = 100
tensor[0,2] = 100 
tensor[1,3] = 100
tensor[0,1] = 100
tensor[1,0] = 100
difference = tensor[0,] - tensor[1,]
print(difference)