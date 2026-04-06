import time
print("------------------------------------------------------")
print("WELCOME TO OUR CAFE !!!")


def details():
    item = input("Enter the item you want to buy : ")
    quantity = int(input("Enter the quantity of item you want to buy : "))
    price = float(input("Enter the price of item : "))
    return item, quantity, price


def calculation(quantity, price):
    total = quantity * price
    is_member = input(
        "Are you memeber of our cafe ? (y/n) : ").lower().replace(".", "").replace(".", "")
    age = int(input("enter your age : "))
    if is_member == "y" and age >= 60:
        total = total * 0.80
    elif is_member == "y" and age < 60:
        total = total * 0.90
    return total, age, quantity, price


def reciept(item, quantity, total):
    print("your food in 3 seconds ")
    for x in range(0, 3):
        time.sleep(1)
        print(x)

    print("Heres your food: ")
    print(
        f"You bought {item.capitalize()} ,{quantity} times, with your subtotal of {round(total, 2)} $ !! ")
    print("Have a nice day !!")

# MAIN PROGRAMME SKELTON


item, quantity, price = details()
total, age, quantity, price = calculation(quantity, price)
reciept(item, quantity, total)
