#include "atkui/framework.h"
using namespace glm;

class Circle{
   public:
   Circle(float radius, vec3 color){
      r = radius;
      col = color;
   }
   float r;
   vec3 col;
};

class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup(){
      v = Vector3(0,0,1);
      angle = 0;
      q.fromAxisAngle(v, angle);
      q_inv = q.inverse();

      Color Palette for circles:
      pallet = {
         vec3(0,165,227)/255.0f,
         vec3(141,215,191)/255.0f,
         vec3(255,150,197)/255.0f,
         vec3(255,87,104)/255.0f,
         vec3(255,162,58)/255.0f };
      
      for(int i = 0; i < 5; i++){
         Circle newCircle = Circle((i * 20) + 50,pallet[i]);
         myCircles.push_back(newCircle);
      }
   }

   virtual void scene(){
      for(Circle& circle : myCircles){
         push();
         drawCircle(circle.col, circle.r); 
         pop();
      }
   }
   void drawCircle(vec3 color, float radius){
      setColor(color);
      float thetaRate = (2.0  * M_PI) / 10.0 ;
      float theta = 0;
      vec3 p1;
      vec3 p2;
      for( int i = 0; i < 20; i++) {
         float px = radius * cos(theta) + 0.5 * width();
         float py = radius * sin(theta) + 0.5 * height();
         float pz = 0;
         p1 = vec3(px, py,pz);
         theta = theta + thetaRate;
         float px_ = radius * cos(theta) + 0.5 * width();
         float py_ = radius * sin(theta) + 0.5 * height();
         p2 = vec3(px_, py_, pz);
         drawLine(p1, p2);
      }
   }
   private:
   std::vector<vec3> pallet;
   std::vector<Circle> myCircles;
   Quaternion q;
   Quaternion q_inv;
   Vector3 v; double angle;
};

int main(int argc, char** argv) {
   Circles viewer;
   viewer.run();
   return 0;
}

