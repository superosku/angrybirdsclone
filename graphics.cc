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
#include "graphics.hh"


Graphics::Graphics() : window(sf::VideoMode(1280, 720), "Game jou", sf::Style::Default/*, settings*/),window_w(1280),window_h(720) {
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
  if(!smoke.loadFromFile("smoke.png")) {
      //THROW ERROR!?!?!?!?!
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
  smoke.setSmooth(true);
  //settings.antialiasingLevel = 8;
  bg.setTexture(bgt);
  window.setView(view);

  c = 32;
  s = 25;
  gx = 0;
  gy = 0;
  // meaning of life
  // okay, not really, the constant used for zoom corrections
  zx = 30.4761904762;
  i=0;
  //j=0;
  temp = 0.0;
  cam_x = 5;
  cam_y = 5;
  catapult_x = convertX(-20);
  catapult_y = convertY(6);
  shoot_aiming = 0;
}


void Graphics::drawMoveableObject(MoveableObject *i) {
  size_t x = convertX(i->getX());
  size_t y = convertY(i->getY());
  MoveableObject::Type type = i->getType();
  if (type == MoveableObject::Type::BasicObstacle) { // Kuutio / Neliö
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
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
        window_w=event.size.width;
        window_h=event.size.height;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
        window.close();
    }
/*
    sf::Vector2f translation = view.getCenter() + view.getSize() / 2.0f;
    if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
        if(translation.x + s < bg.getGlobalBounds().width){
          view.move(s,0);
          gx-=s;
        }
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
        if(translation.x - s > 0 ){
          view.move(-s,0);
          gx+=s;
        }
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
        if(i<0 && translation.y + s < 720 ){
          view.move(0,s);
          gy-=s;
        }
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
        if(i<0 && translation.y - s > 0 ){
          view.move(0,-s);
          gy+=s;
        }
    }*/
    sf::Vector2f center = view.getCenter() - view.getSize() / 2.0f;
    sf::Vector2f corner = view.getCenter() + view.getSize() / 2.0f;
    //std::cout << center.x << " " << center.y << " " << corner.x << " " << corner.y << std::endl;
    if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
        if(corner.x + s < bg.getGlobalBounds().width){
          view.move(s,0);
          gx-=s;
        }
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
        if(center.x - s >= 0 ){
          view.move(-s,0);
          gx+=s;
        }
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
        if(i<0 && corner.y + s <= 720 ){
          view.move(0,s);
          gy-=s;
        }
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
        if(i<0 && center.y - s >= 0 ){
          view.move(0,-s);
          gy+=s;
        }
    }
    if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::F5)) {
        window.setView(window.getDefaultView());
        delete m;
        m = new Map();
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Comma){
        if(i<0){
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
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Period){
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
        if (event.mouseButton.x < (catapult_x+gx+15)*window_w/1280 && event.mouseButton.x > (catapult_x+gx-15)*window_w/1280 &&
          event.mouseButton.y < (catapult_y+gy+15)*window_h/720 && event.mouseButton.y > (catapult_y+gy-15)*window_h/720)
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
            ((catapult_x+gx)*window_w/1280 - event.mouseButton.x)/10.0*1280/window_w, -((catapult_y+gy)*window_h/720 - event.mouseButton.y)/10.0*720/window_h);

        shoot_aiming = 0;
      }
    }
  }
}


void Graphics::drawUnmoveable() {
  // Drawing the background
  window.draw(bg);
  // Drawing the ground
  sf::RectangleShape ground(sf::Vector2f(2560, 720 - convertY(0)));
  ground.setPosition(0, convertY(0));
  ground.setFillColor(sf::Color(29,76,34));
  window.draw(ground);

  // Drawing the catapult
  sf::CircleShape catapult_bg(convertDistance(0.5));
  catapult_bg.setFillColor(sf::Color(80,225,249));
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
    catapult.setFillColor(sf::Color(0,0,0));

  catapult.setOrigin(convertDistance(0.5), convertDistance(0.5));
  if (shoot_aiming)
    catapult.setPosition((sf::Mouse::getPosition(window).x)*1280/window_w-gx, (sf::Mouse::getPosition(window).y)*720/window_h-gy);
    //catapult.setPosition((sf::Mouse::getPosition(window).x)*1280/window_w, (sf::Mouse::getPosition(window).y)*720/window_h);
  else
    //catapult.setPosition(convertX(m->getCatapultX()), convertY(m->getCatapultY()));
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
