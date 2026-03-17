principal = (round(float(input("enter the principal amount : ")), 2))

while principal < 0:
    if principal <= 0:
        print("principal  be negative or zero")
    print("please enter valid principal value !")
    principal = round(float(input("enter the principal amount : ")), 2)


rate = ((round(float(input("enter the interest rate : ")), 2) / 100))
while rate < 0:
    if rate <= 0:
        print("rate of interest cant be negative or zero")
    print("please enter valid rate")
    rate = round(float(input("enter the interest rate : ")), 2)


time = (int(input("enter the time periods elapsed : ")))
while time < 0:
    if time < 0:
        print("time cant be negative or zero")
    print("please enter valid time !")
    time = int(input("enter the time periods elapsed : "))


n = (int(input("enter Number of times interest is compounded per year (monthly = 12, quarterly = 4): ")))
while n < 0:
    if n <= 0:
        print("n (number of times) cant be negative or zero")
    print("please enter valid n!")
    n = int(input(
        "enter Number of times interest is compounded per year (monthly = 12, quarterly = 4): "))


amount = principal * (1 + rate/n) ** time
print(f"your final amount is ${amount:,.2f}")
