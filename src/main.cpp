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
    
    void accelerate(float dt, float x, float y)
    {
        this->velocity.x += x * dt;
        this->velocity.y += y * dt;
    }

    void update_pos(float dt)
    {
        this->position.x += velocity.x * dt;
        this->position.y += velocity.y * dt;
    }

    void draw(sf::RenderWindow& window, float scale)
    {
        sf::CircleShape object(radius * scale * 5);
        object.setFillColor(sf::Color(color.x, color.y, color.z));
        object.setOrigin({(radius * scale * 2.5f), (radius * scale * 2.5f)});
        object.setPosition({position.x * scale, position.y * scale});
        window.draw(object);
    }
};

int main()
{   
    constexpr float G  = 6.674e-11, SCALE = 1e-6;
    constexpr float R_BODY1 = 6.371e6 , R_BODY2 = 1.737e6; // in m 
    constexpr float M_BODY1 = 5.972e24, M_BODY2 = 7.35e22; // in kg
    constexpr float ORBITAL_DISTANCE = 3.844e8; // in m

    int timeDelay = 2; // in ms
    int timeScale = 100;

    sf::Font font("IBMPlexMono-Regular.ttf");
    
    sf::RenderWindow window(sf::VideoMode({3840, 2160}), "Delta Orbital Simulator v1");

    std::vector<Body> bodies = 
    {
        Body(sf::Vector2f((window.getSize().x / SCALE) / 2.f, (window.getSize().y / SCALE) / 2.f), sf::Vector2f{0.0f, 0.0f}, {0, 0, 255}, R_BODY1, M_BODY1, true),
        Body(sf::Vector2f{(window.getSize().x / SCALE) / 2.f - ORBITAL_DISTANCE, (window.getSize().y / SCALE) / 2.f}, sf::Vector2f{0.0f, 1000.0f}, {255, 255, 255}, R_BODY2, M_BODY2, false)
    };
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && timeScale != 0)
            timeScale = 0;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            timeScale = 100;
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            timeScale += 10;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && timeScale > 1)
            timeScale -= 10;

        window.clear();
        
        sf::Text text(font, "Warp: " + std::to_string(timeScale) + "x", 50);
        text.setFillColor(sf::Color(255, 255, 255));
        text.setPosition({3450.f, 2010.f});
        
        float dt = timeDelay * timeScale; 

        for (auto& body : bodies)
        {
            body.update_pos(dt);
            body.draw(window, SCALE);

            if (body.position_locked){continue;}
            
            for (auto& body2 : bodies)
            {
                if (&body == &body2){continue;}
                
                float dx = body2.position.x - body.position.x;
                float dy = body2.position.y - body.position.y;
                float distance = sqrt(dx*dx + dy*dy);
                sf::Vector2f direction = {dx / distance, dy / distance};

                float Gforce = (G * body.mass * body2.mass) / (distance * distance);
                float acceleration1 = Gforce / body.mass;

                sf::Vector2f acceleration = {acceleration1 * direction.x, acceleration1 * direction.y};
                body.accelerate(dt, acceleration.x, acceleration.y);
            }
        }

        window.draw(text);
        window.display();
        sf::sleep(sf::milliseconds(timeDelay));
    }
}