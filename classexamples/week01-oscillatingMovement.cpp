#include "atkui/framework.h"

using glm::vec3;
using namespace std;

class Example : public atkui::Framework {
 public:
  Example() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup(){
    theta = 0.0;
    thetaRate = 0.5;
    h = 200; //scaling for height
  }

  virtual void scene() {
      theta += thetaRate * dt();
      float px = 0.5 * width();
      float py = 0.5 * height() + h * sin(theta);
      setColor(vec3(1,0,0));
      drawSphere(vec3(px, py,0), 100);
  }
  
  private: //best practice
  float theta;
  float thetaRate;
  float h;
};

int main(int argc, char** argv)
{
  Example example;
  example.run();
  return 0;
}
