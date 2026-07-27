from torch import nn
from torch.utils.data import Dataset, DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor

labels_map = {
    0: "T-shirt/top",
    1: "Trouser",
    2: "Pullover",
    3: "Dress",
    4: "Coat",
    5: "Sandal",
    6: "Shirt",
    7: "Sneaker",
    8: "Bag",
    9: "Ankle boot",
}


class NameDataset(Dataset):
    def __init__(self):
        self.image = datasets.FashionMNIST(
            root="data", download=True, train=False, transform=ToTensor()
        )

    def __len__(self):
        return len(self.image)

    def __getitem__(self, idx):
        imagee, label = self.image[idx]
        return imagee, label

    def labelname(self, label, labels_map):
        return labels_map[label]


class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()

        self.flatten = nn.Flatten()

        self.nn_sequential = nn.Sequential(
            nn.Linear(28*28, 784),
            nn.ReLU(),
            nn.Linear(784, 20),
            nn.ReLU(),
            nn.Linear(20, 10)
        )

    def forward(self, original_image):

        x = self.flatten(original_image)
        x = self.nn_sequential(x)

        return x


ds = NameDataset()

loader = DataLoader(ds, batch_size=64, shuffle=True)
model = NeuralNetwork()

images, labels = next(iter(loader))
logits = model(images)
print(logits.shape)
