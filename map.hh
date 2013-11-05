#ifndef MAP
#define MAP

#include <vector>
//#include <Box2D/Box2D.h>
#include <Box2D/Box2D.h>
#include "moveable.hh"
#include "birds.hh"
#include "hostiles.hh"

class Map {
  public:
    Map()
    {
      //TODO:
      //Creation of the Box2D world and ground
      //Creation of birds to the objects variable

      //TODO:
      //Load map (firstly just some background?) from file -> If file does not exists load default map. (Default map could be hardcoded to help test process.)
      

      //Creating a world with gravity
      m_world = new b2World(b2Vec2(0.0f, -10.0f));

      //Adding a default line to the world at 0-level so blocks dont fall freely
      b2BodyDef line_def;
      line_def.position.Set(0.0f, 0.0f);
      b2Body* line_body = m_world->CreateBody(&line_def);

      b2EdgeShape line_shape;
      line_shape.Set( b2Vec2(-50,0), b2Vec2(50,0) );

      line_body->CreateFixture(&line_shape, 0.0f);

      //BasicBird* b = new BasicBird(m_world);
      
      objects.push_back(new BasicBird(m_world));
      objects.push_back(new BasicBird(m_world, 3.1, 10));
      objects.push_back(new BasicObstacle(m_world, 3, 2, 2, 0.5));
      objects.push_back(new BasicObstacle(m_world, 2, 1, 0.5, 0.5));
      objects.push_back(new BasicObstacle(m_world, -2, 1));
      objects.push_back(new BasicObstacle(m_world, -3.5, 5));
     

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

    std::vector<MoveableObject*> getObjects() {return objects;}
  private:
    //Should the dimensions of the world be saved? To ease changing the coordinate systems.
    //List of MoveableObjects currently present in the map
    std::vector<MoveableObject*> objects;
    b2World* m_world;
    //b2Body* m_groundBody;
    size_t totalScore=0;
};

#endif
