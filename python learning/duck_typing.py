class Animals:
    def alive(self):
        print("This animal is alive!")


class Dog(Animals):
    def speak(self):
        print("WOOF!")


class Cat(Animals):
    def speak(self):
        print("MEOW! ")


class Jet():
    def alive(self):
        print("This animal is not alive!")

    def speak(self):
        print("Woosh...!")


objects = [Dog(), Cat(), Jet()]
for x in objects:
    x.alive()
    x.speak()
