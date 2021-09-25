/* Question 3
* Animate the position of a particle based on a cubic polynomial.
*/

#include "atkui/framework.h"
#include <math.h>  /* fmod */
using namespace glm;
using namespace std;

class ParticleCubic : public atkui::Framework {
 public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic,  400, 400) {
  }

  virtual void setup(){
    controlPoints.push_back(vec3(100,  50, 0));
    controlPoints.push_back(vec3(150, 200, 0));
    controlPoints.push_back(vec3(250, 100, 0));
    controlPoints.push_back(vec3(300, 300, 0));
  }

  virtual void scene() {
    drawBernstein();
    drawBall();
  }

  /* 
  * @brief : Function draws Bernstein using helper function Bezier
  */
  void drawBernstein(){
    setColor(vec3(1,1,0));
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
  * @brief : Function draws animated ball on cubic curve
  */
  void drawBall(){
    setColor(vec3(1,0,0));
    double t = fmod(elapsedTime(), 5.0);
    double T = t / 5.0;
    vec3 currentPos = Bezier(controlPoints,T);
    drawSphere(currentPos, 10); 
  }
  private:
  vector<vec3> controlPoints;
};

int main(int argc, char** argv) {
  ParticleCubic viewer;
  viewer.run();
}

