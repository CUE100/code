import os
file_path = "/Users/rohinboora/code/python learning/practise2.py"

if os.path.exists(file_path):
    print(f"{file_path} DETECTED !")

    if os.path.isfile(file_path):
        print("This is a file.")

    elif os.path.isdir(file_path):
        print("This is a directory.")
else:
    print(f"{file_path} NOT DETECTED !")
