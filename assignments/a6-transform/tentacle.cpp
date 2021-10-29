#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;

class Tentacle : public atkui::Framework {
public:
   Tentacle() : atkui::Framework(atkui::Perspective) {}
   virtual ~Tentacle() {}

   virtual void setup() {
      lookAt(vec3(200), vec3(0));

      Joint* root  = new Joint("root");
      root->setLocalTranslation(vec3(0,0,0));
      _tentacle.addJoint(root);

      Joint* joint1  = new Joint("joint1");
      joint1->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint1, root);

      // todo: add more joints
      Joint* joint2  = new Joint("joint2");
      joint2->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint2, joint1);

      Joint* joint3  = new Joint("joint3");
      joint3->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint3, joint2);

      Joint* joint4  = new Joint("joint4");
      joint4->setLocalTranslation(vec3(0,50,0));
      _tentacle.addJoint(joint4, joint3);
      _tentacle.fk(); // compute local2global transforms
   }

   virtual void scene()
   {
      // todo animate joints
      // hint: use glm::angleAxis(...) to compute quaternions for each joint
      //rotate each joint around its local Z axis
      //Use sine to compute the angle of rotation around each joint’s local Z axis.
      //Offset the angle of rotation for each joint to make the animation more interesting

      //Motion
      for(int i = 0; i < _tentacle.getNumJoints(); i++){
         glm::quat rotation = glm::angleAxis(sin(elapsedTime()+ i), glm::vec3(0.0, 0.0f, 1.0f));
         Joint *j = _tentacle.getByID(i);
         j->setLocalRotation(rotation);
      }
      _tentacle.fk(); // computes local2global transforms
      setColor(vec3(0,1,0));

      //Draw
      for(int i = 0; i < _tentacle.getNumJoints() - 1 ; i++){
         Joint* parent = _tentacle.getByID(i);
         Joint* child = _tentacle.getByID(i+1);
         vec3 globalParentPos = parent->getGlobalTranslation();
         vec3 globalPos = child->getGlobalTranslation(); 
         drawEllipsoid(globalParentPos, globalPos, 5); 
      }
   }

protected:
   Skeleton _tentacle;
};

int main(int argc, char** argv)
{
   Tentacle viewer;
   viewer.run();
} 

