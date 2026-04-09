work_today = ["STUDY", "CAR WASH", "WATCH ANIME"]

print("-------------------------------------------------")
print("------------------TO DO LIST APP-----------------")


def to_do():
    print("-------------------------------------------------")
    print("What you want to do? ")
    print("1.Add a task to today's to-do list.(Press 1) ")
    print("2.View todays to-do list.(Press 2) ")
    print("3.Name the task you want to remove.(Press 3)")
    print("4.Quit this app.(Press 4) ")
    print()


def add_task():
    print("-------------------------------------------------")
    while True:
        user_add = input(
            "Enter an work you want to add(press q to main menu): ").upper()
        if user_add == "Q":
            break
        work_today.append(user_add)


def show_task():
    n = 0
    print("------------------YOUR TO DO--------------------")
    for work in work_today:
        n += 1
        print(f"{n}.{work}")


def remove_task_function():
    print("-------------------------------------------------")
    while True:

        remove_task = input(
            "Name the task you want to remove(e to end and return to main menu): ").upper()
        if remove_task == "E":
            break
        if remove_task not in work_today:
            print("Please enter a task that is already in your today's work!")
        else:
            work_today.remove(remove_task)
            show_task()


def exit_function():
    print("-------------------------------------------------")
    print("Thanks for using this app! ")


def running_full():
    is_running = True
    while is_running:
        to_do()
        try:
            user_number = input("Enter your choice: ")
            user_number = int(user_number)
        except ValueError:
            print("Please enter a valid choice!")
        if user_number == 1:
            add_task()
        elif user_number == 2:
            show_task()
        elif user_number == 3:
            remove_task_function()
        elif user_number == 4:
            exit_function()
            is_running = False


def running_read_only():
    is_running = True
    while is_running:
        print("\n--- GUEST MENU ---")
        print("1. View today's to-do list")
        print("2. Quit app")

        try:
            choice = int(input("Enter choice: "))
        except ValueError:
            continue

        if choice == 1:
            show_task()
        elif choice == 2:
            exit_function()
            is_running = False


if __name__ == "__main__":
    running_full()
