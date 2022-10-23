#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unistd.h>


using namespace sf;

const int Xstep  = 0;
const int Ystep  = 2;
const int Width  = 60;
const int Height = 50;

const int WindowWidth = 700;
const int WindowHeight = 384;

const int MapHeight = 16;
const int MapWidth  = 49;

const int ground =  500; //352;

float offsetX = 0, offsetY = 0;

String TitleMap[MapHeight] = {
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "B                                              B",
    "B                                              B",
    "B                                              B",
    "B                                              B",
    "B                                              B",
    "B     00000                                    B",
    "B                                              B",
    "BBBB           BBBBBBBB                        B",
    "B                     B                        B",
    "B                     B                        B",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

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
    Music coin;
    

    Player (Texture &image)
    {
        sprite.setTexture(image);
        rect = FloatRect(0, 0, 60, 50);

        dx = dy = 0;
        currentframe = 1;
        row = 0;
        onGround = true;
        coin.openFromFile("sfx-5.ogg");

        sprite.setTextureRect(IntRect(Width*int(currentframe), 14 + row*Ystep, Width, Height));
    }

    void update (float time)
    {
        rect.left += dx*time;
        Collision(0, coin);

        if (!onGround)
        {
            dy += 0.00000002*time; // +=  d^2(x)/(dt)^2
        }

        rect.top += dy*time;
        onGround  = false;

        Collision(1, coin);

        currentframe += 0.004*time;
                
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

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx = 0;
    }

    void Collision(int dir, Music &coin) // directron = 0 for X, direction = 1 for Y
    {
        for (int i = rect.top/32; i < (rect.top + rect.height)/32; i++)
        {
            for (int j = rect.left/32; j < (rect.left + rect.width)/32; j++)
            {
                if (TitleMap[i][j] == 'B')
                {
                    if ((dx > 0) && (dir == 0)) rect.left = j*32 - rect.width;
                    if ((dx < 0) && (dir == 0)) rect.left = j*32 + 32;

                    if ((dy > 0) && (dir == 1)) { rect.top = i*32 - rect.height; dy = 0; onGround = true; }
                    if ((dy < 0) && (dir == 1)) { rect.top = i*32 + 32; dy = 0; }
                }

                if (TitleMap[i][j] == '0')
                {
                    TitleMap[i][j] = ' ';
                    coin.play();
                }
            }
        }
    }

};

class Enemy
{
public:
    float dx, dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentframe;
    int row;
    bool life;

    Enemy (Texture &image)
    {
        sprite.setTexture(image);
        rect = FloatRect(100, 100, 60, 50);

        dx = dy = 0;
        currentframe = 1;
        row = 0;
        onGround = true;
    }

    void set (int CordX, int CordY)
    {
        rect = FloatRect (CordX, CordY, 60, 50);

        dx = 0.0001;
        currentframe = 1;
        life = true;
        sprite.setTextureRect(IntRect(Width*int(currentframe)  /* +(Xstep-1))*int(currentframe)*/, 14 + row*Ystep, Width, Height));
    }

    void update (float time)
    {
        rect.left += dx*time;
        Collision(0);

        if (!onGround)
        {
            dy += 0.000000025*time; // +=  d^2(x)/(dt)^2
        }

        rect.top += dy*time;
        onGround  = false;

        Collision(1);

        currentframe += 0.00002*time;
                
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

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }

    void Collision(int dir) // directron = 0 for X, direction = 1 for Y
    {
        for (int i = rect.top/32; i < (rect.top + rect.height)/32; i++)
        {
            for (int j = rect.left/32; j < (rect.left + rect.width)/32; j++)
            {
                if (TitleMap[i][j] == 'B')
                {
                    if ((dx > 0) && (dir == 0)) {rect.left = j*32 - rect.width; dx = -dx; printf("Here\n dx = %lf\nrect.left = %f\n", dx, rect.left); continue;}
                    if ((dx < 0) && (dir == 0)) {rect.left = j*32 + 32; dx = -dx; /*printf("Here\n dx = %lf\nrect.left = %f\n", dx, rect.left);*/continue;}
                    
                    if ((dy > 0) && (dir == 1)) { rect.top = i*32 - rect.height; dy = 0; onGround = true; }
                    if ((dy < 0) && (dir == 1)) { rect.top = i*32 + 32; dy = 0; }

                    
                }

                if (TitleMap[i][j] == '0')
                {
                    TitleMap[i][j] = ' ';
                }
            }
        }
    }

};


int main ()
{
    RenderWindow window (VideoMode(WindowWidth, WindowHeight), "Test!");

    CircleShape Mycircle (100.f);
    Mycircle.setFillColor (Color::Green);

    float currentframe = 0;
    Clock clock;
    

    Texture t;
    t.loadFromFile("90323072-dba28f00-df21-11ea-841b-22f77ba85ebe-300x128.png");

    Player p(t);
    Enemy enemy(t);
    enemy.sprite.setColor(Color::Red);

    enemy.set(832, 200);

    p.rect.left = 100;
    p.rect.top = 100;

    /*Vector2f targetSize(100.0f, 100.0f);               Установка необходимого размера спрайта
    s.setScale(
    targetSize.x / s.getLocalBounds().width, 
    targetSize.y / s.getLocalBounds().height);*/
    
    Music music;
    music.openFromFile("super-mario-saundtrek.ogg");
    music.setLoop(true);
    //music.play();

    Music jump;
    jump.openFromFile("sfx-13.ogg");

    Music coin;
    coin.openFromFile("sfx-5.ogg");

    Music death;
    death.openFromFile("mario-smert.ogg");

    sf::Text text; 
    Font font;
    if (!font.loadFromFile("Gaslight with Russian Cyrillic by vania5617sonfan.ttf"))
    {
        printf ("error\n");
    };
    text.setFont(font);
    text.setString("Game over!");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(Vector2f(100, 100));
    
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
                p.dx = -0.03;
                
            }

            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                p.dx = 0.03;
            }

            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                if (p.onGround)
                {
                    p.dy = -0.003; 
                    jump.play();
                    p.onGround = false;
                }
            }

        }
    
        p.update(time);
        enemy.update(time);

    if (p.rect.intersects (enemy.rect))
    {
        if (enemy.life == true)
        {
            if (p.dy > 0) 
            {
                enemy.dx = 0;
                p.dy = -0.002;
                enemy.life = false;
            }
            else 
            {
                p.sprite.setColor(Color::Black);
                death.play();
                text.setPosition ((Vector2f)window.getPosition());
                window.draw(text);
                window.display();
                sleep(3);
                window.close();
            }
        }
    }

    if (p.rect.left > WindowWidth/2)   offsetX = p.rect.left - WindowWidth/2;
    if (p.rect.top  > WindowHeight/2)  offsetY = p.rect.top - WindowHeight/2;

    window.clear(Color::White);

    for (int i = 0; i < MapHeight; i++)
    {
        for (int j = 0; j < MapWidth; j++)
        {
            if (TitleMap[i][j] == 'B') 
            {
                rectangle.setFillColor(Color::Black);
            }

            if (TitleMap[i][j] == '0') 
            {
                rectangle.setFillColor(Color::Green);
            }

            if (TitleMap[i][j] == ' ') continue;

            rectangle.setPosition(j*32 - offsetX, i*32 - offsetY);
            window.draw(rectangle);
        }
    }

    window.draw (p.sprite);
    window.draw (enemy.sprite);
    window.display();

    }
    

    return 0;
}