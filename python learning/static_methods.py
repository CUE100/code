class Employees:
    def __init__(self, name, position):
        self.name = name
        self.position = position

    def get_info(self):
        return f"{self.name}'s postion is {self.position}"

    @staticmethod
    def is_friend(name):
        valid_friends = ["abc", "xyz", "batman"]
        return name in valid_friends


employee = Employees("Spongebob", "CEO")
print(Employees.is_friend("batman"))
print(employee.get_info())
