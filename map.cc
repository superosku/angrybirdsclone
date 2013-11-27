#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <map>
#include <random>
#include <list>
//#include <pair>
#include <Box2D/Box2D.h>

#include "moveable.hh"
#include "hostiles.hh"
#include "birds.hh"
#include "CSVparser.hh"

#include "map.hh"

Map::Map(std::string path)
{
  //Set cannon coordinates
  catapult_x = -20;
  catapult_y = 6;

  //Creating a world with gravity
  m_world = new b2World(b2Vec2(0.0f, -10.0f));

  //Create collision callback -> Box2D calls this instance of Map-class when contact happens
  m_world->SetContactListener(this);
  m_world->SetContactFilter(this);
  //Adding a default line to the world at 0-level so blocks dont fall freely
  b2BodyDef myBodyDef;
  myBodyDef.position.Set(0.0f, -2.0f);
  myBodyDef.type = b2_staticBody;
  b2Body* myBody = m_world->CreateBody(&myBodyDef);

  b2PolygonShape shape;
  shape.SetAsBox(500, 2);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  myBody->CreateFixture(&fixtureDef);

  loadMap(path);
}

Map::~Map()
{
  for(auto i: objects){
    delete i;
  }
  for(auto i: birds){
    delete i;
  }
  for(auto i: objectsQueue){
    delete i;
  }
  delete m_world; //This deletes all box2d stuff, movable object doesnt need to do anything about that
}

void Map::Step()
{
  //Advance Box2D simulation by one step
  m_world->Step(1.0/60.0, 6, 2);
  enemiesLeft=0;
  //Removing of objects which have no energy left
  for(auto i=objects.begin();i!=objects.end();)
  {
    if((*i)->type == MoveableObject::Type::BasicEnemy)
      ++enemiesLeft;
    bodyData* data =static_cast<bodyData*>((*i)->body->GetUserData());
    if(data && ( (data->hasEnergy && data->energy <= 0 ) ||  ( data->timerEnabled && (*i)->timer == 0))){
      //(*i)->getBody()->SetActive(false);
      (*i)->destroy(this);
      killCurrentBird(*i);
      delete *i;
      //delete data;
      i = objects.erase(i);
      continue;
    }
    else if(data && !data->hasEnergy)
      --(*i)->timer;
    ++i;
  }
  for (auto i : objectsQueue)
    objects.push_back(i);
  objectsQueue.clear();
}

void Map::PreSolve(b2Contact* contact, const b2Manifold*)
{
  bodyData* bodyDataA =static_cast<bodyData*>(contact->GetFixtureA()->GetBody()->GetUserData());
  bodyData* bodyDataB =static_cast<bodyData*>(contact->GetFixtureB()->GetBody()->GetUserData());
  if(bodyDataA)
    bodyDataA->object->velocity = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
  if(bodyDataB)
    bodyDataB->object->velocity = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
}

void Map::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
  if(impulse->normalImpulses[0] > 1)
  {
    float maxImpulse = 0.0f;
    for (int i = 0; i < contact->GetManifold()->pointCount; ++i)
    {
      maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
    }
    //If FixtureA has energy, calculate lost energy according to impulse strength and add that to totalscore
    bodyData* bodyDataA =static_cast<bodyData*>(contact->GetFixtureA()->GetBody()->GetUserData());
    bodyData* bodyDataB =static_cast<bodyData*>(contact->GetFixtureB()->GetBody()->GetUserData());
    float deltaEnergy=0;
    if(bodyDataA && bodyDataA->hasEnergy)
    {
      deltaEnergy = maxImpulse * 0.1;
      bodyDataA->energy =  bodyDataA->energy > deltaEnergy? bodyDataA->energy - deltaEnergy: 0 ;
      if(bodyDataB && !bodyDataA->energy)
        contact->GetFixtureB()->GetBody()->SetLinearVelocity(bodyDataB->object->velocity);
    }
    else if (bodyDataA)
    {
      bodyDataA->object->timer = 200 + std::rand()%10*30;
    }

    totalScore += deltaEnergy;

    deltaEnergy=0;
    //If FixtureB has energy, calculate lost energy according to impulse strength and add that to totalscore
    if(bodyDataB && bodyDataB->hasEnergy)
    {
      deltaEnergy = maxImpulse * 0.1;
      bodyDataB->energy =  bodyDataB->energy > deltaEnergy? bodyDataB->energy - deltaEnergy: 0 ;
      if(bodyDataA && !bodyDataB->energy)
        contact->GetFixtureA()->GetBody()->SetLinearVelocity(bodyDataA->object->velocity);
    }
    else if (bodyDataB)
    {
      bodyDataB->object->timer = 200 + std::rand()%10*30;
    }
    totalScore += deltaEnergy;
  }
}

