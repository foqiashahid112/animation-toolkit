#include "atkui/framework.h"
#include "spline.h"
#include "interpolator_catmullrom.h"
#include "interpolator_linear.h"
using namespace glm;

class Unique : public atkui::Framework {
   public:
   Unique() : atkui::Framework(atkui::Orthographic) {
   }
   virtual void setup(){
      for(int i = 0; i < 3; i++){
         Spline spline;
         spline.addInterpolationType("Linear", &linear);
         spline.setInterpolationType("Linear");
         splines.push_back(spline);
      }

      for (std::vector<Spline>::iterator it = splines.begin() ; it != splines.end(); ++it){
         float horizontal = 0.0f;
         float vertical = height();
         for(int i = 0; i <= 10; i++){
            vec3 newControl = vec3(horizontal, vertical, 0.0);
            *it.appendKey(i, newControl);
         }
      }
      //Linear
      float horizontal = 0.0f;
      float vertical = height() - height()/4;
      for(int i = 0; i <= 10; i++){
         vec3 newControl = vec3(horizontal, vertical, 0.0);
         spline.appendKey(i, newControl);
         horizontal += width()/10;
         if(i % 2 == 0){
            vertical = (height() - height()/4) + height()/10;
         } else{
            vertical = height() - height()/4;
         }
      }
   }
   virtual void scene() {
      float t = 0;
      setColor(vec3(1,0,0));
      while(t < spline.getDuration()){
         vec3 point = spline.getValue(t);
         vec3 pointC = spline2.getValue(t);
         t += 0.01;
         vec3 point2 = spline.getValue(t);
         vec3 point2C = spline2.getValue(t);
         drawLine(point, point2);
         drawLine(pointC, point2C);
      }
      drawBallLinear(s);
   }

   void drawBallLinear(Spline thisSpline){
      setColor(vec3(0,1,0));
      double t = fmod(elapsedTime(), thisSpline.getDuration());
      vec3 currentPos = thisSpline.getValue(t);
      drawSphere(currentPos, 20);
   }
   private:
   InterpolatorLinear linear;
   std::vector<Spline> splines;
};


int main(int argc, char** argv)
{
  Unique viewer;
  viewer.run();
  return 0;
}
