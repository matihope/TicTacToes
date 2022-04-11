#include "tictactoelib.h"   
#include "mymathlib.h"
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;

class Line{
    sf::Vertex verts[4];

    public:
        Line(float thickness, sf::Vector2f start, sf::Vector2f end){
            sf::Vector2f norm = normalizeVector({end.x - start.x, end.y - start.y});
            norm *= thickness / 2;
            verts[0].position = start + rotateVecLeft90(norm);
            verts[1].position = start + rotateVecRight90(norm);
            verts[2].position = end + rotateVecRight90(norm);
            verts[3].position = end + rotateVecLeft90(norm);
        }

        void draw(sf::RenderTarget& win){
            win.draw(verts, 4, sf::Quads);
        }
};

class BaseGameObj{
    protected:
        float gap = 15;
        sf::RenderTexture body;
        sf::Sprite drawer;
        float body_size = 150;
        void applyBody(){
            body.display();
            drawer.setTexture(body.getTexture());
        }

    public:
        BaseGameObj(sf::Vector2f pos){
            pos -= {body_size / 2, body_size / 2};
            drawer.setPosition(pos);
            body.create(body_size, body_size);
            body.clear(sf::Color(0, 0, 0, 0));
        }
        
        void draw(sf::RenderTarget& window){
            window.draw(drawer);
        }
};

struct TicTacX: BaseGameObj{
    Line l1 = Line(8, {gap, gap}, {body_size-gap, body_size-gap});
    Line l2 = Line(8, {gap, body_size-gap}, {body_size-gap, gap});
    
    public:
        TicTacX(sf::Vector2f pos): BaseGameObj(pos) {
            l1.draw(body);
            l2.draw(body);
            applyBody();
        }
};

struct TicTacO: BaseGameObj{
    sf::CircleShape circle;
    
    public:
        TicTacO(sf::Vector2f pos): BaseGameObj(pos) {
            int rad1 = body_size/2, rad2 = body_size/2 - 8;
            circle.setRadius(rad1);
            body.draw(circle);
            circle.setOrigin(-(rad1 - rad2), -(rad1 - rad2));
            circle.setFillColor(sf::Color(31, 31, 31));
            circle.setRadius(rad2);
            body.draw(circle);
            applyBody();
        }

        TicTacO(std::pair<int, int> pos): BaseGameObj(sf::Vector2f(pos.first, pos.second)){
            TicTacO(sf::Vector2f(pos.first, pos.second));
        }
};

class Mouse{
    public:
        bool prev = false;

        Mouse(){};

        sf::Vector2i update(sf::RenderWindow& window){
            bool now = sf::Mouse::isButtonPressed(sf::Mouse::Left);
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            sf::Vector2i coords = static_cast<sf::Vector2i>(window.mapPixelToCoords(pos));
            bool clicked = false;
            if(!now && prev){
                clicked = true;
            }
            prev = now;
            if(clicked)
                return coords;
            return {-1, -1};
        }      
};

int main()
{
    sf::VideoMode video_mode(600, 600);
    sf::RenderWindow window(video_mode, "TicTacToe");
    window.setVerticalSyncEnabled(true);
    TicTacToe game(true, false);
    Mouse mouse;
    bool run = true;

    vector<Line> lines = {
        {10, {200, 0}, {200, 600}},
        {10, {400, 0}, {400, 600}},
        {10, {0, 200}, {600, 200}},
        {10, {0, 400}, {600, 400}}
    };
    vector<unique_ptr<BaseGameObj>> objects;
    if(game.CPU_INITS != make_pair(-1, -1)){
        objects.push_back(make_unique<TicTacO>(sf::Vector2f(game.CPU_INITS.second * 200 + 100, game.CPU_INITS.first * 200 + 100)));
    }
    cout << "STARTED" << endl;
    while (run) {

        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type){
                case sf::Event::Closed:
                    run = false;
                    break;

                case sf::Event::KeyPressed:
                    switch(event.key.code){
                        case sf::Keyboard::Q:
                            run = false;
                            break;
                        case sf::Keyboard::R:
                            objects.clear();
                            game = TicTacToe(true, false);
                            if(game.CPU_INITS != make_pair(-1, -1)){
                                objects.push_back(make_unique<TicTacO>(sf::Vector2f(game.CPU_INITS.second * 200 + 100, game.CPU_INITS.first * 200 + 100)));
                            }
                            cout << "GAME: Restart" << endl;
                            break;
                    }
                    break;
            }
        }

        window.clear(sf::Color(31, 31, 31));

        for(auto& l: lines)
            l.draw(window);

        for(auto& obj: objects)
            obj->draw(window);
        
        sf::Vector2i pos = mouse.update(window);
        if(!game.game_over && pos != sf::Vector2i(-1, -1)){
            sf::Vector2i buff = sf::Vector2i(video_mode.width / 6, video_mode.height / 6);
            int multiplier = video_mode.height / 3;

            pos.x /= video_mode.width / 3;
            pos.y /= video_mode.height / 3;
            pair<int, int> CPU = game.make_move(pos.y, pos.x);
            
            sf::Vector2f CPU_pos = sf::Vector2f(CPU.first * multiplier, CPU.second * multiplier);
            CPU_pos += static_cast<sf::Vector2f>(buff);
            
            pos *= multiplier;
            pos += buff;
            CPU_pos = {CPU_pos.y, CPU_pos.x};
            if(CPU != make_pair(-1, -1)){
                objects.push_back(make_unique<TicTacX>(static_cast<sf::Vector2f>(pos)));
                if(CPU != make_pair(-2, -2))
                    objects.push_back(make_unique<TicTacO>(CPU_pos));
            }
        }
        window.display();
    }
    window.close();

    return 0;
}