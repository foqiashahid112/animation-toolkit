#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

std::string filename; 

class MotionViewer : public atkui::Framework
{
public:
   MotionViewer() : atkui::Framework(atkui::Perspective) {
   }

   void setup() {
      BVHReader reader;
      reader.load(filename, skeleton, motion);
      motion.update(skeleton, 0);
   }

   void scene() {

      setColor(vec3(0,0,0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++) {
         Joint* joint = skeleton.getByID(i);
         if (joint->getParent() == 0) continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      if(!paused){
         time += dt();
         motion.update(skeleton, time);
         currentFrame = motion.getKeyID(time);
      }
      if(paused){
        Pose pose = motion.getKey(currentFrame);
        skeleton.setPose(pose);
      }

      drawText(paused? "Paused" : "Playing", 10, 15);
      drawText("Current frame: "+std::to_string(currentFrame), 10, 35);
      drawText("Time scale: "+std::to_string(timeScale), 10, 55);
   }

   virtual void keyUp(int key, int mods) {
      if (key == GLFW_KEY_P) {
         //Pressing the 'P' key should toggle play and pause
         paused = !paused;
      }
      if (key == GLFW_KEY_0){
         //Pressing the '0' key should reset the motion to the first frame
         time = 0.0f;

      }
      if (key == GLFW_KEY_PERIOD){
         //If the motion is paused, pressing '.' should step forward one frame.
         //The motion should wrap to the first frame.
         currentFrame = (currentFrame + 1) % motion.getNumKeys();
      }
      if (key == GLFW_KEY_COMMA){
         //If the motion is paused, pressing ',' should step backward one frame. 
         //The motion should wrap to the last frame.
         currentFrame = (currentFrame -1);
         if(currentFrame <= 0){
            currentFrame = motion.getNumKeys();
         }
      }
      if (key == GLFW_KEY_RIGHT_BRACKET){
         //Pressing the ']' key should increase the playback speed.
         motion.setDeltaTime((1.0 / 2.0) * motion.getDeltaTime());
      }
      if (key == GLFW_KEY_LEFT_BRACKET){
         //Pressing the '[' key should decrease the playback speed.
         motion.setDeltaTime(2* motion.getDeltaTime());
      }
   }

private:
   Skeleton skeleton;
   Motion motion;
   float timeScale = 1.0f;
   int currentFrame = 0; 
   bool paused = false;
   float time = 0;
};


int main(int argc, char** argv) {
   MotionViewer viewer;
   if(argc == 1){
      filename = "../motions/Beta/jump.bvh";
   } else{
      filename = argv[1];
   }
   viewer.run();
}
