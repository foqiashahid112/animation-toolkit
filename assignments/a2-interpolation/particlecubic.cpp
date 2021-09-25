/* Question 2
* Implement an application that draws a cubic curve and its control points
* using both Bernstein polynomials and De Casteljau’s algorithm.
*/

#include "atkui/framework.h"
#include <math.h>  /* fmod */
using namespace glm;

class ParticleCubic : public atkui::Framework {
 public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic,  400, 400) {
  }
  
  void scene() {
    drawBernstein();
    drawBall();
  }
    void drawBernstein(){
    float t = 0.01;
    while( t < 1.00){
      vec3 p1 = Bern(t);
      t+= 0.01;
      vec3 p2 = Bern(t);
      setColor(vec3(1,1,0));
      drawLine(p1,p2);
      }
  }

  vec3 Bern(double t){
      float bern_1 = 1;
      float bern_2 = 0;
      float bern_3 = 0;
      float bern_4 = 0;
      bern_1 = pow((1-t),3.0);
      bern_2 = 3*t*pow((1-t), 2.0);
      bern_3 = 3 * pow(t,2.0) * (1-t);
      bern_4 = pow(t, 3.0);
      return bern_1* B0 + bern_2 * B1 + bern_3 * B2 + bern_4* B3;
  }

  void drawBall(){
    setColor(vec3(1,0,0));

    double t = fmod(elapsedTime(), 5.0);
    double T = t / 5.0;
    vec3 currentPos = Bern(T);
    drawSphere(currentPos, 10); 
  }
  private:
  vec3 B0 = vec3(100,  50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
};

int main(int argc, char** argv) {
  ParticleCubic viewer;
  viewer.run();
}

