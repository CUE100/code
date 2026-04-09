balance = 0
pin = 9192631770


def pin_check():
    global attempts
    max_attempts = 3
    attempts = 0
    while attempts < max_attempts:
        try:
            user_pin = int(input("Enter your pin: "))
        except ValueError:
            print("PLEASE ENTER A VALID PIN!")
            continue
        if user_pin == pin:
            print("-------------------------------------------------")
            print("WELCOME TO OUR APP!")
            return True
        attempts += 1
        remaining = max_attempts - attempts
        print(f"INCORRECT PIN! {remaining} attempts left")
    print("YOUR BANK ACCOUNT IS LOCKED!!")
    return False


def main_menu():
    print("-------------------------------------------------")
    print("What you want to do? ")
    print("1.Show bank balance.(Press 1) ")
    print("2.Deposit money in bank.(Press 2) ")
    print("3.Withdraw money from bank.(Press 3)")
    print("4.Quit this app.(Press 4) ")
    print()


def show():
    print("-------------------------------------------------")
    print(f"Your bank balance is : {balance:.2f}")


def deposit():
    global balance
    try:
        amount = float(
            input("Enter the amount you want to deposit (0 to cancel): "))
        if amount < 0:
            print("Please enter a number greater than 0.")
            return
        if amount == 0:
            return
        balance += amount
        show()
    except ValueError:
        print("Please enter a valid amount!")


def withdraw():
    global balance
    try:
        amount = float(
            input("Enter the amount you want to withdraw (0 to cancel): "))
        if amount < 0:
            print("Please enter a number greater than 0.")
            return
        if amount == 0:
            return
        if balance - amount < 0:
            print("WARNING: YOU ARE IN DEBT!")
        balance -= amount
        show()
    except ValueError:
        print("Please enter a valid amount!")


def exit():
    print("-------------------------------------------------------")
    print("Thanks for using our app! ")


# MAIN PROGRAMME
def main():
    if not pin_check():
        return
    while True:

        main_menu()
        try:
            choice = input("Enter a number to choose: ")
            choice = int(choice)
        except ValueError:
            print("Please enter a valid number ! ")
            continue
        if choice == 1:
            show()
        elif choice == 2:
            deposit()
        elif choice == 3:
            withdraw()
        elif choice == 4:
            exit()
            break
        elif choice not in [1, 2, 3, 4]:
            print("PLEASE ENTER A NUMBER BETWEEN 1 AND 4! ")


if __name__ == '__main__':
    main()
