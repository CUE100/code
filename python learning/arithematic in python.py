import math

radius = int(input("enter the value of radius of circle : "))
circumfrence = 2 * math.pi * radius
print(f"the circumfrence of circle is : {circumfrence}")
area = math.pi * pow(radius, 2)
print(f"the area of circle is : {area}")

perpendicular = float(input("enter the value of perpendicular : "))
base = float(input("enter the value of base :"))
hypotunese = math.sqrt(pow(perpendicular, 2) + pow(base, 2))
print(f"the value of hypotunese is : {hypotunese}")
