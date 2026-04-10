import random
symbols = ["💀", "🫥", "🦇", "💻", "🎶"]
balance = 1000


def get_input(balance):
    while True:
        try:
            print("----------------------------------------------------------")
            bet_amount = float(input("Enter a amount to bet(0 to leave): "))
            if bet_amount < 0:
                print("Please enter a number greater than zero!")
                continue
            if bet_amount == 0:
                print("Thanks for betting with us.")
                break
            if bet_amount > balance:
                print("Please enter a bet less than your fundings!")
                continue
        except ValueError:
            print("Please enter a valid amount! ")
            continue
        return bet_amount


def random_symbols():
    random_symbol1 = random.choice(symbols)
    random_symbol2 = random.choice(symbols)
    random_symbol3 = random.choice(symbols)

    return random_symbol1, random_symbol2, random_symbol3


def actual_game(random_symbol1, random_symbol2, random_symbol3, bet_amount, balance):
    print("----------------------------------------------------------")
    print(f"{random_symbol1} | {random_symbol2} | {random_symbol3}")
    print("----------------------------------------------------------")
    if random_symbol1 == "💀" == random_symbol2 == random_symbol3:
        bet_amount = bet_amount * 5
        print("You won a jackpot !(5X your money)")
        balance = balance + bet_amount
        print("----------------------------------------------------------")
        print(f"Now your total is {bet_amount} with {balance} balance ")
    elif random_symbol1 == "🫥" == random_symbol2 == random_symbol3:
        bet_amount = bet_amount * 10
        print("You won a jackpot !(10X your money)")
        balance = balance + bet_amount
        print("----------------------------------------------------------")
        print(f"Now your total is {bet_amount} with {balance} balance ")
    elif random_symbol1 == "🦇" == random_symbol2 == random_symbol3:
        bet_amount = bet_amount * 20
        print("You won a jackpot !(20X your money)")
        balance = balance + bet_amount
        print("----------------------------------------------------------")
        print(f"Now your total is {bet_amount} with {balance} balance ")
    elif random_symbol1 == "💻" == random_symbol2 == random_symbol3:
        bet_amount = bet_amount * 30
        print("You won a jackpot !(30X your money)")
        balance = balance + bet_amount
        print("----------------------------------------------------------")
        print(f"Now your total is {bet_amount} with {balance} balance ")
    elif random_symbol1 == "🎶" == random_symbol2 == random_symbol3:
        bet_amount = bet_amount * 50
        print("You won a jackpot !(50X your money)")
        balance = balance + bet_amount
        print("----------------------------------------------------------")
        print(f"Now your total is {bet_amount} with {balance} balance ")
    elif bet_amount == 0:
        print("Thanks for betting with us !")
        return
    else:
        print("Better luck this time! ")
        balance = balance - bet_amount
        print(f"Your total is {balance}")

    return balance


def main():

    balance = 1000
    while True:

        bet_amount = get_input(balance)
        random_symbol1, random_symbol2, random_symbol3 = random_symbols()
        balance = actual_game(random_symbol1, random_symbol2,
                              random_symbol3, bet_amount, balance)
        if balance < 1:
            print("You are broke ! :( ")
            break


if __name__ == '__main__':
    main()
