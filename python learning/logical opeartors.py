subscription = True
is_weekend = True
is_country = True
mobile_data = 1000

if ((subscription or is_weekend) and is_country) and mobile_data >= 400:
    print("you can watch movie at high quality !!")
elif ((subscription or is_weekend) and is_country) and mobile_data < 400:
    print("you can watch movie at low quality !!")
else:
    print("you cannot watch the movie :( ")


valid_badge = False
correct_passcode = False
is_admin = True
alarm_active = True

if valid_badge and correct_passcode and not alarm_active:
    print("you are allowed to enter")
elif is_admin and not alarm_active:
    print("admin access granted")
elif not alarm_active:
    print("access denied")
else:
    print("access denied")
