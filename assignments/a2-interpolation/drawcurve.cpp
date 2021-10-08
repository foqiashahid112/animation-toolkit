/* Question 2
* Implement an application that draws a cubic curve and its control points
* using both Bernstein polynomials and De Casteljau’s algorithm.
*/

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
  virtual void setup(){
    controlPoints.push_back(vec3(100,  50, 0));
    controlPoints.push_back(vec3(150, 200, 0));
    controlPoints.push_back(vec3(250, 100, 0));
    controlPoints.push_back(vec3(300, 300, 0));
  }

  virtual void scene(){
    //Draw Control Points
    setColor(vec3(0,0,1));
    drawSphere(controlPoints[0], 10);
    drawSphere(controlPoints[3], 10);

    setColor(vec3(1,1,0));
    drawSphere(controlPoints[1], 10);
    drawSphere(controlPoints[2], 10);

    setColor(vec3(0,0,1));
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

  /* 
  * @brief : Function draws Bernstein using helper function Bezier
  */
  void drawBernstein(){
    float t = 0.00;
    while( t < 1.00){
      vec3 p1 = Bezier(controlPoints, t);
      t+= 0.01;
      vec3 p2 = Bezier(controlPoints, t);
      if(t > 1.00){ p2 = p1; }
      drawLine(p1,p2);
      }
  }
  
  /* 
  * @brief : Helper Function calculates Bezier Basis
  * for cubic curve at time t for given control points
  */
  vec3 Bezier(vector<vec3> ctrlpts, float t){
    float t2 = t * t;
    float t3 = t2 * t;
    float mt = 1 - t;
    float mt2 = mt * mt;
    float mt3 = mt2 * mt;
    return ctrlpts[0] * mt3 + 3.0f * ctrlpts[1]*mt2*t + 3.0f *ctrlpts[2] * mt*t2 + ctrlpts[3]*t3;
  }

  /* 
  * @brief : Draws cubic curve using helper function Casteljaus
  */ 
  void drawCasteljau(){
    float time = 0.00;
    while( time < 1.00){
      vec3 f = Casteljau(controlPoints, time);
      time+= 0.01;
      vec3 s = Casteljau(controlPoints, time);
      if(time > 1.00){s = f;}
      drawLine(f,s);
    }
  }

  /* 
  * @brief : Helper Function uses de Casteljaus algorithm
  * for cubic curves
  */
  vec3 Casteljau(vector<vec3> CPS, float T){
    //Level 1
    vec3 b10 = LERP(CPS.at(0), CPS.at(1), T);
    vec3 b11 = LERP(CPS.at(1), CPS.at(2), T);
    vec3 b12 = LERP(CPS.at(2), CPS.at(3), T);

    //Level 2
    vec3 b20 = LERP(b10,b11, T);
    vec3 b21 = LERP(b11,b12, T);

    //Level 3
    vec3 final = LERP(b20,b21, T);
    return final;
  }

  /* 
  * @brief : Helper Function calculates linear interpotation between
  * any two given points
  */
  vec3 LERP(vec3 init, vec3 final, float time_){
    return vec3((1-time_) * init + time_ * final);
  }

  /* 
  * @brief : If "1" is pressed, draw with Bernstein; if "2" is pressed, draw with De-Casteljaus Algorithms
  */
  void keyUp(int key, int mod) {
    if (key == GLFW_KEY_1) {
      mode = 0; //Draw Bernstein
    }
    else if (key == GLFW_KEY_2) {
      mode = 1; //Draw Casteljau
    }
  }
  private:
  vector<vec3> controlPoints;
  int mode = -1; //mode changes based on key pressed
};

int main(int argc, char **argv) {
  DrawCubic viewer;
  viewer.run();
}
