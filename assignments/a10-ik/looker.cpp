#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ALooker : public atkui::Framework {
public:
   ALooker() : atkui::Framework(atkui::Perspective) {}
   virtual ~ALooker() {}

   void setup() {
      Motion motion;
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, motion);
      motion.update(_skeleton, 0);

      _drawer.color = vec3(1,0,0);
      _head = _skeleton.getByName("Beta:Head");
   }

   void reset(Joint* head) {
      head->setLocalRotation(IdentityQ);
      head->fk();
   }

   void lookAtTarget(Joint* head, const vec3& target) {
      // TODO: Your code here
      vec3 globalPos = head->getGlobalTranslation();
      vec3 targetDirection = target - globalPos;
      vec3 targetDirectionN = normalize(targetDirection);

      vec3 z = targetDirectionN;
      //Assume y is up
      vec3 x = cross(vec3(0,1,0), z);
      vec3 x_N = normalize(x);
      vec3 y = cross(z, x);
      vec3 y_N = normalize(y);
      quat rotation = quat(mat3(x_N,y_N, z));


      Joint* parent = head->getParent();
      quat rparentToGlobal = parent->getGlobalRotation();
      quat localToParent = glm::inverse(rparentToGlobal) * rotation;
      Transform T = Transform::Rot(localToParent);
      head->setLocalRotation(localToParent);
      head->fk();
   }

   void scene() {  
      float r = 100;
      float angle = elapsedTime();
      _target = vec3(r * cos(angle), r * sin(angle) + r, 150);

      reset(_head);
      lookAtTarget(_head, _target);
      _drawer.draw(_skeleton, *this);

      vec3 globalHeadPos = _head->getGlobalTranslation();
      vec3 globalForward = _head->getGlobalRotation() * vec3(0,0,1);

      setColor(vec3(0,0,1));
      drawLine(globalHeadPos, globalHeadPos + 200.0f*globalForward);
      drawSphere(_target, 5);
   }

   Cyclops _drawer;
   Skeleton _skeleton;
   Joint* _head;
   vec3 _target;
};

int main(int argc, char** argv) {
   ALooker viewer;
   viewer.run();
   return 0;
}

