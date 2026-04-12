import random
import string

chars = "" + string.punctuation + string.digits + string.ascii_letters
chars = list(chars)
key = chars.copy()

random.shuffle(key)

while True:
    user_input = input("Enter a message to encrypt: ")
    cipher_text = ""
    for letter in user_input:
        index = chars.index(letter)
        cipher_text += key[index]
    print(f"Original message : {user_input}")
    print(f"Encrypted message : {cipher_text}")
