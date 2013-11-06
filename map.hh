#ifndef MAP
#define MAP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
//#include <Box2D/Box2D.h>
#include <Box2D/Box2D.h>
#include "moveable.hh"
#include "birds.hh"
#include "hostiles.hh"

class Map : public b2ContactListener {
  public:
    Map()
    {
      //Set cannon coordinates
      catapult_x = -9;
      catapult_y = 6;

      //Creating a world with gravity
      m_world = new b2World(b2Vec2(0.0f, -10.0f));

      //Create collision callback -> Box2D calls this instance of map-class when contact happens
      m_world->SetContactListener(this);

      //Adding a default line to the world at 0-level so blocks dont fall freely
      b2BodyDef line_def;
      line_def.position.Set(0.0f, 0.0f);
      b2Body* line_body = m_world->CreateBody(&line_def);
      b2EdgeShape line_shape;
      line_shape.Set( b2Vec2(-50,0), b2Vec2(50,0) );
      line_body->CreateFixture(&line_shape, 0.0f);

      //Add some test structures
      objects.push_back(new BasicObstacle(m_world, 3, 2, 2, 0.5));
      objects.push_back(new BasicObstacle(m_world, 2, 1, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, -2, 1));
      objects.push_back(new BasicObstacle(m_world, -3.5, 5));
      objects.push_back(new BasicObstacle(m_world, 0, 1, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 2, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 3, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 4, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 0, 5, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, 9, 2, 2, 4));

    }
    ~Map()
    {
      for(auto object: objects)
        delete object;
      delete m_world; //This deletes all box2d stuff, movable object doesnt need to do anything about that
    }
    //Ro3
    Map& operator=(const Map&) = delete;
    Map(const Map&) = delete;

    void Step()
    {
      m_world->Step(1.0/60.0, 6, 2);
    }

    //Calculate score and new energies after the impact
    void BeginContact(b2Contact* contact)
    {
      //If FixtureA has energy, calculate lost energy according to masses and add that to totalscore
      bodyData* bodyDataA =static_cast<bodyData*>(contact->GetFixtureA()->GetBody()->GetUserData());
      bodyData* bodyDataB =static_cast<bodyData*>(contact->GetFixtureB()->GetBody()->GetUserData());
      int deltaEnergy=0;
      if(bodyDataA)
      {
        deltaEnergy = contact->GetFixtureB()->GetBody()->GetMass() / contact->GetFixtureA()->GetBody()->GetMass() * bodyDataA->energy * 0.1;
        bodyDataA->energy -= deltaEnergy;
      }
      totalScore += deltaEnergy;

      //If FixtureB has energy, calculate lost energy according to masses and add that to totalscore
      if(bodyDataB)
      {
        deltaEnergy = contact->GetFixtureA()->GetBody()->GetMass() / contact->GetFixtureB()->GetBody()->GetMass() * bodyDataB->energy * 0.1;
        bodyDataB->energy -= deltaEnergy;
      }
      totalScore += deltaEnergy;
      //if energy is <=0 remove object from map
      std::cout << "tatsi!" << std::endl;
    }

    //We do nothing when contact ends
    void EndContact(b2Contact*) {}

    //This can be called to shoot the bird
    void ShootBird(float force, float angle) {
      BasicBird * bird = new BasicBird(m_world, catapult_x, catapult_y);
      bird->setImpulse(10,-2);
      objects.push_back(bird);
    }

    float getCatapultX() {
      return catapult_x;
    }
    float getCatapultY() {
      return catapult_y;
    }

    std::vector<MoveableObject*> getObjects() {return objects;}
  private:
    //Should the dimensions of the world be saved? To ease changing the coordinate systems.
    //List of MoveableObjects currently present in the map
    void loadMap(std::string filepath)
    {
      std::string tmp;
      std::ifstream input(filepath);
      // First line is for map parameters
      std::getline(input,tmp);
      while (std::getline(input,tmp))
      {
        // TODO
      }
    }
    
    std::vector<MoveableObject*> objects;
    b2World* m_world;
    //b2Body* m_groundBody;
    size_t totalScore=0;
    float catapult_x, catapult_y;
};

#endif
