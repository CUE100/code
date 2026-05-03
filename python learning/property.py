class Car:

    def __init__(self, brand, model):
        self._brand = brand
        self._model = model

    @property
    def brand(self):
        return f"{self._brand}."

    @property
    def model(self):
        return f"{self._model}."

    @brand.setter
    def brand(self, value):
        self._brand = value
        print(f"{value}..!!")

    @model.setter
    def model(self, value):
        self._model = value
        print(f"{value}..!!")

    @brand.deleter
    def brand(self):
        del self._brand
        print("Brand has been deleted.")

    @model.deleter
    def model(self):
        del self._model
        print("model has been deleted.")


car = Car("Porsche", 911)
car.brand = "mclaren"
car.model = 9111
del car.brand
del car.model
