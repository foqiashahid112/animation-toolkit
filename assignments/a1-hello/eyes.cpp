#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }
    
    virtual void setup(){
      theta = 0.0;
      theta_Rate = 1;
    }

    virtual void scene() {
      setColor(vec3(1,1,1));
      drawSphere(vec3(0.3 * width(), 0.5 * height(), 0), 90);
      drawSphere(vec3(0.7 * width(), 0.5 * height(), 0), 90);

      theta += theta_Rate * dt();
      float px = 30 * cos(theta);
      float py = 0.5 * height();
      setColor(vec3(0,0,0));
      drawSphere(vec3(0.3 * width() + px, py, 100), 30);
      drawSphere(vec3(0.7 * width() + px, py, 100), 30);
    }
    vec3 left;
    vec3 right;
    float r;
    float theta;
    float theta_Rate;
    float eyeRadius;
};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}
