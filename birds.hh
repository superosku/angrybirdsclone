#ifndef BIRDS
#define BIRDS

#include "moveable.hh"

class Bird : public MoveableObject
{
  size_t actions;
  protected:
  float radius;
  
  public:
  Bird(/*TODO: Parametres of MoveableObject()*/b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t actions = 0) : MoveableObject(world, x, y, MoveableObject::Type::BasicBird), actions(actions)
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
  }
  virtual ~Bird() {}
  
  // Function to check if bird has actions left, returns boolean value.
  virtual bool Action()
  {
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
  float getH() {
    return radius;
  }
  float getW() {
    return radius;
  }
  // virtual ~BasicBird() {}
  
  // Function to perform special action of bird. Returns boolean value to tell if action was performed correctly.
  virtual bool Action()
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

#endif
