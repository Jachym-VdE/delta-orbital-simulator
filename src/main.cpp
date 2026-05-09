#include <SFML/Graphics.hpp>
#include <cmath>

class Body
{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;
    float mass;

    Body(sf::Vector2f position, sf::Vector2f velocity, float radius, float mass)
    {
        this->position = position;
        this->velocity = velocity;
        this->radius = radius;
        this->mass = mass;
    }
    
    void accelerate(float x, float y)
    {
        this->velocity.x += x;
        this->velocity.y += y;
    }

    void update_pos()
    {
        this->position.x += velocity.x;
        this->position.y += velocity.y;
    }

    void draw(sf::RenderWindow& window)
    {
        sf::CircleShape object(radius);
        object.setFillColor(sf::Color(255, 255, 255));
        object.setPosition(position);
        window.draw(object);
    }
};

int main()
{   
    constexpr float G  = 6.674e-11, RADIUS_SCALE = 1e-5;
    constexpr float M_BODY1 = 5.972e24, R_BODY1 = 6.371e6;
    constexpr float M_BODY2 = 7.35e22, R_BODY2 = 1.737e6;

    int TIME_DELAY = 16;
    sf::Font font("IBMPlexMono-Regular.ttf");
    
    sf::RenderWindow window(sf::VideoMode({3840, 2160}), "Delta Orbital Simulator v1");

    std::vector<Body> bodies = 
    {
        Body(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), sf::Vector2f{0.0f, 0.0f}, R_BODY1 * RADIUS_SCALE, M_BODY1 / 100),
        Body(sf::Vector2f{1000.0f, 2100.0f}, sf::Vector2f{-42.0f, -3.0f}, R_BODY2 * RADIUS_SCALE, M_BODY2 / 1000),
    };
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && TIME_DELAY < 100)
            TIME_DELAY++;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && TIME_DELAY > 1)
            TIME_DELAY--;

        window.clear();
        
        sf::Text text(font, "Delay:" + std::to_string(TIME_DELAY) + " ms", 50);
        text.setFillColor(sf::Color(255, 255, 255));
        text.setPosition({3450.f, 2010.f});
        
        for (auto& body : bodies)
        {
            for (auto& body2 : bodies)
            {
                if(&body == &body2){continue;}
                float dx = body2.position.x - body.position.x;
                float dy = body2.position.y - body.position.y;
                float distance = sqrt(dx*dx + dy*dy);
                sf::Vector2f direction = {dx / distance, dy / distance};
                distance *= 1000;

                float Gforce = (G * body.mass * body2.mass) / (distance * distance);
                float acceleration1 = Gforce / body.mass;

                sf::Vector2f acceleration = {acceleration1 * direction.x, acceleration1 * direction.y};
                body.accelerate(acceleration.x, acceleration.y);
            }
            body.update_pos();
            body.draw(window);
        }

        window.draw(text);
        window.display();
        sf::sleep(sf::milliseconds(TIME_DELAY));
    }
}