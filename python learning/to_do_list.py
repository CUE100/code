work_today = []
is_running = True

print("-------------------------------------------------")
print("------------------TO DO LIST APP-----------------")

while is_running:
    print("-------------------------------------------------")
    print("What you want to do? ")
    print("1.Add a task to today's to-do list.(Press 1) ")
    print("2.View todays to-do list.(Press 2) ")
    print("3.Name the task you want to remove.(Press 3)")
    print("4.Quit this app.(Press 4) ")
    print()
    try:
        user_number = input("Enter your choice: ")
        user_number = int(user_number)
    except ValueError:
        print("Please enter a valid choice!")
        continue

    if user_number == 1:
        print("-------------------------------------------------")
        while True:
            user_add = input(
                "Enter an work you want to add(press q to main menu): ").upper()
            work_today.append(user_add)
            if user_add == "Q":
                work_today.remove("Q")
                break

    elif user_number == 2:
        n = 0
        print("------------------YOUR TO DO--------------------")
        for work in work_today:
            n += 1
            print(f"{n}.{work}")

    elif user_number == 3:

        print("-------------------------------------------------")
        while True:
            m = 0
            remove_task = input(
                "Name the task you want to remove(e to end and return to main menu): ").upper()
            if remove_task == "E":
                break
            if remove_task not in work_today:
                print("Please enter a task that is already in your today's work!")

            else:
                print("-------------------------------------------------------")
                work_today.remove(remove_task)
                print("------------------------UPDATED LIST-------------------")
                for work in work_today:
                    m += 1
                    print(f"{m}.{work}")

    elif user_number == 4:
        print("-------------------------------------------------")
        print("Thanks for using this app! ")
        break
