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
  m = new Map(currentMapPath);
  window.setFramerateLimit(60);
  view.reset(sf::FloatRect(0, 0, WINDOW_W, WINDOW_H));
  //if an error occurs withing loadFromFile, SFML tell about it in the console
  font.loadFromFile("QuinzeNarrow.ttf");
  kemma.loadFromFile("images/kemma.png");
  kemma100.loadFromFile("images/kemma100.png");
  kemma250.loadFromFile("images/kemma250.png");
  kemma500.loadFromFile("images/kemma500.png");
  kemma900.loadFromFile("images/kemma900.png");
  kone.loadFromFile("images/kone.png");
  kone100.loadFromFile("images/kone100.png");
  kone900.loadFromFile("images/kone900.png");
  kone250.loadFromFile("images/kone250.png");
  kone500.loadFromFile("images/kone500.png");
  pjk.loadFromFile("images/puu.png");
  pjk100.loadFromFile("images/puu100.png");
  pjk250.loadFromFile("images/puu250.png");
  pjk500.loadFromFile("images/puu500.png");
  pjk900.loadFromFile("images/puu900.png");
  prodeko.loadFromFile("images/prodeko.png");
  prodeko100.loadFromFile("images/prodeko100.png");
  prodeko250.loadFromFile("images/prodeko250.png");
  prodeko500.loadFromFile("images/prodeko500.png");
  prodeko900.loadFromFile("images/prodeko900.png");
  taffa.loadFromFile("images/taffa.png");
  taffa100.loadFromFile("images/taffa100.png");
  taffa250.loadFromFile("images/taffa250.png");
  taffa500.loadFromFile("images/taffa500.png");
  taffa900.loadFromFile("images/taffa900.png");
  tik.loadFromFile("images/tik.png");
  tik100.loadFromFile("images/tik100.png");
  tik250.loadFromFile("images/tik250.png");
  tik500.loadFromFile("images/tik500.png");
  tik900.loadFromFile("images/tik900.png");
  tile1.loadFromFile("images/tiili1.png", sf::IntRect(0, 0, 400, 400));
  tile2.loadFromFile("images/tiili2.png", sf::IntRect(0, 0, 400, 400));
  tile3.loadFromFile("images/tiili3.png", sf::IntRect(0, 0, 400, 400));
  tile4.loadFromFile("images/tiili4.png", sf::IntRect(0, 0, 400, 400));
  wood1.loadFromFile("images/wood1.png");//, sf::IntRect(0, 0, 400, 400));
  wood2.loadFromFile("images/wood2.png");//, sf::IntRect(0, 0, 400, 400));
  wood3.loadFromFile("images/wood3.png");//, sf::IntRect(0, 0, 400, 400));
  wood4.loadFromFile("images/wood4.png");//, sf::IntRect(0, 0, 400, 400));
  tnt.loadFromFile("images/tnt.png");
  amfi1.loadFromFile("images/amfi1.png");
  amfi2.loadFromFile("images/amfi2.png");
  amfi3.loadFromFile("images/amfi3.png");
  amfi4.loadFromFile("images/amfi4.png");
  ratas.loadFromFile("images/hammasratas.png");
  ratas50.loadFromFile("images/hammasratas50.png");
  ratas100.loadFromFile("images/hammasratas100.png");
  ratas250.loadFromFile("images/hammasratas250.png");
  smoke.loadFromFile("images/smoke.png");

  kemma.setSmooth(true);
  kemma100.setSmooth(true);
  kemma250.setSmooth(true);
  kemma500.setSmooth(true);
  kemma900.setSmooth(true);
  kone.setSmooth(true);
  kone100.setSmooth(true);
  kone250.setSmooth(true);
  kone500.setSmooth(true);
  kone900.setSmooth(true);
  prodeko.setSmooth(true);
  prodeko100.setSmooth(true);
  prodeko250.setSmooth(true);
  prodeko500.setSmooth(true);
  prodeko900.setSmooth(true);
  pjk.setSmooth(true);
  pjk100.setSmooth(true);
  pjk250.setSmooth(true);
  pjk500.setSmooth(true);
  pjk900.setSmooth(true);
  taffa.setSmooth(true);
  taffa100.setSmooth(true);
  taffa250.setSmooth(true);
  taffa500.setSmooth(true);
  taffa900.setSmooth(true);
  tik.setSmooth(true);
  tik100.setSmooth(true);
  tik250.setSmooth(true);
  tik500.setSmooth(true);
  tik900.setSmooth(true);
  tile1.setSmooth(true);
  tile2.setSmooth(true);
  tile3.setSmooth(true);
  tile4.setSmooth(true);
  wood1.setSmooth(true);
  wood2.setSmooth(true);
  wood3.setSmooth(true);
  wood4.setSmooth(true);
  tnt.setSmooth(true);
  amfi1.setSmooth(true);
  amfi2.setSmooth(true);
  amfi3.setSmooth(true);
  amfi4.setSmooth(true);
  ratas.setSmooth(true);
  ratas50.setSmooth(true);
  ratas100.setSmooth(true);
  ratas250.setSmooth(true);
  smoke.setSmooth(true);
  //settings.antialiasingLevel = 8;
  window.setView(view);

  c = 32;
  s = 25;
  z=0;
  cam_x = 5;
  cam_y = 5;
  catapult_x = convertX(-20);
  catapult_y = convertY(6);
  shoot_aiming = 0;
}


