cars = {"porsche": "200k", "audi": "30k", "buggati": "1M", "ferrari": "300k"}
# print(help(cars))

# (cars.get("porsche"))

# if cars.get("mclaren"):
# print("This car exists!")
# else:
# print("This car does not exist")

# cars.update({"mclaren": "500k"})
# cars.update({"porsche": "300k"})
# print(cars)

# cars.pop("porsche")
# print(cars)
# cars.popitem()
# print(cars)

# print(cars.clear())
# cars = cars.keys()
# print(cars)

# for car in cars:
#    print(car)

# cars = cars.values()
# print(cars)

# for car in cars:
#  print(car)

# cars = cars.items()
# print(cars)

for key, value in cars.items():
    print(f"{key} : {value}")
