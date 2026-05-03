def add_sprinkles(func):
    def wrapper(*args, **kwargs):
        print("You add sprinkles.")
        func(*args, **kwargs)
    return wrapper


@add_sprinkles
def get_icecream(flavor):
    print(f"HERE IS YOUR ICECREAM OF {flavor}! ")


get_icecream("vanilla")
