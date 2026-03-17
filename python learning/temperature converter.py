temperature = float(input("enter the value of temperature : "))
current_unit = input("enter the current unit of temperature (c/f/k) : ")
unit_to_convert_into = input(
    "enter the unit you want to convert into ? (c/f/k) : ")


a = (temperature * 9/5) + 32  # celsius to fahrenheit
b = temperature + 273.15  # celsius to kelvin
c = (temperature - 32) * 5/9  # fahrenheit to c
d = (temperature - 32) * 5/9 + 273.15  # fahrenheit to kelvin
e = temperature - 273.15  # kelvin to celsius
f = (temperature - 273.15) * 9/5 + 32  # kelvin to fahrenheit


if current_unit == "c" and unit_to_convert_into == "f":
    print(f"{temperature} degree celsius is equal to {a} degree fahrenheit")
elif current_unit == "c" and unit_to_convert_into == "k":
    print(f"{temperature} degree celsius is equal to {b} degree kelvin")
elif current_unit == "f" and unit_to_convert_into == "c":
    print(f"{temperature} degree fahrenheit is equal to {c} degree celsius")
elif current_unit == "f" and unit_to_convert_into == "k":
    print(f"{temperature} degree fahrenheit is equal to {d} degree kelvin")
elif current_unit == "k" and unit_to_convert_into == "c":
    print(f"{temperature} degree kelvin is equal to {e} degree celsius")
elif current_unit == "k" and unit_to_convert_into == "f":
    print(f"{temperature} degree kelvin is equal to {f} degree fahrenheit")
else:
    print("invalid unit of temperature or invalid unit of temperature to convert into")
