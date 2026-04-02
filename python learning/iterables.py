fruits = ["apple", "banana", "coconut"]
cars = ("bmw", "porsche", "mercedes")
weapons = {"toyata truck ", "leapord tank" "b2 bomber"}
name = "Bruce wayne"
dictionary = {1: "hello", 2: "bye", 3: "introverts response"}

for fruit in fruits:
    print(fruit, end=" ,")
print()
for car in cars:
    print(car, end=" , ")
print()
for weapon in weapons:
    print(weapon, end=" , ")
print()
for character in name:
    print(character, end=" ")
print()
for key in dictionary:
    print(key, end=", ")
print()
for value in dictionary.values():
    print(value, end=" ,")
print()
for key, values in dictionary.items():
    print(key, values, end=" : ")
print()
