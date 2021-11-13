#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <cmath>
#include <string>

using namespace glm;
using namespace atk;
using namespace atkui;

class AReorient : public atkui::Framework
{
public:
   AReorient() : atkui::Framework(atkui::Perspective) {}
   virtual ~AReorient() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _motion);

      _heading = 0;
      _offset = vec3(0);
      _offset[1] = _motion.getKey(0).rootPos[1];
      _time = 0;

      _reoriented = reorient(_motion, _offset, _heading);
   }

   Motion reorient(const Motion& motion, const vec3& pos, float heading)
   {
      Motion result;
      result.setFramerate(motion.getFramerate());

      // todo: your code here
      vec3 displacement2 = pos;
      quat rotation2 = glm::angleAxis(heading, vec3(0,1,0));
      Transform Desired(rotation2, displacement2); // desired transformation

      Pose rootPose = motion.getKey(0);
      vec3 rootDisplacement = rootPose.rootPos;
      quat rootRotation = rootPose.jointRots[0];
      Transform T0(rootRotation, rootDisplacement);
      Transform T_inverse = T0.inverse();
      //Transform change = Desired*T_inverse;
      
      for(int i = 0; i < motion.getNumKeys(); i++){
         Pose currentPose = motion.getKey(i);
         vec3 thisDisplacement = currentPose.rootPos;
         quat thisRotation = currentPose.jointRots[0];
         Transform currentPoseT(thisRotation, thisDisplacement);
         Transform change = Desired*T_inverse*currentPoseT;
         quat newRotation = change.r();
         vec3 newDisplacement = change.t();
         currentPose.rootPos = newDisplacement;
         currentPose.jointRots[0] = newRotation;
         result.appendKey(currentPose);
      }
      return result;
   }

   void update()
   {
      _time += dt() * 0.5;
      _reoriented.update(_skeleton, _time);
   }

   void scene()
   {  
      update();
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
   }

   void keyUp(int key, int mods) 
   {
      if (key == GLFW_KEY_LEFT)
      {
         _heading += M_PI/8;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
      }
      else if (key == GLFW_KEY_RIGHT)
      {
         _heading -= M_PI/8;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
      }

      if (key == 'W')
      {
         _offset[2] += 25;
         _reoriented = reorient(_motion, _offset, _heading);
         std::cout << _offset << std::endl;
         _time = 0;
      }
      else if (key == 'S')
      {
         _offset[2] -= 25;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
         std::cout << _offset << std::endl;
      }
      else if (key == 'A')
      {
         _offset[0] += 25;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
         std::cout << _offset << std::endl;
      }
      else if (key == 'D')
      {
         _offset[0] -= 25;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
         std::cout << _offset << std::endl;
      }
   }

   Skeleton _skeleton;
   Motion _motion;
   Motion _reoriented;
   vec3 _offset;
   float _heading;
   float _time;
};

int main(int argc, char** argv)
{
   AReorient viewer;
   viewer.run();
   return 0;
}

