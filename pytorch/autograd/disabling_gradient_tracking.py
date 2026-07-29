import torch

x = torch.ones(5)  # input tensor
y = torch.zeros(3)  # expected output
w = torch.randn(5, 3, requires_grad=True)
b = torch.randn(3, requires_grad=True)
z = torch.matmul(x, w)+b
loss = torch.nn.functional.binary_cross_entropy_with_logits(z, y)
print(z.requires_grad)
with torch.no_grad():
    z = torch.matmul(x, w) + b   # recomputed here, inside no_grad
    print(z.requires_grad)        # False
print(z.requires_grad)            # still False, because z was overwritten
#There are reasons you might want to disable gradient tracking:
#To mark some parameters in your neural network as frozen parameters.
#To speed up computations when you are only doing forward pass, because computations on tensors that do not track gradients would be more efficient.