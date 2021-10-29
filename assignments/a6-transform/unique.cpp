#include "atkui/framework.h"
#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atk/toolkit.h"

using namespace atk;
using glm::vec3;
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective) {
   }
   virtual ~Unique() {}

   virtual void setup(){
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

        for(int i = 0; i < 5; i++)
            tentacles.push_back(_tentacle);
   }
     

   virtual void scene(){
    for(Skeleton _tentacle: tentacles){
        float a = 3;
        float b = 0.2;
        float displacement = 0.2;
        for(int i = 0; i < _tentacle.getNumJoints(); i++){
         glm::quat rotation = glm::angleAxis(a * sin(b* elapsedTime()+ i + displacement), glm::vec3(0.0, 0.0f, 1.0f));
         Joint *j = _tentacle.getByID(i);
         j->setLocalRotation(rotation);
        }
        displacement += 0.2;
      _tentacle.fk(); // computes local2global transforms
    }
      setColor(vec3(0,1,0));

      //Draw
      for(Skeleton _tentacle: tentacles){
        for(int i = 0; i < _tentacle.getNumJoints() - 1 ; i++){
            Joint* parent = _tentacle.getByID(i);
            Joint* child = _tentacle.getByID(i+1);
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation(); 
            drawEllipsoid(globalParentPos, globalPos, 10); 
        }
      }
   }
   protected:
   std::vector<Skeleton> tentacles;
   Skeleton _tentacle;
};

int main(int argc, char** argv) {
   Unique viewer;
   viewer.run();
   return 0;
}
