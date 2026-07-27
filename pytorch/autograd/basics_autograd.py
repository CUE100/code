import torch

a = torch.ones(5)
b = torch.rand(3)

x = torch.rand(5,3,requires_grad=True)
y = torch.rand(3,requires_grad=True)

z = torch.matmul(a,x) + y

loss = torch.nn.functional.binary_cross_entropy_with_logits(z,b)
#             |
#             \/
loss.backward()
#             |
#             \/
print(x.grad)
print(y.grad)
#             |
#             \/

print(f"Gradient function for z = {z.grad_fn}")
print(f"Gradient function for loss = {loss.grad_fn}")