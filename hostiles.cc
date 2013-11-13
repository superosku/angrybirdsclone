#include "hostiles.hh"

//class Map;

void Hostile::destroy(Map* m)
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
  }
}

