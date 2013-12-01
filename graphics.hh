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
#include <map>
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
    sf::Texture menu;
    sf::Font font;

    sf::CircleShape catapult;
    std::map<size_t,std::vector<sf::Texture*>> textures;
    sf::ContextSettings settings;
    sf::View defaultView = window.getDefaultView();
    Map* m = nullptr;
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
    //std::string currentMapPath = "maps/basic_map.csv";
    std::vector<sf::Text> maps; // = readDir("maps");
    size_t currentMapI = 0;

    enum gamePhase
    {
      Menu,
      Game
    };
    gamePhase phase = gamePhase::Menu;

    enum menuAction
    {
      Play = 0,
      Previous = 1,
      Next = 2,
      Quit = 3
    };

    std::map<size_t,std::string> menuTitle = {{0,"Play"},{1,"Previous"},{2,"Next"},{3,"Quit"}};

  public:
    ~Graphics() {
      //Free all loaded textures
      for(auto i = textures.begin();i != textures.end();++i)
       for(auto j = i->second.begin();j != i->second.end();++j)
        delete *j;

      if (m != nullptr)
        delete m;
    }
    Graphics();
    void drawMoveableObject(MoveableObject *i);
    void drawMoveableObjects();
    void pollGameEvents();
    void drawUnmoveable();
    void drawCircle(std::vector<sf::Texture*>, MoveableObject*);
    void drawSquare(std::vector<sf::Texture*>, MoveableObject*);

    void run() {
      //Test for directory listing
        //std::vector<std::string> list = readDir(".");
        //for(auto i: list)
        //  std::cout << i << std::endl;
      while (window.isOpen()) {
        window.clear(/*sf::Color(160,160,255)*/);
        switch(phase)
        {
          case(gamePhase::Menu):
            runMenu();
            break;

          case(gamePhase::Game):
            runGame();
            break;
        }
        window.display(); // Display windows
      }
    }

    void runMenu(){
     //Reset zoom variables (might be != 0 if eg. user pressed escape during zooming)
     z=0;
     zoomDelta = 0;
     xDelta = 0;
     yDelta = 0;

     view=defaultView;
     window.setView(view);
     sf::Vector2f center = defaultView.getCenter();
     sf::Vector2f size = defaultView.getSize();
     sf::Vector2f leftEdge = center-size/2.0f;

     //background
     menu.loadFromFile("images/menu.png");
     menu.setSmooth(1);
     sf::RectangleShape backg(size);
     backg.setPosition(leftEdge);
     backg.setTexture(&menu);
     //backg.setFillColor(sf::Color(113,218,226));
     window.draw(backg);

     //buttons
     /*sf::Vector2f fpos(center.x-90.0f,60.0f);
     sf::Vector2f bsize(180.0f,80.0f);
     sf::Vector2f inc(0.0f,90.0f);

     for (size_t i = 0; i < 4; i++)
     {
       sf::RectangleShape tmp(bsize);
       tmp.setPosition(fpos + (float)i*inc);
       if ((i == 1 && currentMapI == 0) || (i == 2 && currentMapI == (maps.size()-1)))
        tmp.setFillColor(sf::Color(0,44,5));
       else
        tmp.setFillColor(sf::Color(0,104,55));
       window.draw(tmp);
       std::ostringstream ss;
       ss << menuTitle[i];
       sf::Text t(ss.str(),font);
       t.setPosition(tmp.getPosition()+tmp.getSize()/2.0f-sf::Vector2f(t.getGlobalBounds().width,t.getGlobalBounds().height)/2.0f);
       window.draw(t);
     }
*/

     //window.setView(defaultView);
     pollMenuEvents();
     std::ostringstream ss;
     for(size_t i = 0;i < maps.size();++i){
       sf::Vector2f mouse =window.mapPixelToCoords(sf::Mouse::getPosition(window));
       maps[i].setPosition(window.mapPixelToCoords(sf::Vector2i(20,i*(maps[i].getCharacterSize()))));
       if(maps[i].getGlobalBounds().contains(mouse.x,mouse.y))
         maps[i].setColor(sf::Color::Red);
       else
         maps[i].setColor(sf::Color::White);
       window.draw(maps[i]);
     }
      /* ss << (i.getString() == maps[currentMapI].getString()? " * ": "   ") << std::string(i.getString()) << std::endl;
     sf::Text t(ss.str(),font);*/




     //t.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
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
        /*if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
          m = new Map(currentMapPath);
          phase = gamePhase::Game;
        }*/
        if(event.type == sf::Event::MouseButtonReleased){
          if (event.mouseButton.button == sf::Mouse::Left){}
            sf::Vector2f mouse =window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
            for(size_t i = 0;i < maps.size();++i)
              if(maps[i].getGlobalBounds().contains(mouse.x,mouse.y)){
                m = new Map("maps/" + maps[i].getString());
                phase = gamePhase::Game;
                break;
              }
        }
            /*ssize_t act = -1;


            for (size_t i = 0; i < 4; i++)
            {
              sf::Vector2f mouse =window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),view);
              if (abs(mouse.x-view.getCenter().x) < 90 && abs(mouse.y-90.0f*i-105.0f) < 40)
                act = i;
              //std::cout << mouse.x << " " << mouse.y << " " << abs(mouse.x-view.getCenter().x) << " " << abs(mouse.y-90.0f*i-105.0f) << std::endl;
            }
            switch(act)
            {
              case(menuAction::Play):
                std::cout << "play" << std::endl;

                break;
              case(menuAction::Previous):
                std::cout << "prev" << std::endl;
                if (currentMapI > 0)
                  currentMapI--;
                break;
              case(menuAction::Next):
                std::cout << "next" << std::endl;
                if (currentMapI < (maps.size()-1))
                  currentMapI++;
                break;
              case(menuAction::Quit):
                std::cout << "quit" << std::endl;
                window.close();
                break;
            }*/
          }
        }

    void runGame(){
        //pollGameEvents();
        window.setView(view);
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
        pollGameEvents();
        if(m && m->isEnd())
          phase= gamePhase::Menu;
    }

    int convertX(float x) const {
      return x * c + WINDOW_W/2.0 + cam_x * c;
    }
    int convertY(float y) const {
      return -y * c + WINDOW_H/2.0 + cam_y * c;
    }
    int convertDistance(float d) const {
      return c * d;
    }
    float convertDistanceReverse(int d) const {
      return d / c +0.1;
    }
    std::vector<std::string> readDir(std::string dir);
};

#endif


