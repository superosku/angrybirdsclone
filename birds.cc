#include "birds.hh"

void Bird::destroy(Map* m){
    b2World* world = body->GetWorld();
    float x = getX();
    float y = getY();
    float w = getW();
    float h = getH();
    b2Vec2 v = body->GetLinearVelocity();
    body->SetActive(false);

    for (size_t i = 0; i < BREAKDOWN_R*w*h; ++i)
    {
      if(type != MoveableObject::Type::Else){
        float a = 2*PI/(BREAKDOWN_R*w*h);

        Gear* tmp = new Gear(world,(x+0.2*cos(a*i)),(y+0.2*sin(a*i)));

        tmp->setImpulse(0.075*v.x,0.25);

        m->addObject(tmp);
      }
    }
}

