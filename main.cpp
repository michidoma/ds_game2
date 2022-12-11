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

int main(){
    srand(time(0));
    int width = 1280, height = 720;
    RenderWindow window(VideoMode(width, height), "Game2");
    window.setFramerateLimit(60);
    Uint8 alpha=255;

    step s;
    s.mid.setFillColor(Color{103,96,95,alpha});
    s.mid.setSize(Vector2f(300, 10));
    s.mid.setPosition((width - 300) / 2, height - 150);
    s.left.setFillColor(Color{ 103,96,95,alpha });
    s.left.setSize(Vector2f(10, 30));
    s.left.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 30);
    s.right.setFillColor(Color{ 103,96,95,alpha });
    s.right.setSize(Vector2f(10, 30));
    s.right.setPosition(s.mid.getPosition().x + s.mid.getSize().x - 10, s.left.getPosition().y);

    while (s.mid.getPosition().y - 10 > 0) {
        stepsOnWindow.push_back(s);
        s.mid.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 60);
        s.left.setPosition(s.left.getPosition().x, s.left.getPosition().y - 60);
        alpha = rand() % 2 * 255;
        s.left.setFillColor(Color{ 103,96,95,alpha });
        alpha = rand() % 2 * 255;
        s.right.setPosition(s.right.getPosition().x, s.right.getPosition().y - 60);
        s.right.setFillColor(Color{ 103,96,95,alpha });
    }

    CircleShape ball;
    ball.setFillColor(Color{224,181,154});
    ball.setRadius(15);
    ball.setPosition(width / 2, height - 150 - ball.getRadius()*2);

    float xVelocity = 3;
    float yVelocity = 0;
    //int t = 0;
    int check = -1;
    int down = 0;

    // Background texture
    Texture texture;
    if (!texture.loadFromFile("seamless.jpg")) {
        cout << "Background texture file unshihad aldaa garlaa\n";
        exit(-1);
    }
    texture.setRepeated(true);
    Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, width, height));

    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed) 
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Escape)) 
                window.close();
            if (Mouse::isButtonPressed(Mouse::Left)) {
                yVelocity = -5;                  // Дээшээ явахад босоо координат хасагдана
                check = ball.getPosition().y-75; // 75-аар үсэрнэ
                stepsOnWindow.push_back(s);      // Дээшлэх бүрт шинээр шат гарч ирэх
            }
        }

        itr = stepsOnWindow.begin();
        //advance(itr, 2);
        itr++;
        itr++;
        // s.right.getPosition().x - ээс хэтэрвэл эсвэл s.left.getPosition().x + 10 -с хэтэрвэл
        if (ball.getPosition().x + 30 >= 780) 
            if(itr->right.getFillColor().a == 255)
                xVelocity *= -1;
        else {
                if (ball.getPosition().x <= 500 && itr->left.getFillColor().a == 255)
                    xVelocity *= -1;
        }

        if (ball.getPosition().y == check) {
            // Дээш 75 үсэрсэн -> буцаад 15 буух
            if (yVelocity < 0) {
                yVelocity *= -1;
                check = ball.getPosition().y + 15;
            }
            // Буусан -> y тэнхлэгийн дагуу хөдлөхгүй
            else {
                yVelocity = 0;
                check = 0;
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

        ball.setPosition(ball.getPosition().x + xVelocity, ball.getPosition().y + yVelocity);

        // Цонхноос гарвал устгах
        if(stepsOnWindow.front().left.getPosition().y >= height)
            stepsOnWindow.pop_front();

        window.clear();
        window.draw(sprite);
        for (itr = stepsOnWindow.begin(); itr != stepsOnWindow.end(); itr++) {
            window.draw(itr->left);
            window.draw(itr->mid);
            window.draw(itr->right);
        }
        window.draw(ball);
        window.display();

        //t++;
    }

    return 0;
}