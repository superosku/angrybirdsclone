#ifndef BIRDS
#define BIRDS

//#include "map.hh"
#include "moveable.hh"
#include "hostiles.hh" // BlastBullet

#define N_BULLETS 32
#define PI 3.14159265359

class Map;

class Bird : public MoveableObject
{
  size_t actions;
  protected:
  float radius;
  
  public:
  Bird(/*TODO: Parametres of MoveableObject()*/b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t actions = 0) : MoveableObject(world, x, y, MoveableObject::Type::BasicBird, 0), actions(actions)
  {
    b2CircleShape shape;
    shape.m_radius = 0.5f;
    radius = 0.5f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= d;
    fixtureDef.restitution = 0.4f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    body->SetActive(false);
  }
  virtual ~Bird() {}
  
  float getH() {
    return radius;
  }
  float getW() {
    return radius;
  }
  
  // Function to check if bird has actions left, returns boolean value.
  virtual bool action(Map*)
  {
    std::cout << "action in Bird-class" << std::endl;
    if (actions > 0)
    {
      actions--;
      return (true);
    }
    return (false);
  }
};

// Basic & boring bird for testing purposes.
class BasicBird : public Bird
{
  // No private stuff yet :(
  
  public:
  BasicBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f/*TODO: Parametres of MoveableObject()*/) : Bird(/*TODO: Parametres of MoveableObject()*/world , x, y, d)
  {
    //TODO: Initialization (might be empty).
  }
  /*float getH() {
    return radius;
  }
  float getW() {
    return radius;
  }*/
  // virtual ~BasicBird() {}
  
  // Function to perform special action of bird. Returns boolean value to tell if action was performed correctly.
  virtual bool action(Map*)
  {
    /* DEMO:
     * For use in birds that have special action to check if action is allowed; if bird has actions left.
    
    if (!Bird::Action())
      return(false);
      
    // If bird has some special action: implement here.
    return(true);
    */
    return(true);
  }
};

class BouncyBird : public Bird
{
  public:
  BouncyBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 2) : Bird(world , x, y, d, a){}
  
  virtual bool action(Map* m)
  {
    if (!Bird::action(m))
      return(false);
    
    std::cout << "action in BouncyBird-class" << std::endl;
    
    setImpulse(0,7.5);
    
    return(true);
  }
};

class TangentBird : public Bird
{
  public:
  TangentBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 1) : Bird(world , x, y, d, a){}
  
  virtual bool action(Map* m)
  {
    if (!Bird::action(m))
      return(false);
      
    std::cout << "action in TangentBird-class" << std::endl;
    
    b2Vec2 v = body->GetLinearVelocity();
    float a = atan(v.y/v.x);
    setImpulse(15*cos(a),15*sin(a));
    
    return(true);
  }
};

class BombBird : public Bird
{
  public:
  BombBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 1) : Bird(world , x, y, d, a){}
  
  virtual bool action(Map* m)
  {
    if (!Bird::action(m))
      return(false);
    
    std::cout << "action in BombBird-class" << std::endl;
    
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
  
  return(true);
  }
};

#endif
