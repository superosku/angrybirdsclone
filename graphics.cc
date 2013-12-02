#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>

#include "map.hh"
#include "moveable.hh"
#include "hostiles.hh"
#include "birds.hh"
#include "graphics.hh"

Graphics::Graphics() : window(sf::VideoMode(WINDOW_W, WINDOW_H), "Angry birds clone", sf::Style::Default/*, settings*/) {
  //m = new Map(currentMapPath);
  window.setFramerateLimit(60);
  view.reset(sf::FloatRect(0, 0, WINDOW_W, WINDOW_H));
  //if an error occurs withing loadFromFile, SFML tells about it in the console
  std::vector<std::string> imgDir = readDir("images");
  ssize_t n = imgDir.size();//!(imgDir.size() % 2) ? imgDir.size() : imgDir.size() + 1;
  n -= n*3/2;//(n+1)/2;

  sf::Vector2f center = view.getCenter();
  sf::Vector2f inc(10.0f,0.0f);
  sf::Vector2f size = view.getSize();
  sf::Vector2f leftEdge = center-size/2.0f;
  sf::Vector2f bsize(10.0f,10.0f);

  window.setView(view);

  //background
  sf::RectangleShape backg(size);
  backg.setPosition(leftEdge);
  backg.setFillColor(sf::Color(0,0,0));
  window.draw(backg);
  window.display();

  for(auto i: imgDir)
  {
   sf::RectangleShape tmp(bsize);
   tmp.setPosition(center+(float)(n++)*inc);
   tmp.setFillColor(sf::Color(255,255,255));
   window.draw(tmp);
   window.display();

   for(auto j: readDir("images/"+i)){
    sf::Texture * k = new sf::Texture;
    k->loadFromFile("images/"+i+"/"+j);
    k->setSmooth(1);
    textures[std::atoi(i.c_str())].push_back(k);
   }
  }
  font.loadFromFile("QuinzeNarrow.ttf");

  //Background is still loaded statictly
  amfi1.loadFromFile("images/amfi1.png");window.display();
  amfi2.loadFromFile("images/amfi2.png");window.display();
  amfi3.loadFromFile("images/amfi3.png");window.display();
  amfi4.loadFromFile("images/amfi4.png");window.display();

  //window.setView(view);

  c = 32;
  s = 25;
  z=0;
  cam_x = 5;
  cam_y = 5;
  catapult_x = convertX(-20);
  catapult_y = convertY(6);
  shoot_aiming = 0;

     for(auto i: readDir("maps")){
       sf::Text t(i,font);
       t.setPosition(0,0);
       maps.push_back(t);
     }

}



//This method draws all moveable objects -> it calls drawCircle- and drawSquare-methods to do the real drawing in most cases
void Graphics::drawMoveableObjects() {
  std::vector<MoveableObject*> objects = m->getObjects();
  for(auto& i: objects)
  {
    MoveableObject::Type type = i->getType();
    switch(type)
    {
       case(MoveableObject::Type::BasicBird):
         /*drawCircle(textures[type],i);
         break;*/
       case(MoveableObject::Type::BouncyBird):
         /*drawCircle(textures[type],i);
         break;*/
       case(MoveableObject::Type::TangentBird):
         /*drawCircle(textures[type],i);
         break;*/
       case(MoveableObject::Type::BombBird):
         /*drawCircle(textures[type],i);
         break;*/
       case(MoveableObject::Type::GravityBird):
         /*drawCircle(textures[type],i);
         break;*/
       case(MoveableObject::Type::BasicEnemy):
         /*drawCircle(textures[type],i);
         break;*/
       case(MoveableObject::Type::Gear_t):
         /*drawCircle(textures[type],i);
         break;*/
       case(MoveableObject::Type::Smoke_t):
         drawCircle(textures[type],i);
         break;
       case(MoveableObject::Type::Brick):
         /*drawSquare(textures[type],i);
         break;*/
       case(MoveableObject::Type::Glass):
         /*drawSquare(textures[type],i);
         break;*/
       case(MoveableObject::Type::Wood):
         /*drawSquare(textures[type],i);
         break;*/
       case(MoveableObject::Type::TNT):
         drawSquare(textures[type],i);
         break;
       case(MoveableObject::Ground):
       {
         sf::ConvexShape shape;
         shape.setPointCount(i->getPointList().size());
         size_t counter = 0;
         for (auto &point : i->getPointList()) {
           shape.setPoint(counter, sf::Vector2f(convertX(point.first), convertY(point.second)));
           counter ++;
         }
         shape.setFillColor(sf::Color(128, 64, 36));
         window.draw(shape);
         break;
       }
       default:
         sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
         shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
         shape.setFillColor(sf::Color(30, 30, 30));
         shape.setOutlineColor(sf::Color(15, 15, 15));
         shape.setOutlineThickness(2);
         shape.setPosition(convertX(i->getX()),convertY(i->getY()));
         shape.setRotation(i->getAngle() * -57.295);
         window.draw(shape);
         break;
    }
  //Draw energy of an object (for debugging purposes)
  bodyData* data =static_cast<bodyData*>(i->getBody()->GetUserData());
  if(data->hasEnergy)
  {
    std::ostringstream ss;
    ss << std::setprecision(2) << data->energy;
    sf::Text t;
    //t.setPosition(x,y);
    t.setPosition(convertX(i->getX()),convertY(i->getY()));
    t.setCharacterSize(20);
    t.setString(ss.str());
    t.setFont(font);
    window.draw(t);
  }


  }
}


