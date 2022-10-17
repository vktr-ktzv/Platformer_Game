#include <SFML/Graphics.hpp>


using namespace sf;

const int Xstep  = 0;
const int Ystep  = 2;
const int Width  = 60;
const int Height = 50;

const int MapHeight = 12;
const int MapWidth  = 49;

const int ground = 500;


String TitleMap[MapHeight] = {
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "B                                              B",
    "B                                              B",
    "B                                              B",
    "B                                              B",
    "B                                              B",
    "B     00000                                    B",
    "B                                              B",
    "BBBB            BBBBBBBB                       B",
    "B                     BB                       B",
    "B                     BB                       B",
    "BBBBBBBBBBBBBB       BBBBBBBBBBBBBBBBBBBBBBBBBBB"

};

class Player
{
public:
    float dx, dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentframe;
    int row;

    Player (Texture &image)
    {
        sprite.setTexture(image);
        rect = FloatRect(0, 0, 60, 50);

        dx = dy = 0;
        currentframe = 0;
        row = 0;
        onGround = true;
    }

    void update (float time)
    {
        rect.left += dx*time;

        if (!onGround)
        {
            dy += 0.000000025*time; // +=  d^2(x)/(dt)^2
        }

        rect.top += dy*time;

        onGround  = false;

        currentframe += 0.002*time;
                
        if (currentframe > 5)
        {
            if (row == 1)
            {
                row -= 2;
            }
            row++;
            currentframe -= 5;
        }
        
        if (dx > 0) sprite.setTextureRect(IntRect(Width*int(currentframe)  /* +(Xstep-1))*int(currentframe)*/, 14 + row*Ystep, Width, Height));
        if (dx < 0) sprite.setTextureRect(IntRect((Width /*+ (Xstep-1)*/)*int(currentframe) + Width, 14 + row*Ystep, -Width, Height));
            

        if (rect.top > ground)
        {
            rect.top = ground;
            dy = 0;
            onGround = true;
        }

        sprite.setPosition(rect.left, rect.top);

        dx = 0;
    }

};

int main ()
{
    RenderWindow window (VideoMode(2000, 2000), "Test!");
    

    CircleShape Mycircle (100.f);
    Mycircle.setFillColor (Color::Green);

    float currentframe = 0;
    

    Texture t;
    t.loadFromFile("90323072-dba28f00-df21-11ea-841b-22f77ba85ebe-300x128.png");

    Player p(t);

    /*Vector2f targetSize(100.0f, 100.0f);
    s.setScale(
    targetSize.x / s.getLocalBounds().width, 
    targetSize.y / s.getLocalBounds().height);*/
    

    Clock clock;

    RectangleShape rectangle;
    rectangle.setSize(Vector2f(32, 32));

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                p.dx = -0.05;
                
            }

            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                p.dx = 0.05;
            }

            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                if (p.onGround)
                {
                    p.dy = -0.004; 
                    p.onGround = false;
                }
            }

           /* if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                s.move (0, 0.5);
            }*/

        }
    
    p.update(time);

    window.clear();

    for (int i = 0; i < MapHeight; i++)
    {
        for (int j = 0; j < MapWidth; j++)
        {
            if (TitleMap[i][j] == 'B') 
            {
                rectangle.setFillColor(Color::White);
            }

            if (TitleMap[i][j] == '0') 
            {
                rectangle.setFillColor(Color::Green);
            }

            if (TitleMap[i][j] == ' ') continue;

           // printf("here\n");
            rectangle.setPosition(j*32, i*32);
            window.draw(rectangle);
        }
    }

    window.draw (p.sprite);
    window.display();

    }


    return 0;
}