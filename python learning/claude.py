import tkinter as tk
import random

WIDTH, HEIGHT = 640, 520
BG = "#0d0d16"
PADDLE_COLOR = "#50dcff"
BALL_COLOR = "#ffe650"
TEXT_COLOR = "#f0f0f0"

BRICK_ROWS = 6
BRICK_COLS = 10
BRICK_W = WIDTH // BRICK_COLS
BRICK_H = 22
BRICK_TOP = 60

ROW_COLORS = ["#000000", "#ff9f43", "#ffd450", "#50ff90", "#50c8ff", "#c56bff"]

POWERUP_COLORS = {
    "wide": "#50ff90",
    "slow": "#c8c8ff",
    "multi": "#ffd450",
}


class Breakout:
    def __init__(self, root):
        self.root = root
        self.root.title("Breakout")
        self.root.resizable(False, False)

        self.top = tk.Frame(root, bg="#15151f", height=44)
        self.top.pack(fill="x")
        self.score_label = tk.Label(self.top, text="Score: 0", fg=TEXT_COLOR, bg="#15151f",
                                     font=("Consolas", 15, "bold"))
        self.score_label.pack(side="left", padx=14, pady=8)
        self.lives_label = tk.Label(self.top, text="Lives: 3", fg="#ff6b6b", bg="#15151f",
                                     font=("Consolas", 14, "bold"))
        self.lives_label.pack(side="right", padx=14, pady=8)
        self.level_label = tk.Label(self.top, text="Level: 1", fg="#ffd450", bg="#15151f",
                                     font=("Consolas", 13, "bold"))
        self.level_label.pack(side="right", padx=14, pady=8)

        self.canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg=BG, highlightthickness=0)
        self.canvas.pack()

        self.canvas.bind("<Motion>", self.on_mouse)
        self.root.bind("<KeyPress>", self.on_key)
        self.root.bind("<Left>", lambda e: self.nudge(-1))
        self.root.bind("<Right>", lambda e: self.nudge(1))

        self.high_score = 0
        self.mouse_x = WIDTH // 2
        self.new_game()

    # ---------- setup ----------
    def new_game(self, keep_score=False):
        self.paddle_w = 90
        self.paddle_h = 14
        self.paddle_x = WIDTH / 2 - self.paddle_w / 2
        self.paddle_y = HEIGHT - 40

        if not keep_score:
            self.score = 0
            self.lives = 3
            self.level = 1

        self.balls = [self.make_ball()]
        self.bricks = self.make_bricks()
        self.powerups = []
        self.particles = []

        self.wide_timer = 0
        self.slow_timer = 0

        self.alive = True
        self.won_pause = False
        self.started = False

        self.score_label.config(text=f"Score: {self.score}")
        self.lives_label.config(text=f"Lives: {self.lives}")
        self.level_label.config(text=f"Level: {self.level}")

        self.render()
        self.loop()

    def make_ball(self):
        speed = 4.5 + (self.level - 1) * 0.4 if hasattr(self, "level") else 4.5
        ang = random.uniform(-0.6, 0.6)
        return {"x": WIDTH / 2, "y": self.paddle_y - 10, "dx": speed * math.sin(ang) if False else speed * 0.5,
                "dy": -speed, "r": 7, "stuck": True}

    def make_bricks(self):
        bricks = []
        rows = min(BRICK_ROWS + (self.level - 1), 9) if hasattr(self, "level") else BRICK_ROWS
        for row in range(rows):
            for col in range(BRICK_COLS):
                hp = 1 if row < rows - 2 else 2
                bricks.append({
                    "x": col * BRICK_W, "y": BRICK_TOP + row * BRICK_H,
                    "w": BRICK_W - 3, "h": BRICK_H - 3,
                    "color": ROW_COLORS[row % len(ROW_COLORS)],
                    "hp": hp, "alive": True
                })
        return bricks

    # ---------- input ----------
    def on_mouse(self, e):
        self.mouse_x = e.x
        if not self.started:
            self.render()

    def nudge(self, d):
        self.paddle_x += d * 25
        self.paddle_x = max(0, min(WIDTH - self.paddle_w, self.paddle_x))

    def on_key(self, e):
        k = e.keysym.lower()
        if k == "space":
            self.launch()
        if k == "r" and not self.alive:
            self.new_game()
        if k == "escape":
            self.root.destroy()

    def launch(self):
        for b in self.balls:
            if b.get("stuck"):
                b["stuck"] = False
        self.started = True

    # ---------- loop ----------
    def loop(self):
        if not self.alive:
            return

        target = max(self.paddle_w / 2, min(WIDTH - self.paddle_w / 2, self.mouse_x))
        self.paddle_x += (target - self.paddle_w / 2 - self.paddle_x) * 0.35

        slow = 0.55 if self.slow_timer > 0 else 1.0
        if self.slow_timer > 0:
            self.slow_timer -= 1
        if self.wide_timer > 0:
            self.wide_timer -= 1
            if self.wide_timer == 0:
                self.paddle_w = 90

        for b in self.balls:
            if b["stuck"]:
                b["x"] = self.paddle_x + self.paddle_w / 2
                continue
            b["x"] += b["dx"] * slow
            b["y"] += b["dy"] * slow

            if b["x"] - b["r"] < 0:
                b["x"] = b["r"]; b["dx"] *= -1
            if b["x"] + b["r"] > WIDTH:
                b["x"] = WIDTH - b["r"]; b["dx"] *= -1
            if b["y"] - b["r"] < BRICK_TOP - 20 + 20:
                pass
            if b["y"] - b["r"] < 0:
                b["y"] = b["r"]; b["dy"] *= -1

            # paddle collision
            if (self.paddle_y < b["y"] + b["r"] < self.paddle_y + self.paddle_h and
                    self.paddle_x - 4 < b["x"] < self.paddle_x + self.paddle_w + 4 and b["dy"] > 0):
                hit_pos = (b["x"] - (self.paddle_x + self.paddle_w / 2)) / (self.paddle_w / 2)
                b["dx"] = hit_pos * 5.5
                b["dy"] = -abs(b["dy"])
                b["y"] = self.paddle_y - b["r"]

            # brick collision
            for br in self.bricks:
                if not br["alive"]:
                    continue
                if (br["x"] < b["x"] < br["x"] + br["w"] and
                        br["y"] < b["y"] < br["y"] + br["h"]):
                    br["hp"] -= 1
                    if br["hp"] <= 0:
                        br["alive"] = False
                        self.score += 10
                        self.add_particles(br["x"] + br["w"] / 2, br["y"] + br["h"] / 2, br["color"])
                        if random.random() < 0.18:
                            self.spawn_powerup(br["x"] + br["w"] / 2, br["y"] + br["h"] / 2)
                    b["dy"] *= -1
                    break

        # remove fallen balls
        self.balls = [b for b in self.balls if b["y"] - b["r"] < HEIGHT]
        if not self.balls:
            self.lives -= 1
            self.lives_label.config(text=f"Lives: {self.lives}")
            if self.lives <= 0:
                self.game_over()
                return
            self.balls = [self.make_ball()]
            self.started = False

        # powerups fall
        collected = []
        for p in self.powerups:
            p["y"] += 3
            if p["y"] > HEIGHT:
                collected.append(p)
                continue
            if (self.paddle_x < p["x"] < self.paddle_x + self.paddle_w and
                    self.paddle_y < p["y"] < self.paddle_y + self.paddle_h):
                collected.append(p)
                self.apply_powerup(p["type"])
                self.add_particles(p["x"], p["y"], POWERUP_COLORS[p["type"]])
        for p in collected:
            self.powerups.remove(p)

        for pt in self.particles:
            pt[0] += pt[2]; pt[1] += pt[3]; pt[4] -= 1
        self.particles = [pt for pt in self.particles if pt[4] > 0]

        self.score_label.config(text=f"Score: {self.score}")

        if all(not br["alive"] for br in self.bricks):
            self.level += 1
            self.level_label.config(text=f"Level: {self.level}")
            self.new_game(keep_score=True)
            return

        self.render()
        self.root.after(16, self.loop)

    def spawn_powerup(self, x, y):
        ptype = random.choice(list(POWERUP_COLORS.keys()))
        self.powerups.append({"x": x, "y": y, "type": ptype})

    def apply_powerup(self, ptype):
        if ptype == "wide":
            self.paddle_w = 150
            self.wide_timer = 500
        elif ptype == "slow":
            self.slow_timer = 300
        elif ptype == "multi":
            if self.balls:
                b = self.balls[0]
                for ang in (-0.4, 0.4):
                    import math as m
                    nb = dict(b)
                    speed = m.hypot(b["dx"], b["dy"]) or 5
                    base_ang = m.atan2(b["dy"], b["dx"])
                    nb["dx"] = m.cos(base_ang + ang) * speed
                    nb["dy"] = m.sin(base_ang + ang) * speed
                    nb["stuck"] = False
                    self.balls.append(nb)

    def add_particles(self, x, y, color, n=10):
        for _ in range(n):
            ang = random.uniform(0, 6.28)
            spd = random.uniform(1, 3.5)
            self.particles.append([x, y, __import__("math").cos(ang) * spd,
                                    __import__("math").sin(ang) * spd, 18, color])

    # ---------- render ----------
    def render(self):
        self.canvas.delete("all")

        for br in self.bricks:
            if not br["alive"]:
                continue
            alpha_color = br["color"] if br["hp"] == 1 else "#ffffff"
            self.canvas.create_rectangle(br["x"], br["y"], br["x"] + br["w"], br["y"] + br["h"],
                                          fill=br["color"], outline=alpha_color)

        for pt in self.particles:
            a = pt[4] / 18
            r = 3 * a + 1
            self.canvas.create_oval(pt[0] - r, pt[1] - r, pt[0] + r, pt[1] + r, fill=pt[5], outline="")

        for p in self.powerups:
            c = POWERUP_COLORS[p["type"]]
            self.canvas.create_oval(p["x"] - 9, p["y"] - 9, p["x"] + 9, p["y"] + 9, fill=c, outline="white")
            self.canvas.create_text(p["x"], p["y"], text=p["type"][0].upper(), fill="black",
                                     font=("Consolas", 9, "bold"))

        self.canvas.create_rectangle(self.paddle_x, self.paddle_y, self.paddle_x + self.paddle_w,
                                      self.paddle_y + self.paddle_h, fill=PADDLE_COLOR, outline="")

        for b in self.balls:
            self.canvas.create_oval(b["x"] - b["r"], b["y"] - b["r"], b["x"] + b["r"], b["y"] + b["r"],
                                     fill=BALL_COLOR, outline="")

        if not self.started:
            self.canvas.create_text(WIDTH // 2, HEIGHT - 70, text="Move mouse, press SPACE to launch",
                                     fill="#aaaaaa", font=("Consolas", 13))

    def game_over(self):
        self.alive = False
        self.high_score = max(self.high_score, self.score)
        self.canvas.create_rectangle(0, 0, WIDTH, HEIGHT, fill="black", stipple="gray50")
        self.canvas.create_text(WIDTH // 2, HEIGHT // 2 - 30, text="GAME OVER",
                                 fill="#ff5050", font=("Consolas", 32, "bold"))
        self.canvas.create_text(WIDTH // 2, HEIGHT // 2 + 10,
                                 text=f"Score: {self.score}   Level: {self.level}   High: {self.high_score}",
                                 fill=TEXT_COLOR, font=("Consolas", 14, "bold"))
        self.canvas.create_text(WIDTH // 2, HEIGHT // 2 + 42,
                                 text="Press R to Restart or ESC to Quit",
                                 fill="#aaaaaa", font=("Consolas", 12))


import math

if __name__ == "__main__":
    root = tk.Tk()
    g = Breakout(root)
    root.mainloop()