MoveableObject::Type Map::getNextBirdType() const {
  if (birds.empty())
    return Bird::Type::Else;
  return birds.back()->getType();
}

void Map::ShootBird(float x, float y)
{
  //BasicBird * bird = new BasicBird(m_world, catapult_x, catapult_y);
  if (birds.empty())
  {
    std::cout << "No birds left..." << std::endl;
    return;
  }

  current_b = birds.back();
  current_b->getBody()->SetActive(true);
  current_b->setImpulse(x, y);
  std::cout << "Shot Bird!" << std::endl;
  objects.push_back(current_b);
  birds.pop_back();
}

void Map::addObject(MoveableObject* o)
{
  objectsQueue.push_back(o);
}

void Map::removeObject(MoveableObject* o)
{
  auto erase_iter = std::remove(objects.begin(),objects.end(),o);
  objects.erase(erase_iter);
  //delete o;
}

void Map::loadMap(std::string filepath)
{
  std::string tmpStr;
  std::vector<std::string> tmpVec;
  std::ifstream input(filepath);
  // First line is for map parameters
  std::getline(input,tmpStr);
  // Object_id,x,y,w,h,d,energy,image_path
  while (std::getline(input,tmpStr))
  {
    tmpVec = toSTRVEC(tmpStr);
    if (validSTRVEC(tmpVec))
    {
      switch(std::atoi(tmpVec[0].c_str()))
      {
        case (MoveableObject::Type::BasicBird):
          birds.push_back(new BasicBird(m_world, catapult_x, catapult_y, std::atof(tmpVec[5].c_str())));
          break;
        case (MoveableObject::Type::BouncyBird):
          birds.push_back(new BouncyBird(m_world, catapult_x, catapult_y, std::atof(tmpVec[5].c_str())));
          break;
        case (MoveableObject::Type::GravityBird):
          birds.push_back(new GravityBird(m_world, catapult_x, catapult_y, std::atof(tmpVec[5].c_str())));
          break;
        case (MoveableObject::Type::TangentBird):
          birds.push_back(new TangentBird(m_world, catapult_x, catapult_y, std::atof(tmpVec[5].c_str())));
          break;
        case (MoveableObject::Type::BombBird):
          birds.push_back(new BombBird(m_world, catapult_x, catapult_y, std::atof(tmpVec[5].c_str())));
          break;
        case (MoveableObject::Type::Brick):
          objects.push_back(new BasicObstacle(m_world, std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str()), std::atof(tmpVec[3].c_str()), std::atof(tmpVec[4].c_str()),std::atof(tmpVec[5].c_str()),std::atof(tmpVec[6].c_str())));
          break;
        case (MoveableObject::Type::Glass):
          objects.push_back(new BasicObstacle(m_world, std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str()), std::atof(tmpVec[3].c_str()), std::atof(tmpVec[4].c_str()),std::atof(tmpVec[5].c_str()),std::atof(tmpVec[6].c_str()),MoveableObject::Type::Glass));
          break;
        case (MoveableObject::Type::Wood):
          objects.push_back(new BasicObstacle(m_world, std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str()), std::atof(tmpVec[3].c_str()), std::atof(tmpVec[4].c_str()),std::atof(tmpVec[5].c_str()),std::atof(tmpVec[6].c_str()),MoveableObject::Type::Wood));
          break;
        case (MoveableObject::Type::TNT):
          objects.push_back(new TNT(m_world, std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str())/*, std::atof(tmpVec[3].c_str()), std::atof(tmpVec[4].c_str()),std::atof(tmpVec[5].c_str()),std::atof(tmpVec[6].c_str()))*/));
          break;
        case (MoveableObject::Type::BasicEnemy):
          objects.push_back(new BasicEnemy(m_world, std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str()),std::atof(tmpVec[5].c_str()),std::atof(tmpVec[6].c_str())));
          break;
        case (MoveableObject::Type::Ground):
          std::list<std::pair<float, float>> temp_list;
          for (size_t i = 4; i < tmpVec.size(); i += 2) {
            std::pair<float, float> temp_pair;
            temp_pair.second = std::atof(tmpVec[i].c_str());
            temp_pair.first = std::atof(tmpVec[i-1].c_str());
            std::cout << "Adding: " << temp_pair.first << " " << temp_pair.second << std::endl;
            temp_list.push_back(temp_pair);
          }
          objects.push_back(new Ground(m_world, temp_list /*std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str()), */));
          break;
      }
    }
  }
}
