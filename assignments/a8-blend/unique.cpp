// #include "atkui/framework.h"
// using namespace glm;

// int main(int argc, char** argv) {
//    return 0;
// }

#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~Unique() {}

   void setup()
   {
      _alpha = 0.5;
      BVHReader reader;
      reader.load("../motions/Beta/left_strafe_walking.bvh", _skeleton, _motion1);
      reader.load("../motions/Beta/walking.bvh", _skeleton, _motion2);
      reader.load("../motions/Beta/right_turn_90.bvh", _skeleton, _motion3);
      reader.load("../motions/Beta/samba_dancing.bvh", _skeleton, _dance);

      _blend = blend(_motion1, _motion2, _alpha);
      //find location (offset) and rotation 
      Pose lastPose = _blend.getKey(_blend.getNumKeys() - 1);
      vec3 positionLastPose = lastPose.rootPos;
      quat rotationLastPose = lastPose.jointRots[0];
      _offset = positionLastPose;
      _heading = rotationLastPose;

      _reoriented3 = reorient(_motion3, _offset, _heading);

      _mixed = addMotion(_blend, _reoriented3);

      Pose lastPose1 = _mixed.getKey(_mixed.getNumKeys() - 1);
      vec3 positionLastPose1 = lastPose1.rootPos;
      quat rotationLastPose1 = lastPose1.jointRots[0];
      _offset1 = positionLastPose1;
      _heading1 = rotationLastPose1;
      _motion2Reoriented = reorient(_motion2, _offset1, _heading1);
      _mixed2 = addMotion(_mixed, _motion2Reoriented);

      Pose lastPose2 = _mixed2.getKey(_mixed2.getNumKeys()-1);
      vec3 positionLastPose2 = lastPose2.rootPos;
      quat rotationLastPose2 = lastPose2.jointRots[0];
      _offset2 = positionLastPose2;
      _heading2 = rotationLastPose2;
      _danceReoriented = reorient(_dance, _offset2, _heading2);

      _mixed3 = blendedMotion(_mixed2, _danceReoriented, _alpha); //intermediary motion
      _mixed4 = addMotion(_mixed2, _mixed3);
      _mixed5 = addMotion(_mixed4, _danceReoriented); //
      //_mixed3 = addMotion(_mixed2, _danceReoriented);
   }

   Motion addMotion(const Motion& m1, const Motion& m2){
      Motion mixed;
      mixed.setFramerate(m1.getFramerate());
      // float duration1 = m1.getDuration();
      // float duration2 = m2.getDuration();
      // float durationNew = duration1 + duration2;

      Pose toAddPose;
      for(int i = 0; i< m1.getNumKeys(); i++){
         toAddPose = m1.getKey(i);
         mixed.appendKey(toAddPose);
      }
      for(int i = 0; i < m2.getNumKeys(); i++){
         toAddPose = m2.getKey(i);
         mixed.appendKey(toAddPose);
      }
      return mixed;
   }

   Motion reorient(const Motion& motion, const vec3& pos, quat rotation)
   {
      Motion result;
      result.setFramerate(motion.getFramerate());

      // todo: your code here
      vec3 displacement2 = pos;
      quat rotation2 = rotation;
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


   Motion blendedMotion(const Motion& m1, const Motion& m2, double alpha){
      Motion blend;
      blend.setFramerate(m1.getFramerate());
      // todo: replace the following line with your code
      Pose pose1 = m1.getKey(m1.getNumKeys() -1);
      Pose pose2 = m2.getKey(0);
      for(float i = 0; i < 10; i++){
         Pose newPose = Pose::Lerp(pose1, pose2, alpha);
         blend.appendKey(newPose);
      }
      return blend;
   }

   Motion blend(const Motion& m1, const Motion& m2, double alpha)
   {
      Motion blend;
      blend.setFramerate(m1.getFramerate());
      // todo: replace the following line with your code
      float duration_1 = m1.getDuration();
      float duration_2 = m2.getDuration();
      float duration = duration_1 * (1-alpha) + duration_2 * alpha;
      float deltaT = 1/blend.getFramerate();
      for(float t = 0; t < duration; t+=deltaT){
         float t1 = (t / duration) * duration_1;
         float t2 = (t / duration) * duration_2;
         Pose pose1 = m1.getValue(t1);
         Pose pose2 = m2.getValue(t2);
         Pose newPose = Pose::Lerp(pose1, pose2, alpha);
         blend.appendKey(newPose);
      }
      //blend.appendKey(m1.getKey(0)); // placeholder
      return blend;
   }

   void scene()
   { 
      drawMotion(_mixed5, vec3(0,0,0));
      //drawMotion(_blend, vec3(0,0,0));
      drawText(std::to_string(_alpha), 10, 15);
   }

   void drawMotion(const Motion& m, const vec3& offset)
   {
      double t = elapsedTime() * 0.5;
      double time = m.getNormalizedDuration(t) * m.getDuration();
      m.update(_skeleton, time, false);

      SkeletonDrawer drawer;
      drawer.pos = offset;
      drawer.draw(_skeleton, *this);
   }


protected:
   Skeleton _skeleton;
   Motion _blend;
   Motion _motion1;
   Motion _motion2;
   Motion _motion3;
   Motion _dance;
   Motion _mixed;
   Motion _mixed2;
   Motion _mixed3;
   Motion _mixed4;
   Motion _mixed5;
   double _alpha;

   Motion _reoriented3;
   Motion _motion2Reoriented;
   Motion _danceReoriented;
   vec3 _offset;
   vec3 _offset1;
   vec3 _offset2;
   quat _heading;
   quat _heading1;
   quat _heading2;
};

int main(int argc, char** argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}


