#include <cmath>
#include "spline.h"
#include "math.h"
#include "interpolator_linear.h"

// global interpolator to use as default
static InterpolatorLinear gDefaultInterpolator; 

Spline::Spline() : 
  mDirty(true),
  mInterpolator(&gDefaultInterpolator) {
}

Spline::~Spline() {
}

void Spline::addInterpolationType(const std::string& name, Interpolator* interp) {
  mInterpolators[name] = interp;
}

void Spline::setInterpolationType(const std::string& name) {
  assert(mInterpolators.count(name) != 0);
  mInterpolator = mInterpolators[name];
  mDirty = true;
}

const std::string& Spline::getInterpolationType() const {
  return mInterpolator->getType();
}

void Spline::editKey(int keyID, const glm::vec3& value) {
  assert(keyID >= 0 && keyID < (int) mKeys.size());
  mKeys[keyID] = value;
  mDirty = true;
}

int Spline::appendKey(float time, const glm::vec3& value) {
  mKeys.push_back(value);
  mTimes.push_back(time);
  mDirty = true;
  return mKeys.size();
}

void Spline::deleteKey(int keyID) {
  //assert(keyID >= 0 && keyID < (int) mKeys.size());
  mKeys.erase(mKeys.begin() + keyID);
  mTimes.erase(mTimes.begin() + keyID);
  mDirty = true;
}

glm::vec3 Spline::getKey(int keyID) const {
  //assert(keyID >= 0 && keyID < (int) mKeys.size());
  return mKeys[keyID];
}

float Spline::getTime(int keyID) const {
  assert(keyID >= 0 && keyID < (int) mKeys.size());
  return mTimes[keyID];
}

int Spline::getNumKeys() const {
  return (int) mKeys.size();
}

void Spline::clear() {
  mKeys.clear();
  mTimes.clear();
  mInterpolator->clearControlPoints();
}

float Spline::getDuration() const {
  if (mKeys.size() == 0) return 0;
  int lastIdx = mKeys.size() - 1;
  return mTimes[lastIdx];
}

int Spline::getNumControlPoints() const {
  return mInterpolator->getNumControlPoints();
}

const glm::vec3& Spline::getControlPoint(int id) const {
  return mInterpolator->getControlPoint(id);
}

int Spline::getNumSegments() const {
  if (mKeys.size() > 1) return mKeys.size()-1;
  return 0;
}

void Spline::computeControlPoints() {
  mInterpolator->computeControlPoints(mKeys);
}

void Spline::editControlPoint(int id, const glm::vec3& v) {
  mInterpolator->editControlPoint(id, v);
}

// void Spline::naturalHermite() {
//   if(getInterpolationType() == "Hermite")
//     mInterpolator->setClamped(false);
// }

glm::vec3 Spline::getValue(float t) const {
  if (mDirty) 
  {
    mInterpolator->computeControlPoints(mKeys);
    mDirty = false; //mDirty is like a switch, everytime we append it becomes true so we computeControl points again
  }

  if(getNumControlPoints() == 0){
    return glm::vec3(0,0,0);
  }
  int segment = 0;
  for (int i = 0; i < mTimes.size(); i++){
    if(t <= getTime(0)){
      segment = 0;
      t = 0;
      break;
    } else if(t >= getDuration()){
      segment = mKeys.size()-2;
      t = getDuration();
      break;
    } else if(t > mTimes.at(i) && t <  mTimes.at(i+1)){
      segment = mTimes.at(i);
      break;
    }
  }

  //segment is supposed to be keyID
  float norm_t = (t - getTime(segment)) / (getTime(segment+1) - getTime(segment));
  glm::vec3 ans = mInterpolator->interpolate(segment, norm_t);
  
  return ans;
}

