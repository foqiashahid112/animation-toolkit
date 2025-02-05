#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Butterfly : public atkui::Framework
{
public:
   Butterfly() : atkui::Framework(atkui::Perspective) {
   }

   void setup() {
      Joint* body = new Joint("Body");
      body->setLocalTranslation(vec3(1,2,1)*100.0f);
      body->setLocalRotation(glm::angleAxis(glm::radians<float>(45.0f), vec3(0,1,0)));
      skeleton.addJoint(body);

      Joint* lwing = new Joint("LWing");
      lwing->setLocalTranslation(vec3(0.1,0,0)*100.0f);
      skeleton.addJoint(lwing, body);

      Joint* rwing = new Joint("RWing");
      rwing->setLocalTranslation(vec3(-0.1,0,0)*100.0f);
      skeleton.addJoint(rwing, body);

      skeleton.fk();
   }

   void scene() {
      Joint* body = skeleton.getByName("Body");
      body->setLocalRotation(glm::angleAxis(-float(M_PI)/4.0f, vec3(1,1,0)));
      Joint* lwing = skeleton.getByName("LWing");
      lwing->setLocalRotation(glm::angleAxis((1.0f/2.0f)*sin(elapsedTime()), vec3(0,0,1)));

      Joint* rwing = skeleton.getByName("RWing");
      rwing->setLocalRotation(glm::angleAxis(-(1.0f/2.0f)*sin(elapsedTime()), vec3(0,0,1))); 

      skeleton.fk();

      // attach geometry to skeleton 
      Transform B = body->getLocal2Global(); 
      Transform LT = lwing->getLocal2Global(); 
      Transform RT = rwing->getLocal2Global();
 

      // draw body
      Transform bodyGeometry(
         glm::angleAxis(glm::pi<float>()*0.5f, vec3(1,0,0)), // rotation
         vec3(0), vec3(25, 200, 25)); // position, scale

      Transform lwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(-80,0,0), 
         vec3(120,20,200));

      Transform rwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(80,0,0), 
         vec3(120,20,200));

      Transform lwingTopGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(-80,0,50),
         vec3(120,20, 100));

      Transform lwingBottomGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(-80, 0,-100),
         vec3(120,20, 200));
      
      Transform rwingTopGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(80,0,50), 
         vec3(120,20,100));

      Transform rwingBottomGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(80,0,-100), 
         vec3(120,20,200));

      setColor(vec3(0.4, 0.4, 0.8));
      push();
      transform(B * bodyGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(1, 1, 0.0));
      push();
      transform(LT * lwingTopGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0.847, 0.690, 0.827));
      push();
      transform(LT * lwingBottomGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(1, 1, 0.0));
      push();
      transform(RT * rwingTopGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0.847, 0.690, 0.827));
      push();
      transform(RT * rwingBottomGeometry);
      drawSphere(vec3(0), 1);
      pop();


   }

private:
   Skeleton skeleton;
};


int main(int argc, char** argv) {
   Butterfly viewer;
   viewer.run();
}
