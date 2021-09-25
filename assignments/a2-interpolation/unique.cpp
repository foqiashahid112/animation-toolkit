#include <iostream>
#include "atkui/framework.h"
#include <math.h>  
using namespace glm;
using namespace std;
class DrawN : public atkui::Framework
{
 public:
  DrawN() : atkui::Framework(atkui::Orthographic, 400, 400) {
  }

  virtual void setup(){
      B0 = vec3(100,  50, 0);
      B1 = vec3(150, 200, 0);
      B2 = vec3(250, 100, 0);
      B3 = vec3(300,300,0);
      B4 = vec3(350,350, 0);
  }

  virtual void scene(){
   setColor(vec3(0,0,1));
    drawSphere(B0, 10);
    drawSphere(B1, 10);
    drawSphere(B2, 10);
    drawSphere(B3, 10);
    drawSphere(B4, 10);
   drawBernstein();
  }
    void drawBernstein(){
    float bern_1 = 1;
    float bern_2 = 0;
    float bern_3 = 0;
    float bern_4 = 0;
    float bern_5 = 0;
    float t = 0.00;
   while( t < 1.00){
      vec3 p1 = bern_1 * B0 + bern_2 * B1 + bern_3 * B2 + bern_4 * B3 + bern_5 * B4;
      t+= 0.01;
      bern_1 = pow((1-t),4.0);
      bern_2 = 4*t*pow((1-t), 3.0);
      bern_3 = 6 * pow(t,2.0) * pow((1-t),2);
      bern_4 = 4* pow(t, 3.0) * (1-t);
      bern_5 = pow(t, 4.0);
      vec3 p2;
      if(t == 1.00){
        p2 = p1;
      }
      p2 = bern_1* B0 + bern_2 * B1 + bern_3 * B2 + bern_4* B3 + bern_5 * B4;
      drawLine(p1,p2);
      }
  }

  void drawCurvePoints(vector<vec3> my_ctrlpts,float t){
     if(my_ctrlpts.size() == 1){
        drawSphere(my_ctrlpts[0],10);
     }
     vector<vec3> newpoints;
     for(int i = 0; i < my_ctrlpts.size()-1; i++){
        newpoints[i] = (1-t) * my_ctrlpts[i] + t * my_ctrlpts[i + 1];
     }
     drawCurvePoints(newpoints, t);
  }

   vec3 mouseMove(float x, float y) {
    float _mouseX = x;
    float _mouseY = height() - y;
    return vec3(_mouseX, _mouseY,0);
  }

  private:
  vec3 B0;
  vec3 B1;
  vec3 B2;
  vec3 B3;
  vec3 B4;
};

int main(int argc, char **argv) {
  DrawN viewer;
  viewer.run();
}