//Draw all circle shaped objects
void Graphics::drawCircle(std::vector<sf::Texture*> texture, MoveableObject* i){
    int k = std::min(std::max(z/2,0),(int)texture.size()-1);
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(texture[k]);
    shape.setPosition(convertX(i->getX()),convertY(i->getY()));
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
 /*      std::ostringstream ss;
    ss << std::setprecision(2) << k;
    sf::Text t;
    t.setPosition(convertX(i->getX()),convertY(i->getY()));
    t.setCharacterSize(20);
    t.setString(ss.str());
    t.setFont(font);
    window.draw(t);*/
}

//Draw all square shaped objects (except )
void Graphics::drawSquare(std::vector<sf::Texture*> texture, MoveableObject* i){
    sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(texture.size() > 1){
      if(i->getEnergy() > 0.75 && i->getEnergy() <= 1)
        shape.setTexture(texture[0]);
      if(i->getEnergy() > 0.5 && i->getEnergy() <=0.75)
        shape.setTexture(texture[1]);
      if(i->getEnergy() > 0.25 && i->getEnergy() <= 0.5)
        shape.setTexture(texture[2]);
      if(i->getEnergy() > 0 && i->getEnergy() <= 0.25)
        shape.setTexture(texture[3]);
    } else
      shape.setTexture(texture[0]);
    shape.setPosition(convertX(i->getX()),convertY(i->getY()));
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
}

//This method will handle all events during game
void Graphics::pollGameEvents() {
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
    if(event.type == sf::Event::KeyPressed){
      //ESC to menu
      if(event.key.code == sf::Keyboard::Escape){
        if(m->isWin())
          saveHighScore("maps/" + maps[currentMapI].getString());
        phase = gamePhase::Menu;
        delete m;
        m = nullptr;
      }
      //Arrows to move view
      if(event.key.code == sf::Keyboard::Right){
        xDelta +=s;
      }
      if(event.key.code == sf::Keyboard::Left){
        xDelta -= s;
      }
      if(event.key.code == sf::Keyboard::Down){
        yDelta += s;
      }
      if(event.key.code == sf::Keyboard::Up){
        yDelta -=s;
      }
      //R & F5 to reload map
      if(event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::F5) {
        view = defaultView;
        delete m;
        m = new Map("maps/" + maps[currentMapI].getString());
      }
      if(event.key.code == sf::Keyboard::D) {
        std::cout << "End: " << m->isEnd() << " Win: " << m->isWin() << std::endl;
      }
    }
    //the zooming with keys or mouse wheel
    if(zoomDelta < 50 && ( (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Comma) || ( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta < 0))){
        zoomDelta +=s;
        z--;
    }
    if( zoomDelta > -50 && ( (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Period) || ( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0))){
        zoomDelta -=s;
        z++;
    }
    //shooting the birds
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse =window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),view);
        if (mouse.x < catapult_x+15 && mouse.x > catapult_x-15 && mouse.y < catapult_y+15 && mouse.y > catapult_y-15)
         shoot_aiming = 1;
    }
    if(event.type == sf::Event::MouseButtonReleased){
      if (event.mouseButton.button == sf::Mouse::Left) {
        if ((!shoot_aiming) && (m->getCurrentBird() != nullptr)) {
          std::cout << "action event" << std::endl;
          m->getCurrentBird()->action(m);
        }
      }
      if (event.mouseButton.button == sf::Mouse::Left) {
        if (shoot_aiming) {
          sf::Vector2f location = catapult.getPosition();
          //std::cout << "Shot Bird" << std::endl;
          m->ShootBird((catapult_x - location.x)/10.0, -(catapult_y - location.y)/10.0);
          shoot_aiming = 0;
        }
      }
    }
  }
  int k = 10*(z > 0? z : 1) ;
  yDelta = std::min(yDelta,50);
  xDelta = std::min(xDelta,50);
  if(xDelta > 0 || yDelta > 0)
   view.move(xDelta > 0 ? 5*xDelta/k: 0,yDelta > 0 ? 5*yDelta/k:0);
  else if(xDelta < 0 || yDelta < 0)
   view.move(xDelta < 0 ? 5*xDelta/k: 0,yDelta < 0 ? 5*yDelta/k:0);
  view.zoom( zoomDelta > 0 ? std::pow(1.01f,zoomDelta/10) : std::pow(0.99f,-zoomDelta/10)  );
  if (xDelta >0)
   --xDelta;
  if (xDelta <0)
   ++xDelta;
  if (yDelta >0)
   --yDelta;
  if (yDelta <0)
   ++yDelta;
  if(zoomDelta > 0)
   --zoomDelta;
  if(zoomDelta < 0)
   ++zoomDelta;
}


