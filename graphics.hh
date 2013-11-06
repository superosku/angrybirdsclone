#ifndef G_H
#define G_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "map.hh"

class Graphics {
  private:
    sf::RenderWindow window;
    //sf::Texture bgt;
    //sf::Sprite bg;
    sf::Texture fox;
    sf::Texture sheep;
    sf::ContextSettings settings;
    Map m;
    // the constant used to change coordinates
    size_t c;
    // Things for camera movenment. In box2d meters
    // 0,0 means center of the screen
    float cam_x, cam_y;

  public:
    Graphics() : window(sf::VideoMode(1280, 720), "Game jou", sf::Style::Default/*, settings*/) {
      window.setFramerateLimit(60);
      //bgt.loadFromFile("kentta.png");
      //bg.setTexture(bgt);
      fox.loadFromFile("kettu.png");
      sheep.loadFromFile("lammas.png");
      fox.setSmooth(true);
      sheep.setSmooth(true);
      //settings.aliasingLevel = 8;
      c = 50;
      cam_x = 0;
      cam_y = 5;
    }
    size_t convertX(float x) {
      return x * c + 1280/2 + cam_x * c;
    }
    size_t convertY(float y) {
      return -y * c + 720/2 + cam_y * c;
    }
    size_t convertDistance(float d) {
      return c * d;
    }
    void run() {
      while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed)
            window.close();
        }
        window.clear(sf::Color(160,160,255));
        //window.draw(bg);
        sf::RectangleShape ground(sf::Vector2f(1280, 720 - convertY(0)));
        ground.setPosition(0, convertY(0));
        ground.setFillColor(sf::Color(100,255,100));
        window.draw(ground);
        
        std::vector<MoveableObject*> objects = m.getObjects();
      
        //TODO get type and radius/lengths of sides/etc
        for(auto& i: objects)
        {
          size_t x = convertX(i->getX());
          size_t y = convertY(i->getY());
          size_t type = i->getType();
          //getType
          //getImagepath

          if (type == 1) { // Kuutio / Neliö
            sf::RectangleShape rect(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
            rect.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            rect.setFillColor(sf::Color(255, 128, 64));
            rect.setPosition(x,y);
            rect.setRotation(i->getAngle() * -57.295);
            window.draw(rect);
          }
          if (type == 2) { // Pallo
            sf::CircleShape circle(convertDistance(i->getW()));
            circle.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
            //circle.setFillColor(sf::Color(200, 100, 100));
            circle.setTexture(&fox);
            circle.setPosition(x,y);
            circle.setRotation(i->getAngle() * -57.295);
            window.draw(circle);
          }
          /*if("pallo")
          i->getRadius
          //create new ball accordingly 
          else if("nelio")
          i->getXlength
          i->getYlength
          //create new square
          */

          //TODO get x and y
          //i->getX
          //i->getY

          //set position
          //rect.setPosition(x,y);
          //rect.setRotation(i->getAngle() * -57.295);
          //draw
          //window.draw(rect);
        }     
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