void Graphics::drawMoveableObject(MoveableObject *i) {
  int x = convertX(i->getX());
  int y = convertY(i->getY());
  MoveableObject::Type type = i->getType();
  if (type == MoveableObject::Type::Brick) { // Kuutio / Neliö
    sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(i->getEnergy() > 0.75 && i->getEnergy() <= 1)
      shape.setTexture(&tile1);
    if(i->getEnergy() > 0.5 && i->getEnergy() <=0.75)
      shape.setTexture(&tile2);
    if(i->getEnergy() > 0.25 && i->getEnergy() <= 0.5)
      shape.setTexture(&tile3);
    if(i->getEnergy() > 0 && i->getEnergy() <= 0.25)
      shape.setTexture(&tile4);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::Wood) { // Kuutio / Neliö
    sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(i->getEnergy() > 0.75 && i->getEnergy() <= 1)
      shape.setTexture(&wood1);
    if(i->getEnergy() > 0.5 && i->getEnergy() <=0.75)
      shape.setTexture(&wood2);
    if(i->getEnergy() > 0.25 && i->getEnergy() <= 0.5)
      shape.setTexture(&wood3);
    if(i->getEnergy() > 0 && i->getEnergy() <= 0.25)
      shape.setTexture(&wood4);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::TNT) { // TNT
    sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(&tnt);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BlastBullet_t) { // Kuutio / Neliö
    sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setFillColor(sf::Color(30, 30, 30));
    shape.setOutlineColor(sf::Color(15, 15, 15));
    shape.setOutlineThickness(2);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::Smoke_t) { // savu
    sf::RectangleShape shape(sf::Vector2f(convertDistance(2), convertDistance(2)));
    shape.setOrigin(convertDistance(1), convertDistance(1));
    shape.setTexture(&smoke);
    shape.setPosition(x,y);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::Gear_t) { // hammasratas
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>5)
        shape.setTexture(&ratas250);
    else if(z>3 && z<=5) 
        shape.setTexture(&ratas100);
    else if(z>1 && z<=3) 
        shape.setTexture(&ratas50);
    else if(z<=1)
        shape.setTexture(&ratas);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BasicBird) { // Pallo, taffa
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>6)
        shape.setTexture(&taffa900);
    else if(z>4 && z<=6) 
        shape.setTexture(&taffa500);
    else if(z>2 && z<=4) 
        shape.setTexture(&taffa250);
    else if(z>1 && z<=2) 
        shape.setTexture(&taffa100);
    else if(z<=1)
        shape.setTexture(&taffa);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BouncyBird) { // Pallo, prodeko
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>6)
        shape.setTexture(&prodeko900);
    else if(z>4 && z<=6) 
        shape.setTexture(&prodeko500);
    else if(z>2 && z<=4) 
        shape.setTexture(&prodeko250);
    else if(z>1 && z<=2) 
        shape.setTexture(&prodeko100);
    else if(z<=1)
        shape.setTexture(&prodeko);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::TangentBird) { // Pallo, pjk
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>6)
        shape.setTexture(&pjk900);
    else if(z>4 && z<=6) 
        shape.setTexture(&pjk500);
    else if(z>2 && z<=4) 
        shape.setTexture(&pjk250);
    else if(z>1 && z<=2)
        shape.setTexture(&pjk100);
    else if(z<=1)
        shape.setTexture(&pjk);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BombBird) { // Pallo, tik
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>6)
        shape.setTexture(&tik900);
    else if(z>4 && z<=6) 
        shape.setTexture(&tik500);
    else if(z>2 && z<=4) 
        shape.setTexture(&tik250);
    else if(z>1 && z<=2) 
        shape.setTexture(&tik100);
    else if(z<=1)
        shape.setTexture(&tik);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::GravityBird) { // Pallo, tik
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>6)
        shape.setTexture(&kemma900);
    else if(z>4 && z<=6) 
        shape.setTexture(&kemma500);
    else if(z>2 && z<=4) 
        shape.setTexture(&kemma250);
    else if(z>1 && z<=2) 
        shape.setTexture(&kemma100);
    else if(z<=1)
        shape.setTexture(&kemma);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::BasicEnemy) { // Pallo, kik
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>6)
        shape.setTexture(&kone900);
    else if(z>4 && z<=6) 
        shape.setTexture(&kone500);
    else if(z>2 && z<=4) 
        shape.setTexture(&kone250);
    else if(z>1 && z<=2) 
        shape.setTexture(&kone100);
    else if(z<=1)
        shape.setTexture(&kone);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Ground) { // Maa
    sf::ConvexShape shape;
    shape.setPointCount(i->getPointList().size());
    size_t counter = 0;
    for (auto &point : i->getPointList()) {
      shape.setPoint(counter, sf::Vector2f(convertX(point.first), convertY(point.second)));
      counter ++;
    }
    shape.setFillColor(sf::Color(128, 64, 36));
    window.draw(shape);
  }

  //Draw energy of an object (for debugging purposes)
  /*bodyData* data =static_cast<bodyData*>(i->getBody()->GetUserData());
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
*/
}
void Graphics::drawMoveableObjects() {
  // Drawing all movable objects
  std::vector<MoveableObject*> objects = m->getObjects();
  for(auto& i: objects)
  {
    drawMoveableObject(i);
  }
}


