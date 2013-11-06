#ifndef MOVEABLE
#define MOVEABLE

#include <string>
#include "Box2D/Box2D.h"

struct bodyData {
  float energy;
};

//Abstract base class. Provides API to get coordinates of objects.
class MoveableObject 
{
  public:
    enum Type {
      Else,
      BasicBird,
      BasicEnemy,
      BasicObstacle
    };
    typedef float mass_t;
    // TODO: Overloaded constructors for circle and square formed objects.
    MoveableObject(b2World* world, float x = 0.0f, int y= 0.0f, MoveableObject::Type type = MoveableObject::Type::Else)
    {
      this->type = type;
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

    void setImpulse(float x, float y) {
      body->ApplyLinearImpulse(b2Vec2(x,y), body->GetWorldCenter());
    }
    MoveableObject::Type getType() {
      return type;
    }
    float getX() {
      return body->GetPosition().x;
    }
    float getY() {
      return body->GetPosition().y;
    }
    float getAngle() { 
      return body->GetAngle();
    }
    virtual float getH() {
      return 1.0f;
    }
    virtual float getW() {
      return 1.0f;
    }
    mass_t getMass() {
      return body->GetMass();
    }

    //Public variable to store path to the image of an object
    const std::string imagePath;
  protected:
    b2Body* body;
    MoveableObject::Type type;
};

#endif
