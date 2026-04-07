import random
import time

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
