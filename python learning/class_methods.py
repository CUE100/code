class Students:
    total_gpa = 0
    count = 0

    def __init__(self, name, gpa):
        self.name = name
        self.gpa = gpa
        Students.count += 1
        Students.total_gpa += gpa

    def get_infpo(self):
        print(f"The name of student is {self.name} with {self.gpa} gpa.")

    @classmethod
    def avg_gpa(cls):
        return f"Average gpa of student is {Students.total_gpa / Students.count:.2f}"


a = Students("A", 9)
b = Students("B", 3.8)
c = Students("C", 3.8)

Students.get_infpo(a)
