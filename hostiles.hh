#ifndef HOSTILES
#define HOSTILES


#include <list>
//#include <pair>
#include <Box2D/Box2D.h>
#include "moveable.hh"
#include "map.hh"

//#define BREAKDOWN_R 2.5

class Hostile : public MoveableObject
{
  // Energy of the hostile is saved to box2d userdata, something from 100 to 1000
  public:
  Hostile(b2World* world, float x = 0.0f, float y = 0.0f, float w = 1.0f, float h = 1.0f, size_t energy = 10, MoveableObject::Type type = MoveableObject::Type::Else, float a = 0, float t = 300) : MoveableObject(world, x, y, w, h, type, energy, a, t) {}

  virtual ~Hostile() {}
  /*Hostile& operator=(const Hostile&) = delete;
  Hostile(const Hostile&) = delete;*/

  // Function to check if hostile has energy left after hit, returns energy after hit as size_t.
  
  // Function to perform special action of obstacles/enemies. Returns energy left after hit as size_t value.
  virtual size_t hit(mass_t)
  {
    /* DEMO:
     * For use in obstacles/enemies that have special action in case of hit or destruction.

    // Check if obstacle was destructed by hit (has zero energy).
    size_t new_energy = Hostile::hit(mass);
    if (new_energy)
    {
      // If something special happens when obstacle is hit: implement here.
    }
    else
    {
      // If something special happens when obstacle is destructed: implement here.
    }
    return (new_energy);
     */
    return 0;
  }
  
  virtual void destroy(Map*);
};

class Ground : public MoveableObject
{
  public:
    Ground(b2World* world, /*float x = 0.0f, float y = 0.0f, */std::list<std::pair<float, float>>& pointlist) : MoveableObject(world, 0.0, 0.0, 1.0, 1.0, MoveableObject::Ground, 0, 0, 0, false) {
      point_list = pointlist; // This is in MoveableObject. Set it so drawing can use pointlist

      b2Vec2 *vertices = new b2Vec2[point_list.size()];
      size_t counter = 0;
      for (auto &point : point_list) {
        vertices[counter].Set(point.first, point.second);
        counter ++;
      }
      
      b2PolygonShape shape;
      shape.Set(vertices, point_list.size());

      b2FixtureDef fixtureDef;
      fixtureDef.shape = &shape;
      fixtureDef.density = 1.0f;
      fixtureDef.friction = 0.3f;

      body->CreateFixture(&fixtureDef);

      delete [] vertices;
    }
  private:
};

class BasicObstacle : public Hostile
{
  public:
  BasicObstacle(b2World* world, float x = 0.0f, float y = 0.0f, float w = 1.0f, float h = 1.0f, float d = 1.0f, float e = 10,  MoveableObject::Type t = MoveableObject::Type::Brick) : Hostile(world, x, y, w, h, e, t)
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= d;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
  }

};

class TNT : public Hostile
{
  public:
  TNT(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.5f, float h = 0.5f, float d = 10.0f, float e = 1) : Hostile(world, x, y, w, h, e, MoveableObject::Type::TNT)
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= d;
    fixtureDef.friction = 1.5f;

    body->CreateFixture(&fixtureDef);
  }
  
  void destroy(Map* m);
};

class Smoke : public Hostile
{
  public:
  Smoke(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.05f, float h = 0.05f) : Hostile(world, x, y, w, h,0,MoveableObject::Type::Smoke_t,0,60+std::rand()%20*15) 
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 3.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(-0.1);
    body->SetLinearDamping(0.9);
    //body->SetGravityScale(0.0f);
  }
  virtual void destroy(Map*){}
};


class BlastBullet : public Hostile
{
  public:
  BlastBullet(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.05f, float h = 0.05f) : Hostile(world, x, y, w, h,0,MoveableObject::Type::BlastBullet_t,0,120+std::rand()%20*15) 
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 3.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    //body->SetGravityScale(-0.1);
    //body->SetLinearDamping(0.9);
    //body->SetGravityScale(0.0f);
  }
  virtual void destroy(Map* m){
    Smoke* tmp = new Smoke(this->body->GetWorld(),this->getX(),this->getY());
    //tmp->setImpulse(25*cos(a*i),25*sin(a*i));
    m->addObject(tmp);
  }
};



class Particle : public Hostile
{
  public:
  Particle(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.05f, float h = 0.05f, MoveableObject::Type t = MoveableObject::Type::Else) : Hostile(world, x, y, w, h,0,t,0,180+std::rand()%20*15) 
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
  }

  virtual void destroy(Map*){}
};

class BasicEnemy : public Hostile
{
  public:
  BasicEnemy(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f,float e = 1) : Hostile(world, x, y, 0.5f, 0.5f, e, MoveableObject::Type::BasicEnemy)
  {
    b2CircleShape shape;
    shape.m_radius = 0.5f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= d;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
  }
  virtual ~BasicEnemy() {}
  virtual void destroy(Map*){}
};

#endif
