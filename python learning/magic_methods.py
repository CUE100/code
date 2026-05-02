# magic methods
class Book:

    def __init__(self, title, author, pages):
        self.title = title
        self.author = author
        self.pages = pages

    def __lt__(self, other):
        return self.pages < other.pages

    def __gt__(self, other):
        return self.pages > other.pages

    def __eq__(self, other):
        return self.author == other.author and self.title == other.title

    def __add__(self, other):
        return self.pages + other.pages

    def __contains__(self, keyword):
        return keyword in self.title or keyword in self.author

    def __getitem__(self, key):
        if key == "title":
            return self.title
        elif key == "author":
            return self.author


book1 = Book("ATOMIC HABITS", "JAMES CLEAR", 300)
book2 = Book("HYPER FOCUS", "CHRIS BAILEY", 200)
print(book1 == book2)
