#include "atkui/framework.h"
#include "atkmath/quaternion.h"

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
      v = atkmath::Vector3(0,0,1);
      //v = vec3(0,0,1);
      angle = M_PI / 4;
      q.fromAxisAngle(v, angle);
      q_inv = q.inverse();

      //Color Palette for circles:
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
         X = 0.5 * width();
         Y = 0.5 * height();
         vec3 p_prev = vec3(X,Y,0);
         vec3 p_new = q * p_prev * q_inv;
         push();
         translate(p_new);
         drawCircle(vec3(0,0,0), circle.col, circle.r);
         pop();
      }
   }
   void drawCircle(vec3 location, vec3 color, float radius){
      setColor(color);
      float thetaRate = (2.0  * M_PI) / 10.0 ;
      float theta = 0;
      vec3 p1;
      vec3 p2;
      for( int i = 0; i < 20; i++) {
         float px = radius * cos(theta) + location[0];
         float py = radius * sin(theta) + location[1];
         float pz = 0;
         p1 = vec3(px, py,pz);
         theta = theta + thetaRate;
         float px_ = radius * cos(theta) + location[0];
         float py_ = radius * sin(theta) + location[1];
         p2 = vec3(px_, py_, pz);
         drawLine(p1, p2);
      }
   }
   private:
   float X, Y;
   std::vector<vec3> pallet;
   std::vector<Circle> myCircles;
   atkmath::Quaternion q;
   atkmath::Quaternion q_inv;
   atkmath::Vector3 v;
   // vec3 v;
   double angle;
};

int main(int argc, char** argv) {
   Circles viewer;
   viewer.run();
   return 0;
}

