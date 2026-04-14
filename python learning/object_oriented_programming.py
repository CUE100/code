# ONE OF THE MOST IMPORTANT TOPICS
class Car:
    def __init__(self, model, year, color, engine):
        self.model = model
        self.year = year
        self.color = color
        self.engine = engine

    def details(self):
        print(
            f"You have {self.year} - {self.model} with {self.engine} of {self.color} color")

    def electric_car(self, battery_size):
        print(
            f"You have {self.year} - {self.model} with {self.engine} of {self.color} color and has {battery_size}kg batter weight.")


car1 = Car("Porsche911", "2026", "Aesthetic white-grey", "12CC(V12 engine)")
car2 = Car("Tesla", 2026, "white", "V6")
car2.electric_car(12)
