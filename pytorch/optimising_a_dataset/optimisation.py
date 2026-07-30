import torch
from torch import nn
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import v2

training_data = datasets.FashionMNIST(
    root="data",
    download=False,
    train=True,
    transform=v2.Compose([v2.ToImage(), v2.ToDtype(torch.float32, scale=True)])
)

test_data = datasets.FashionMNIST(
    root="data",
    download=False,
    train=False,
    transform=v2.Compose([v2.ToImage(), v2.ToDtype(torch.float32, scale=True)])
)

train_dataloaders = DataLoader(training_data,batch_size=64)
test_dataloaders = DataLoader(test_data,batch_size=64)

class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.sequential = nn.Sequential(
            nn.Linear(28*28,512),
            nn.ReLU(),
            nn.Linear(512,512),
            nn.ReLU(),
            nn.Linear(512,10)
        )

    def forward(self,x):
        x = self.flatten(x)
        logits = self.sequential(x)
        return logits

model = NeuralNetwork()

loss_fn = nn.CrossEntropyLoss()
optimized = torch.optim.SGD(model.parameters(),lr=0.001)

num_epochs = 5

for epoch in range(num_epochs):
    for images, labels in train_dataloaders:
        predictions = model(images)         # forward pass
        loss = loss_fn(predictions, labels)  # compute loss
        loss.backward()                      # backward pass — fills .grad
        optimized.step()                     # gradient descent — updates weights
        optimized.zero_grad()                # reset grads for next batch
    print(f"epoch {epoch}, loss = {loss.item()}")