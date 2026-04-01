print("------------------WELCOME TO CUSTOM PC BUILDER----------------")


def custom_pc(cpu, ram="16", *peripherals, **order_details):
    print(f"CPU model = {cpu}")
    print("----------------------------------------")
    print(f"Ram = {ram}gb")
    print("----------------------------------------")
    print(f"Extra items for computer :")
    for peripheral in peripherals:
        print(f"{peripheral}", end=", ")
    print()
    print("----------------------------------------")
    print(f"Additional details :- ")
    for key, value in order_details.items():
        print(f"{key} = {value}", end=" , ")


custom_pc("m5 max", "24", "monitor",
          "mouse", "keyboard", color="red", cover="brown")
