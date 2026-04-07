import random
import time


def menu():
    print("Which game would you like to play?")
    print("To play dice rolling game: Press 1.")
    print("To play number guessing game: Press 2.")
    print("To play rock , paper , scissor: Press 3.")
    print("To play coin toss game: Press 4.")
    print("To quit this app : Press 5. ")


def user_input():
    while True:
        user_input1 = input("Press the number of game you want to play: ")
        try:
            user_input1 = int(user_input1)
        except ValueError:
            print("Please enter a valid number! ")
            continue
        return user_input1


def dice_game():

    dice_roll_min = 1
    dice_roll_max = 6
    dice_faces = {
        1: """
        ------- 
        |       |
        |   o   |
        |       |
        ------- 
        """,

        2: """
        ------- 
        | o     |
        |       |
        |     o |
        ------- 
        """,

        3: """
        ------- 
        | o     |
        |   o   |
        |     o |
        ------- 
        """,

        4: """
        ------- 
        | o   o |
        |       |
        | o   o |
        ------- 
        """,

        5: """
        ------- 
        | o   o |
        |   o   |
        | o   o |
        ------- 
        """,

        6: """
        ------- 
        | o   o |
        | o   o |
        | o   o |
        ------- 
        """
    }

    while True:
        total_sum = 0
        try:
            num_dice = int(
                input("Enter the number of dices you want to roll(max 10 ; 0 to quit): "))
        except ValueError:
            print("⚠️ Please enter a valid number. ")
            continue
        if num_dice == 0:
            break
        if num_dice < 1 or num_dice > 10:
            print("⚠️ please enter a number between 1 and 10: ")
            continue

        for dice in range(num_dice):
            random_dice = random.randint(dice_roll_min, dice_roll_max)
            print(dice_faces[random_dice], end="")
            total_sum += random_dice
        print(f"Your total is: {total_sum}")


def random_num():

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


def RPS():
    import random
    options = ["rock", "paper", "scissors"]
    print("--- Welcome to Rock, Paper, Scissors! ---")

    while True:
        bot_choice = random.choice(options)
        user_choice = input(
            "\nEnter rock, paper, or scissors (or 'q' to quit): ").lower().strip()

        if user_choice == 'q':
            print("Thanks for playing!")
            break

        if user_choice not in options:
            print("Invalid input. Please try again.")
            continue

        print("Bot result in few seconds...")
        for x in reversed(range(4)):
            time.sleep(1)
            print(x)

        print(f"Bot chose: {bot_choice}")

        if user_choice == bot_choice:
            print(f"Both chose {user_choice}. It's a tie!")

        elif (user_choice == "rock" and bot_choice == "scissors") or \
            (user_choice == "paper" and bot_choice == "rock") or \
                (user_choice == "scissors" and bot_choice == "paper"):
            print("You won against the bot!")

        else:
            print("You lost against the bot!")


def coin_toss():

    while True:
        choice = input("Enter HEADS OR TAILS ('q' to quit): ").upper()
        a = "HEADS"
        b = "TAILS"
        bot_choice = random.choice([a, b])
        if "Q" in choice:
            break
        elif choice not in ["HEADS", "TAILS"]:
            print("Please enter a valid choice !")
            continue
        elif choice == bot_choice:
            print("YOU WON!")
            continue
        else:
            print("YOU LOST !")


while True:
    menu()
    choice = user_input()
    if choice == 1:
        dice_game()
    elif choice == 2:
        random_num()
    elif choice == 3:
        RPS()
    elif choice == 4:
        coin_toss()
    else:
        break
