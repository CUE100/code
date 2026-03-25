# car buying showroom
cars = []
prices = []
total = 0

while True:
    car_name = input("enter a car to buy(e to end shopping) : ").lower()

    if car_name == "e":
        print("THANKS FOR COMING! ")
        break

    elif True:

        price = float(input("enter the price of car: "))
        total = total + price
        cars.append(car_name)
        prices.append(price)

        print("----YOUR CARS---")
        for car_name in cars:
            print(f"{car_name}")
        for price in prices:
            print(f"price of car is {price} respectively !")
        print(f"Your total is {total:,.2f} ")