void Graphics::pollEvents() {
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
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
        window.close();
    }
    if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
        xDelta +=s;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
        xDelta -= s;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
        yDelta += s;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
        yDelta -=s;
    }
    if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::F5)) {
        view = defaultView;
        delete m;
        m = new Map(currentMapPath);
    }
    if( zoomDelta < 50 && ( (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Comma) || ( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta < 0))){
        zoomDelta +=s;
        z--;
    }
    if( zoomDelta > -50 && ( (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Period) || ( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0))){
        zoomDelta -=s;
        z++;
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse =window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),view);
        if (mouse.x < catapult_x+15 && mouse.x > catapult_x-15 && mouse.y < catapult_y+15 && mouse.y > catapult_y-15)
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
        sf::Vector2f location = catapult.getPosition();
        //std::cout << "Shot Bird" << std::endl;
        m->ShootBird((catapult_x - location.x)/10.0, -(catapult_y - location.y)/10.0);
        shoot_aiming = 0;
      }
    }
  }
  int k = 10*(z > 0? z : 1) ;
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
  //catapult_bg.setPosition(convertX(m->getCatapultX()), convertY(m->getCatapultY()));
  catapult_bg.setPosition(catapult_x, catapult_y);

  catapult.setRadius(convertDistance(0.5));
  //set texture according to upcoming bird
  MoveableObject::Type nextType = m->getNextBirdType();
  if(nextType == MoveableObject::Type::BasicBird) {
    if(z>6)
        catapult.setTexture(&taffa900, 1);
    else if(z>4 && z<=6) 
        catapult.setTexture(&taffa500, 1);
    else if(z>2 && z<=4) 
        catapult.setTexture(&taffa250, 1);
    else if(z>1 && z<=2) 
        catapult.setTexture(&taffa100, 1);
    else if(z<=1)
        catapult.setTexture(&taffa, 1);
  } 
  if(nextType == MoveableObject::Type::BombBird) {
    if(z>6)
        catapult.setTexture(&tik900, 1);
    else if(z>4 && z<=6) 
        catapult.setTexture(&tik500, 1);
    else if(z>2 && z<=4) 
        catapult.setTexture(&tik250, 1);
    else if(z>1 && z<=2) 
        catapult.setTexture(&tik100, 1);
    else if(z<=1)
        catapult.setTexture(&tik, 1);
  }
  if(nextType == MoveableObject::Type::BouncyBird) {
    if(z>6)
        catapult.setTexture(&prodeko900, 1);
    else if(z>4 && z<=6) 
        catapult.setTexture(&prodeko500, 1);
    else if(z>2 && z<=4) 
        catapult.setTexture(&prodeko250, 1);
    else if(z>1 && z<=2)
        catapult.setTexture(&prodeko100, 1);
    else if(z<=1)
        catapult.setTexture(&prodeko, 1);
  }
  if(nextType == MoveableObject::Type::TangentBird) {
    if(z>6)
        catapult.setTexture(&pjk900, 1);
    else if(z>4 && z<=6) 
        catapult.setTexture(&pjk500, 1);
    else if(z>2 && z<=4) 
        catapult.setTexture(&pjk250, 1);
    else if(z>1 && z<=2) 
        catapult.setTexture(&pjk100, 1);
    else if(z<=1)
        catapult.setTexture(&pjk, 1);
  }
  if(nextType == MoveableObject::Type::GravityBird) {
    if(z>6)
        catapult.setTexture(&kemma900, 1);
    else if(z>4 && z<=6) 
        catapult.setTexture(&kemma500, 1);
    else if(z>2 && z<=4) 
        catapult.setTexture(&kemma250, 1);
    else if(z>1 && z<=2) 
        catapult.setTexture(&kemma100, 1);
    else if(z<=1)
        catapult.setTexture(&kemma, 1);
  }
  if(nextType == MoveableObject::Type::Else)
    catapult.setFillColor(sf::Color(0,0,0));

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

std::vector<std::string> Graphics::readDir(std::string dirToRead){
  DIR *d;
  struct dirent *dir;
  std::vector<std::string> dirlist;
  d = opendir(dirToRead.c_str());
  if(d){
    while ((dir = readdir(d)) != NULL)
    {
      dirlist.push_back(dir->d_name);
    }
    for(size_t i=0;i<dirlist.size();i++)
          std::cout<<dirlist[i]<<std::endl;
    closedir(d);
  }
  return dirlist;
}
