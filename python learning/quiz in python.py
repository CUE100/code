questions = (("What's color of water? "), ("How many planet in solar system? "),
             ("how is dakshwaligame ? "), ("what is name of our galaxy? "), ("why school is worst for learning? "))

options = (("A.Green ", "B.Blue", "C.Red", "D.Black"), ("A.6", "B.7", "C.8", "D.9"), ("A.Worst", "B.Neutral", "C.Good", "D.Best"), ("A.Milky way",
           "B.Alpha centuri", "C.Andrmeda galaxy", "D.Nether"), ("A.Because it doesnot teach skill", "B.Because its not valuable", "C.I oppose it", "D.IDK"))

correct_answers = ("B", "C", "A",
                   "A", "B")

users_guess = []

question_num = 0

for x in questions:
    for y in x:
        print(y, end="")
    print()

    for z in options:
        for i in z:
            print(i, end=" ")
        print()

        user_answers = input("Enter your answer (A , B, C ,D): ").upper()
        print("----------------------------")

        question_num = question_num + 1

        while user_answers not in ("A", "B", "C", "D"):
            print("Enter a valid response! ")
            user_answers = input("enter your answer (A , B, C ,D): ").upper()
            users_guess.append(user_answers)
        else:
            users_guess.append(user_answers)
            break
