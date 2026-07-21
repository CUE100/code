import tkinter as tk
import random

# ---- Config ----
CELL = 20
COLS, ROWS = 30, 22
WIDTH, HEIGHT = COLS * CELL, ROWS * CELL
SPEED_START = 120  # ms per tick (lower = faster)

BG = "#121218"
GRID = "#1c1c24"
HEAD_COLOR = "#50dc78"
BODY_COLOR = "#28aa5a"
FOOD_COLOR = "#e64646"
TEXT_COLOR = "#f0f0f0"


class SnakeGame:
    def __init__(self, root):
        self.root = root
        self.root.title("Snake")
        self.root.resizable(False, False)

        self.top_bar = tk.Frame(root, bg="#181820", height=50)
        self.top_bar.pack(fill="x")
        self.score_label = tk.Label(self.top_bar, text="Score: 0", fg=TEXT_COLOR,
                                     bg="#181820", font=("Consolas", 16, "bold"))
        self.score_label.pack(side="left", padx=15, pady=10)
        self.high_label = tk.Label(self.top_bar, text="High: 0", fg="#aaaaaa",
                                    bg="#181820", font=("Consolas", 12))
        self.high_label.pack(side="right", padx=15, pady=10)

        self.canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg=BG, highlightthickness=0)
        self.canvas.pack()

        self.high_score = 0
        self.root.bind("<Key>", self.on_key)

        self.new_game()

    def new_game(self):
        self.snake = [(COLS // 2, ROWS // 2), (COLS // 2 - 1, ROWS // 2), (COLS // 2 - 2, ROWS // 2)]
        self.direction = (1, 0)
        self.next_direction = (1, 0)
        self.food = self.random_food()
        self.score = 0
        self.speed = SPEED_START
        self.alive = True
        self.score_label.config(text="Score: 0")
        self.draw_grid_lines()
        self.tick()

    def random_food(self):
        while True:
            pos = (random.randint(0, COLS - 1), random.randint(0, ROWS - 1))
            if pos not in self.snake:
                return pos

    def draw_grid_lines(self):
        self.canvas.delete("gridline")
        for x in range(0, WIDTH, CELL):
            self.canvas.create_line(x, 0, x, HEIGHT, fill=GRID, tags="gridline")
        for y in range(0, HEIGHT, CELL):
            self.canvas.create_line(0, y, WIDTH, y, fill=GRID, tags="gridline")

    def on_key(self, event):
        k = event.keysym.lower()
        if self.alive:
            if k in ("up", "w") and self.direction != (0, 1):
                self.next_direction = (0, -1)
            elif k in ("down", "s") and self.direction != (0, -1):
                self.next_direction = (0, 1)
            elif k in ("left", "a") and self.direction != (1, 0):
                self.next_direction = (-1, 0)
            elif k in ("right", "d") and self.direction != (-1, 0):
                self.next_direction = (1, 0)
        else:
            if k == "r":
                self.new_game()
        if k == "escape":
            self.root.destroy()

    def tick(self):
        if not self.alive:
            return

        self.direction = self.next_direction
        hx, hy = self.snake[0]
        new_head = (hx + self.direction[0], hy + self.direction[1])

        if (new_head[0] < 0 or new_head[0] >= COLS or
                new_head[1] < 0 or new_head[1] >= ROWS or
                new_head in self.snake):
            self.game_over()
            return

        self.snake.insert(0, new_head)
        if new_head == self.food:
            self.score += 1
            self.score_label.config(text=f"Score: {self.score}")
            self.food = self.random_food()
            self.speed = max(50, SPEED_START - self.score * 4)
        else:
            self.snake.pop()

        self.render()
        self.root.after(self.speed, self.tick)

    def render(self):
        self.canvas.delete("cell")
        # food
        self.draw_cell(self.food, FOOD_COLOR)
        # snake
        for i, seg in enumerate(self.snake):
            self.draw_cell(seg, HEAD_COLOR if i == 0 else BODY_COLOR)

    def draw_cell(self, pos, color, inset=2):
        x, y = pos
        x0, y0 = x * CELL + inset, y * CELL + inset
        x1, y1 = x0 + CELL - inset * 2, y0 + CELL - inset * 2
        self.canvas.create_rectangle(x0, y0, x1, y1, fill=color, outline="", tags="cell")

    def game_over(self):
        self.alive = False
        self.high_score = max(self.high_score, self.score)
        self.high_label.config(text=f"High: {self.high_score}")

        self.canvas.create_rectangle(0, 0, WIDTH, HEIGHT, fill="black", stipple="gray50", tags="cell")
        self.canvas.create_text(WIDTH // 2, HEIGHT // 2 - 30, text="GAME OVER",
                                 fill="#ff5050", font=("Consolas", 32, "bold"), tags="cell")
        self.canvas.create_text(WIDTH // 2, HEIGHT // 2 + 10,
                                 text=f"Score: {self.score}   High Score: {self.high_score}",
                                 fill=TEXT_COLOR, font=("Consolas", 16, "bold"), tags="cell")
        self.canvas.create_text(WIDTH // 2, HEIGHT // 2 + 45,
                                 text="Press R to Restart or ESC to Quit",
                                 fill="#aaaaaa", font=("Consolas", 12), tags="cell")


if __name__ == "__main__":
    root = tk.Tk()
    game = SnakeGame(root)
    root.mainloop()