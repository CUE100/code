rows = int(input("enter the number of rows: "))
columns = rows - 1
symbol = input("Enter the symbol for triangle: ")

for row in range(rows):
    for column in range(columns):
        print(symbol, end="")
    print()
    columns = columns - 1
