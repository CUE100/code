text_data = "Hello if you are seeing this message then remember:" \
    "1 NPC escaped, 8 billion remaining"

file_path = "outputs.txt"

with open(file_path, "w") as file:
    file.write(text_data)
    print("SUCCESSFULY DONE ! ")
