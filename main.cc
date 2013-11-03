#include <vector>
#include <iostream>
#include "map.hh"
int main(){
  Map m;
  for (size_t i = 0; i < 100; i ++) {
    std::vector<MoveableObject*> objects = m.getObjects();
    if (i % 10 == 0) {
      std::cout << "Step " << i << ":" << std::endl;
      for (auto &i : objects) {
        std::cout << i->getX() << " " << i->getY() << " " << i->getAngle() << std::endl;
      }
    }
    m.Step();
  }
}
