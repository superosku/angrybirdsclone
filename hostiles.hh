#ifndef HOSTILES
#define HOSTILES

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
  
  virtual void destroy(Map*);/*
  {
    b2World* world = body->GetWorld();
    float x = getX();
    float y = getY();
    float w = getW();
    float h = getH();
    float a = getAngle();
    
    for (ssize_t i = -BREAKDOWN_N_X; i < BREAKDOWN_N_X; i++)
    {
      for (ssize_t j = -BREAKDOWN_N_Y; j < BREAKDOWN_N_Y; j++)
      {
        if ((i == 0) || (j == 0)) continue;
        
        float p_x = x + cos(a)*(i*w/(BREAKDOWN_N_X*2)-w/(BREAKDOWN_N_X*4));
        float p_y = y + sin(a)*(j*h/(BREAKDOWN_N_Y*2)-h/(BREAKDOWN_N_Y*4));
        if (p_x == x) p_x += i*w/(BREAKDOWN_N_X*2)-w/(BREAKDOWN_N_X*4);
        if (p_y == y) p_y += j*h/(BREAKDOWN_N_Y*2)-h/(BREAKDOWN_N_Y*4);
        float p_w = w/(BREAKDOWN_N_X*2);
        float p_h = h/(BREAKDOWN_N_Y*2);
        
        Particle* tmp = new Particle(world,p_x,p_y,p_w,p_h,a,getType());
        m->addObject(tmp);
      }
    }*/
  /*Particle(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.05f, float h = 0.05f, float a = 0, MoveableObject::Type t)
    
    b2World* world = body->GetWorld();
    
    float x = getX();
    float y = getY();
    
    for (size_t i = 0; i < N_BULLETS; ++i)
    {
      float a = 2*PI/N_BULLETS;
      
      BlastBullet* tmp = new BlastBullet(world,x,y);
      tmp->setImpulse(50*cos(a*i),50*sin(a*i));
      
      m->addObject(tmp);
    }
  
  m->killCurrentBird(this);
  m->removeObject(this);
  //delete this;
  //bodyData* bData =static_cast<bodyData*>(body->GetUserData());
  timer = 0;
  }*/
};

class BasicObstacle : public Hostile
{
  // No private stuff yet :(

  public:
  BasicObstacle(b2World* world, float x = 0.0f, float y = 0.0f, float w = 1.0f, float h = 1.0f, float d = 1.0f, float e = 10) : Hostile(world, x, y, w, h, e, MoveableObject::Type::BasicObstacle)  
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= d;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
  }

  // virtual ~BasicObstacle();
};

class TNT : public Hostile
{
  public:
  TNT(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.5f, float h = 0.5f, float d = 10.0f, float e = 1) : Hostile(world, x, y, w, h, e, MoveableObject::Type::BasicObstacle)
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= d;
    fixtureDef.friction = 1.5f;

    body->CreateFixture(&fixtureDef);
  }
  
  void destroy(Map* m);/*
  {
    b2World* world = body->GetWorld();
    
    float x = getX();
    float y = getY();
    
    std::cout << "Blast at " << x << ", " << y << "." << std::endl;
    
    body->SetActive(false);
    
    for (size_t i = 0; i < N_BULLETS; ++i)
    {
      float a = 2*PI/N_BULLETS;
      
      BlastBullet* tmp = new BlastBullet(world,(x+0.2*cos(a*i)),(y+0.2*sin(a*i)));
      tmp->setImpulse(50*cos(a*i),50*sin(a*i));
      
      m->addObject(tmp);
    }
    m->killCurrentBird(this);
    m->removeObject(this);
    timer = 0;
  }*/
};

class BlastBullet : public Hostile
{
  public:
  BlastBullet(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.05f, float h = 0.05f) : Hostile(world, x, y, w, h,0,MoveableObject::Type::BlastBullet_t,0,180+((int)(120*x) % 120)) 
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 5.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    //body->SetGravityScale(0.0f);
  }
  virtual void destroy(Map*){}
};

class Particle : public Hostile
{
  public:
  Particle(b2World* world, float x = 0.0f, float y = 0.0f, float w = 0.05f, float h = 0.05f, MoveableObject::Type t = MoveableObject::Type::Else) : Hostile(world, x, y, w, h,0,t,0,180+((int)(120*x) % 120)) 
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
