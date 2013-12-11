#ifndef G_H
#define G_H

#define WINDOW_W 1280
#define WINDOW_H 720
#define MAX_FORCE 200

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
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
    //All needed textures are loaded to this map
    std::map<size_t,std::vector<sf::Texture*>> textures;
    sf::ContextSettings settings;
    sf::View defaultView = window.getDefaultView();
    Map* m = nullptr;
    // the constant used to change coordinates
    int c;
    // the constant used to move the screen
    int s;
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
    std::vector<sf::Text> maps;
    size_t currentMapI = 0;
    
    //List needed for drawing the dots to projectory of the bird
    std::list<std::pair<float, float>> dot_list;
    int dot_counter = 0;

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

    //These methods are used to draw objects
    void drawMoveableObjects();
    void drawUnmoveable();
    void drawCircle(std::vector<sf::Texture*>, MoveableObject*);
    void drawSquare(std::vector<sf::Texture*>, MoveableObject*);

    //Method to poll all events during the gameplay
    void pollGameEvents();


    //Saves the highscore to the first row of the map
    void saveHighScore(std::string mapName);

    void run() {
      //Test for directory listing
        //std::vector<std::string> list = readDir(".");
        //for(auto i: list)
        //  std::cout << i << std::endl;
      while (window.isOpen()) {
        window.clear();
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

     view=defaultView;
     window.setView(view);
     sf::Vector2f center = defaultView.getCenter();
     sf::Vector2f size = defaultView.getSize();
     sf::Vector2f leftEdge = center-size/2.0f;

     //menu background
     menu.loadFromFile("images/menu.png");
     menu.setSmooth(1);
     sf::RectangleShape backg((sf::Vector2f)menu.getSize());
     sf::RectangleShape backgr(size);
     backg.setPosition(center-(sf::Vector2f)menu.getSize()/2.0f);
     backgr.setPosition(leftEdge);
     backg.setTexture(&menu);
     backgr.setFillColor(sf::Color(113,218,226));
     window.draw(backgr);
     window.draw(backg);

     pollMenuEvents();
     std::ostringstream ss;
     for(size_t i = 0;i < maps.size();++i){
       sf::Vector2f mouse =window.mapPixelToCoords(sf::Mouse::getPosition(window));
       if(maps[i].getGlobalBounds().contains(mouse.x,mouse.y))
       {
         maps[i].setPosition(window.mapPixelToCoords(sf::Vector2i(40,i*(maps[i].getCharacterSize()))));
         maps[i].setColor(sf::Color::Red);
       }
       else
       {
         maps[i].setPosition(window.mapPixelToCoords(sf::Vector2i(20,i*(maps[i].getCharacterSize()))));
         maps[i].setColor(sf::Color::White);
       }
       window.draw(maps[i]);
     }
    }

    //This method polls all events which will affect menu behaviour
    void pollMenuEvents();

    //This method handles running the actual game
    void runGame()
    {
        //pollGameEvents();
        window.setView(view);
        m->Step(); // Advance simulation
        drawUnmoveable();
        drawMoveableObjects() ;

        //Draw score, display and advance the simulation one step ahead
        window.setView(defaultView);
        std::ostringstream ss;
        ss << "Score: " << m->getScore() << std::endl << "Highscore: " << m->getHighScore() << std::endl << "Birds left: " << m->getBirdsLeft() << std::endl << "Enemies left: "
          << m->getEnemyCount() << std::endl << "Game won: " << (m->isWin()?"Yes":"No") << std::endl << (m->isEnd()?"press ESC for menu":"");
        sf::Text t(ss.str(),font);
        t.setPosition(window.mapPixelToCoords(sf::Vector2i(0,0)));
        window.draw(t);
        pollGameEvents();
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

