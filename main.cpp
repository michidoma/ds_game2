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
    int width = 1280, height = 720;
    RenderWindow window(VideoMode(width, height), "Game2");
    window.setFramerateLimit(60);

    step s;
    s.mid.setFillColor(Color::Yellow);
    s.mid.setSize(Vector2f(300, 10));
    s.mid.setPosition((width - 300) / 2, height - 150);
    s.left.setFillColor(Color::Yellow);
    s.left.setSize(Vector2f(10, 30));
    s.left.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 30);
    s.right.setFillColor(Color::Yellow);
    s.right.setSize(Vector2f(10, 30));
    s.right.setPosition(s.mid.getPosition().x + s.mid.getSize().x - 10, s.left.getPosition().y);

    while (s.mid.getPosition().y - 10 > 0) {
        stepsOnWindow.push_back(s);
        s.mid.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 60);
        s.left.setPosition(s.left.getPosition().x, s.left.getPosition().y - 60);
        s.right.setPosition(s.right.getPosition().x, s.right.getPosition().y - 60);
    }

    CircleShape ball;
    ball.setFillColor(Color::Cyan);
    ball.setRadius(15);
    ball.setPosition(width / 2, height - 150 - ball.getRadius()*2);

    float xVelocity = 3;
    float yVelocity = 0;
    int t = 0;
    int check = -1;
    int down = 0;

    View view;
    view.setSize(width, height);
    view.setCenter(width / 2, height / 2);

    // Background texture
    Texture texture;
    if (!texture.loadFromFile("sky.jpg")) {
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
                yVelocity = -5; // Дээшээ явахад босоо координат хасагдана
                check = ball.getPosition().y-75;
                stepsOnWindow.push_back(s);
                s.mid.setPosition(s.mid.getPosition().x, s.mid.getPosition().y - 60);
                s.left.setPosition(s.left.getPosition().x, s.left.getPosition().y - 60);
                s.right.setPosition(s.right.getPosition().x, s.right.getPosition().y - 60);
            }
        }

        // s.right.getPosition().x - ээс хэтэрвэл эсвэл s.left.getPosition().x + 10 -с хэтэрвэл
        if (ball.getPosition().x >= 780 - 30 || ball.getPosition().x <= 500)
            xVelocity *= -1;

        if (ball.getPosition().y == check) {
            if (yVelocity < 0) {
                yVelocity *= -1;
                check = ball.getPosition().y + 15;
            }
            else {
                yVelocity = 0;
                check = 1;
                view.setCenter(view.getCenter().x, view.getCenter().y - 5);
            }
        }

        if (check > 0 && check < 12) {
            view.setCenter(view.getCenter().x, view.getCenter().y - 5);
            check++;
        }

        ball.setPosition(ball.getPosition().x + xVelocity, ball.getPosition().y + yVelocity);

        // Цонхноос гарвал устгах
        if (stepsOnWindow.front().left.getPosition().y >= view.getCenter().y + height / 2)
            stepsOnWindow.pop_front();

        window.clear();
        window.draw(sprite);
        window.setView(view);
        cout << view.getCenter().y << endl;
        for (itr = stepsOnWindow.begin(); itr != stepsOnWindow.end(); itr++) {
            window.draw(itr->left);
            window.draw(itr->mid);
            window.draw(itr->right);
        }
        window.draw(ball);
        window.display();

        cout << stepsOnWindow.size() << endl;
        t++;
    }

    return 0;
}