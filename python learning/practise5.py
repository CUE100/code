rows = int(input("enter the number of rows: "))
columns = int(input("enter the number of columns: "))
symbol = input("Enter the symbol for rectangle: ")

for row in range(rows):
    for column in range(columns):
        print(symbol, end="")
    print()
