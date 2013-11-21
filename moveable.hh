#ifndef MOVEABLE
#define MOVEABLE

#define BREAKDOWN_R 3
#define N_BULLETS 16
#define PI 3.14159265359

#include <list>
#include <string>
#include "Box2D/Box2D.h"
//#include "map.hh"

class Map;
class MoveableObject;

struct bodyData {
  float energy;
  float fullEnergy;
  bool hasEnergy;
  bool timerEnabled;
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
      GravityBird = 104,
      
      Brick = 200,
      TNT = 201,
      Glass = 202,
      Wood = 203,

      BasicEnemy = 300,

      Ground = 400,

      BlastBullet_t = 800,
      Gear_t = 801,
      Else = 900,
      Smoke_t = 901
    };
    typedef float mass_t;
    
    MoveableObject(b2World* world, float x = 0.0f, int y= 0.0f, float w = 1.0f, float h = 1.0f, MoveableObject::Type type = MoveableObject::Type::Else, float energy= 100.0f, float angle = 0, float t = 180+std::rand()%20*15, bool dynamic = true): timer(t), w(w), h(h)
    {
      this->type = type;
      b2BodyDef bodyDef;
      if (dynamic)
        bodyDef.type = b2_dynamicBody;
      else
        bodyDef.type = b2_staticBody;
      bodyDef.bullet= true;
      bodyDef.position.Set(x, y);
      bodyDef.angle = angle;
      body = world->CreateBody(&bodyDef);

      bodyData* data = new bodyData;
      data->energy = energy;
      data->fullEnergy = energy;
      data->hasEnergy = energy?1:0;
      data->timerEnabled = t?1:0;
      data->object = this;
      body->SetUserData(data);
    }
    virtual ~MoveableObject()
    {
     body->GetWorld()->DestroyBody(body);
    }
    virtual const std::list<std::pair<float, float>> getPointList() {
      return point_list;
    }
    
    virtual bool action(Map*){return(true);}
    
    //Ro3
    MoveableObject& operator=(const MoveableObject&) = delete;
    MoveableObject(const MoveableObject&) = delete;

    void setImpulse(float x, float y) {
      //body->ApplyLinearImpulse(b2Vec2(x,y), body->GetWorldCenter(),true);
      body->ApplyLinearImpulse(b2Vec2(x,y), body->GetWorldCenter());
      //body->SetLinearVelocity(b2Vec2(x,y));
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
    std::list<std::pair<float, float>> point_list; // Used in ground object only :/
    b2Body* body;
    b2Vec2 velocity;
    MoveableObject::Type type;
    size_t timer;
    float w, h;

};

#endif
