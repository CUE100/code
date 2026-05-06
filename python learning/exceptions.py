try:
    number = int(input("Enter a number: "))
    print(number + "hello")
except ZeroDivisionError:
    print("You cant divide a number by zero!(404) ")
except ValueError:
    print("Please enter an valid number! (404) ")
except TypeError:
    print("There's an type error ! (404)")
except Exception:
    print("Something went wrong...")
