#include "atkui/framework.h"
#include "spline.h"
#include "interpolator_linear.h"
using namespace glm;

class Unique : public atkui::Framework {
   public:
   Unique() : atkui::Framework(atkui::Orthographic) {
   }
   virtual void setup(){
      //InterpolatorLinear linear;
      //std::vector<Spline> splines;
      for(int i = 0; i < 3; i++){
         Spline spline;
         spline.addInterpolationType("Linear", &linear);
         spline.setInterpolationType("Linear");
         splines.push_back(spline);
      }

      for (std::vector<Spline>::iterator it = splines.begin() ; it != splines.end(); ++it){
         Spline thisSpline = *it;
         float vertical = height();
         float horizontal = 0.0f;
         for(int i = 0; i <= 10; i++){ //create 10 control points
            vec3 newControl = vec3(horizontal, vertical, 0.0);
            thisSpline.appendKey(i, newControl);
            horizontal += width() / 10;
            if(i % 2 == 1){
               vertical  = height() - 1 * (height() / 3.0f);
            } else{
               vertical = height();
            }
         }
         thisSpline.computeControlPoints();
         // std::cout << "control pts in setup " << thisSpline.getNumControlPoints() << std::endl;
         // std::cout<< "keys in setup" << thisSpline.getNumKeys() <<std::endl; 
         // std::cout << "duration" << thisSpline.getDuration() << std::endl;
      }
   }
   virtual void scene() {
      setColor(vec3(1,0,0));
      for (std::vector<Spline>::iterator it = splines.begin() ; it != splines.end(); ++it){ 
         Spline spline = *it;
         std::cout << "control pts " << spline.getNumControlPoints() << std::endl;
         std::cout<< "keys" << spline.getNumKeys() <<std::endl;
         std::cout << "Duration" << spline.getDuration() << std::endl;
         float t = 0.0f;
         while(t < spline.getDuration()){
         vec3 point = spline.getValue(t);
         t += 0.01;
         vec3 point2 = spline.getValue(t);
         drawLine(point, point2);
         }
      }
   }

   void drawBallLinear(Spline thisSpline){
      setColor(vec3(0,1,0));
      double t = fmod(elapsedTime(), thisSpline.getDuration());
      vec3 currentPos = thisSpline.getValue(t);
      drawSphere(currentPos, 20);
   }
   InterpolatorLinear linear;
   std::vector<Spline> splines;
};


int main(int argc, char** argv)
{
  Unique viewer;
  viewer.run();
  return 0;
}
