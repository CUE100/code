fruits = ["apple", "banana", "coconut"]
cars = ("bmw", "porsche", "mercedes")
weapons = {"toyata truck ", "leapord tank" "b2 bomber"}
name = "Bruce wayne"
dictionary = {1: "hello", 2: "bye", 3: "introverts response"}

if "apple" in fruits:
    print("This fruit is in your cart.")
else:
    print("This fruit is not in your cart.")

if "porsche" not in cars:
    print("This car is in your garage.")
else:
    print("This car is not in your garage.")

if "gernades" in weapons:
    print("This weapon is in ammunation shop.")
else:
    print("This weapon is not in ammunation shop.")

if "r" in name:
    print("This letter is in your name.")
else:
    print("This letter is not in your name.")

if 1 not in dictionary:
    print(f"1 value is: {dictionary[1]}")
