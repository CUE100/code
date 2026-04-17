from abc import ABC, abstractmethod


class Shapes(ABC):
    @abstractmethod
    def area(self):
        pass


class Square(Shapes):
    def __init__(self, side):
        self.side = side

    def area(self):
        return self.side ** 2


class Triangle(Shapes):
    def __init__(self, base, height):
        self.base = base
        self.height = height

    def area(self):
        return self.base * self.height * 1/2


shapes = [Square(10), Triangle(10, 20)]

for x in shapes:
    print(x.area())
