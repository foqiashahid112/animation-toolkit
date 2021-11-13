#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>
#include "atk/joint.h"

using namespace atk;
using namespace atkui;
using namespace glm;

class ASplice : public atkui::Framework
{
public:
   ASplice() : atkui::Framework(atkui::Perspective) {}
   virtual ~ASplice() {}

   void setup()
   {
      _alpha = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   Motion spliceUpperBody(const Motion& lower, const Motion& upper, float alpha)
   {
      Motion result;
      result.setFramerate(lower.getFramerate());//The motion returned by blend should have a framerate equal to orig
      for(int i = 0; i < lower.getNumKeys(); i++){ //for each pose in lower
         Pose pose_orig = lower.getKey(i); //original pose
         Pose pose_upper = upper.getKey(i+120); //Motion for modifying the upper body
         Pose new_pose = pose_orig;
         for(int ii = 0; ii < _skeleton.getNumJoints(); ii++){ // for each joint
            Joint* j = _skeleton.getByID(ii);
            if(isUpperBody(j)){ //if joint is in upper body
               quat q = glm::slerp(pose_upper.jointRots[ii], pose_orig.jointRots[ii], alpha);
               new_pose.jointRots[ii] =  q; //pose_upper.jointRots[ii]; 
               }
         }
         result.appendKey(new_pose);
      }
      return result;
   }

   bool isUpperBody(Joint* j){
      for(Joint* current = j; current != NULL; current = current->getParent()){
         if(current->getName() == "Beta:Spine1"){
            return true;
         }
      }
      return false;
   }

   void scene()
   {  
      _splice.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: "+std::to_string(_alpha), 10, 15);
   }

   void keyUp(int key, int mods) 
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha+0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha-0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
};

int main(int argc, char** argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}

