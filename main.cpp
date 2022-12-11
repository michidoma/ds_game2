#include <SFML/Graphics.hpp>
#include<iostream>
#include<list>
using namespace sf;
using namespace std;

struct step {
    RectangleShape left;
    RectangleShape mid;
    RectangleShape right;
};
typedef struct step step;

list<step> stepsOnWindow;
list<step>::iterator itr;
Uint8 alpha = 255;
int width = 1280, height = 720;

step randomize(step s) {
    alpha = rand() % 2 * 255;
    s.left.setFillColor(Color{ 103,96,95,alpha });
    alpha = rand() % 2 * 255;
    s.right.setFillColor(Color{ 103,96,95,alpha });
    return s;
};

step init(step s) {
    s.mid.setFillColor(Color{ 103,96,95,alpha });
    s.mid.setSize(Vector2f(300, 10));
    s.mid.setPosition((width - 300) / 2, height - 30);
    s.left.setFillColor(Color{ 103,96,95,alpha });
    s.left.setSize(Vector2f(10, 30));
    s.left.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 30);
    s.right.setFillColor(Color{ 103,96,95,alpha });
    s.right.setSize(Vector2f(10, 30));
    s.right.setPosition(s.mid.getPosition().x + s.mid.getSize().x - 10, s.left.getPosition().y);

    for (int i = 0; i < 3; i++) {
        stepsOnWindow.push_back(s);
        s.mid.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 60);
        s.left.setPosition(s.left.getPosition().x, s.left.getPosition().y - 60);
        s.right.setPosition(s.right.getPosition().x, s.right.getPosition().y - 60);
    }

    while (s.mid.getPosition().y - 10 > 0) {
        stepsOnWindow.push_back(s);
        s.mid.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 60);
        s.left.setPosition(s.left.getPosition().x, s.left.getPosition().y - 60);
        s.right.setPosition(s.right.getPosition().x, s.right.getPosition().y - 60);
        s = randomize(s);
    }
    return s;
};

//string intToString(int x) {
//
//}

int main(){
    srand(time(0));

    int score = 0;
    step s;
    s = init(s);

    CircleShape ball;
    ball.setFillColor(Color{224,181,154});
    ball.setRadius(15);
    ball.setPosition(width / 2, height - 150 - ball.getRadius()*2);

    float xVelocity = 3;
    float yVelocity = 0;
    int t = 1;
    int check = -1;
    int down = 0;
    int active=1;

    // Background texture
    Texture texture;
    if (!texture.loadFromFile("background.jpg")) {
        cout << "Background texture file unshihad aldaa garlaa\n";
        exit(-1);
    }
    texture.setRepeated(true);
    Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, width, height));

    Texture ending;
    if (!ending.loadFromFile("ending.png")) {
        cout << "Background texture file unshihad aldaa garlaa\n";
        exit(-1);
    }
    Sprite spriteEnd;
    spriteEnd.setTexture(ending);
    spriteEnd.scale(width/spriteEnd.getGlobalBounds().width, height / spriteEnd.getGlobalBounds().height);
    spriteEnd.setTextureRect(IntRect(0, 0, width, height));

    Font font;
    if (!font.loadFromFile("Livemonodemo.otf")) {
        cout << "font file unshihad aldaa garlaa\n";
        exit(-1);
    }
    Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(Color{ 0, 143, 17 }); // #008F11
    text.setStyle(Text::Bold);

    RenderWindow window(VideoMode(width, height), "Game2");
    window.setFramerateLimit(90);

    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed) 
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Escape)) 
                window.close();
            if (active == 1) {
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    score++;
                    yVelocity = -5;                  // Дээшээ явахад босоо координат хасагдана
                    check = 465;                     // 75-аар үсэрнэ, 540-75=465
                    stepsOnWindow.push_back(s);      // Дээшлэх бүрт шинээр шат гарч ирэх
                    s = randomize(s);
                }
            }
            else if (active == 0) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                    active = 1;
                    ball.setPosition(width / 2, height - 150 - ball.getRadius() * 2);
                    xVelocity = 3;
                    yVelocity = 0;
                    t = 1;
                    check = -1;
                    down = 0;
                    score = 0;
                }
            }
        }

        if (active == 1) {
            // 2 талдаа ойх эсэх
            if (ball.getPosition().x + 30 >= 780) {
                itr = stepsOnWindow.begin();
                advance(itr, 2);
                // Хана байгаа бол
                if (itr->right.getFillColor().a == 255)
                    xVelocity *= -1;
                // Хана байхгүй бол унана
                else {
                    yVelocity = 4;
                    active = 2;
                    down = t + 90;
                }
            }
            else {
                if (ball.getPosition().x <= 500) {
                    itr = stepsOnWindow.begin();
                    advance(itr, 2);
                    if (itr->left.getFillColor().a == 255)
                        xVelocity *= -1;
                    else {
                        yVelocity = 4;
                        active = 2;
                        down = t + 90;
                    }
                }
            }

            if (ball.getPosition().y == check) {
                // Дээш 75 үсэрсэн -> буцаад 15 буух
                if (yVelocity < 0) {
                    yVelocity *= -1;
                    check = 480; //540->480 үсрэх
                }
                // Буусан -> y тэнхлэгийн дагуу хөдлөхгүй
                else {
                    yVelocity = 0;
                    check = 0; // 12 хүртэл доошилно, 12*5=60
                    down = 5;
                }
            }

            // үсрэхэд шатнууд доошлох
            if (check >= 0 && check < 12) {
                for (itr = stepsOnWindow.begin(); itr != stepsOnWindow.end(); itr++) {
                    itr->left.setPosition(itr->left.getPosition().x, itr->left.getPosition().y + down);
                    itr->mid.setPosition(itr->mid.getPosition().x, itr->mid.getPosition().y + down);
                    itr->right.setPosition(itr->right.getPosition().x, itr->right.getPosition().y + down);
                }
                ball.setPosition(ball.getPosition().x, ball.getPosition().y + down);
                check++;
            }

            // Цонхноос гарвал устгах
            if (stepsOnWindow.front().left.getPosition().y >= height)
                stepsOnWindow.pop_front();
        }

        if (active == 1 || active == 2) {
            if (down == t)
                active = 0;
            ball.setPosition(ball.getPosition().x + xVelocity, ball.getPosition().y + yVelocity);

            window.clear();
            window.draw(sprite);
            for (itr = stepsOnWindow.begin(); itr != stepsOnWindow.end(); itr++) {
                window.draw(itr->left);
                window.draw(itr->mid);
                window.draw(itr->right);
            }
            window.draw(ball);
            window.display();
        }

        if (active == 0) {
            alpha = 255;
            stepsOnWindow.clear();
            s = init(s);
            string string = "Score: ";
            string += to_string(score - 1);
            text.setString(string);
            text.setPosition(270, 200);
            text.setCharacterSize(30);
            window.setTitle("Game Over!");
            window.draw(spriteEnd);
            window.draw(text);
            text.setString("press SPACE to play again");
            text.setPosition(270, 470);
            window.draw(text);
            window.display();
        }

        t++;
    }

    return 0;
}