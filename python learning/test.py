print("")
print("")
print("")
print("WELCOME TO OUR CAFE !!!")

item = input("enter the item you want to buy : ")
quantity = int(input("enter the quantity of item you want to buy : "))
price = float(input("enter the price of item : "))


if not item.isalpha():
    print("your item should not contain numbers !! ")

else:
    print("heres your full details : ")


total = quantity * price

is_member = (input("are you memeber of our cafe ? (y/n) : ").lower()
             ).replace(".", "").replace(".", "")
age = int(input("enter your age : "))

if is_member == "y" and age >= 60:
    total = total * 0.80
    print("as you are above 60 y and are member , you get 20 percent discount ")
    print(f"so your subtotal is {round(total)} ")

elif is_member == "y" and age < 60:
    total = total * 0.90
    print("as you are below 60 y and are member , you get 10 percent discount ")
    print(f"so your subtotal is {round(total)}")

else:
    print("you have to pay full price :( ")

status = "VIP user as subtotal greater than 50 $" if total >= 50 else "standard user as subtotal less than 50 $"
print(status)
print(
    f"you bought {item.capitalize()} ,{quantity} times, with your subtotal of {round(total)} $ !! ")
print("have a nice day !!")
