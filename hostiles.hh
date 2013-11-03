#ifndef HOSTILES
#define HOSTILES

#include <Box2D/Box2D.h>
#include "moveable.hh"

class Hostile : public MoveableObject
{
  size_t energy;// Energy of hostile, something from 100 to 1000

  public:
  Hostile(b2World* world, float x = 0.0f, float y = 0.0f, size_t energy = 100) : MoveableObject(/*TODO: Parametres of MoveableObject()*/world, x, y), energy(energy)
  {
    //TODO: Initialization (might be empty).
  }
  virtual ~Hostile() {}
  /*Hostile& operator=(const Hostile&) = delete;
  Hostile(const Hostile&) = delete;*/

  // Function to check if hostile has energy left after hit, returns energy after hit as size_t.
  virtual size_t hit(mass_t mass)
  {
    size_t strength = (mass/MoveableObject::getMass())*100;
    if (energy > strength)
    {
      energy -= strength;
    }
    else
      energy = 0;
    return energy;
  }
};

class BasicObstacle : public Hostile
{
  // No private stuff yet :(

  public:
  BasicObstacle(b2World* world, float x = 0.0f, float y = 0.0f) : Hostile(/*TODO: Parametres of MoveableObject()*/world, x, y)
  {
    b2PolygonShape shape;
    shape.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density= 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
  }

  // virtual ~BasicObstacle();

  // Function to perform special action of obstacles/enemies. Returns energy left after hit as size_t value.
  virtual size_t hit(mass_t mass)
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
    return(Hostile::hit(mass));
  }
};

#endif
