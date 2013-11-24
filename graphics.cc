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


Graphics::Graphics() : window(sf::VideoMode(WINDOW_W, WINDOW_H), "Game jou", sf::Style::Default/*, settings*/) {
  m = new Map(currentMapPath);
  window.setFramerateLimit(60);
  view.reset(sf::FloatRect(0, 0, WINDOW_W, WINDOW_H));
  if(!font.loadFromFile("QuinzeNarrow.ttf")) {
    //Throw error or something
  }
  if(!kemma.loadFromFile("images/kemma.png")){
    //Throw error
  }
  if(!kone.loadFromFile("images/kone.png")){
    //Throw error    
  }
  if(!kone100.loadFromFile("images/kone100.png")){
    //Throw error    
  } 
  if(!kone900.loadFromFile("images/kone900.png")){
    //Throw error    
  }
  if(!kone250.loadFromFile("images/kone250.png")){
    //Throw error    
  } 
  if(!kone500.loadFromFile("images/kone500.png")){
    //Throw error    
  }
  if(!prodeko.loadFromFile("images/prodeko.png")){
      //Throw error
  }
  if(!pjk.loadFromFile("images/puu.png")){
      //Throw error
  }
  if(!tik.loadFromFile("images/tik.png")){
      //Throw error
  }
  if(!tile1.loadFromFile("images/tiili1.png", sf::IntRect(0, 0, 400, 400))){
      //Throw error
  }
  if(!tile2.loadFromFile("images/tiili2.png", sf::IntRect(0, 0, 400, 400))){
      //Throw error
  }
  if(!tile3.loadFromFile("images/tiili3.png", sf::IntRect(0, 0, 400, 400))){
      //Throw error
  }
  if(!tile4.loadFromFile("images/tiili4.png", sf::IntRect(0, 0, 400, 400))){
      //Throw error
  }
  if(!tnt.loadFromFile("images/tnt.png")){
      //Throw error
  }
  if(!bgt.loadFromFile("images/bg.png")){
      //Throw error
  }
  if(!ratas.loadFromFile("images/hammasratas.png")){
      //Throw error
  }
  if(!smoke.loadFromFile("images/smoke.png")) {
      //THROW ERROR!?!?!?!?!
  }

  kemma.setSmooth(true);
  kone.setSmooth(true);
  kone100.setSmooth(true);
  kone250.setSmooth(true);
  kone500.setSmooth(true);
  kone900.setSmooth(true);
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
  smoke.setSmooth(true);
  //settings.antialiasingLevel = 8;
  bg.setTexture(bgt);
  window.setView(view);

  c = 32;
  s = 25;
  // meaning of life
  // okay, not really, the constant used for zoom corrections
  zx = 30.4761904762;
  z=0;
  //j=0;
  temp = 0.0;
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
  if (type == MoveableObject::Type::TNT) { // TNT
    sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(&tnt);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BlastBullet_t) { // Kuutio / Neliö
    //sf::RectangleShape shape(sf::Vector2f(convertDistance(2), convertDistance(2)));
    sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));

    //shape.setOrigin(convertDistance(1), convertDistance(1));
    //shape.setTexture(&smoke);
    shape.setFillColor(sf::Color(30, 30, 30));
    shape.setOutlineColor(sf::Color(15, 15, 15));
    shape.setOutlineThickness(2);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::Smoke_t) { // Kuutio / Neliö
    sf::RectangleShape shape(sf::Vector2f(convertDistance(2), convertDistance(2)));
    //sf::RectangleShape shape(sf::Vector2f(convertDistance(i->getW()) * 2, convertDistance(i->getH()) * 2));
    //shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));

    shape.setOrigin(convertDistance(1), convertDistance(1));
    shape.setTexture(&smoke);
    //shape.setFillColor(sf::Color(30, 30, 30));
    //shape.setOutlineColor(sf::Color(15, 15, 15));
    //shape.setOutlineThickness(2);
    shape.setPosition(x,y);
    //shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::Gear_t) { // hammasratas
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(&ratas);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BasicBird) { // Pallo
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(&kemma);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BouncyBird) { // Pallo
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(&prodeko);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::TangentBird) { // Pallo
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(&pjk);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::Type::BombBird) { // Pallo
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    shape.setTexture(&tik);
    shape.setPosition(x,y);
    shape.setRotation(i->getAngle() * -57.295);
    window.draw(shape);
  }
  if (type == MoveableObject::BasicEnemy) { // Pallo
    sf::CircleShape shape(convertDistance(i->getW()));
    shape.setOrigin(convertDistance(i->getW()), convertDistance(i->getH()));
    if(z>45)
        shape.setTexture(&kone900);
    else if(z>30 && z<=45) 
        shape.setTexture(&kone500);
    else if(z>15 && z<=30) 
        shape.setTexture(&kone250);
    else if(z>5 && z<=15) 
        shape.setTexture(&kone100);
    else if(z<=5)
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
          view.move(s,0);
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
          view.move(-s,0);
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
          view.move(0,s);
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
          view.move(0,-s);
    }
    if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::F5)) {
        view = defaultView;
        delete m;
        m = new Map(currentMapPath);
    }
    /*if(event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0)
        view.setCenter(window.mapPixelToCoords(sf::Vector2i(event.mouseWheel.x,event.mouseWheel.y),view));*/
    if( (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Comma) || ( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta < 0)){
        view.zoom(1.05f);
        z--;
        //temp = zx*std::pow(1.05, i);
        //view.move(temp, 0);
        // TODO fix the catapult coordinates when zooming
        /*j--;
        std::cout << "original catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
        catapult_x=catapult_x*std::pow(0.952308952381, (j/2));
        catapult_y=catapult_y*std::pow(0.952308952381, (j/2));
        std::cout << "changed catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
        */
    }
    
    if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Period) || ( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0)){
        view.zoom(0.952308952381);
        //temp = -zx*std::pow(1.05, i);
        z++;
        //view.move(temp, 0);
        // TODO fix the catapult coordinates when zooming
        /*j++;
        std::cout << "original catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
        catapult_x=catapult_x*std::pow(1.05, (j/2));
        catapult_y=catapult_y*std::pow(1.05, (j/2));
        std::cout << "changed catapult x: " << catapult_x << ", catapult y: " << catapult_y << std::endl;
        */
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse =window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),view);
        std::cout << "catapult_x: " << catapult_x << ", catapult_y: "<< catapult_y << ", mouse x: " << mouse.x << ", mouse y: " << mouse.y << std::endl;
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
