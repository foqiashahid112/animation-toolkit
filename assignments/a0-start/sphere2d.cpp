#include "atkui/framework.h"

using glm::vec3;

class Sphere2D : public atkui::Framework {
 public:
  Sphere2D() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(1,0,0));
    double x = width() * 0.5;
    double y = height() * 0.5;

    //draw a sphere behind center sphereRadius
    double radius_2 = 200;
    drawSphere(vec3(x,y,-x), radius_2);
    
    //draw a green sphere at center of screen
    setColor(vec3(0,1,0));
    double radius = 100;
    drawSphere(vec3(x,y,0),radius);
  }
};

int main(int argc, char** argv)
{
  Sphere2D viewer;
  viewer.run();
  return 0;
}
