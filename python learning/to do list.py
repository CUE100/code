work_today = []
is_running = True

while is_running:
    print("What you want to do? ")
    print("1.Add a task to today's to-do list.(Press 1) ")
    print("2.View todays to-do list.(Press 2) ")
    print("3.Quit this app.(Press 3) ")
    try:
        user_number = input("Enter your choice: ")
        user_number = int(user_number)
    except ValueError:
        print("Please enter a valid choice!")
        continue
    if user_number == 1:
        user_add = input
