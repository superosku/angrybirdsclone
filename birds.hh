#ifndef BIRDS
#define BIRDS

#include "hostiles.hh" // BlastBullet
#include "moveable.hh"

class Bird : public MoveableObject
{
  protected:
    size_t actions;

  public:
  Bird(b2World* world, float x = 0.0f, float y = 0.0f, float r = 0.5f, float d = 1.0f, size_t actions = 0, MoveableObject::Type t = MoveableObject::Type::BasicBird) : MoveableObject(world, x, y, r, r, t, 0), actions(actions)
  {
    b2CircleShape shape;
    shape.m_radius = r;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= d;
    fixtureDef.restitution = 0.4f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    if(type != MoveableObject::Type::Gear_t)
      body->SetActive(false);
  }
  virtual ~Bird() {}
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
  virtual void destroy(Map* m);
};

// Basic & boring bird for testing purposes.
class BasicBird : public Bird
{
  // No private stuff yet :(

  public:
  BasicBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f/*TODO: Parametres of MoveableObject()*/) : Bird(/*TODO: Parametres of MoveableObject()*/world , x, y, 0.5f, d)
  {
    //Initialization (might be empty).
  }

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
  BouncyBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 2) : Bird(world , x, y, 0.5f, d, a, MoveableObject::Type::BouncyBird){}

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
  TangentBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 1) : Bird(world , x, y, 0.5f, d, a, MoveableObject::Type::TangentBird){}

  virtual bool action(Map* m)
  {
    if (!Bird::action(m))
      return(false);

    std::cout << "action in TangentBird-class" << std::endl;

    b2Vec2 v = body->GetLinearVelocity();
    //float a = atan(v.y/v.x);
    //setImpulse(50*cos(a),50*sin(a));
    setImpulse(1.2*v.x,1.2*v.y);

    return(true);
  }
};

class BombBird : public Bird
{
  public:
  BombBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 1) : Bird(world , x, y, 0.5f, d, a, MoveableObject::Type::BombBird){}

  virtual bool action(Map* m)
  {
    if (!Bird::action(m))
      return(false);

    std::cout << "action in BombBird-class" << std::endl;

    b2World* world = body->GetWorld();

    float x = getX();
    float y = getY();

    std::cout << "Blast at " << x << ", " << y << "." << std::endl;

    body->SetActive(false);
    BlastBullet* tmp;

    for (size_t i = 0; i < N_BULLETS; ++i)
    {
      float a = 2*PI/N_BULLETS;

      tmp = new BlastBullet(world,(x+0.2*cos(a*i)),(y+0.2*sin(a*i)));
      tmp->setImpulse(20*cos(a*i),20*sin(a*i));

      m->addObject(tmp);
    }
    m->setCurrentBird(tmp);
    timer = 0;

  return(true);
  }
  void destroy(Map* m){
   actions=1;
   action(m);
  }
};

class Gear : public Bird
{
  public:

  Gear(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 0) : Bird(world, x, y, 0.25f, d, a, MoveableObject::Type::Gear_t){}

  virtual void destroy(Map*){}
};

class GravityCircle: public Bird
{
  public:
  GravityCircle(b2World* world, float x = 0.0f, float y = 0.0f, float r = 0.5, float d = 1.0f, size_t a = 0) : Bird(world, x, y, r, d, a, MoveableObject::Type::GravityCircle_t)
  {
    body->SetActive(false);
  }
  virtual void destroy(Map*){}
};

class GravityBird : public Bird
{
  public:
  GravityBird(b2World* world, float x = 0.0f, float y = 0.0f, float d = 1.0f, size_t a = 1) : Bird(world , x, y, 0.5f, d, a, MoveableObject::Type::GravityBird){}
 virtual bool action(Map* m)
  {
    if (!Bird::action(m))
      return(false);
    //Reverses gravity for all objects within 10 meters radius
    for ( b2Body* b = body->GetWorld()->GetBodyList(); b; b = b->GetNext()){
      bodyData* data =static_cast<bodyData*>(b->GetUserData());
      if(b2Distance(body->GetPosition(), b->GetPosition()) < 10 && b != body && data && data->object->getType() != MoveableObject::Type::BasicEnemy && b->IsActive()){
        //std::cout << data->object->getType() << std::endl;
        b->SetGravityScale(-1*(b->GetGravityScale()));
        b->SetAwake(1);
      }
    }
    GravityCircle* tmp;
    tmp = new GravityCircle(body->GetWorld(),this->getX(),this->getY());
    m->addObject(tmp);
    //body->GetWorld()->SetGravity( b2Vec2(0,9.8) );
    std::cout << "action in GravityBird-class" << std::endl;
    return(true);
  }
};

#endif
