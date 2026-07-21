from torch.utils.data import Dataset
from torch import nn


class MyDataset(Dataset):
    """Custom dataset class."""

    def __init__(self, num):
        self.num = num

    def __len__(self):
        return len(self.num)

    def __getitem__(self, idx):
        number = self.num[idx]
        if self.num[idx] % 2 == 0:
            label = idx == 0
        elif self.num[idx] % 2 != 0:
            label = idx == 1
        return label, number


class NeuralNetwork(nn.Module):
    """Neural network model."""

    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.sequential = nn.Sequential(
            nn.Linear(1, 1),
            nn.ReLU(),
            nn.Linear(1, 1)
        )

    def forward(self, x):
        """Forward pass."""
        x = self.flatten(x)
        logits = self.sequential(x)
        return logits
