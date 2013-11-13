#ifndef G_H
#define G_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "map.hh"
#include "moveable.hh"

class Graphics {
  private:
    sf::RenderWindow window;
    //sf::Texture bgt;
    //sf::Sprite bg;
    sf::Font font;
    sf::Texture fox;
    sf::Texture sheep;
    sf::ContextSettings settings;
    Map m;
    // the constant used to change coordinates
    int c;
    // Things for camera movenment. In box2d meters
    // 0,0 means center of the screen
    float cam_x, cam_y;
    // Mouse pos. needed for cannon
    size_t shoot_aiming;

  public:
    Graphics() : window(sf::VideoMode(1280, 720), "Game jou", sf::Style::Default/*, settings*/) {
      window.setFramerateLimit(60);
      //bgt.loadFromFile("kentta.png");
      //bg.setTexture(bgt);
      if (!font.loadFromFile("QuinzeNarrow.ttf")) {
        //Throw error or something
      }
      if(!fox.loadFromFile("kettu.png")){
        //Throw error
      }
      if(!sheep.loadFromFile("lammas.png")){
        //Throw error
      }
      fox.setSmooth(true);
      sheep.setSmooth(true);
      //settings.aliasingLevel = 8;
      c = 50;
      cam_x = 0;
      cam_y = 5;
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
          if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Check if we are pressing near catapult
            if (event.mouseButton.x < convertX(m.getCatapultX()+0.5) and event.mouseButton.x > convertX(m.getCatapultX()-0.5) and
                event.mouseButton.y > convertY(m.getCatapultY()+0.5) and event.mouseButton.y < convertY(m.getCatapultY()-0.5))
              shoot_aiming = 1;
          }
          if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if ((!shoot_aiming) && (m.getCurrentBird() != nullptr)) {
                std::cout << "action event" << std::endl;
                m.getCurrentBird()->action(m);
            }
          }
          if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (shoot_aiming) {
              //std::cout << "Shot Bird" << std::endl;
              m.ShootBird(
                   (convertX(m.getCatapultX()) - event.mouseButton.x)/10.0,
                  -(convertY(m.getCatapultY()) - event.mouseButton.y)/10.0);
              shoot_aiming = 0;
              }
          }
        }
        window.clear(sf::Color(160,160,255));

        // Drawing the ground
        sf::RectangleShape ground(sf::Vector2f(1280, 720 - convertY(0)));
        ground.setPosition(0, convertY(0));
        ground.setFillColor(sf::Color(100,255,100));
        window.draw(ground);

        // Drawing the catapult
        sf::CircleShape catapult_bg(convertDistance(0.5));
        catapult_bg.setFillColor(sf::Color(100,100,200));
        catapult_bg.setOrigin(convertDistance(0.5), convertDistance(0.5));
        catapult_bg.setPosition(convertX(m.getCatapultX()), convertY(m.getCatapultY()));

        sf::CircleShape catapult(convertDistance(0.5));
        catapult.setTexture(&fox);
        catapult.setOrigin(convertDistance(0.5), convertDistance(0.5));
        if (shoot_aiming) 
          catapult.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        else
          catapult.setPosition(convertX(m.getCatapultX()), convertY(m.getCatapultY()));
        window.draw(catapult_bg);
        window.draw(catapult);
        
        // Drawing current bird.
        /*if (m.getCurrentBird() != nullptr) {
          Bird* i = m.getCurrentBird();
          size_t x = convertX(i->getX());
          size_t y = convertY(i->getY());
          sf::CircleShape circle(convertDistance(i->getW()));
          circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
          //circle.setFillColor(sf::Color(200, 100, 100));
          circle.setTexture(&fox);
          circle.setPosition(x,y);
          circle.setRotation(i->getAngle() * -57.295);
          window.draw(circle);
        }*/
        
        // Drawing all movable objects
        std::vector<MoveableObject*> objects = m.getObjects();
        //std::cout << "jou" << std::endl;
        for(auto& i: objects)
        {
          size_t x = convertX(i->getX());
          size_t y = convertY(i->getY());
          MoveableObject::Type type = i->getType();
          //getType
          //getImagepath
          if (type == MoveableObject::Type::BasicObstacle) { // Kuutio / Neliö
            sf::RectangleShape rect(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
            rect.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            rect.setFillColor(sf::Color(255, 128, 64));
            rect.setOutlineColor(sf::Color(128, 64, 32));
            rect.setOutlineThickness(2);
            rect.setPosition(x,y);
            rect.setRotation(i->getAngle() * -57.295);
            window.draw(rect);
          }
          if (type == MoveableObject::BasicBird) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            //circle.setFillColor(sf::Color(200, 100, 100));
            circle.setTexture(&fox);
            circle.setPosition(x,y);
            circle.setRotation(i->getAngle() * -57.295);
            window.draw(circle);
          }
          if (type == MoveableObject::BasicEnemy) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            //circle.setFillColor(sf::Color(200, 100, 100));
            circle.setTexture(&sheep);
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
        ss << "Points: " << m.getScore() << std::endl << "Birds left: " << m.getBirdsLeft() << std::endl << "Enemies left: " << m.getEnemyCount();
        window.draw(sf::Text(ss.str(), font));
        window.display();
        m.Step();
      }
    }
};

/*
int main()
{
    // Declare and create a new window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Game");
    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);

    sf::Texture bgt;
    sf::Sprite bg;
   
    sf::CircleShape circle(50);
    circle.setOrigin(50,50);
    sf::RectangleShape rect(sf::Vector2f(100, 100));
    //ground is and 'object' too and should be created here instead of being static part of background
    sf::RectangleShape ground(sf::Vector2f(1280, 150));
    
    rect.setOrigin(50,50);
    circle.setFillColor(sf::Color(100, 250, 50));
    rect.setFillColor(sf::Color(100, 100, 100));
    ground.setFillColor(sf::Color(100, 250, 50));
    ground.setPosition(0, 570);
    
    bgt.loadFromFile("kentta.png");
    bg.setTexture(bgt);
    Map m;
    
    //the constant used to change coordinates
    size_t c = 50;

    while(window.isOpen())
    {
	sf::Event event;
	while(window.pollEvent(event))
	{
	    if(event.type == sf::Event::Closed)
		window.close();
	}
	window.clear();
	window.draw(bg);
        //draw ground
        window.draw(ground);

        std::vector<MoveableObject*> objects = m.getObjects();
      
        //TODO get type and radius/lengths of sides/etc
        for(auto& i: objects)
        {
            size_t x = i->getX()*c + 12*c;
            size_t y = i->getY()*(-1)*c + 8*c;
            //getType
            //getImagepath

              *if("pallo")
                  i->getRadius
                  //create new ball accordingly 
                else if("nelio")
                  i->getXlength
                  i->getYlength
                  //create new square
              /

            //TODO get x and y
            //i->getX
            //i->getY
	
            //set position
            rect.setPosition(x,y);
            rect.setRotation(i->getAngle() * -57.295);
            //draw
            window.draw(rect);
        }     

	window.display();
        m.Step();
    }
    return 0;
}
*/

#endif

