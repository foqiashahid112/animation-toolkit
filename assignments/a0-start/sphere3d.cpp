#include "atkui/framework.h"

using glm::vec3;
using namespace std;

class Sphere3D : public atkui::Framework {
 public:
  Sphere3D() : atkui::Framework(atkui::Perspective) {
  }

  virtual void setup(){
    currentPos = vec3(0,0,0);
    vel = vec3(0,0,0);
    spacePressed = false;
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    // draw a sphere at center of the world
    float radius = 50.0;
    drawSphere(currentPos, radius);
    if (spacePressed == true){
      currentPos = currentPos + vel * dt();
    }
  }

  //@Override keyUp in Sphere3D
  void keyUp(int key,int mods){
    if (key == GLFW_KEY_SPACE){
      //Green sphere moves in random direction
      vel = 100.0f * agl::randomUnitVector();
      spacePressed = true;
    }
    if(key == GLFW_KEY_R){
      //reset position of the sphere to (0,0,0)
      currentPos = vec3(0,0,0);
      spacePressed = false;
    }
  }
  bool spacePressed;
  vec3 currentPos;
  vec3 vel;
};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
