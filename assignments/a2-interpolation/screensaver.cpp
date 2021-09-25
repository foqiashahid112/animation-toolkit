#include "atkui/framework.h"
#include <math.h>       /* fmod */
using namespace glm;
using namespace std;

class Screensaver : public atkui::Framework {
 public:
  Screensaver() : atkui::Framework(atkui::Orthographic, 400, 400) {
  }

  void setup() {
    ctrlpts_1 = generator();
    ctrlpts_2 = generator();
    t = 0.0;
    color1 = agl::randomUnitVector();
    color2 = agl::randomUnitVector();
    colorc = color1;
    timer = 0.0;
    interval = 0.01;
    maxsize = 50;
  }

  void scene(){
    //curves.push_back(ctrlpts_1);
    //curves.push_back(ctrlpts_2);
    drawBernstein(ctrlpts_1[0], ctrlpts_1[1],ctrlpts_1[2],ctrlpts_1[3], color1);
    drawBernstein(ctrlpts_2[0],ctrlpts_2[1],ctrlpts_2[2],ctrlpts_2[3], color2);

    //vector<vec3> fir = curves.back();
    //curves.pop_back();
    //vector<vec3> sec = curves.back();
    //curves.pop_back();
    // drawBernstein(fir[0], fir[1],fir[2],fir[3], color1);
    // drawBernstein(sec[0],sec[1],sec[2],sec[3], color2);
    
    float rate = 0.5;
    if(t > 1){
      for(int i = 0; i < 4; i++){
        ctrlpts_1[i] = ctrlpts_2[i];
      }
      ctrlpts_2 = generator();
      t = 0.0;
      color1 = agl::randomUnitVector();
      color2 = agl::randomUnitVector();
      colorc = color1;
    }
      colorc = color_interp(color1, color2, t);
      addCurrent(t, colorc);
      t+= rate * dt(); 
    
     for(std::list<vector<vec3>>::iterator it=curves.begin(); it != curves.end(); ++it){
       drawCurve(*it, colorc);
     }

    timer += interval * dt();
  }

  void drawCurve(vector<vec3> mycurve, vec3 color){
    drawBernstein(mycurve[0], mycurve[1], mycurve[2], mycurve[3], colorc);
  }

  void addCurrent(float T, vec3 color){
    ctrlpts_c = generator_2(ctrlpts_1, ctrlpts_2, T);
    //drawBernstein(ctrlpts_c[0], ctrlpts_c[1],ctrlpts_c[2],ctrlpts_c[3], colorc); 
    if(curves.size() > maxsize){
      curves.pop_front();
    }
      curves.push_back(ctrlpts_c);
    
  }

  vec3 color_interp(vec3 init, vec3 final, float T){
    return vec3(init * (1-T) + final * T);
  }
  vector<vec3> generator_2(vector<vec3> fir, vector<vec3> sec, float T){
    vector<vec3> ctrlpts;
    for(int i = 0; i < 4; i++){
      ctrlpts.push_back(fir[i] * (1-T) + sec[i] * T);
    }
    return ctrlpts;
  }

  vector<vec3> generator(){
    vector<vec3> ctrlpts;
    float max = 400.0f;
    float x;
    float y;
    for(int i = 0; i < 4; i++){
      x = fmod(rand(),max);
      y = fmod(rand(),max);
      vec3 ctrl = vec3(x,y,0);
      ctrlpts.push_back(ctrl);
    }
    return ctrlpts;
  }

  vec3 Bern(vec3 B0, vec3 B1, vec3 B2, vec3 B3, double t){
    float bern_1 = 1;
    float bern_2 = 0;
    float bern_3 = 0;
    float bern_4 = 0;
    bern_1 = pow((1-t),3.0);
    bern_2 = 3*t*pow((1-t), 2.0);
    bern_3 = 3 * pow(t,2.0) * (1-t);
    bern_4 = pow(t, 3.0);
    return bern_1* B0 + bern_2 * B1 + bern_3 * B2 + bern_4* B3;
  }

  void drawBernstein(vec3 B0, vec3 B1, vec3 B2, vec3 B3, vec3 color){
    setColor(color);
    float t = 0.00;
    while( t < 1.00){
      vec3 p1 = Bern(B0,B1, B2, B3,t);
      t+= 0.01;
      vec3 p2 = Bern(B0,B1,B2,B3,t);
      drawLine(p1,p2);
    }
  }

  list<vector<vec3>> curves;
  vector<vec3> ctrlpts_1;
  vector<vec3> ctrlpts_2;
  vector<vec3> ctrlpts_c;
  vec3 color1;
  vec3 color2;
  vec3 colorc;
  float t;
  float timer;
  int maxsize;
  float interval;
};

int main(int argc, char** argv) {
  srand(time(NULL));
  Screensaver viewer;
  viewer.run();
}

