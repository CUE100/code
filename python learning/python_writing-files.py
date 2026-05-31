superheros = ["batman", "superman", "robin"]

file_path = "/Users/rohinboora/code/python learning/output.txt"

with open(file_path, "a") as file:
    for superhero in superheros:
        file.write("\n" + superhero)
        print("SUCCESSFULY DONE ! ")
