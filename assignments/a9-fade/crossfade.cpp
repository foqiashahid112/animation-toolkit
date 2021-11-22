#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <stdio.h>
#include <string.h>

using namespace atk;
using namespace atkui;
using namespace glm;

class ACrossfadeViewer : public atkui::Framework
{
public:
  ACrossfadeViewer() : atkui::Framework(atkui::Perspective) {}
  virtual ~ACrossfadeViewer() {}

  void loadMotion1(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion1_);
  }

  void loadMotion2(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion2_);
  }

  void crossfade(int numBlendFrames)
  {
    assert(motion1_.getNumKeys() > 0);
    assert(motion2_.getNumKeys() > 0);
    assert(numBlendFrames > 0);
    assert(numBlendFrames <= motion1_.getNumKeys());
    assert(numBlendFrames <= motion2_.getNumKeys());
    int start1 = motion1_.getNumKeys() - numBlendFrames;
    int start2 = 0;
    blend_.setFramerate(motion1_.getFramerate());
      Pose toAddPose;
      //1.1: Append
      // for(int i = 0; i< motion1_.getNumKeys(); i++){
      //    toAddPose = motion1_.getKey(i);
      //    blend_.appendKey(toAddPose);
      // }
      // for(int i = 0; i < motion2_.getNumKeys(); i++){
      //    toAddPose = motion2_.getKey(i);
      //    blend_.appendKey(toAddPose);
      // }


    //First add motion 1 until the frames you want to blend
    for(int i = 0; i< motion1_.getNumKeys() - numBlendFrames; i++){
      toAddPose = motion1_.getKey(i);
      blend_.appendKey(toAddPose);
    }

    //Compute Desired Transformation (motion 1's key before blending)
    vec3 displacementD = motion1_.getKey(motion1_.getNumKeys() - numBlendFrames).rootPos; //-1 instead of numBlendFrames
    quat rotationD = motion1_.getKey(motion1_.getNumKeys() - numBlendFrames).jointRots[0];
    // float heading = glm::eulerAngles(rotationD)[1];
    // quat therotation = glm::angleAxis(heading, vec3(0,1,0));
    Transform Desired(rotationD, displacementD);

    //Compute Motion 2's first key's transform and then take inverse
    Pose rootPose = motion2_.getKey(0);
    vec3 rootDisplacement = rootPose.rootPos;
    quat rootRotation = rootPose.jointRots[0];
    Transform T1(rootRotation, rootDisplacement);
    Transform T_inverse = T1.inverse();
    //std::cout << "T Inverse" << T_inverse << std::endl;
    float alpha = 0;
    for(int i = 0; i < numBlendFrames; i++){
      Pose pose1 = motion1_.getKey(start1 + i);
      Pose pose2 = motion2_.getKey(start2 + i);
      vec3 thisDisplacement = pose2.rootPos;
      quat thisRotation = pose2.jointRots[0]; 
      Transform newPoseT(thisRotation, thisDisplacement);
      Transform change = Desired*T_inverse*newPoseT;
      //std::cout << i << " " << change << std::endl;
      quat newRotation = change.r();
      vec3 newDisplacement = change.t();
      Pose newPose = Pose::Lerp(pose1, pose2, alpha);
      newPose.rootPos = newDisplacement;
      newPose.jointRots[0] = newRotation;
      blend_.appendKey(newPose);
      alpha+=(1.0/numBlendFrames);
    }
    for(int i = numBlendFrames; i < motion2_.getNumKeys(); i++){
      toAddPose = motion2_.getKey(i);
      vec3 thisDisplacement = toAddPose.rootPos;
      quat thisRotation = toAddPose.jointRots[0];
      Transform toAddPoseT(thisRotation, thisDisplacement);
      Transform change = Desired*T_inverse*toAddPoseT;
      quat newRotation = change.r();
      vec3 newDisplacement = change.t();
      toAddPose.rootPos = newDisplacement;
      toAddPose.jointRots[0] = newRotation; 
      blend_.appendKey(toAddPose);

    }
  }

  void save(const std::string &filename)
  {
    BVHWriter writer;
    writer.save(filename, skeleton_, blend_);
  }

  void scene()
  {
    blend_.update(skeleton_, elapsedTime());
    drawer_.draw(skeleton_, *this);
  }

private:
  Skeleton skeleton_;
  SkeletonDrawer drawer_;
  Motion motion1_;
  Motion motion2_;
  Motion blend_;
};

std::string PruneName(const std::string &name)
{
  int idx1 = name.find_last_of("/");
  int idx2 = name.find_last_of(".");
  return name.substr(idx1 + 1, idx2 - idx1 - 1);
}

int main(int argc, char **argv)
{
  std::string motion1 = "../motions/Beta/walking.bvh";
  std::string motion2 = "../motions/Beta/jump.bvh";
  int numFrames = 10;

  try
  {
    for (int i = 0; i < argc; i++)
    {
      if (strncmp(argv[i], "-m1", 3) == 0)
        motion1 = argv[i + 1];
      else if (strncmp(argv[i], "-m2", 3) == 0)
        motion2 = argv[i + 1];
      else if (strncmp(argv[i], "-nframes", 7) == 0)
        numFrames = std::stoi(argv[i + 1]);
    }
  }
  catch (const std::exception &e)
  {
    std::cout << "usage: crossfade -m1 <path> -m2 <path> -nframes <num frames>\n";
  }

  ACrossfadeViewer viewer;
  viewer.loadMotion1(motion1);
  viewer.loadMotion2(motion2);
  viewer.crossfade(numFrames);

  std::string saveName = "blend-" + PruneName(motion1) + PruneName(motion2);
  saveName += "-" + std::to_string(numFrames) + ".bvh";
  viewer.save(saveName);

  viewer.run();
  return 0;
}
