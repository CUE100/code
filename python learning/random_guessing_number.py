import random

# Use 101 so that 100 is included in the range check
rand_num = random.randint(1, 100)
attempts = 0

print("----------------- PYTHON NUMBER GUESSING GAME -----------------")

while True:

    user_input = input(
        "Enter a number between 1 and 100 (or 'q' to quit): ")

    if user_input.lower() == 'q':
        break
    try:
        user_num = int(user_input)

    except ValueError:
        print("Please enter a valid number!")

    if user_num < 1 or user_num > 100:
        print("INVALID INPUT! Stay between 1 and 100.")
        continue
    attempts += 1
    if user_num > rand_num:
        print("The random number is less!")
    elif user_num < rand_num:
        print("The random number is greater!")
    else:
        print(
            f"CORRECT! You identified the number in {attempts} attempts!")
        break
