#include "atkui/framework.h"

using namespace glm;
using namespace std;

class Circle{
   public:
   Circle(float radius, vec3 color, string direction){
      r = radius;
      col = color;
      dir = direction;
   }
   float r;
   vec3 col;
   string dir;
};

class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      theta = 0; //initialization
      thetaRate = 0.1;
      angle = 2*M_PI / 36; // 360 / 36 = 10
      radiuses = {50.0, 100.0, 150, 200, 250, 300, 350, 400}; //radius of circular motion
      dir = "c"; //clockwise
      pallet =
      {
      vec3(0,165,227)/255.0f,
      vec3(141,215,191)/255.0f,
      vec3(255,150,197)/255.0f,
      vec3(255,87,104)/255.0f,
      vec3(255,162,58)/255.0f
      };
         for(int i = 0; i < 8; i++){
           Circle newCircle = Circle(radiuses[i],pallet[i % 5], dir);
            myCircles.push_back(newCircle); 
            if(dir == "c"){
               dir = "ac";
            }
            else if(dir == "ac"){
               dir ="c";
            }
         }
   }

   virtual void scene() {
      for(Circle& circle : myCircles){
         setColor(circle.col);
         theta = 0;
         float px;
         float py;
         for( int i = 0; i < 36; i++) {
            if(circle.dir == "c"){
               px = circle.r * cos(theta + thetaRate * elapsedTime()) + 0.5 * width();
               py = circle.r * sin(theta + thetaRate * elapsedTime()) + 0.5 * height();
            } else if(circle.dir == "ac"){
               px = circle.r * cos(theta - thetaRate * elapsedTime()) + 0.5 * width();
               py = circle.r * sin(theta - thetaRate * elapsedTime()) + 0.5 * height(); 
            }
            drawSphere(vec3(px, py,0), 20);
            theta = theta + angle;
         }
      }
   }
   private:
   std::vector<vec3> pallet;
   float thetaRate;
   float theta;
   std::vector<float> radiuses;
   float angle;
   std::vector<Circle> myCircles;
   string dir;

};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
