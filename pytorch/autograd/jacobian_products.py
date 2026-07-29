import torch

x = torch.tensor([2.0, 3.0], requires_grad=True)

y1 = x[0] * x[1]      # y1 = 2*3 = 6
y2 = x[0] + x[1]       # y2 = 2+3 = 5
y = torch.stack([y1, y2])   # y = [6, 5]

v = torch.tensor([1.0, 1.0])   # weight both outputs equally
y.backward(v)

print(x.grad)