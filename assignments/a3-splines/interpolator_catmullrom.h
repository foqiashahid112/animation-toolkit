#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_
#include <iostream>
#include <string>
#include <vector>
#include "interpolator.h"
#include "glm/ext.hpp"

//--------------------------------
// Catmull-Rom 
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
    InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

    virtual glm::vec3 interpolate(int segment, double u) const {
      std::vector<glm::vec3> segmentCtrlPoints;
      if(segment == 0){
         for(int i = 0; i < 4; i++){
            segmentCtrlPoints.push_back(mCtrlPoints[i]);
         }
      }
      int i = (segment * 2) + segment;
      while(i < ((segment*2) + segment + 4)){
         segmentCtrlPoints.push_back(mCtrlPoints[i]);
         i++;
      }
      float t = float(u);
      float t2 = t * t;
      float t3 = t2 *t;
      float mt = 1 - t; 
      float mt2 = mt * mt;
      float mt3 = mt2 * mt;
      //Now that we have this segments control points, lets compute the interpolated value on the given segment
      // see: interpolator.h and writeup for details
       return segmentCtrlPoints[0] * mt3 + 3.0f * segmentCtrlPoints[1]*mt2*t + 3.0f * segmentCtrlPoints[2]*mt*t2 + segmentCtrlPoints[3] * t3;
      
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
      mCtrlPoints.clear();
      glm::vec3 b_0, b_1,b_2,b_3;
      if(keys.size() >= 2){
         for(int i = 0; i < keys.size()-1; i++){
            b_0 = keys[i];
            mCtrlPoints.push_back(b_0);
            if(i == 0){
               b_1 = keys[i] + (1.0f/6.0f) * (keys[i+1]- keys[i]);
               mCtrlPoints.push_back(b_1);
            } else{
               b_1 = keys[i] + (1.0f/6.0f) * (keys[i+1]-keys[i-1]);
               mCtrlPoints.push_back(b_1);
            }
            if(i == keys.size()-2){
               b_2 = keys[i+1] - (1.0f/6.0f) * (keys[keys.size() -1] - keys[i]);
               mCtrlPoints.push_back(b_2);
               
            } else{
               b_2 = keys[i+1] - (1.0f/6.0f) * (keys[i+2]-keys[i]);
               mCtrlPoints.push_back(b_2);
            }
            if(i == keys.size()- 2){
               b_3 = keys[i+1];
               mCtrlPoints.push_back(b_3);
            }
         }
      }
    }    
};

#endif
