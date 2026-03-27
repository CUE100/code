questions = (("What's color of water? "), ("How many planet in solar system? "),
             ("How many elements in our periodic table?  "), ("What is name of our galaxy? "), ("How many bones do humans have? "))

options = (("A.Green ", "B.Blue", "C.Red", "D.Black"), ("A.6", "B.7", "C.8", "D.9"), ("A.118", "B.117", "C.112", "D.119"), ("A.Milky way",
           "B.Alpha centuri", "C.Andrmeda galaxy", "D.Nether"), ("A.206", "B.306", "C.406", "D.IDK"))

correct_answers = ("B", "C", "A",
                   "A", "A")

users_guess = []
score = 0
question_num = 0


for question in questions:
    print("--------------------------")
    print("--------------------------")
    print(question)

    for option in options[question_num]:
        print(option)

    print("--------------------------")

    users_answer = input("enter your answer (A,B,C,D): ").upper()
    users_guess.append(users_answer)
    print("")

    if users_answer == correct_answers[question_num]:
        print("Correct !!")
        score += 1
    else:
        print("Incorrect !! ")
        print(f"The correct answer is {correct_answers[question_num]}")
    question_num = question_num + 1


print("--------------------------")
print("        RESULT            ")
print("--------------------------")


print("your guesses: ", end=" ")
for guess in users_guess:
    print(guess, end=" ")
print()
print("Correct answer: ", end=" ")
for answer in correct_answers:
    print(answer, end=" ")

score = score / question_num * 100
print()
print(f"Your final score is {score} % .")
print()
