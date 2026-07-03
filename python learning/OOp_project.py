import math

# Global Physics Constants
G = 6.67430e-11 
TIME_STEP = 3600  # 1 hour per step

class CelestialBody:
    def __init__(self, name, mass, x, y, vx, vy):
        self.name = name
        self.mass = mass
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy

    def calculate_force(self, other):
        # Calculate distance using Pythagorean theorem
        dx = other.x - self.x
        dy = other.y - self.y
        dist = math.sqrt(dx**2 + dy**2)
        
        # Newton's Law: F = G * (m1 * m2) / r^2
        force = (G * self.mass * other.mass) / (dist**2)
        
        # Return force vector components
        fx = force * (dx / dist)
        fy = force * (dy / dist)
        return fx, fy

class PhysicsEngine:
    def __init__(self):
        self.bodies = []

    def add_body(self, body):
        self.bodies.append(body)

    def update(self):
        for body in self.bodies:
            for other in self.bodies:
                if body != other:
                    fx, fy = body.calculate_force(other)
                    # Physics: a = F / m
                    body.vx += (fx / body.mass) * TIME_STEP
                    body.vy += (fy / body.mass) * TIME_STEP
            
            # Update position
            body.x += body.vx * TIME_STEP
            body.y += body.vy * TIME_STEP

# --- Setup ---
engine = PhysicsEngine()
sun = CelestialBody("Sun", 1.989e30, 0, 0, 0, 0)
earth = CelestialBody("Earth", 5.972e24, 1.496e11, 0, 0, 29780)

engine.add_body(sun)
engine.add_body(earth)

# --- Simulation Loop ---
for hour in range(24):
    engine.update()
    print(f"Hour {hour+1}: Earth X={earth.x:.2e}, Y={earth.y:.2e}")
