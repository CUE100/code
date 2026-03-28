# car concession programme
showroom = {"porsche": 200000, "audi": 30000,
            "bmw": 80000, "mclaren": 300000}
cars = []
total = 0
print("---------------CARS SHOWROOM-----------------")
for key, value in showroom.items():
    print(f"{key:8} : {value:,}")
print("----------------------------------------------")

while True:
    user_car = input("enter a car to buy(q to quit): ").lower()
    if user_car == "q":
        break
    elif showroom.get(user_car) is not None:
        cars.append(user_car)
print("______________YOUR CARS________________________")
for car in cars:
    print(car, end=" ")
    print("_----------------_ ")
    total = total + showroom.get(car)
print(f"Your total is {total:,.2f}")
