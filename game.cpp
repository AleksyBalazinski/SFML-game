#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <cmath>
using namespace sf;
double deg_to_rad(double degrees);
int main()
{
    const unsigned int width = 800; //screen dimentions
    const unsigned int height = 600;
    RenderWindow window(VideoMode(width, height), "Gravity");
    window.setFramerateLimit(30); //30 FPS
    const double dt = 0.0333; //duration of one frame

    //::font::
    Font font;
    font.loadFromFile("airstrike.ttf");

    //::create a green background::
    RectangleShape ground(Vector2f(800, 20));
    ground.setPosition(0, 580);
    ground.setFillColor(Color(33, 155, 33));

    //::create a movable tank::
    //hull
    double scale = 0.3;
    Sprite hull;
    Texture hull_img;
    hull_img.loadFromFile("tank.png");
    hull.setTexture(hull_img);
    hull.setScale(0.2, 0.2);
    hull.setPosition(0, 504);

    //gun
    Sprite gun;
    Texture gun_img;
    gun_img.loadFromFile("gun.png");
    gun.setTexture(gun_img);
    gun.setScale(0.2, 0.2);
    gun.setPosition(60, 510);

    //::create a bullet (missle)::
    double v = 1000; //velocity in mps
    CircleShape bullet(9);
    bullet.setPosition(124, 520); 
    bullet.setFillColor(Color::Blue);
    

    //::create exposion::
    Sprite explosion;
    Texture explosion_img;
    explosion_img.loadFromFile("explosion.png");
    explosion.setTexture(explosion_img);
    explosion.setScale(1, 1);
    //Sound boom;
    //SoundBuffer boom_audio;
    //boom_audio.loadFromFile("explosion-sound.wav");
    //boom.setBuffer(boom_audio);
    //::create the Godzilla::
    Sprite godzilla;
    Texture godzilla_img;
    godzilla_img.loadFromFile("godzilla.png");
    godzilla.setTexture(godzilla_img);
    godzilla.setScale(1, 1);
    godzilla.setPosition(405, 270);
    int hp = 100;

    //::create healthbar::
    float hb_x = 60;
    RectangleShape healthbar(Vector2f(hb_x, 20));
    healthbar.setPosition(godzilla.getPosition().x, godzilla.getPosition().y - 20);
    healthbar.setFillColor(Color(33, 155, 33));
    RectangleShape fullHealthbar(Vector2f(hb_x + 10, 30));
    fullHealthbar.setPosition(godzilla.getPosition().x - 5, godzilla.getPosition().y - 25);
    fullHealthbar.setFillColor(Color::White);
    //::main loop::
    bool fired = false; //flag for firing a bullet
    bool touched = false; //flag for sygnalizing that godzilla touched the tank
    bool killed = false; //flag for signalizing that godzilla was killed
    bool hit = false;
    int k = 0; //counts loops for bullet
    int n = 0; //counts loops for godzilla
    double angle = 0; //current value of the angle
    double pos_x = 124; 
    double pos_y = 520;
    double t_elapsed = 0; //how long was the bullet airbone
    double T_elapsed = 0; //how long was godzilla alive
    double g = 150;
    while (window.isOpen())
    {
        Event someEvent;
        while (window.pollEvent(someEvent)) //collecting info about pending events
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
                    if(fired == false) angle -= 5;
                }
                //likewise...
                if (someEvent.key.code == Keyboard::Up)
                {
                    gun.rotate(-5);
                    if(fired == false) angle += 5;

                }
                if (someEvent.key.code == Keyboard::Space) fired = true;

            }
        }
        // Clear the window
        window.clear();
        //bullet mechanics
        Vector2f bullet_position = bullet.getPosition();
        if (fired == true)
        {
            //window's boundaries detection
            
            if (bullet_position.x < width && bullet_position.y < height && bullet_position.x > 0 && bullet_position.y > 0)
            {
                bullet.setPosition(
                    pos_x + 64 * cos(deg_to_rad(angle)) + (v * cos(deg_to_rad(angle)) * t_elapsed) * scale,
                    pos_y - 64 * sin(deg_to_rad(angle)) + (-v * sin(deg_to_rad(angle)) * t_elapsed + 0.5 * g * t_elapsed * t_elapsed) * scale
                );
                window.draw(bullet);
                k++;
                t_elapsed = k * dt;
            }
            else
            {
                //explosion.setPosition(0, 0); //sygnalize that the bullet is out of screen
                //window.draw(explosion);
                fired = false;
                //return to initial values of parameters
                bullet.setPosition(pos_x + 64 * cos(deg_to_rad(angle)), pos_y - 64 * sin(deg_to_rad(angle)));
                k = 0;
                t_elapsed = 0;
            }
        }
        //godzilla's movement and collision detection
        if (touched == false && killed == false)
        {
            godzilla.setPosition(405 - 10 * T_elapsed, 270); //moves from right to left (speed = 10)
            n++;
            T_elapsed = n * dt;
        }

        Vector2f godzilla_position = godzilla.getPosition();
        if (bullet_position.x > godzilla_position.x && bullet_position.x < godzilla_position.x + 395
                && bullet_position.y > 270 && bullet_position.y < 580) //FUCK!!! YOU IDIOT XDDD
        {
            if (hit == false)
            {
                hp -= 20;
                hb_x -= 12;
                Vector2f new_size(hb_x, 20);
                healthbar.setSize(new_size);
                explosion.setPosition(0, 0); //sygnalize that the bullet hit godzilla
                //boom.play();
                window.draw(explosion);
                hit = true;
            }
        }
        else
        {
            hit = false;
        }

        if (hp <= 0)
        {
            Text text;
            text.setFont(font);
            text.setString("YOU WON!!!");
            text.setCharacterSize(50); // in pixels, not points!
            text.setFillColor(Color::Red);
            text.setStyle(Text::Bold);
            window.draw(text);
            killed = true;
        }

        if (hull.getPosition().x + 131 > godzilla_position.x)
        {
            Text text;
            text.setFont(font);
            text.setString("GAME OVER!!!");
            text.setCharacterSize(50); // in pixels, not points!
            text.setFillColor(Color::Red);
            text.setStyle(Text::Bold);
            window.draw(text);
            touched = true;
        }

        //Draw stuff
        healthbar.setPosition(godzilla.getPosition().x, godzilla.getPosition().y - 20);
        fullHealthbar.setPosition(godzilla.getPosition().x - 5, godzilla.getPosition().y - 25);
        window.draw(ground);
        window.draw(hull);
        window.draw(gun);
        window.draw(godzilla); //unlike the bullet Godzilla is always displayed on the screen
        window.draw(fullHealthbar);
        window.draw(healthbar);
        window.display();
    }
}

double deg_to_rad(double degrees)
{
    return ((3.14159 / 180) * degrees);
}
