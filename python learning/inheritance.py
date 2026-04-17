class Hero:

    def __init__(self, name, age):
        self.name = name
        self.age = age
        self.villan_name = "Slade"

    def villans(self):
        print(f"{self.name} is beating villans.")

    def rest(self):
        print(f"{self.name} is resting.")


class villans(Hero):

    def beatten(self):
        print(f"{self.villan_name} has been beaten by {self.name}")


class Batman(villans):
    def like(self, adjective):
        print(f"I like batman as he is {adjective}")


class Robin(villans):
    def quality(self, quality):
        print(f"Robin has {quality}")


class Superman(villans):
    def worst(self):
        print("I hate superman as he got beatten by batman.")


bruce = Batman("Bruce wayne", 35)
dick = Robin("dick", 24)
clark = Superman("Clark Kent", 50)

bruce.beatten()
print("-" * 10)
dick.rest()
print("-" * 10)
clark.beatten()
print("-" * 10)
bruce.like("coooooooll")
print("-" * 10)
dick.quality("strength")
print("-" * 10)
clark.worst()
