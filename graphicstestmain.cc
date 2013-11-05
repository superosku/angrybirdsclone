#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "map.hh"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Test");
    sf::Texture bgt;
    sf::Sprite bg;
    sf::CircleShape circle(50);
    circle.setOrigin(50,50);
    sf::RectangleShape rect(sf::Vector2f(100, 100));
    sf::RectangleShape ground(sf::Vector2f(1280, 150));
    rect.setOrigin(50,50);
    circle.setFillColor(sf::Color(100, 250, 50));
    rect.setFillColor(sf::Color(100, 100, 100));
    ground.setFillColor(sf::Color(100, 250, 50));
    ground.setPosition(0, 570);
    bgt.loadFromFile("kentta.png");
    bg.setTexture(bgt);
    Map m;
    size_t c = 50;
    //ground is and 'object' too and should be created here instead of being static part of background
    
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
              */

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
