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

    user_guess_input = input("Enter a letter for your guess: ").upper()
    length_word = len(user_guess_input)
    if length_word != 1 or user_guess_input in string.digits or user_guess_input in string.punctuation or "" == user_guess_input:
        print("Please enter a letter.")

    else:
        guessed_letters.append(user_guess_input)
    return user_guess_input


def display_board(word, guessed_letters):
    board_string = ""
    for letter in word:
        if letter in guessed_letters:
            board_string = board_string + letter + " "
        else:
            board_string = board_string + "_"

    print(board_string)


def main():
    guessed_letters = []
    word = random_word()
    while True:
        user_guess(guessed_letters)
        display_board(word, guessed_letters)


if __name__ == '__main__':
    main()
