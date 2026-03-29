import random

print("-----------------------------------------------------")

rand_num = random.randint(1, 100)
user_num = int(input("enter a number between 1 and 100 : "))
attempts = 0

while user_num not in range(1, 100):
    user_num = int(input("enter a number between 1 and 100 : "))
    if user_num in range(1, 100):
        break

while True:

    if user_num > rand_num:
        print("-----------------------------------------------------")
        print("random number is less!")
        user_num = int(input("enter a number between 1 and 100 : "))
        attempts += 1

    elif user_num < rand_num:
        print("-----------------------------------------------------")
        print("random number is greater!")
        user_num = int(input("enter a number between 1 and 100 : "))
        attempts += 1

    if user_num == rand_num:
        print("-----------------------------------------------------")
        print("you have identified correct number!")
        attempts += 1
        break

print("-----------------------------------------------------")
print(f"You took {attempts} attempts to guess random number! ")
print("-----------------------------------------------------")
