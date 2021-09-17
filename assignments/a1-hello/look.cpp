#include "atkui/framework.h"

using namespace glm;

class Look : public atkui::Framework {
 public:
  Look() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;
    
    eyeRadius = 100;
  }

  virtual void scene() {
    vec2 mousePosition = agl::Window::mousePosition();
    if(mouseIsDown(GLFW_MOUSE_BUTTON_LEFT) == true){
      mouseMove(mousePosition[0], mousePosition[1]);
    }
    vec3 target = vec3(_mouseX, _mouseY, 0);
    setColor(vec3(1,0,0));
    drawSphere(target, 5);

    float c1_x = 0.3 * width();
    float c2_x = 0.7 * width();
    float c_y = 0.5 * height();
    float e1x = target[0] - c1_x;
    float e1y = target[1] - c_y;
    float e2x = target[0] - c2_x;
    float e2y = target[1] - c_y;

    float angle1 = atan2(e1y,e1x);
    float angle2 = atan2(e2y, e2x);

    setColor(vec3(1,1,1));
    drawSphere(vec3(c1_x, c_y, 0), eyeRadius);
    drawSphere(vec3(c2_x, c_y, 0), eyeRadius);
      
    float px1 = 0.2 * eyeRadius * cos(angle1);
    float py1 = 0.2 * eyeRadius * sin(angle1);
    float px2 = 0.2 * eyeRadius * cos(angle2);
    float py2 = 0.2 * eyeRadius * sin(angle2);
    setColor(vec3(0,0,0));
    drawSphere(vec3(c1_x + px1, c_y + py1, 100), 0.4 * eyeRadius);
    drawSphere(vec3(c2_x + px2, c_y + py2, 100), 0.4 * eyeRadius);

  }

  void mouseMove(int x, int y) {
    _mouseX = x;
    _mouseY = height() - y;
  }

 private:
  int _mouseX;
  int _mouseY;

  float eyeRadius;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
