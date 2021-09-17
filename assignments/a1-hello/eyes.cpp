#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }
    
    virtual void setup(){
      theta = 0.0;
      thetaRate = 0.8;
      eyeRadius = 100;
    }

    virtual void scene() {
      float c1_x = 0.3 * width();
      float c2_x = 0.7 * width();
      float c_y = 0.5 * height();
      setColor(vec3(1,1,1));
      drawSphere(vec3(c1_x, c_y, 0), eyeRadius);
      drawSphere(vec3(c2_x, c_y, 0), eyeRadius);

      theta += thetaRate * dt();
      float px = 0.2 * eyeRadius * cos(theta);
      setColor(vec3(0,0,0));
      drawSphere(vec3(c1_x + px, c_y, 100), 0.4 * eyeRadius);
      drawSphere(vec3(c2_x + px, c_y, 100), 0.4 * eyeRadius);
    }
    float theta;
    float thetaRate;
    float eyeRadius;
};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}
