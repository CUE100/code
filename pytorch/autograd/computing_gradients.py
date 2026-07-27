import torch

x = torch.ones(5)
y = torch.randn(3)

a = torch.randn(5,3,requires_grad=True)
b = torch.randn(3,requires_grad=True)

z = torch.matmul(x,a)+b 
print(z.shape)
loss= torch.nn.functional.binary_cross_entropy_with_logits(z,y)

loss.backward()
print(a.grad ,a )
print(b.grad , b)