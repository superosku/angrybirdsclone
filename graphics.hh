#ifndef G_H
#define G_H

#define WINDOW_W 1280
#define WINDOW_H 720


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
    sf::Texture bgt;
    sf::Sprite bg;
    sf::Font font;
    sf::Texture kemma;
    sf::Texture kone;
    sf::Texture prodeko;
    sf::Texture pjk;
    sf::Texture tik;
    sf::Texture tile1;
    sf::Texture tile2;
    sf::Texture tile3;
    sf::Texture tile4;
    sf::Texture tnt;
    sf::Texture ratas;
    sf::Texture smoke;

    sf::ContextSettings settings;
    sf::View defaultView = window.getDefaultView();
    Map* m;
    // the constant used to change coordinates
    int c;
    // the constant used to move the screen
    int s;
    // the constants to correct view and catapult when zooming
    float zx; 
    int i/*, j*/;
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
        window.draw(sf::Text(ss.str(),font));
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

