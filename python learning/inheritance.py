class Hero:

    def __init__(self, name, age):
        self.name = name
        self.age = age

    def villans(self):
        print(f"{self.name} is beating villans.")

    def rest(self):
        print(f"{self.name} is resting.")


class Batman(Hero):
    def like(self, adjective):
        print(f"I like batman as he is {adjective}")


class Robin(Hero):
    def quality(self, quality):
        print(f"Robin has {quality}")


class Superman(Hero):
    def worst(self):
        print("I hate superman as he got beatten by batman.")


bruce = Batman("Bruce wayne", 35)
dick = Robin("dick", 24)
clark = Superman("Clark Kent", 50)

bruce.villans()
print("-" * 10)
dick.rest()
print("-" * 10)
clark.villans()
print("-" * 10)
bruce.like("coooooooll")
print("-" * 10)
dick.quality("strength")
print("-" * 10)
clark.worst()
