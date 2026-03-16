net_worth = float(input("enter your net worth : "))
if net_worth <= 0:
    print("you are in debt ")
elif net_worth > 0 and net_worth <= 100000:
    print("you are poor")
elif net_worth > 100000 and net_worth <= 1000000:
    print("you are middle class")
elif net_worth > 1000000 and net_worth <= 100000000:
    print("you are rich")
else:
    print("you are ultra rich!!!")


response = input("do you like batman ? (y/n)  :  ")
if response == "y":
    print("we can be friends")
elif response == "n":
    print("we cannot be friends :(")
else:
    print("dont you have a answer for this question ?")


is_batman = False
if is_batman:
    print("we can be friends")
else:
    print("we cannot be friends :(")
