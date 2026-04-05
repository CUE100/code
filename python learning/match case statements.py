def is_weekend(day):
    match day:
        case 6 | 7:
            return "Its a weekend!"
        case 1 | 2 | 3 | 4 | 5:
            return "Its not a weekend"
        case _:
            return "Its not even a day! "


print(is_weekend(3))
