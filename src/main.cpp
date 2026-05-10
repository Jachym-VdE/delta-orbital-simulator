#include <SFML/Graphics.hpp>
#include <cmath>

class Body
{
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector3i color;
    float radius;
    float mass;
    bool position_locked;

    Body(sf::Vector2f position, sf::Vector2f velocity, sf::Vector3i color, float radius, float mass, bool position_locked)
    {
        this->position = position;
        this->velocity = velocity;
        this->color = color;
        this->radius = radius;
        this->mass = mass;
        this->position_locked = position_locked;
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
        object.setFillColor(sf::Color(color.x, color.y, color.z));
        object.setPosition(position);
        window.draw(object);
    }
};

int main()
{   
    constexpr float G  = 6.674e-11, RADIUS_SCALE = 1e-5;
    constexpr float R_BODY1 = 6.371e6 , R_BODY2 = 1.737e6; 
    constexpr float M_BODY1 = 5.972e24, M_BODY2 = 7.35e22; // in kg

    int timeDelay = 16; // in ms

    sf::Font font("IBMPlexMono-Regular.ttf");
    
    sf::RenderWindow window(sf::VideoMode({3840, 2160}), "Delta Orbital Simulator v1");

    std::vector<Body> bodies = 
    {
        Body(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), sf::Vector2f{0.0f, 0.0f}, {0, 0, 255}, R_BODY1 * RADIUS_SCALE, M_BODY1 / 100, true),
        Body(sf::Vector2f{1200.0f, 1500.0f}, sf::Vector2f{-30.0f, -50.0f}, {255, 255, 255}, R_BODY2 * RADIUS_SCALE, M_BODY2 / 1000, false)
    };
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && timeDelay < 100)
            timeDelay++;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && timeDelay > 1)
            timeDelay--;

        window.clear();
        
        sf::Text text(font, "Delay:" + std::to_string(timeDelay) + " ms", 50);
        text.setFillColor(sf::Color(255, 255, 255));
        text.setPosition({3450.f, 2010.f});
        
        for (auto& body : bodies)
        {
            for (auto& body2 : bodies)
            {
                if(&body == &body2){continue;}
                if(body.position_locked){continue;}

                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                    sf::sleep(sf::milliseconds(100));

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
        sf::sleep(sf::milliseconds(timeDelay));
    }
}