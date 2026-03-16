# name = input("enter your name")

# a = len(name)
# a = name.find("r")
# a = name.rfind("  ")
# a = name.capitalize()
# a = name.upper()
# a = name.lower()
# a = name.isdigit()
# a = name.isalpha()
# a = name.count("r")
# a = name.replace("r", "l")


# print(a)


name = input("enter you name : ")
length = len(name)
count = name.count(" ")
digits = name.isalpha()

if length <= 12 and count == 0 and digits == True:
    print("User input is valid !!")
elif length > 12 or 1 < count > 1 or digits == False:
    print("your input must have :# 1. username is no more than 12 characters and must not contain any space and must not conatin any digit ")
    print("user imput not valid")

else:
    print("some error occured")


# if len(username) > 12:
# print("Your username can't be more than 12 characters")
# elif not username. find("") == -1:
# print( "Your username can't contain spaces")
# elif not username.isalpha():
# print("Your username can't contain numbers")
# else:
# print (f"Welcome {username}")
