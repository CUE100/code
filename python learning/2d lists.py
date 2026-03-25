cars = ["ferrari", "porsche911",  "BMW"]
guns = ["mp5", "revolver",  "bazooka"]
games = ["GTA6", "minecraft", "lokicraft", "dakshwaligame"]

boys_interest = [cars, guns, games]

print(boys_interest)
print(boys_interest[0][0])
for x in boys_interest:
    for y in x:
        print(y, end=" ")
    print()
