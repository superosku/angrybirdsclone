#include "hostiles.hh"

//class Map;

void Hostile::destroy(Map* m)
{
  b2World* world = body->GetWorld();
  float x = getX();
  float y = getY();
  float w = getW();
  float h = getH();
  b2Vec2 v = body->GetLinearVelocity();
  body->SetActive(false);

  if(type != MoveableObject::Type::Else)
    for (size_t i = 0; i < BREAKDOWN_R*w*h; ++i)
    {
      float a = 2*PI/(BREAKDOWN_R*w*h);
      Particle* tmp = new Particle(world,(x+0.2*cos(a*i)),(y+0.2*sin(a*i)),0.2,0.1,getType());
      tmp->setImpulse(0.075*v.x,0.25);
      m->addObject(tmp);
    }
}

void TNT::destroy(Map* m)
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
      tmp->setImpulse(25*cos(a*i),25*sin(a*i));
      
      m->addObject(tmp);
    }
    m->killCurrentBird(this);
    m->removeObject(this);
    timer = 0;
}
