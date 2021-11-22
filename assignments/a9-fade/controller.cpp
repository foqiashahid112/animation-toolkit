#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include "atk/toolkit.h"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;
using namespace atk;

class WASDSimple : public atkui::Framework
{
public:
  WASDSimple() : atkui::Framework(atkui::Perspective) {}

  virtual void setup()
  {
    BVHReader reader;
    reader.load("../motions/Beta/walking.bvh", _skeleton, _walk);
    _drawer.showAxes = true;

    Pose frame0 = _walk.getKey(0);
    Pose frameN = _walk.getKey(_walk.getNumKeys() - 1);
    vec3 delta = frameN.rootPos - frame0.rootPos;
    float distance = delta.length();
    float duration = _walk.getDuration();
    speed = distance / duration;
  }

  virtual void scene()
  {
    update();
    _drawer.draw(_skeleton, *this);

    // draw heading
    vec3 p = _skeleton.getRoot()->getGlobalTranslation();
    p[1] = 10; // set height close above the floor

    setColor(vec3(0, 1, 1));
    push();
    translate(p);
    rotate(_heading, vec3(0, 1, 0));
    translate(vec3(0,0,25));
    scale(vec3(10, 10, 50));
    drawCylinder(vec3(0), 1.0f);
    pop();
  }

  virtual void update()
  {
    _walk.update(_skeleton, elapsedTime());

    // TODO: Your code here
    // TODO: Override the default camera to follow the character
    Joint* Head = _skeleton.getByName("Beta:Head");
    // quat initialRotation = _walk.getKey(0).jointRots[0];
    // Transform initialRotT(initialRotation,vec3(0,0,0));
    // vec3 headDirLocal = initialRotT.transformVector(vec3(1,0,0));
    vec3 headDirLocal = vec3(0,0,100); //Forward Z direction 
    quat identity = glm::angleAxis(0.0f, vec3(1,0,0));
    Transform T(identity, headDirLocal);
    Transform localToGlobal = Head->getLocal2Global();
    vec3 headDirGlobal = localToGlobal.transformVector(headDirLocal);
    vec3 headPosition = Head->getGlobalTranslation();
    vec3 pos = headPosition - (3.0f * headDirGlobal); // Use global position of the character’s head to compute camera position
    vec3 look = headDirGlobal; //global positions for the camera’s position and for the target where the camera is looking.
    lookAt(pos, look, vec3(0, 1, 0)); //up

    // update heading when key is down
    if (keyIsDown('D')) _heading -= 0.05;
    if (keyIsDown('A')) _heading += 0.05;
    
    vec3 fixedPos = _walk.getKey(0).rootPos;
    for(int i = 0; i < _walk.getNumKeys(); i++){
      Pose currentPose = _walk.getKey(i);
      vec3 direction = vec3(cos(_heading), 0, sin(_heading)); //heading is an angle
      vec3 velocity = speed * direction;
      fixedPos = fixedPos + velocity * dt();
      currentPose.rootPos = fixedPos;
      quat rotation = glm::angleAxis(_heading, vec3(0,1,0));
      currentPose.jointRots[0] = rotation;
      _walk.editKey(i, currentPose);
      vec3 headPosition = Head->getGlobalTranslation();
    vec3 pos = headPosition - (3.0f * headDirGlobal);
    }

  }

protected:
  float _heading;
  float speed;

  Motion _walk;
  Skeleton _skeleton;
  atkui::SkeletonDrawer _drawer;
};

int main(int argc, char **argv)
{
  WASDSimple viewer;
  viewer.run();
}
