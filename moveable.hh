#ifndef MOVEABLE
#define MOVEABLE

#include <string>
#include "Box2D/Box2D.h"

//Abstract base class. Provides API to get coordinates of objects.
class MoveableObject 
{
  public:
    typedef float mass_t;
    // TODO: Overloaded constructors for circle and square formed objects.
    MoveableObject(b2World* world, float x = 0.0f, int y= 10.0f, float angle = 0.0f, float density = 1.0f)
    {
      b2BodyDef bodyDef;
      bodyDef.type = b2_dynamicBody;
      bodyDef.position.Set(x, y);
      body = world->CreateBody(&bodyDef);
      // TODO:
      //Initialize Box2D, save returned pointer to private variable
      //Initialize imagePath which tells SFML where to find images to draw
    }
    virtual ~MoveableObject()
    {
     //TODO:
     //Free reserved pointer
    }
    //Ro3
    MoveableObject& operator=(const MoveableObject&) = delete;
    MoveableObject(const MoveableObject&) = delete;

    //TODO: Scaling (Static/dynamic?) and 
    //int getX() { return body->x; }
    float getX() {
      return body->GetPosition().x;
    }
    //int getY() { return body->y; }
    float getY() {
      return body->GetPosition().y;
    }
    float getAngle() { 
      return body->GetAngle();
    }
    mass_t getMass() {
      return body->GetMass();
    }

    //Public variable to store path to the image of an object
    const std::string imagePath;
  protected:
    b2Body* body;
};

#endif
