#ifndef MOVEABLE
#define MOVEABLE

#define BREAKDOWN_R 2.5
#define N_BULLETS 16
#define PI 3.14159265359

#include <string>
#include "Box2D/Box2D.h"
//#include "map.hh"

class Map;
class MoveableObject;

struct bodyData {
  float energy;
  float fullEnergy;
  bool hasEnergy;
  MoveableObject* object;
};

//Abstract base class. Provides API to get coordinates of objects.
class MoveableObject 
{
  public:
    enum Type {
      BasicBird = 100,
      BouncyBird = 101,
      TangentBird = 102,
      BombBird = 103,
      BasicEnemy = 300,
      BasicObstacle = 200,
      BlastBullet_t = 800,
      Else = 900
    };
    typedef float mass_t;
    
    MoveableObject(b2World* world, float x = 0.0f, int y= 0.0f, float w = 1.0f, float h = 1.0f, MoveableObject::Type type = MoveableObject::Type::Else, float energy= 100.0f, float angle = 0, float t = 300): timer(t), w(w), h(h)
    {
      this->type = type;
      b2BodyDef bodyDef;
      bodyDef.type = b2_dynamicBody;
      bodyDef.bullet= true;
      bodyDef.position.Set(x, y);
      bodyDef.angle = angle;
      body = world->CreateBody(&bodyDef);

      bodyData* data = new bodyData;
      data->energy = energy;
      data->fullEnergy = energy;
      data->hasEnergy = energy?1:0;
      data->object = this;
      body->SetUserData(data);
    }
    virtual ~MoveableObject()
    {
     body->GetWorld()->DestroyBody(body);
    }
    
    virtual bool action(Map*){return(true);}
    
    //Ro3
    MoveableObject& operator=(const MoveableObject&) = delete;
    MoveableObject(const MoveableObject&) = delete;

    void setImpulse(float x, float y) {
      body->ApplyLinearImpulse(b2Vec2(x,y), body->GetWorldCenter(),true);
      //body->ApplyLinearImpulse(b2Vec2(x,y), body->GetWorldCenter());
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
      return h;
    }
    virtual float getW() {
      return w;
    }
    mass_t getMass() {
      return body->GetMass();
    }
    b2Body* getBody(){
      return body;
    }
    float getEnergy(){
      bodyData* data =static_cast<bodyData*>(body->GetUserData());
      return (data->hasEnergy && data->fullEnergy != 0) ? data->energy/data->fullEnergy : 1;
    }
    virtual void destroy(Map*){}

    //Public variable to store path to the image of an object
    const std::string imagePath;
  friend class Map;
  protected:
    b2Body* body;
    b2Vec2 velocity;
    MoveableObject::Type type;
    size_t timer;
    float w, h;

};

#endif
