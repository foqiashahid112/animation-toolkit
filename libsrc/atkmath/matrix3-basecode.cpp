#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "atkmath/quaternion-basecode.cpp"

namespace atkmath {

Vector3 Matrix3::toEulerAnglesXYZ() const
{
   Vector3 angles;
   if(m13 ==1){ // y is 90 degrees
      angles[1] = PI/2.0f;
      angles[0] = 0.0f;
      angles[2] = atan2(m32,m22);
   } else if (m13 == -1){ //y is -90 degrees
      angles[1] = -PI/2.0f;
      angles[0] = atan2(-m21,m22); //for some reason dividng by m32 doesnt work 
      angles[2] = 0.0f;
   } else{
      angles[1] = asin(m13);
      angles[0]= atan2(-m23,m33);
      angles[2] = atan2(-m12, m11);
   }
   return angles;
}

Vector3 Matrix3::toEulerAnglesXZY() const
{
   Vector3 angles;
   if(m12 == 1){ //z is -90
      angles[2] = -PI/2.0f;
      angles[1] = 0.0f;
      angles[0] = atan2(-m31,-m21);
   } else if(m12 == -1){ //z is 90
      angles[2] = PI/2.0f;
      angles[1] = 0.0f;
      angles[0] = atan2(m31,m21);
   } else{
      angles[2] = asin(-m12);
      angles[1] = atan2(m13,m11);
      angles[0] = atan2(m32,m22);
   }
   return angles;
}

Vector3 Matrix3::toEulerAnglesYXZ() const
{
   Vector3 angles;
   if(m23 == 1){ // x is -90
      angles[0] = -PI/2.0f;
      angles[1] = 0.0f;
      angles[2] = atan2(-m31,m11);
   } else if(m23 == -1){ //x is 90
      angles[0] = PI/2.0f;
      angles[1] = 0.0f;
      angles[2] = atan2(-m12, m32);
   } else{
      angles[0] = asin(-m23);
      angles[1] = atan2(m13,m33);
      angles[2] = atan2(m21,m22);
   }
   return angles;
}

Vector3 Matrix3::toEulerAnglesYZX() const
{
   Vector3 angles;
   if(m21 == 1){ //z is 90 degrees
   angles[2] = PI/2.0;
   angles[1] = 0.0f;
   angles[0] = atan2(m32,m33);
   } else if(m21 == -1){ //z is -90 degrees
   angles[2] = -PI/2.0f;
   angles[1] = atan2(m13,m12);// 0.0f;
   angles[0] = 0.0f;//atan2(m32,m33);
   } else{ //
   angles[2] = asin(m21);
   angles[1] = atan2(-m31,m11);
   angles[0] = atan2(-m23, m22);
   }
   return angles;
}

Vector3 Matrix3::toEulerAnglesZXY() const
{
   Vector3 angles;
   if(m32 == 1){ // 90 Degrees
      angles[0] = PI / 2.0f;
      angles[1] = 0.0f;
      angles[2] = atan2(m21,m11);
   } else if(m32 == -1){ // -90
      angles[0] = -PI/2.0f;
      angles[1] = 0.0f;
      angles[2] = atan2(m21,m11);
   } else{ 
      angles[0] = asin(m32);
      angles[1] = atan2(-m31,m33);
      angles[2] = atan2(-m12, m22);
   }
   return angles;
}

Vector3 Matrix3::toEulerAnglesZYX() const
{
   Vector3 angles;
   if(m31 == -1){ //90 degrees
      angles[1] = PI / 2.0f;
      angles[2] = 0.0; //z
      angles[0] = atan2(m12, m13);
   } else if(m31 == 1){ //-90 degrees
      angles[1] = -PI/2.0f;  //y
      angles[2] = 0.0; //z
      angles[0] = atan2(-m12, -m13); //x
   }
   else {
      angles[1] = asin(-(this->m31)); //y
      angles[2] = atan2(this->m21, this->m11); //z
      angles[0] = atan2(this->m32, this->m33); //x
   }
   return angles;
}

void Matrix3::fromEulerAnglesXYZ(const Vector3& angleRad)
{
   //*this = Identity;
   Matrix3 xyz;
   xyz = Rx(angleRad[0])*Ry(angleRad[1])*Rz(angleRad[2]);
   *this = xyz; 
}

void Matrix3::fromEulerAnglesXZY(const Vector3& angleRad)
{
   //*this = Identity;
   Matrix3 xzy;
   xzy = Rx(angleRad[0])*Rz(angleRad[2])*Ry(angleRad[1]);
   *this = xzy;
}

void Matrix3::fromEulerAnglesYXZ(const Vector3& angleRad)
{
   //*this = Identity;
   Matrix3 yxz;
   yxz = Ry(angleRad[1])*Rx(angleRad[0])*Rz(angleRad[2]);
   *this = yxz;
}

void Matrix3::fromEulerAnglesYZX(const Vector3& angleRad)
{
   //*this = Identity;
   Matrix3 yzx; 
   yzx = Ry(angleRad[1])*Rz(angleRad[2])*Rx(angleRad[0]);
   *this = yzx;
}

void Matrix3::fromEulerAnglesZXY(const Vector3& angleRad)
{
   //*this = Identity;
   Matrix3 zxy; 
   zxy = Rz(angleRad[2])*Rx(angleRad[0])*Ry(angleRad[1]);
   *this = zxy;
}

void Matrix3::fromEulerAnglesZYX(const Vector3& angleRad)
{
   //*this = Identity;
   Matrix3 zyx; 
   zyx = Rz(angleRad[2])*Ry(angleRad[1])*Rx(angleRad[0]); 
   *this = zyx;
}

void Matrix3::toAxisAngle(Vector3& axis, double& angleRad) const
{
   Quaternion q;
   q.fromMatrix(*this);
   q.toAxisAngle(axis, angleRad);
}

void Matrix3::fromAxisAngle(const Vector3& axis, double angleRad)
{
   Matrix3 m;
   float x = axis[0];
   float y = axis[1];
   float z = axis[2];
   float xx = x*x;
   float yy = y*y;
   float zz = z*z;
   float xy = x*y;
   float yz = y*z;
   float xz = x*z;
   float c_theta = cos(angleRad);
   float s_theta = sin(angleRad);
   m[0][0] = c_theta + (1-c_theta)*xx;
   m[0][1]= -s_theta*x + (1 - c_theta)*xy;
   m[0][2] = y*s_theta + x*y*(1-c_theta);
   m[1][0] = -z*s_theta + (1-c_theta)*xy;
   m[1][1]= c_theta + (1-c_theta)*yy;
   m[1][2] = -x*s_theta + (1-c_theta)*yz;
   m[2][0] = s_theta*y + (1-c_theta)*xy;
   m[2][1]= -x*s_theta + (1-c_theta)*yz;
   m[2][2] = c_theta + (1-c_theta)*zz;
   *this = m;
}

Matrix3 Matrix3::Rz(double angleRad){
   Matrix3 Rz;
   Rz[0][0] = cos(angleRad); Rz[0][1] = -sin(angleRad); Rz[0][2] = 0;
   Rz[1][0] = sin(angleRad); Rz[1][1] = cos(angleRad); Rz[1][2] = 0;
   Rz[2][0] = 0;              Rz[2][1] = 0;           Rz[2][2] = 1;
    //std::cout << "rz" << Rz <<std::endl;
   return Rz;
}

Matrix3 Matrix3::Ry(double angleRad){
   Matrix3 Ry;
   Ry[0][0] = cos(angleRad); Ry[0][1] = 0; Ry[0][2] = sin(angleRad);
   Ry[1][0] = 0;              Ry[1][1] = 1; Ry[1][2] = 0;
   Ry[2][0] = -sin(angleRad); Ry[2][1] = 0; Ry[2][2] = cos(angleRad);
   //std::cout << "ry" << Ry <<std::endl;
   return Ry;
}

Matrix3 Matrix3::Rx(double angleRad){
   Matrix3 Rx;
   Rx[0][0] = 1; Rx[0][1] = 0; Rx[0][2] = 0;
   Rx[1][0] = 0; Rx[1][1] = cos(angleRad); Rx[1][2] = -sin(angleRad);

   Rx[2][0] = 0; Rx[2][1] = sin(angleRad); Rx[2][2] = cos(angleRad);
    //std::cout << "rx" << Rx <<std::endl;
   return Rx;
}

}