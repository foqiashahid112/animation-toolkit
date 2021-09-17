#include "atkui/framework.h"

using namespace glm;

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    a = 100;
    k = 3;
    theta = 0;
    thetaRate = 0.5;
  }

  virtual void scene() {
    if(theta > 2* M_PI){
      theta = 0;
      k +=1;
    }
      float px = 0.5 * width() + a * cos(k * theta) * cos(theta);
      history_x.push_back(px);
      float py = 0.5 * height() + a * cos(k * theta) * sin(theta);
      history_y.push_back(py);

      if(history_x.size() > 300){
        history_x.erase(history_x.begin());
        history_y.erase(history_y.begin());
      }
      setColor(vec3(1,0,0));
      drawSphere(vec3(px, py, 0), 10);
      theta = theta + thetaRate * dt();

      for(int i = 0; i < history_x.size(); i++){
        setColor(vec3(i / 2 ,0,0));
        float x = history_x[i];
        float y = history_y[i];
        drawSphere(vec3(x,y,0), i % 20);
      }

  }
  float a;
  float k;
  float theta;
  float thetaRate;
  std::vector <float> history_x;
  std::vector <float> history_y;
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}

