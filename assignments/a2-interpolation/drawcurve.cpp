#include <iostream>
#include "atkui/framework.h"
#include <math.h>  
using namespace glm;
using namespace std;
class DrawCubic : public atkui::Framework
{
 public:
  DrawCubic() : atkui::Framework(atkui::Orthographic, 400, 400) {
  }
  virtual void scene(){
    setColor(vec3(0,0,1));
    drawSphere(B0, 10);
    drawSphere(B3, 10);

    setColor(vec3(1,1,0));
    drawSphere(B1, 10);
    drawSphere(B2, 10);

    if(mode == 0){
      drawBernstein();
    }
    else if(mode == 1){
      drawCasteljau();
    }
    else{
      return;
    }
  }

  void drawBernstein(){
    float bern_1 = 1;
    float bern_2 = 0;
    float bern_3 = 0;
    float bern_4 = 0;
    float t = 0.00;
    while( t < 1.00){
      vec3 p1 = bern_1 * B0 + bern_2 * B1 + bern_3 * B2 + bern_4 * B3;
      t+= 0.01;
      bern_1 = pow((1-t),3.0);
      bern_2 = 3*t*pow((1-t), 2.0);
      bern_3 = 3 * pow(t,2.0) * (1-t);
      bern_4 = pow(t, 3.0);
      vec3 p2;
      if(t == 1.00){
        p2 = p1;
      }
      p2 = bern_1* B0 + bern_2 * B1 + bern_3 * B2 + bern_4* B3;
      drawLine(p1,p2);
      }
  }

  void drawCasteljau() {
    float t = 0.01;
    while(t < 1.00){
      //Level 1
      vec3 b_0_1 = LERP(B0, B1, t);
      vec3 b_1_1 = LERP(B1, B2, t);
      vec3 b_2_1 = LERP(B2, B3, t);

      //Level 2
      vec3 b_0_2 = LERP(b_0_1, b_1_1, t);
      vec3 b_1_2 = LERP(b_1_1, b_2_1, t);

      //Level 3
      vec3 b_0_3 = LERP(b_0_2, b_1_2, t);
      vec3 p1 = b_0_3;

      t+= 0.01;
      vec3 b_0_1_ = LERP(B0, B1, t);
      vec3 b_1_1_ = LERP(B1, B2, t);
      vec3 b_2_1_ = LERP(B2, B3, t);

      //Level 2
      vec3 b_0_2_ = LERP(b_0_1_, b_1_1_, t);
      vec3 b_1_2_ = LERP(b_1_1, b_2_1_, t);

      //Level 3
      vec3 b_0_3_ = LERP(b_0_2_, b_1_2_, t);
      vec3 p2 = b_0_3_; 
      drawLine(p1,p2);
    }
  }

  vec3 LERP(vec3 b0, vec3 b1, float t){
    vec3 lerp = (1-t) * b0 + t * b1;
    return lerp;
  }

  void keyUp(int key, int mod) {
    if (key == GLFW_KEY_1) {
      mode = 0; //Draw Bernstein
    }
    else if (key == GLFW_KEY_2) {
      mode = 1; //Draw Casteljau
    }
  }
  private:
  vec3 B0 = vec3(100,  50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  int mode = -1;

};

int main(int argc, char **argv) {
  DrawCubic viewer;
  viewer.run();
}
