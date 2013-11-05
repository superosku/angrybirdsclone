#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "map.hh"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Test");
    sf::Texture bgt;
    sf::Sprite bg;
    bgt.loadFromFile("kentta.png");
    bg.setTexture(bgt);
    Map m;

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

        std::vector<MoveableObject*> objects = m.getObjects();
      
        //TODO get type and radius/lengths of sides/etc
        for(auto& i: objects)
        {
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
            //draw
        }     

	window.display();
    }
    return 0;
}
