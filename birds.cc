#include "birds.hh"

void Bird::destroy(Map* m){
    b2World* world = body->GetWorld();
    float x = getX();
    float y = getY();
    b2Vec2 v = body->GetLinearVelocity();
    body->SetActive(false);

    if(type != MoveableObject::Type::Else)
      for (size_t i = 0; i < GEAR_COUNT; ++i)
      {
         float a = 2*PI/(GEAR_COUNT);
         Gear* tmp = new Gear(world,(x+0.2*cos(a*i)),(y+0.2*sin(a*i)));
         tmp->setImpulse(0.075*v.x,0.25);
         m->addObject(tmp);
      }
}

