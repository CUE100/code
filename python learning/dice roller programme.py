import random

dice_roll_min = 1
dice_roll_max = 6
dice_faces = {
    1: """
     ------- 
    |       |
    |   o   |
    |       |
     ------- 
    """,

    2: """
     ------- 
    | o     |
    |       |
    |     o |
     ------- 
    """,

    3: """
     ------- 
    | o     |
    |   o   |
    |     o |
     ------- 
    """,

    4: """
     ------- 
    | o   o |
    |       |
    | o   o |
     ------- 
    """,

    5: """
     ------- 
    | o   o |
    |   o   |
    | o   o |
     ------- 
    """,

    6: """
     ------- 
    | o   o |
    | o   o |
    | o   o |
     ------- 
    """
}


while True:
    total_sum = 0
    try:
        num_dice = int(
            input("Enter the number of dices you want to roll(max 10 ; 0 to quit): "))
    except ValueError:
        print("⚠️ Please enter a valid number. ")
        continue
    if num_dice == 0:
        break
    if num_dice < 1 or num_dice > 10:
        print("⚠️ please enter a number between 1 and 10: ")
        continue

    for dice in range(num_dice):
        random_dice = random.randint(dice_roll_min, dice_roll_max)
        print(dice_faces[random_dice], end="")
        total_sum += random_dice
    print(f"Your total is: {total_sum}")
