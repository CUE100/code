import torch

points = torch.tensor([[
    [0.0, 0.0],
    [1.0, 2.0],
    [3.0, 1.0],
    [5.0, 5.0],
    [2.0, 4.0],
    [4.0, 0.0],
]])

points_b = points.reshape(1, 6, 2)   
points_a = points.reshape(6, 1, 2)   

diff = points_a - points_b
squared = diff ** 2
summed = squared.sum(dim=2)         
manual_distances = torch.sqrt(summed)  

print("Manual:")
print(manual_distances)
