#ifndef G_H
#define G_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "map.hh"

class Graphics {
  private:
    sf::RenderWindow window;
    sf::Texture bgt;
    sf::Sprite bg;
    Map m;
    //the constant used to change coordinates
    size_t c;

  public:
    Graphics() : window(sf::VideoMode(1280, 720), "Game jou") {
      window.setFramerateLimit(60);
      c = 50;
    }
    void run() {
      bgt.loadFromFile("kentta.png");
      bg.setTexture(bgt);
      while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed)
            window.close();
        }
        window.clear();
        window.draw(bg);
        
        std::vector<MoveableObject*> objects = m.getObjects();
      
        //TODO get type and radius/lengths of sides/etc
        for(auto& i: objects)
        {
          size_t x = i->getX()*c + 12*c;
          size_t y = i->getY()*(-1)*c + 12*c;
          size_t type = i->getType();
          //getType
          //getImagepath

          if (type == 1) { // Kuutio / Neliö
            sf::RectangleShape rect(sf::Vector2f(i->getW()*c*2, i->getH()*c*2));
            rect.setOrigin(i->getW()*c, i->getH()*c);
            rect.setFillColor(sf::Color(100, 100, 100));
            rect.setPosition(x,y);
            rect.setRotation(i->getAngle() * -57.295);
            window.draw(rect);
          }
          if (type == 2) { // Pallo
            sf::CircleShape circle(i->getW()*c);
            circle.setOrigin(i->getW()*c, i->getH()*c);
            circle.setFillColor(sf::Color(200, 100, 100));
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

              /*if("pallo")
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

