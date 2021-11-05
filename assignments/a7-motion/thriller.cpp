#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"
#include <random>

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   virtual void setup() {
      pallet =
      {
      vec3(0,165,227)/255.0f,
      vec3(141,215,191)/255.0f,
      vec3(255,150,197)/255.0f,
      vec3(255,87,104)/255.0f
      };
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      vec3 position;
      vec3 color = vec3(1,0,0);
      float LOW = 1.0f;
      float HIGH = 30.0f;
      float size = LOW + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(HIGH-LOW)));
      _devil = Devil(position, color, size);
      for(int i = 0; i < 3; i++){
         for(int j = 0; j < 4; j++){
            color = abs(agl::randomUnitVector());
            float jitter1 = 50.0f + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(700.0f-50.0f)));
            float jitter2 = 50.0f + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(800.0f-50.0f)));
            position = vec3(i*20 + jitter1,0, j*20 + jitter2);
            _devil = Devil(position, color, size);
            Devils.push_back(_devil);
         }
      }
   }

   virtual void scene() {
      for(Devil _devil : Devils){
         _devil.draw(_skeleton, *this);
      }
      if (!_paused) _motion.update(_skeleton, elapsedTime());
      _devil.draw(_skeleton, *this);
   }

   virtual void keyUp(int key, int mods) {
      if (key == 'P') _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   Devil _devil;
   std::vector<Devil> Devils;
   std::vector<vec3> pallet;
};

int main(int argc, char** argv) {
   Thriller viewer;
   viewer.run();
}

