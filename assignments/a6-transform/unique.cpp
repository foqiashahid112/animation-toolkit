#include "atkui/framework.h"
#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atk/toolkit.h"
#include <vector>

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

        for(int i = 0; i < 10; i++){
            tentacles.push_back(_tentacle);
        }
   }

   virtual void scene(){

        int k;
        std::vector<vec3> directions = {glm::vec3(0.0, 0.0f, 1.0f), glm::vec3(0.0, 1.0f, 0.0f),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(0.0, 1.0f, 1.0f),glm::vec3(1.0, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.0f, 1.0f), glm::vec3(0.7, 0.3f, 1.2f), glm::vec3(0.0f, 0.3f, 1.0f), glm::vec3(0.2f, 0.0f, 1.5f), glm::vec3(1.3, 2.0f, 1.5f)};
        for(std::vector<Skeleton>::iterator it = tentacles.begin(); it != tentacles.end(); ++it){
            int numJoints = it->getNumJoints();
            for(int i = 0; i < numJoints; i++){
            glm::quat rotation = glm::angleAxis(sin((1.0f/2.0f)*elapsedTime()+ i), directions[k]); 
            Joint *j = it->getByID(i);
            j->setLocalRotation(rotation);
            k = (k+1) % directions.size();
            }
            it->fk();
        }
        setColor(vec3(0,1,0));

        //Draw
        for(int i = 0; i < tentacles.size(); i++){
            for(int j = 0; j < tentacles.at(i).getNumJoints() - 1 ; j++){
                Joint* parent = tentacles.at(i).getByID(j);
                Joint* child = tentacles.at(i).getByID(j+1);
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
