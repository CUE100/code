#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

    // Define a simple structure for our floating particles
    struct Particle
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float radius;
    float lifetime;
    float maxLifetime;
};

int main()
{
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Create the window (600x600) with a modern style (Titlebar | Close)
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML 2.6.2 - Dynamic Particle Orb", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Color Palette (HSL-like modern premium colors)
    const sf::Color bgColor(15, 23, 42);              // Deep Slate Blue (#0f172a)
    const sf::Color particleColor(45, 212, 191, 150); // Semi-transparent Cyan/Teal (#2dd4bf)
    const sf::Color orbColor1(167, 139, 250);         // Soft Violet (#a78bfa)
    const sf::Color orbColor2(129, 140, 248);         // Soft Indigo (#818cf8)

    // Particle system settings
    std::vector<Particle> particles;
    const int maxParticles = 80;

    // Clock for timing animations
    sf::Clock clock;

    // Main event loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit or press Escape
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }

        // Get elapsed time in seconds
        float time = clock.getElapsedTime().asSeconds();

        // 1. Update/Spawn Particles
        // Spawn a new particle occasionally if we are under the limit
        if (particles.size() < maxParticles && std::rand() % 4 == 0)
        {
            Particle p;
            // Spawn around the center of the window with some noise
            float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
            float distance = 10.f + static_cast<float>(std::rand() % 40);
            p.position = sf::Vector2f(300.f, 300.f) + sf::Vector2f(std::cos(angle) * distance, std::sin(angle) * distance);

            // Slow outward expansion velocity
            float speed = 0.5f + static_cast<float>(std::rand() % 100) / 100.f;
            p.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

            // Random soft colors between cyan/blue and purple
            if (std::rand() % 2 == 0)
            {
                p.color = sf::Color(45, 212, 191, 0); // Teal
            }
            else
            {
                p.color = sf::Color(167, 139, 250, 0); // Purple
            }

            p.radius = 2.f + static_cast<float>(std::rand() % 4);
            p.lifetime = 0.f;
            p.maxLifetime = 2.0f + static_cast<float>(std::rand() % 200) / 100.f; // 2 to 4 seconds
            particles.push_back(p);
        }

        // Update existing particles
        for (auto it = particles.begin(); it != particles.end();)
        {
            it->lifetime += 1.f / 60.f; // approx 1/60s per frame

            if (it->lifetime >= it->maxLifetime)
            {
                it = particles.erase(it);
            }
            else
            {
                // Move particle
                it->position += it->velocity;

                // Add a small swirling force around the center
                sf::Vector2f toCenter = sf::Vector2f(300.f, 300.f) - it->position;
                float dist = std::sqrt(toCenter.x * toCenter.x + toCenter.y * toCenter.y);
                if (dist > 1.f)
                {
                    // Swirl tangent force
                    sf::Vector2f tangent(-toCenter.y / dist, toCenter.x / dist);
                    it->position += tangent * 0.3f;
                }

                // Fade in and then fade out
                float ratio = it->lifetime / it->maxLifetime;
                int alpha = 0;
                if (ratio < 0.2f)
                {
                    alpha = static_cast<int>((ratio / 0.2f) * 180.f);
                }
                else
                {
                    alpha = static_cast<int>((1.f - (ratio - 0.2f) / 0.8f) * 180.f);
                }
                it->color.a = static_cast<sf::Uint8>(std::max(0, std::min(255, alpha)));

                ++it;
            }
        }

        // 2. Rendering
        window.clear(bgColor);

        // Draw background particle field
        for (const auto &p : particles)
        {
            sf::CircleShape pShape(p.radius);
            pShape.setFillColor(p.color);
            pShape.setOrigin(p.radius, p.radius);
            pShape.setPosition(p.position);
            window.draw(pShape);
        }

        // Draw dynamic pulsing central orb (layer 1: outer soft glow)
        float pulseScale1 = 1.0f + 0.08f * std::sin(time * 2.5f);
        float pulseRadius1 = 120.f * pulseScale1;
        sf::CircleShape glowOuter(pulseRadius1);
        glowOuter.setOrigin(pulseRadius1, pulseRadius1);
        glowOuter.setPosition(300.f, 300.f);
        // Soft purple gradient-like glow using transparency
        glowOuter.setFillColor(sf::Color(orbColor1.r, orbColor1.g, orbColor1.b, 35));
        window.draw(glowOuter);

        // Draw dynamic pulsing central orb (layer 2: middle glow)
        float pulseScale2 = 1.0f + 0.05f * std::cos(time * 3.0f);
        float pulseRadius2 = 80.f * pulseScale2;
        sf::CircleShape glowInner(pulseRadius2);
        glowInner.setOrigin(pulseRadius2, pulseRadius2);
        glowInner.setPosition(300.f, 300.f);
        // Cyan-ish mid glow
        glowInner.setFillColor(sf::Color(45, 212, 191, 60));
        window.draw(glowInner);

        // Draw dynamic pulsing central orb (layer 3: solid center)
        float pulseScale3 = 1.0f + 0.03f * std::sin(time * 4.0f);
        float pulseRadius3 = 45.f * pulseScale3;
        sf::CircleShape core(pulseRadius3);
        core.setOrigin(pulseRadius3, pulseRadius3);
        core.setPosition(300.f, 300.f);
        core.setFillColor(orbColor2);
        window.draw(core);

        // Draw a rotating polygon in the core for structure
        float polyRadius = 25.f;
        sf::CircleShape poly(polyRadius, 3 + (static_cast<int>(time) % 4)); // Tri, Quad, Pent, Hex rotating over time
        poly.setOrigin(polyRadius, polyRadius);
        poly.setPosition(300.f, 300.f);
        poly.setFillColor(sf::Color(255, 255, 255, 180));
        poly.setRotation(time * 45.f); // Rotate 45 degrees per second
        window.draw(poly);

        // Display frame
        window.display();
    }

    return 0;
}
