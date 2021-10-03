#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "interpolator.h"

//--------------------------------
// Hermite 
//--------------------------------
using namespace std;
using namespace Eigen;

class InterpolatorHermite : public Interpolator
{
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1,0,0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const {
        float t = float(u);
        float t2 = t * t;
        float t3 = t2 *t;
        float mt = 1 - t; 
        float mt2 = mt * mt;
        float mt3 = mt2 * mt;
        
        float H0 = mt3 + (3* mt2*t);
        float H1 = t * mt2;
        float H2 = -t2 + t3;
        float H3 = 3.0f*t2 - (2.0f*t3);
        return H0 * mCtrlPoints[segment*2] + H1 * mCtrlPoints[segment*2 + 1] + H2 * mCtrlPoints[segment*2 +3] + H3 * mCtrlPoints[segment*2 +2];

    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
        mCtrlPoints.clear();
        Eigen::MatrixXd A = Eigen::MatrixXd::Constant(keys.size(), keys.size(), 0);
        MatrixXd P(keys.size(),3); //known points
        MatrixXd P_(keys.size(),3); //unknown slopes for each point
       std::vector<glm::vec3> knowns; 
        // natural endpoints
        if(mIsClamped == false){
            A(0, 0) = 2;
            A(0,1) = 1;
            A(keys.size() - 1, keys.size() - 1) = 1;
           for(int i = 0; i < keys.size() -2; i++){
                    A(i+1, i) = 1;
                    A(i+1, i+1) = 4;
                    A(i+1, i+2) = 1;
            } 
            A(keys.size()-1, keys.size()-2) = 1;
            A(keys.size()-1, keys.size()-1) = 2;

            knowns.push_back(3.0f *(keys[1] - keys[0]));
            for(int i = 0; i < keys.size()-2; i++){
            knowns.push_back(3.0f* (keys[i+2] - keys[i]));
            }
            knowns.push_back(3.0f *(keys[keys.size()-1] - keys[keys.size()-2]));
        } else{ //Clamped
            A(0, 0) = 1;
            A(keys.size() - 1, keys.size() - 1) = 1;
            for(int i = 0; i < keys.size() -2; i++){
                A(i+1, i) = 1;
                A(i+1, i+1) = 4;
                A(i+1, i+2) = 1;
            }
            knowns.push_back(glm::vec3(1,0,0));
            for(int i = 0; i < keys.size()-2; i++){
            knowns.push_back(3.0f* (keys[i+2] - keys[i]));
            }
            knowns.push_back(glm::vec3(1,0,0));
        }

        for(int i = 0; i < keys.size(); i++){
            P(i, 0) = knowns.at(i)[0];
            P(i, 1) = knowns.at(i)[1];
            P(i, 2) = knowns.at(i)[2];
        }
        P_ = A.inverse() * P;
        glm::vec3 slope;
         for(int i = 0; i < keys.size(); i++){
            mCtrlPoints.push_back(keys[i]);
            slope = glm::vec3(P_(i,0), P_(i,1), P(i,2));
            mCtrlPoints.push_back(slope);
        } 
    }

    void setClamped(bool c) { mIsClamped = c; } //if true, then clampled, if false, then natural
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3& v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = true;
    glm::vec3 mClampDir;
};

#endif
