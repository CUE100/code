# while loop = execute some code WHILE some condition remains true

name = input("Enter your name: ")
while name == "":
    print("You did not enter your name")
    name = input("Enter your name: ")

print(f"Hello {name}")

age = int(input("enter your age : "))
while age <= 0:
    print("you are in previous time")
    age = int(input("enter your age : "))
print(f"your age is {age}")

hero = input("enter the name of superhero you like (type im batman to quit) : ")


while not hero == "im batman":
    print(f"you like {hero} ")

    hero = input(
        "enter the name of another superhero you like (type im batman to quit) : ")

print("byee")


num1 = int(input("enter your number betweeen (1 and 10): "))

while not 10 > num1 > 1:
    print(f"your number is {num1} but it should be 10 > num1 > 1")
    num1 = int(input("enter your number betweeen (1 and 10): "))

print("byee")
print(" :))))))))))) ")
print(" :))))))))))) ")
