#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
double deg_to_rad(double degrees);
int main()
{
    const unsigned int width = 800; //screen dimentions
    const unsigned int height = 600;
    RenderWindow window(VideoMode(width, height), "Gravity");
    window.setFramerateLimit(30); //30 FPS
    double dt = 0.0333; //duration of one frame
    //::create a green background::
    RectangleShape ground(Vector2f(800, 20));
    ground.setPosition(0, 580);
    ground.setFillColor(Color(33, 155, 33));
    //::create a movable tank::
    double scale = 0.3;
    Sprite hull;
    Texture hull_img;
    hull_img.loadFromFile("tank.png");
    hull.setTexture(hull_img);
    hull.setScale(0.2, 0.2);
    hull.setPosition(400, 520);
    //create a rotatable gun (barrel)
    //RectangleShape gun(Vector2f(20, 10));
    //gun.setPosition(405, 558); //slightly different coordinates
    //gun.setFillColor(Color::Yellow);
    Sprite gun;
    Texture gun_img;
    gun_img.loadFromFile("gun.png");
    gun.setTexture(gun_img);
    gun.setScale(0.2, 0.2);
    gun.setPosition(450, 520);
    //::create a bullet (missle)::
    double v = 1000; //velocity in mps
    CircleShape bullet(9);
    bullet.setPosition(450, 520); //inside the barrel 405,558
    bullet.setFillColor(Color::Blue);
    //::create exposion::
    Sprite explosion;
    Texture explosion_img;
    explosion_img.loadFromFile("explosion.png");
    explosion.setTexture(explosion_img);
    explosion.setScale(1, 1);
    //::main loop::
    bool fired = false; //flag for firing a bullet
    int k = 0; //counts loops
    double angle = 0; //current value of the angle
    double pos_x = 405;
    double pos_y = 558;
    double t_elapsed = 0; 
    double g = 150;
    while (window.isOpen())
    {
        Event someEvent;
        while (window.pollEvent(someEvent))
        {
            if (someEvent.type == Event::Closed) window.close();
            if (someEvent.type == Event::KeyPressed)
            {
                if (someEvent.key.code == Keyboard::Escape) window.close();
                //mechanics of tank's movement
                if (someEvent.key.code == Keyboard::Right)
                {
                    hull.move(5, 0);
                    gun.move(5, 0);
                    if (fired == false)
                    {
                        bullet.move(5, 0);
                        pos_x += 5;
                    }
                }
                if (someEvent.key.code == Keyboard::Left)
                {
                    hull.move(-5, 0);
                    gun.move(-5, 0);
                    if (fired == false)
                    {
                        bullet.move(-5, 0);
                        pos_x -= 5;
                    }
                }
                //if down-arrow is pressed, barrel rotates down by 5 degrees
                if (someEvent.key.code == Keyboard::Down)
                {
                    gun.rotate(5);
                    angle -= 5;
                }
                //likewise...
                if (someEvent.key.code == Keyboard::Up)
                {
                    gun.rotate(-5);
                    angle += 5;
                }
                if (someEvent.key.code == Keyboard::Space) fired = true;
            }
        }
        // Clear the window and apply grey background
        window.clear();
        //bullet mechanics
        if (fired == true)
        {
            bullet.setPosition(
                pos_x + (v * cos(deg_to_rad(angle)) * t_elapsed) * scale,
                pos_y + (-v * sin(deg_to_rad(angle)) * t_elapsed + 0.5 * g * t_elapsed * t_elapsed) * scale
            );
            Vector2f position = bullet.getPosition();
            if (position.x > width || position.y > height || position.x < 0 || position.y < 0)
            {
                explosion.setPosition(0,0);
                window.draw(explosion);
            }
            window.draw(bullet);
            k++;
            t_elapsed = k * dt;
        }
        //Draw stuff
        window.draw(ground);
        window.draw(hull);
        window.draw(gun);
        window.display();
    }
}

double deg_to_rad(double degrees)
{
    return ((3.14159 / 180) * degrees);
}
