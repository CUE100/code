def credit_card_num(bank, area, main_num):
    return f"{bank}-{area}-{main_num}"


my_credit_num = credit_card_num(bank="001", area="136027", main_num="123456")
print(my_credit_num)
