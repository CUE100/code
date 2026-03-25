pin = "1234"
balance = 1000
attempts = 0
user_pin = input("enter your pin! ")

while attempts < 5 and not user_pin == pin:
    user_pin = input("enter your pin! ")
    attempts = attempts + 1
    if attempts >= 5:
        print("locked !!!!!")

if pin == user_pin:
    while True:
        print("1) check balance")
        print("2)deposit money ")
        print("3)exit")
        ask = int(input("enter your prefrence : "))
        if ask == 1:
            print(f"your balance is {balance}")
        elif ask == 2:
            deposit_money = int(
                input("enter the amount you want to deposit : "))
            balance = balance + deposit_money
            print(f"now your total balance is {balance}")
        elif ask == 3:
            print("bye")
            break
