shape_ask = input(
    "enter the shape you want to build(rectangle/square/triangle): ")
number_of_rows = int(input("enter the number of rows: "))
number_of_columns = int(input(
    "enter the number of columns(it wouldnt matter in case of square or triangle only rows will be taken): "))
symbol = input("enter the symbol/input for shape design: ")

if shape_ask == "rectangle":
    for row in range(number_of_rows):
        for column in range(number_of_columns):
            print(symbol, end="")
        print()
elif shape_ask == "square":
    for row in range(number_of_rows):
        for column in range(number_of_rows):
            print(symbol, end="")
        print()
elif shape_ask == "triangle":
    number_of_columns = 1
    for row in range(number_of_rows):
        for column in range(number_of_columns):
            print(symbol, end="")
        print()
        number_of_columns = number_of_columns + 1
