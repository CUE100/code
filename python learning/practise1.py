for box in range(1, 6):
    print(box)
    ask = input("Is the box damaged? (y/n) : ")

    if ask == "n":
        print(f"Box {box} is shelved.")

    elif ask == "y":
        while True:
            ask2 = input("Enter the word fix three times to repair : ")
            if ask2 == "fix fix fix":
                print(f"Box {box} is shelved.")
                break
