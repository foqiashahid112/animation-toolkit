#include "atkui/framework.h"

using glm::vec3;
using namespace std;

class Example : public atkui::Framework {
 public:
  Example() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup(){
    theta = 0.0; //initialization
    thetaRate = 0.2; //rate at which theta will increase
    r = 200.0; //radius of circular motion
  }

  virtual void scene() {
      theta += thetaRate * dt();
      float px = r * cos(theta) + 0.5 * width();
      float py = r * sin(theta) + 0.5 * height();
      setColor(vec3(1,0,0));
      drawSphere(vec3(px, py,0), 100);
  }
  
  private: //best practice
  float theta;
  float thetaRate;
  float r;
};

int main(int argc, char** argv)
{
  Example example;
  example.run();
  return 0;
}
