#ifndef MAP
#define MAP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <map>

#include <Box2D/Box2D.h>
#include "CSVparser.hh"
#include "moveable.hh"

class MoveableObject;

class Map : public b2ContactListener,b2ContactFilter {
  public:
    Map(std::string path = "maps/basic_map.csv");
    ~Map();
    //Ro3
    Map& operator=(const Map&) = delete;
    Map(const Map&) = delete;

    void Step();

    void PreSolve(b2Contact* contact, const b2Manifold*);

    //Calculate score and new energies after the impact
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) { // WONT WORK IF CONST!
      bodyData* bodyDataA =static_cast<bodyData*>(fixtureA->GetBody()->GetUserData());
      bodyData* bodyDataB =static_cast<bodyData*>(fixtureB->GetBody()->GetUserData());
      if((bodyDataA && bodyDataA->object->type == MoveableObject::Type::Smoke_t ) ||(bodyDataB && bodyDataB->object->type == MoveableObject::Type::Smoke_t ))
        return (false);
      return (true);
    }

    //This can be called to shoot the bird
    void ShootBird(float x, float y);

    float getCatapultX() const {
      return catapult_x;
    }
    float getCatapultY() const {
      return catapult_y;
    }
    int getScore() const {
      return totalScore;
    }
    size_t getBirdsLeft() const {
      return birds.size();
    }
    size_t getEnemyCount() const {
      return enemiesLeft;
    }

    size_t getHighScore() const {
      return highScore;
    }
    
    bool isEnd() const {
      return ((!getBirdsLeft() || !getEnemyCount()) && current_b == nullptr);
    }
    
    bool isWin() const {
      return (!getEnemyCount());
    }

    void addObject(MoveableObject* o);
    void removeObject(MoveableObject* o);

    std::vector<MoveableObject*> getObjects() const {return objects;}

    void setCurrentBird(MoveableObject* a)
    {
      current_b = a;
    }

    MoveableObject* getCurrentBird() const {
      return(current_b);
    }

    void killCurrentBird() {current_b = nullptr;std::cout << "killed current bird" << std::endl;}

    void killCurrentBird(MoveableObject* a)
    {
      if (a == current_b) {
        current_b = nullptr;
        std::cout << "killed current bird" << std::endl;
      }
    }
    
    MoveableObject::Type getNextBirdType() const;

  private:
    void loadMap(std::string filepath);
    //List of MoveableObjects currently present in the map

    // TODO better validation...
    bool validSTRVEC(std::vector<std::string> vec)
    {
      try
      {
        if (vec.size() < 7)
        {
          std::cout << "Invalid line in map-file: Not enough columns." << std::endl;
          return(false);
        }
        size_t t_id = std::atoi(vec[0].c_str());

        for (auto i : valid[t_id])
        {
          std::atoi(vec[i].c_str());
          std::atof(vec[i].c_str());
        }
      }
      catch (...)
      {
        std::cout << "Invalid line in map-file: Data threw an error" << std::endl;
        return (false);
      }
      return (true);
    }

    std::vector<MoveableObject*> birds;
    std::vector<MoveableObject*> objects;
    std::vector<MoveableObject*> objectsQueue;
    b2World* m_world;
    //b2Body* m_groundBody;
    MoveableObject* current_b = nullptr;
    size_t highScore=0;
    float totalScore=0;
    float catapult_x, catapult_y;
    size_t enemiesLeft=0;
    std::map<size_t,std::vector<size_t>> valid = {{100,{0,5}},
                                                  {101,{0,5}},
                                                  {102,{0,5}},
                                                  {103,{0,5}},
                                                  {200,{0,1,2,3,4,5,6}},
                                                  {201,{0,1,2}},
                                                  {300,{0,1,2,5,6}}};
};

#endif
