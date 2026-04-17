class Shapes:
    def __init__(self, name,  color, filled, rotating):
        self.color = color
        self.filled = filled
        self.rotating = rotating
        self.name = name

    def describe(self):
        print(f"This is a {self.name} of {self.color} and {'is rotating' if self.filled == True else 'is not filled'}, {'is rotating' if self.rotating == True else 'is not roating.'}")

    def is_rotating(self):
        print(
            f"{' This shape is rotating' if self.rotating == True else 'This shape is not roating.'}")


class Square(Shapes):
    def __init__(self, name, color, filled, rotating, side):
        super().__init__(name, color, filled, rotating)
        self.side = side

    def area(self):
        print(f"Area of this square is {self.side ** 2}")


class Triangle(Shapes):
    def __init__(self, name, color, filled, rotating, height, base):
        super().__init__(name, color, filled, rotating)
        self.base = base
        self.height = height

    def area(self):
        print(f"Area of this square is {1/2 * self.height * self.base}")


square = Square("Square", "Red", True, True, 10)
triangle = Triangle("Triangle", "Blue", False, True, 10, 20)
print("_" * 30)
square.describe()
print("_" * 30)
triangle.describe()
print("_" * 30)
print(square.color)
