#include "atkui/framework.h"
#include <random>
#include <cstdlib>

using namespace glm;

class particle{
  public:
  particle(vec3 currentPosition, vec3 velocity, vec3 RGBcolor){
    currentPos =currentPosition;
    vel = velocity;
    color = RGBcolor;
  }
  vec3 currentPos;
  vec3 vel;
  vec3 color;
};

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }
  virtual void setup() {
   for(int i = 0; i < 150; i++){
      float rand = 20 + (std::rand() % 200) ;
      vec3 baseColor = vec3(0.2,0.8,0.2);
      vec3 current = 500.0f * abs(agl::randomUnitVector());
      current[2] = 0;
      particle pNew = particle(current, rand * vec3(1,1,0), baseColor + fRand(-0.4,0.4));
      myParticles.push_back(pNew);
    }
  }
  virtual void scene() {
     for(particle& p : myParticles){
      setColor(p.color);
      double radius = 10;
      if(p.currentPos[0] > width()){
        p.currentPos[0] = 0;
      }
      if(p.currentPos[1] > height()){
        p.currentPos[1] = 0;
      }
      p.currentPos = p.currentPos + p.vel * dt();
      drawSphere(p.currentPos, radius);
    }
  }

//https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
//Modified Sachin singh jagawat's solution
  virtual vec3 fRand(double fMin, double fMax)
{
    double f1 = ((double)rand()) / RAND_MAX;
    f1 = fMin + (fMax - fMin) * f1;
    double f2 = ((double)rand()) / RAND_MAX;
    f2 = fMin + (fMax - fMin) * f2;
    double f3 = ((double)rand()) / RAND_MAX;
    f3 = fMin + (fMax - fMin) * f3;
    return vec3(f1,f2,f3);
}
  std::vector<particle> myParticles;
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
