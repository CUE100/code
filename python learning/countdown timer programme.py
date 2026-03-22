import time

a = int(input("enter the countdown seconds : "))
for x in reversed(range(1, a)):
    time.sleep(1)
    seconds = x % 60
    minutes = int(x / 60) % 60
    hours = int(((x / 60)/60) % 24)
    days = x // (24 * 60 * 60)
    print(f"{days:02}:{hours:02}:{minutes:02}:{seconds:02}")

print("times up !!!")
