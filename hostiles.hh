#ifndef HOSTILES
#define HOSTILES

#include <Box2D/Box2D.h>
#include "moveable.hh"

class Hostile : public MoveableObject
{
  // Energy of the hostile is saved to box2d userdata, something from 100 to 1000

  public:
  Hostile(b2World* world, float x = 0.0f, float y = 0.0f, size_t energy = 100) : MoveableObject(/*TODO: Parametres of MoveableObject()*/world, x, y)
  {
    bodyData* data = new bodyData;
    data->energy = energy;
    data->hasEnergy = 1;
    body->SetUserData(data);
  }
  virtual ~Hostile() {}
  /*Hostile& operator=(const Hostile&) = delete;
  Hostile(const Hostile&) = delete;*/

  // Function to check if hostile has energy left after hit, returns energy after hit as size_t.
};

class BasicObstacle : public Hostile
{
  private:
    float w, h;
  // No private stuff yet :(

  public:
  BasicObstacle(b2World* world, float x = 0.0f, float y = 0.0f, float w = 1.0f, float h = 1.0f) : Hostile(/*TODO: Parametres of MoveableObject()*/world, x, y)
  {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);
    this->w = w;
    this->h = h;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
  }
  float getH() {
    return h;
  }
  float getW() {
    return w;
  }
  size_t getType() {
    return 1;
  }

  // virtual ~BasicObstacle();

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
};

#endif
