#include "ik_controller.h"
#include <cmath>
#include <iostream>

using namespace atk;
using namespace glm;
using namespace std;

bool IKController::solveIKAnalytic(Skeleton& skeleton, 
    int jointid, const vec3& goalPos, float epsilon) {
  if (jointid == -1) return true;

  Joint* ankle = skeleton.getByID(jointid);
  if (!ankle->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* knee = ankle->getParent();
  if (!knee->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* hip = knee->getParent();

  // TODO: Your code here
  return true;
}

// solveIKCCD positions the joint given by jointid so its global position
// is located at goalPos
//
// param skeleton: the character to modify
// param jointid: the ID of the joint to pose
// param goalPos: the target position for jointid (global pos)
// param chain: the list of joints to "nudge" towards the goal
// param threshold: when the given joint is within threshold of the goal, stop iterating
// param maxIters: the max number of iterations to try to reach the goal
//
// return true/false based on whether we could reach the goal
// side effect: skeleton should by posed such that jointid is located at goalPos (or in this direction of
// goalPos if the target is out of reach)
bool IKController::solveIKCCD(Skeleton& skeleton, int jointid, 
    const vec3& goalPos, const std::vector<Joint*>& chain, 
    float threshold, int maxIters, float nudgeFactor) {
  // There are no joints in the IK chain for manipulation
  if (chain.size() == 0) return true;
  Joint* currentJoint = skeleton.getByID(jointid);
  vec3 pos = currentJoint->getGlobalTranslation();
  float distance = length(goalPos -pos);
  int Iters = 0;
  while(distance > threshold && Iters < maxIters){
    for(int i = 0; i < chain.size(); i++){
      Joint* thisJoint = chain[i];
      vec3 thisPosition = thisJoint->getGlobalTranslation();
      //nudge the joint towards pd
      vec3 e = goalPos - pos;
      if(e == vec3(0,0,0)){
       continue;
      }
      vec3 r = pos - thisPosition;
      vec3 R_Cross_E = cross(r, e);
      if(R_Cross_E == vec3(0,0,0)){
        continue;
      }
      float R_Cross_E_length = length(R_Cross_E);
      float re_plus_rr = dot(r,e) + dot(r,r);
      float phi = atan2(R_Cross_E_length, re_plus_rr);
      vec3 axis = R_Cross_E/ R_Cross_E_length;
      float nudge = nudgeFactor * phi;
      Joint* parent = thisJoint->getParent();
      quat thisRotation = angleAxis(nudge, inverse(parent->getGlobalRotation())*axis);
      thisJoint->setLocalRotation(thisRotation * thisJoint->getLocalRotation());
      thisJoint->fk();
      //update p with end effector
      pos = currentJoint->getGlobalTranslation(); 
      distance = length(goalPos -pos);
    }
    Iters++;
  }
  // TODO: Your code here
  return false;
}
