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
  
  for (size_t i = 0; i < BREAKDOWN_R*w*h; ++i)
  {
    if(type != MoveableObject::Type::Else){
      float a = 2*PI/(BREAKDOWN_R*w*h);
      
      Particle* tmp = new Particle(world,(x+0.2*cos(a*i)),(y+0.2*sin(a*i)),0.2,0.1,getType());
      
      tmp->setImpulse(0.075*v.x,0.25);
      
      m->addObject(tmp);
    }
  }
  
  /*for (ssize_t i = -BREAKDOWN_N_X; i < BREAKDOWN_N_X; i++)
  {
  for (size_t i = 0; i < N_BULLETS; ++i)
    {
      float a = 2*PI/N_BULLETS;
      
      BlastBullet* tmp = new BlastBullet(world,(x+0.2*cos(a*i)),y+0.2*sin(a*i));
      tmp->setImpulse(50*cos(a*i),50*sin(a*i));
      
      m->addObject(tmp);
    }
    for (ssize_t j = -BREAKDOWN_N_Y; j < BREAKDOWN_N_Y; j++)
    {
      if ((i == 0) || (j == 0)) continue;
      
      float p_x = x + cos(a)*(i*w/(BREAKDOWN_N_X*2)-w/(BREAKDOWN_N_X*4));
      float p_y = y + sin(a)*(j*h/(BREAKDOWN_N_Y*2)-h/(BREAKDOWN_N_Y*4));
      if (p_x == x) p_x += i*w/(BREAKDOWN_N_X*2)-w/(BREAKDOWN_N_X*4);
      if (p_y == y) p_y += j*h/(BREAKDOWN_N_Y*2)-h/(BREAKDOWN_N_Y*4);
      if (p_y <= 0) p_y = p_y *(-1) + 0.5;
      float p_w = w/(BREAKDOWN_N_X*2);
      float p_h = h/(BREAKDOWN_N_Y*2);
      
      Particle* tmp = new Particle(world,p_x,p_y,p_w,p_h,a,getType());
      
      tmp->setImpulse(0.2*v.x,(v.y < 0 ? 0 : (v.y *0.2)));
      tmp->setImpulse(0,0.2);
      
      m->addObject(tmp);
    }
  }*/
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
