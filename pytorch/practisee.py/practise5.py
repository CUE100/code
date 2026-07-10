import torch

data = (3, 3)
zeros = torch.zeros(data) #i ahve choosed a shape of 3x3 matrix and then i have filled it with zeros
zeros[0][0] = 1
zeros[0][1] = 1 
zeros[0][1] = -1
zeros[2,2] = 1

if torch.all(zeros.diagonal() == 1) or torch.all(zeros.diagonal() == -1) or zeros.sum(dim=0) == 3 or zeros.sum(dim=1) == 3 or zeros.sum(dim=0) == -3 or zeros.sum(dim=1) == -3:
    print("You Won!")
    print(zeros)