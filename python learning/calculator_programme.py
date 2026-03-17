first_number = float(input("enter the first number : "))
second_number = float(input("enter the second number : "))
operation = input(
    "enter the operation you want to perform ( + , - , * , / ) : ")


a = first_number + second_number
b = first_number - second_number
c = first_number * second_number
d = first_number / second_number


if operation == "+":
    print(f"the sum of {first_number} and {second_number} is : {a}")
elif operation == "-":
    print(f"the difference of {first_number} and {second_number} is : {b}")
elif operation == "*":
    print(f"the product of {first_number} and {second_number} is : {c}")
elif operation == "/":
    print(f"the quotient of {first_number} and {second_number} is : {d}")
else:
    print("invalid operation !")
