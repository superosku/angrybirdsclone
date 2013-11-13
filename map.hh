#ifndef MAP
#define MAP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <map>

#include <Box2D/Box2D.h>
#include "moveable.hh"
#include "birds.hh"
#include "hostiles.hh"
#include "CSVparser.hh"

class Map : public b2ContactListener {
  public:
    Map()
    {
      //Set cannon coordinates
      catapult_x = -9;
      catapult_y = 6;

      //Creating a world with gravity
      m_world = new b2World(b2Vec2(0.0f, -10.0f));

      //Create collision callback -> Box2D calls this instance of Map-class when contact happens
      m_world->SetContactListener(this);

      //Adding a default line to the world at 0-level so blocks dont fall freely
      b2BodyDef line_def;
      line_def.position.Set(0.0f, 0.0f);
      b2Body* line_body = m_world->CreateBody(&line_def);
      b2EdgeShape line_shape;
      line_shape.Set( b2Vec2(-50,0), b2Vec2(50,0) );
      line_body->CreateFixture(&line_shape, 0.0f);

      //Add some test structures
/*      objects.push_back(new BasicObstacle(m_world, 3, 2, 2, 0.5));
      objects.push_back(new BasicObstacle(m_world, 2, 1, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, -2, 1));
      objects.push_back(new BasicObstacle(m_world, -3.5, 5));
      objects.push_back(new BasicObstacle(m_world, 0, 1, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 2, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 3, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 4, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 5, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 6, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 7, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 9, 2, 2, 4));*/
      loadMap("basic_map.csv");
    }
    ~Map()
    {
      for(auto object: objects){
        delete static_cast<bodyData*>(object->body->GetUserData());
        delete object;
      }
      delete m_world; //This deletes all box2d stuff, movable object doesnt need to do anything about that
    }
    //Ro3
    Map& operator=(const Map&) = delete;
    Map(const Map&) = delete;

    void Step()
    {
      //Advance Box2D simulation by one step
      m_world->Step(1.0/60.0, 6, 2);

      //Removing of objects which have no energy left
      for(auto i=objects.begin();i!=objects.end();)
      {
        bodyData* bodyDataA =static_cast<bodyData*>((*i)->body->GetUserData());
        if(bodyDataA && ( (bodyDataA->hasEnergy && bodyDataA->energy <= 0 ) ||  bodyDataA->object->timer == 0 )){
         killCurrentBird(*i);
         delete *i;
         delete bodyDataA;
         i = objects.erase(i);
        }
        else if(bodyDataA && !bodyDataA->hasEnergy)
        {
         --bodyDataA->object->timer;
         ++i;
        }
        else
         ++i;
      }
    }
    void PreSolve(b2Contact* contact, const b2Manifold*)
    {
      bodyData* bodyDataA =static_cast<bodyData*>(contact->GetFixtureA()->GetBody()->GetUserData());
      bodyData* bodyDataB =static_cast<bodyData*>(contact->GetFixtureB()->GetBody()->GetUserData());
      if(bodyDataA)
       bodyDataA->object->velocity = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
      if(bodyDataB)
       bodyDataB->object->velocity = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
    }

    //Calculate score and new energies after the impact
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
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
       bodyDataA->object->timer = 300;
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
       bodyDataB->object->timer = 300;
      }
      totalScore += deltaEnergy;
     }
    }

    //This can be called to shoot the bird
    void ShootBird(float x, float y) {
      //BasicBird * bird = new BasicBird(m_world, catapult_x, catapult_y);
      if (birds.empty())
      {
        std::cout << "No birds left..." << std::endl;
        return;
      }
      else
      {
        current_b = birds.back();
        current_b->getBody()->SetActive(true);
        current_b->setImpulse(x, y);
        std::cout << "Shot Bird!" << std::endl;
        objects.push_back(current_b);
        birds.pop_back();
      }
    }

    float getCatapultX() {
      return catapult_x;
    }
    float getCatapultY() {
      return catapult_y;
    }
    size_t getScore() {
      return totalScore;
    }
    size_t getBirdsLeft() {
      return birds.size();
    }

    std::vector<MoveableObject*> getObjects() {return objects;}
    Bird* getCurrentBird() {return(current_b);}
    
    void killCurrentBird() {current_b = nullptr;std::cout << "killed current bird" << std::endl;}
    
    void killCurrentBird(MoveableObject* a)
    {
      if (a == current_b) {
        current_b = nullptr;
        std::cout << "killed current bird" << std::endl;
      }
    }
  private:
    //Should the dimensions of the world be saved? To ease changing the coordinate systems.
    //List of MoveableObjects currently present in the map
    void loadMap(std::string filepath)
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
            case (100):
              birds.push_back(new BasicBird(m_world, catapult_x, catapult_y, std::atof(tmpVec[5].c_str())));
              break;
            case (101):
              birds.push_back(new BouncyBird(m_world, catapult_x, catapult_y, std::atof(tmpVec[5].c_str())));
              break;
            case (200):
              objects.push_back(new BasicObstacle(m_world, std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str()), std::atof(tmpVec[3].c_str()), std::atof(tmpVec[4].c_str()),std::atof(tmpVec[5].c_str()),std::atof(tmpVec[6].c_str())));
              break;
            case (300):
              objects.push_back(new BasicEnemy(m_world, std::atof(tmpVec[1].c_str()), std::atof(tmpVec[2].c_str()),std::atof(tmpVec[5].c_str()),std::atof(tmpVec[6].c_str())));
              break;
          }
        }
      }
    }
    
    // TODO better validation...
    bool validSTRVEC(std::vector<std::string> vec)
    {
      try
      {
        if (vec.size() < 7)
        {
          std::cout << "Invalid line in map-file: Not enough columns." << std::endl;
          return(false);
        }
        size_t t_id = std::atoi(vec[0].c_str());
        
        for (auto i : valid[t_id])
        {
          std::atoi(vec[i].c_str());
          std::atof(vec[i].c_str());
        }
      }
      catch (...)
      {
        std::cout << "Invalid line in map-file: Data threw an error" << std::endl;
        return (false);
      }
      return (true);
    }
    
    std::vector<Bird*> birds;
    std::vector<MoveableObject*> objects;
    b2World* m_world;
    //b2Body* m_groundBody;
    Bird* current_b = nullptr;
    float totalScore=0;
    float catapult_x, catapult_y;
    std::map<size_t,std::vector<size_t>> valid = {{100,{0,5}},
                                                  {101,{0,5}},
                                                  {200,{0,1,2,3,4,5,6}},
                                                  {300,{0,1,2,5,6}}};
};

#endif
