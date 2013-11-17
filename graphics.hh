#ifndef G_H
#define G_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
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

    sf::ContextSettings settings;
    Map* m;
    // the constant used to change coordinates
    int c;
    // the constant used to move the screen
    size_t s;
    // the constants to correct catapult coordinates when screen moves
    int gx, gy;
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

  public:
    ~Graphics() {
      delete m;
    }
    Graphics() : window(sf::VideoMode(1280, 720), "Game jou", sf::Style::Default/*, settings*/) {
      m = new Map();
      window.setFramerateLimit(60);
      view.reset(sf::FloatRect(0, 0, 1280, 720));
      if(!font.loadFromFile("QuinzeNarrow.ttf")) {
        //Throw error or something
      }
      if(!kemma.loadFromFile("kemma.png")){
        //Throw error
      }
      if(!kone.loadFromFile("kone.png")){
        //Throw error
      }
      if(!prodeko.loadFromFile("prodeko.png")){
          //Throw error
      }
      if(!pjk.loadFromFile("puu.png")){
          //Throw error
      }
      if(!tik.loadFromFile("tik.png")){
          //Throw error
      }
      if(!tile1.loadFromFile("tiili1.png", sf::IntRect(0, 0, 400, 400))){
          //Throw error
      }
      if(!tile2.loadFromFile("tiili2.png", sf::IntRect(0, 0, 400, 400))){
          //Throw error
      }
      if(!tile3.loadFromFile("tiili3.png", sf::IntRect(0, 0, 400, 400))){
          //Throw error
      }
      if(!tile4.loadFromFile("tiili4.png", sf::IntRect(0, 0, 400, 400))){
          //Throw error
      }
      if(!tnt.loadFromFile("tnt.png")){
          //Throw error
      }
      if(!bgt.loadFromFile("bg.png")){
          //Throw error
      }
      if(!ratas.loadFromFile("hammasratas.png")){
          //Throw error
      }
      
      kemma.setSmooth(true);
      kone.setSmooth(true);
      prodeko.setSmooth(true);
      pjk.setSmooth(true);
      tik.setSmooth(true);
      tile1.setSmooth(true);
      tile2.setSmooth(true);
      tile3.setSmooth(true);
      tile4.setSmooth(true);
      tnt.setSmooth(true);
      bgt.setSmooth(true);
      ratas.setSmooth(true);
      //settings.antialiasingLevel = 8;
      bg.setTexture(bgt);
      window.setView(view);

      c = 32;
      s = 25;
      gx = 0;
      gy = 0;
      zx = 30.4761904762;
      i=0;
      //j=0;
      temp = 0.0;
      cam_x = 5;
      cam_y = 10;
      catapult_x = 190;
      catapult_y = 480;
      shoot_aiming = 0;
    }
    int convertX(float x) {
      return x * c + 1280/2 + cam_x * c;
    }
    int convertY(float y) {
      return -y * c + 720/2 + cam_y * c;
    }
    int convertDistance(float d) {
      return c * d;
    }
    float convertDistanceReverse(int d) {
      return d / c;
    }
    void run() {
      while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
            window.close();
          }
          if(event.type == sf::Event::Resized)
          {
              sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
              window.setView(sf::View(visibleArea));
          }
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
              window.close();
          }
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
              view.move(s,0);
              gx-=s;
          }
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
              view.move(-s,0);
              gx+=s;
          }
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
              view.move(0,s);
              gy-=s;
          }
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
              view.move(0,-s);
              gy+=s;
          }
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)){
              view.zoom(1.05f);
              i++;
              temp = zx*std::pow(1.05, i);
              view.move(temp, 0);
              // TODO fix the catapult coordinates when zooming
              /*j--;
              std::cout << "original catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
              catapult_x=catapult_x*std::pow(0.952308952381, (j/2));
              catapult_y=catapult_y*std::pow(0.952308952381, (j/2));
              std::cout << "changed catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
              */
          }
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Period)){
              view.zoom(0.952308952381);
              temp = -zx*std::pow(1.05, i);
              i--;
              view.move(temp, 0);
              // TODO fix the catapult coordinates when zooming
              /*j++;
              std::cout << "original catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
              catapult_x=catapult_x*std::pow(1.05, (j/2));
              catapult_y=catapult_y*std::pow(1.05, (j/2));
              std::cout << "changed catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
              */
          }
          if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Check if we are pressing near catapult
            /*if (event.mouseButton.x < convertX(m->getCatapultX()+0.5) and event.mouseButton.x > convertX(m->getCatapultX()-0.5) and
                event.mouseButton.y > convertY(m->getCatapultY()+0.5) and event.mouseButton.y < convertY(m->getCatapultY()-0.5))
            */
              std::cout << "catapult_x: " << catapult_x+gx << ", catapult_y: " << catapult_y+gy << ", mouse x: " 
                  << event.mouseButton.x << ", mouse y: " << event.mouseButton.y << std::endl;
              if (event.mouseButton.x < (catapult_x+gx+15) && event.mouseButton.x > (catapult_x+gx-15) &&
                event.mouseButton.y < (catapult_y+gy+15) && event.mouseButton.y > (catapult_y+gy-15))
              shoot_aiming = 1;
          }
          if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if ((!shoot_aiming) && (m->getCurrentBird() != nullptr)) {
                std::cout << "action event" << std::endl;
                m->getCurrentBird()->action(m);
            }
          }
          if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (shoot_aiming) {
              //std::cout << "Shot Bird" << std::endl;
              m->ShootBird(
                   /*(convertX(m->getCatapultX()) - event.mouseButton.x)/10.0,
                  -(convertY(m->getCatapultY()) - event.mouseButton.y)/10.0);*/
                  (catapult_x+gx - event.mouseButton.x)/10.0, -(catapult_y+gy - event.mouseButton.y)/10.0);

              shoot_aiming = 0;
              } 
          }
          if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
              delete m;
              m = new Map();
            }
          }
        }
        window.clear(/*sf::Color(160,160,255)*/);

        window.setView(view);
        window.draw(bg);
        // Drawing the ground
        sf::RectangleShape ground(sf::Vector2f(2560, 720 - convertY(0)));
        ground.setPosition(0, convertY(0));
        ground.setFillColor(sf::Color(29,76,34));
        window.draw(ground);

        // Drawing the catapult
        sf::CircleShape catapult_bg(convertDistance(0.5));
        catapult_bg.setFillColor(sf::Color(47,60,74));
        catapult_bg.setOrigin(convertDistance(0.5), convertDistance(0.5));
        //catapult_bg.setPosition(convertX(m->getCatapultX()), convertY(m->getCatapultY()));
        catapult_bg.setPosition(catapult_x, catapult_y);

        sf::CircleShape catapult(convertDistance(0.5));
        //set texture according to upcoming bird
        MoveableObject::Type currentType = m->getNextBirdType();
        if(currentType == MoveableObject::Type::BasicBird)
          catapult.setTexture(&kemma);
        if(currentType == MoveableObject::Type::BombBird)
          catapult.setTexture(&tik);
        if(currentType == MoveableObject::Type::BouncyBird)
          catapult.setTexture(&prodeko);
        if(currentType == MoveableObject::Type::TangentBird)
          catapult.setTexture(&pjk);
        if(currentType == MoveableObject::Type::Else)
          catapult.setFillColor(sf::Color(100,100,200));

        catapult.setOrigin(convertDistance(0.5), convertDistance(0.5));
        if (shoot_aiming) 
          catapult.setPosition(sf::Mouse::getPosition(window).x-gx, sf::Mouse::getPosition(window).y-gy);
        else
          //catapult.setPosition(convertX(m->getCatapultX()), convertY(m->getCatapultY()));
          catapult.setPosition(catapult_x, catapult_y);
        window.draw(catapult_bg);
        window.draw(catapult);
        
        // Drawing all movable objects
        std::vector<MoveableObject*> objects = m->getObjects();
        for(auto& i: objects)
        {
          size_t x = convertX(i->getX());
          size_t y = convertY(i->getY());
          MoveableObject::Type type = i->getType();
          if (type == MoveableObject::Type::BasicObstacle) { // Kuutio / Neliö
            sf::RectangleShape rect(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
            rect.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            if(i->getEnergy() > 0.75 && i->getEnergy() <= 1)
              rect.setTexture(&tile1);
            if(i->getEnergy() > 0.5 && i->getEnergy() <=0.75)
              rect.setTexture(&tile2);
            if(i->getEnergy() > 0.25 && i->getEnergy() <= 0.5)
              rect.setTexture(&tile3);
            if(i->getEnergy() > 0 && i->getEnergy() <= 0.25)
              rect.setTexture(&tile4);
            rect.setPosition(x,y);
            rect.setRotation(i->getAngle() * -57.295);
            window.draw(rect);
          }
          if (type == MoveableObject::Type::TNT) { // TNT
            sf::RectangleShape rect(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
            rect.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            rect.setTexture(&tnt);
            rect.setPosition(x,y);
            rect.setRotation(i->getAngle() * -57.295);
            window.draw(rect);
          }
          if (type == MoveableObject::Type::BlastBullet_t) { // Kuutio / Neliö
            sf::RectangleShape rect(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
            rect.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            rect.setFillColor(sf::Color(30, 30, 30));
            rect.setOutlineColor(sf::Color(15, 15, 15));
            rect.setOutlineThickness(2);
            rect.setPosition(x,y);
            rect.setRotation(i->getAngle() * -57.295);
            window.draw(rect);
          }
          if (type == MoveableObject::Type::BasicBird) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            circle.setTexture(&kemma);
            circle.setPosition(x,y);
            circle.setRotation(i->getAngle() * -57.295);
            window.draw(circle);
          } 
          if (type == MoveableObject::Type::BouncyBird) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            circle.setTexture(&prodeko);
            circle.setPosition(x,y);
            circle.setRotation(i->getAngle() * -57.295);
            window.draw(circle);
          }
          if (type == MoveableObject::Type::TangentBird) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            circle.setTexture(&pjk);
            circle.setPosition(x,y);
            circle.setRotation(i->getAngle() * -57.295);
            window.draw(circle);
          }
          if (type == MoveableObject::Type::BombBird) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            circle.setTexture(&tik);
            circle.setPosition(x,y);
            circle.setRotation(i->getAngle() * -57.295);
            window.draw(circle);
          }
          if (type == MoveableObject::BasicEnemy) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            circle.setTexture(&kone);
            circle.setPosition(x,y);
            circle.setRotation(i->getAngle() * -57.295);
            window.draw(circle);
          }

          //Draw energy of an object (for debugging purposes)
          bodyData* data =static_cast<bodyData*>(i->getBody()->GetUserData());
          if(data->hasEnergy)
          {
            std::ostringstream ss;
            ss << std::setprecision(2) << data->energy;
            sf::Text t;
            t.setPosition(x,y);
            t.setCharacterSize(20);
            t.setString(ss.str());
            t.setFont(font);
            window.draw(t);
          }
        }
        //Draw score, display and advance the simulation one step ahead
        std::ostringstream ss;
        ss << "Points: " << m->getScore() << std::endl << "Birds left: " << m->getBirdsLeft() << std::endl << "Enemies left: " << m->getEnemyCount();
        window.draw(sf::Text(ss.str(), font));
        window.display();
        m->Step();
      }
    }
};

#endif

