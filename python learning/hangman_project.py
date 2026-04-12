import random
import string
hangman_stages = {0:
                  """
      +---+
      |   |
      O   |
     /|\\  |
     / \\  |
          |
    =========
    """,

                  1: """
      +---+
      |   |
      O   |
     /|\\  |
     /    |
          |
    =========
    """,
                  2: """
      +---+
      |   |
      O   |
     /|\\  |
          |
          |
    =========
    """,
                  3: """
      +---+
      |   |
      O   |
     /|    |
          |
          |
    =========
    """,
                  4: """
      +---+
      |   |
      O   |
      |   |
          |
          |
    =========
    """,
                  5: """
      +---+
      |   |
      O   |
          |
          |
          |
    =========
    """,
                  6: """
      +---+
      |   |
          |
          |
          |
          |
    =========
    """
                  }
words = [
    "PYTHON", "GUITAR", "CASTLE", "PLANET", "ROCKET",
    "JUNGLE", "OCEAN", "WIZARD", "DRAGON", "DIAMOND",
    "MYSTERY", "ZIGZAG", "JAZZ", "RHYTHM", "OXYGEN", "DINASOUR"
]


def random_word():
    word = random.choice(words)
    return word


def user_guess(guessed_letters):
    while True:
        print("-" * 40)
        user_guess_input = input("Enter a letter for your guess: ").upper()
        print("-" * 40)
        length_word = len(user_guess_input)

        if length_word != 1 or user_guess_input in string.digits or user_guess_input in string.punctuation or "" == user_guess_input:
            print("Please enter a letter.")

        elif user_guess_input in guessed_letters:
            print("YOU HAVE ALREADY ENTERED IT !")

        else:
            guessed_letters.append(user_guess_input)
            break
        return user_guess_input


def display_board(word, guessed_letters):
    lives = 6
    wrong_guesses = 0
    board_string = ""
    for letter in word:
        if letter in guessed_letters:
            board_string = board_string + letter + " "
        else:
            board_string = board_string + "_ "
    for guess in guessed_letters:
        if guess not in word:
            wrong_guesses += 1

    lives -= wrong_guesses

    for key, value in hangman_stages.items():
        if key == lives:
            print(value)
            print(f"{lives} tries remaining.")
            print(board_string)
    return lives, board_string


def main():

    guessed_letters = []
    word = random_word()
    while True:

        lives, board_string = display_board(word, guessed_letters)
        if lives == 0:
            print("-" * 40)
            print("GAME OVER! YOU LOST!")
            print(f"The word was : {word}")
            print("-" * 40)
            break
        if "_" not in board_string:
            print("-" * 40)
            print("YOU WON THIS GAME!")
            print("-" * 40)
            break
        user_guess(guessed_letters)


if __name__ == '__main__':
    main()
