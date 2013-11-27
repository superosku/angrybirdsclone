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
    sf::Texture glass1;
    sf::Texture glass2;
    sf::Texture glass3;
    sf::Texture glass4;
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
    int ss;
    // the constants to correct view and catapult when zooming
    float zx;
    int z;
    float temp;
    int zoomDelta = 0;
    int xDelta = 0;
    int yDelta = 0;
    // Things for camera movenment. In box2d meters
    // 0,0 means center of the screen
    float cam_x, cam_y;
    int catapult_x, catapult_y;
    // Mouse pos. needed for cannon
    size_t shoot_aiming;
    std::string currentMapPath = "maps/basic_map.csv";

    enum gamePhase
    {
      Menu,
      Game
    };
    gamePhase phase = gamePhase::Menu;

  public:
    ~Graphics() {
      delete m;
    }
    Graphics();
    void drawMoveableObject(MoveableObject *i);
    void drawMoveableObjects();
    void pollGameEvents();
    void drawUnmoveable();

    void run() {
      //Test for directory listing
        //std::vector<std::string> list = readDir(".");
        //for(auto i: list)
        //  std::cout << i << std::endl;
      while (window.isOpen()) {
        window.clear(/*sf::Color(160,160,255)*/);
        window.setView(view);
        switch(phase)
        {
          case(gamePhase::Menu):
            runMenu();
            std::cout << "MENU" << std::endl;
            break;

          case(gamePhase::Game):
            runGame();
            break;
        }
        window.display(); // Display windows
      }
    }

    void runMenu(){
     pollMenuEvents();
     sf::Vector2f center = view.getCenter();
     sf::Vector2f size = view.getSize();
     sf::Vector2f leftEdge = center-size/2.0f;

     sf::RectangleShape ground3(size);
     ground3.setPosition(leftEdge);
     ground3.setFillColor(sf::Color(113,218,226));
     window.draw(ground3);
    }

    void pollMenuEvents() {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
        if(event.type == sf::Event::Resized)
        {
          view.setSize(event.size.width, event.size.height);
          defaultView=view;
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
          phase = gamePhase::Game;
          std::cout << "CLICK" << std::endl;
        }
      }
    }

    void runGame(){
        pollGameEvents();
        m->Step(); // Advance simulation
        drawUnmoveable();
        drawMoveableObjects() ;

        //Draw score, display and advance the simulation one step ahead
        window.setView(defaultView);
        std::ostringstream ss;
        ss << "Points: " << m->getScore() << std::endl << "Birds left: " << m->getBirdsLeft() << std::endl << "Enemies left: " << m->getEnemyCount();
        sf::Text t(ss.str(),font);
        t.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
        window.draw(t);
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


