weight = int(input("Enter the weight (in kg/lb/gram): "))
current_unit = input("enter the current unit of weight (kg/lb/gram): ")
unit_to_convert_into = input("enter the unit of weight (kg/lb/gram): ")


a = weight * 0.45359237  # pound to kilogram
b = weight * 1000  # kilogram to gram
c = weight * 2.20462  # kilogram to pound
d = weight * 0.001  # gram to kilogram
e = weight * 0.00220462  # gram to pound
f = weight * 453.59237  # pound to gram


if current_unit == "kg" and unit_to_convert_into == "pound":
    print(f" {weight} kg is equal to {c} pound")
elif current_unit == "kg" and unit_to_convert_into == "gram":
    print(f" {weight} kg is equal to {b} gram")
elif current_unit == "pound" and unit_to_convert_into == "kg":
    print(f"{weight} pound is equal to {a} kg")
elif current_unit == "pound" and unit_to_convert_into == "gram":
    print(f"{weight} pound is equal to {f} gram ")
elif current_unit == "gram" and unit_to_convert_into == "kg":
    print(f"{weight} gram is equal to {d} kg")
elif current_unit == "gram" and unit_to_convert_into == "pound":
    print(f"{weight} gram is equal to {e} pound")
else:
    print("invalid unit of weight or invalid unit to convert into")
