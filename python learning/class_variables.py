class students:

    class_num = 10
    school = "DPS"
    num_students = 0

    def __init__(self, name, rollno):
        self.name = name
        self.rollno = rollno
        students.num_students = self.num_students + 1

    def name_display(self):
        print(f"Your name is {self.name}.")

    def full_details(self):
        print(f"Your name is {self.name}.")
        print(f"your class num is: {self.class_num}.")
        print(f"Your roll no is: {self.rollno}")


student_id = students("Batman", 10)
student_id2 = students("Robin", 12)
student_id.full_details()
print("-" * 40)
student_id2.full_details()
print("-" * 40)
print(f"There are {students.num_students} number of students.")
print("-" * 40)
print(f"Your school name is {student_id.school}")
