#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include "atk/toolkit.h"
#include "atkui/skeleton_drawer.h"
#include "atkui/framework.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace atk;
using namespace glm;

class Unique : public atkui::Framework
{
 public:
  Unique() : atkui::Framework(atkui::Perspective),
                mDrawer(),
                mGoalPosition()
  {
    mDrawer.showAxes = true;
    mDrawer.color = vec3(0.6, 1.0, 0.4);
  }

  virtual ~Unique()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void setup()
  {
     t = 0;
    Joint *shoulder = new Joint("Shoulder");
    mActor.addJoint(shoulder);
    shoulder->setLocalTranslation(vec3(0, 0, 0));

    Joint *elbow = new Joint("Elbow");
    mActor.addJoint(elbow, shoulder);
    elbow->setLocalTranslation(vec3(100, 0, 0));

    Joint *wrist = new Joint("Wrist");
    mActor.addJoint(wrist, elbow);
    wrist->setLocalTranslation(vec3(80, 0, 0));

    mActor.fk();
    mGoalInitialPosition = vec3(500, 500, 500);
    mGoalPosition = mGoalInitialPosition;//wrist->getGlobalTranslation();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
#endif
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
  }

//   void drawGui()
//   {
//     ImGui_ImplOpenGL3_NewFrame();
//     ImGui_ImplGlfw_NewFrame();
//     ImGui::NewFrame();

//     ImGui::Begin("Demo Controls");
//     ImGui::SetWindowSize(ImVec2(150, 150));
//     ImGui::SetWindowPos(ImVec2(5, 5));
//     ImGui::SliderFloat("X", &mGoalPosition[0], -500.0f, 500.0f);
//     ImGui::SliderFloat("Y", &mGoalPosition[1], -500.0f, 500.0f);
//     ImGui::SliderFloat("Z", &mGoalPosition[2], -500.0f, 500.0f);
//     if (ImGui::Button("Reset")) reset();
//     ImGui::End();

//     // Rendering
//     ImGui::Render();
//     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//   }

//   void reset()
//   {
//     for (int i = 0; i < mActor.getNumJoints(); i++)
//     {
//       mActor.getByID(i)->setLocalRotation(atk::IdentityQ);
//     }
//     mActor.fk();
//     mGoalPosition = mActor.getByID(2)->getGlobalTranslation();
//   }

  void draw()
  {
    mDrawer.draw(mActor, *this);
    drawFloor(2000, 20, 50);

    vec2 screenPos = worldToScreen(mGoalPosition);

    setColor(vec3(1.0, 0.0, 1.0));
    ortho(0, width(), 0, height(), -1000, 1000);
    renderer.lookAt(vec3(0, 0, 1), vec3(0), vec3(0, 1, 0));

    vec3 c = vec3(screenPos, 0);
    vec3 v1 = c + vec3(10, 0, 0);
    vec3 v2 = c - vec3(10, 0, 0);
    vec3 h1 = c + vec3(0, 10, 0);
    vec3 h2 = c - vec3(0, 10, 0);

    beginShader("unlit");
    drawCircle(c, 5.0f);
    drawLine(v1, v2);
    drawLine(h1, h2);
    endShader();
   float LO = 400.0f; 
   float HI = 1000.0f;
   float x_initial = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
   float y_initial = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
   float z_initial = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
   float lo = 20;
   float hi = 130;
   float x_final = lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
   float y_final = lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
   float z_final = lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
    // reset projection
    perspective(glm::radians(60.0f), width() / (float)height(), 10.0f, 2000.0f);
    renderer.lookAt(camera.position(), camera.look(), camera.up());
      vec3 mGoalFinalPosition = vec3(60.0f, 40.0f, 130.0f);
      if(t > 1.1){
         t = 0.00;
        //  for(int i = 0; i < 2; i+=dt()){
        //  mGoalPosition = mGoalFinalPosition;
        //  }
         mGoalInitialPosition = vec3(x_initial, y_initial, z_initial);
         mGoalFinalPosition = vec3(x_final, y_final, z_final);
      }
      mGoalPosition = lerp(mGoalInitialPosition, mGoalFinalPosition, t);
      t+=dt();
    //drawGui();
    solveIKTwoLink(mActor, mGoalPosition);
  }

   vec3 lerp(vec3 initial, vec3 final, float time){
      return (1-t)*initial + t* final;
   }
  void solveIKTwoLink(Skeleton &skeleton, const vec3 &goalPosition)
  {
    // todo: implement two link IK algorithm
    Joint* root = skeleton.getByID(0);
    Joint* middle = skeleton.getByID(1);
    Joint* end = skeleton.getByID(2);
    vec3 rootLocation = root->getGlobalTranslation();
    vec3 middleLocation = middle->getGlobalTranslation();
    vec3 endLocation = end->getGlobalTranslation();
    float r = length(goalPosition - rootLocation);
   //  float l1 = middleLocation[0] - rootLocation[0];
   //  float l2 = endLocation[0] - middleLocation[0];
   float l1 = length(middleLocation - rootLocation);
   float l2 = length(endLocation - middleLocation);

    float rr = (r*r - l1*l1 - l2*l2)/ (-2.0f * l1 * l2);
    if(rr < -1){
      rr = -1;
    }
    if(rr > 1){
      rr = 1;
    }
    float phi = acos(rr);
    float theta2Z = phi - M_PI;
    float ll = (-l2 * sin(theta2Z)) / r;
    float theta1Z = asin(ll);
    float gamma = asin(goalPosition[1]/r);
    float beta = atan2(-1.0f * goalPosition[2], goalPosition[0]);
    root->setLocalRotation(glm::angleAxis(beta, vec3(0,1,0))*glm::angleAxis(gamma, vec3(0,0,1))*glm::angleAxis(theta1Z, vec3(0,0,1)));
    middle->setLocalRotation(glm::angleAxis(theta2Z, vec3(0,0,1)));
    skeleton.fk();
  }

 private:
  atk::Skeleton mActor;
  atkui::SkeletonDrawer mDrawer;
  glm::vec3 mGoalPosition;
  float t;
  vec3 mGoalInitialPosition;
};

int main(int argc, char **argv)
{
  Unique viewer;
  viewer.run();
  return 0;
}
