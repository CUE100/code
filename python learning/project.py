class Car:
    def turn_on(self):
        print("You start the engine.")
        return self

    def drive(self):
        print("You step on the gas.")
        return self  # Returns the object itself

    def brake(self):
        print("You step on the brakes.")


my_car = Car()
# Because each method returns 'self', you can chain them together:
my_car.turn_on().drive().brake()
