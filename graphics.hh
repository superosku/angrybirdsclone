#ifndef G_H
#define G_H

#define WINDOW_W 1280
#define WINDOW_H 720

#define MAX_FORCE 200

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <dirent.h>
#include "map.hh"
#include "moveable.hh"
#include "hostiles.hh"
#include "birds.hh"

class Graphics {
  private:
    sf::RenderWindow window;
    sf::View view;
    sf::Texture amfi1;
    sf::Texture amfi2;
    sf::Texture amfi3;
    sf::Texture amfi4;
    sf::Font font;
    sf::Texture kemma;
    sf::Texture kemma100;
    sf::Texture kemma250;
    sf::Texture kemma500;
    sf::Texture kemma900;
    sf::Texture kone;
    sf::Texture kone100;
    sf::Texture kone250;
    sf::Texture kone500;
    sf::Texture kone900;
    sf::Texture prodeko;
    sf::Texture prodeko100;
    sf::Texture prodeko250;
    sf::Texture prodeko500;
    sf::Texture prodeko900;
    sf::Texture pjk;
    sf::Texture pjk100;
    sf::Texture pjk250;
    sf::Texture pjk500;
    sf::Texture pjk900;
    sf::Texture taffa;
    sf::Texture taffa100;
    sf::Texture taffa250;
    sf::Texture taffa500;
    sf::Texture taffa900;
    sf::Texture tik;
    sf::Texture tik100;
    sf::Texture tik250;
    sf::Texture tik500;
    sf::Texture tik900;
    sf::Texture tile1;
    sf::Texture tile2;
    sf::Texture tile3;
    sf::Texture tile4;
    sf::Texture wood1;
    sf::Texture wood2;
    sf::Texture wood3;
    sf::Texture wood4;
    sf::Texture tnt;
    sf::Texture ratas;
    sf::Texture ratas50;
    sf::Texture ratas100;
    sf::Texture ratas250;
    sf::Texture smoke;

    sf::CircleShape catapult;

    sf::ContextSettings settings;
    sf::View defaultView = window.getDefaultView();
    Map* m;
    // the constant used to change coordinates
    int c;
    // the constant used to move the screen
    int s;
    // the constants to correct view and catapult when zooming
    float zx; 
    int z;
    float temp;
    // Things for camera movenment. In box2d meters
    // 0,0 means center of the screen
    float cam_x, cam_y;
    int catapult_x, catapult_y;
    // Mouse pos. needed for cannon
    size_t shoot_aiming;
    std::string currentMapPath = "maps/basic_map.csv";

  public:
    ~Graphics() {
      delete m;
    }
    Graphics(); 
    void drawMoveableObject(MoveableObject *i); 
    void drawMoveableObjects();
    void pollEvents();
    void drawUnmoveable();

    void run() {
      //Test for directory listing
        //std::vector<std::string> list = readDir(".");
        //for(auto i: list)
        //  std::cout << i << std::endl;
      while (window.isOpen()) {
        pollEvents();
        window.clear(/*sf::Color(160,160,255)*/);
        window.setView(view);
        drawUnmoveable(); 
        drawMoveableObjects() ;
        
        //Draw score, display and advance the simulation one step ahead
        window.setView(defaultView);
        std::ostringstream ss;
        ss << "Points: " << m->getScore() << std::endl << "Birds left: " << m->getBirdsLeft() << std::endl << "Enemies left: " << m->getEnemyCount();
        sf::Text t(ss.str(),font);
        t.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
        window.draw(t);

        window.display(); // Display windows
        m->Step(); // Advance simulation
      }
    }

    int convertX(float x) {
      return x * c + WINDOW_W/2.0 + cam_x * c;
    }
    int convertY(float y) {
      return -y * c + WINDOW_H/2.0 + cam_y * c;
    }
    int convertDistance(float d) {
      return c * d;
    }
    float convertDistanceReverse(int d) {
      return d / c +0.1;
    }
    std::vector<std::string> readDir(std::string dir);
};

#endif

