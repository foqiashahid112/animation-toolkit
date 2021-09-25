/* Question 1
* Use linear interpolation to gradually change from one color to another
* based on position.
* 
*/
#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class Gradient : public atkui::Framework {
 public:
  Gradient() : atkui::Framework(atkui::Orthographic) {
  }
  virtual void setup(){
    C_nw = vec3(1,1,0);
    C_ne = vec3(0,1,1);
    C_sw = vec3(1,0,0);
    C_se = vec3(1,0,1);
    t_y = 0;
    t_x = 0;
    n = 10;
    nn = (1.0/10.0);
  }

  virtual void scene() {
    //std::cout << "nn: "<< nn << std::endl;
    py = 0.5 * nn * height();
    for(int i = 0; i < n; i++){
      t_y = float(py) / float(height());
      px = 0.5 * nn * width();
      for (int j = 0; j < n; j++){
        t_x = float(px) / float(width());
        vec3 x_col_2 = C_nw * (1-t_x) + C_ne * t_x;
        vec3 x_col_1 = C_sw * (1-t_x) + C_se * t_x;
        vec3 y_col = x_col_1 * (1-t_y) + x_col_2 * t_y;
        setColor(y_col);
        drawCube(vec3(px,py,0), vec3(nn* width(),nn*height(),0));
        px += nn *width();
      }
      py += nn * height();
      //std::cout << t_y << std::endl;
    }
  }
  float nn;
  int n;
  float t_x;
  float t_y;
  vec3 C_nw;
  vec3 C_ne;
  vec3 C_sw;
  vec3 C_se;
  float px;
  float py;
};

int main(int argc, char **argv) {
  Gradient viewer;
  viewer.run();
}
