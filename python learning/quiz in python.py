questions = (("What's color of water? "), ("How many planet in solar system? "),
             ("how is dakshwaligame ? "), ("what is name of our galaxy? "), ("why school is worst for learning? "))

options = (("A.Green ", "B.Blue", "C.Red", "D.Black"), ("A.6", "B.7", "C.8", "D.9"), ("A.Worst", "B.Neutral", "C.Good", "D.Best"), ("A.Milky way",
           "B.Alpha centuri", "C.Andrmeda galaxy", "D.Nether"), ("A.Because it doesnot teach skill", "B.Because its not valuable", "C.I oppose it", "D.IDK"))

correct_answers = ("B", "C", "A",
                   "A", "B")

users_guess = []

question_num = 0

for x in questions:
    print(x)
    print()
    users_answer = input("enter your answer (A,B,C,D)")