//Draw all objects which location cannot be changed from map-file
void Graphics::drawUnmoveable() {
  // Drawing the air and ground
  sf::Vector2f center = view.getCenter();
  sf::Vector2f size = view.getSize();
  sf::Vector2f leftEdge = center-size/2.0f;
  sf::Vector2f rightEdge = center+size/2.0f;

  sf::RectangleShape ground3(size);
  ground3.setPosition(leftEdge);
  ground3.setFillColor(sf::Color(113,218,226));
  window.draw(ground3);
  sf::RectangleShape ground2(sf::Vector2f(size.x, rightEdge.y));
  ground2.setPosition(leftEdge.x,430);
  ground2.setFillColor(sf::Color(0,104,55));
  window.draw(ground2);
  sf::RectangleShape ground(sf::Vector2f(size.x, rightEdge.y));
  ground.setPosition(leftEdge.x, 520);
  ground.setFillColor(sf::Color(29,76,34));
  window.draw(ground);
  sf::RectangleShape bg(sf::Vector2f(2560, 720));
  if(z>5)
      bg.setTexture(&amfi4);
  else if(z>3 && z<=5)
      bg.setTexture(&amfi3);
  else if(z>1 && z<=3)
      bg.setTexture(&amfi2);
  else if(z<=1)
      bg.setTexture(&amfi1);
  bg.setPosition(0,0);

  // Drawing the background
  window.draw(bg);

  // Drawing the catapult
  sf::CircleShape catapult_bg(convertDistance(0.5));
  catapult_bg.setFillColor(sf::Color(80,225,249));
  catapult_bg.setOrigin(convertDistance(0.5), convertDistance(0.5));
  catapult_bg.setPosition(catapult_x, catapult_y);

  catapult.setRadius(convertDistance(0.5));
  //Set texture according to upcoming bird
  MoveableObject::Type nextType = m->getNextBirdType();
  if ( nextType && textures.find(nextType) != textures.end() ) {
    int k = std::min(std::max(z,0),(int)textures[nextType].size()-1);
    catapult.setTexture(textures[nextType][k],1);
  }
  if(nextType == MoveableObject::Type::Else)
    catapult.setFillColor(sf::Color(0,0,0));
  else
    catapult.setFillColor(sf::Color(255,255,255));

  catapult.setOrigin(convertDistance(0.5), convertDistance(0.5));
  if (shoot_aiming){
    sf::Vector2f mouse =window.mapPixelToCoords(sf::Mouse::getPosition(window),view);
    sf::Vector2f location = sf::Vector2f(catapult_x,catapult_y);
    float d = std::sqrt(std::pow(mouse.x -location.x,2)+std::pow(mouse.y -location.y,2));
    if(d > MAX_FORCE)
     catapult.setPosition(location.x + MAX_FORCE*(mouse.x -location.x)/d, location.y+ MAX_FORCE*(mouse.y -location.y)/d);
    else
     catapult.setPosition(mouse.x,mouse.y);
  }
  else
    catapult.setPosition(catapult_x, catapult_y);
  window.draw(catapult_bg);
  window.draw(catapult);
}


//Method to read all filenames in a directory
std::vector<std::string> Graphics::readDir(std::string dirToRead){
  DIR *d;
  struct dirent *dir;
  std::vector<std::string> dirlist;
  d = opendir(dirToRead.c_str());
  if(d){
    while ((dir = readdir(d)) != NULL)
    {
      if(dir->d_name[0] != '.')
        dirlist.push_back(dir->d_name);
    }
    closedir(d);
  }
  std::sort(dirlist.begin(),dirlist.end());
  return dirlist;
}

