#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

namespace atkmath {

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, double t)
{
	// TODO
	return Quaternion(1,0,0,0);
}

void Quaternion::toAxisAngle (Vector3& axis, double& angleRad) const
{
	angleRad = 2.0 * acos(mW);
	if(angleRad == 0){
		axis[0] = 0;
		axis[1] = 0;
		axis[2] = 1;
	} else{
		axis[0] = mX / sin(angleRad /2.0);
		axis[1] = mY / sin(angleRad /2.0);
		axis[2] = mZ / sin(angleRad / 2.0);
	}
	axis.normalize();
}

void Quaternion::fromAxisAngle (const Vector3& axis, double angleRad)
{
	mX = axis[0] * double(sin(angleRad / 2.0));
	mY = axis[1] * double(sin(angleRad / 2.0));
	mZ = axis[2] * double(sin(angleRad / 2.0));
	mW = double(cos(angleRad / 2.0));
	normalize();

}

Matrix3 Quaternion::toMatrix () const
{
	Matrix3 m;
	double xx = mX * mX;
	double yy = mY * mY;
	double zz = mZ * mZ;
	double xy = mX * mY;
	double xz = mX * mZ;
	double yz = mY * mZ;
	double wz = mW * mZ;
	double wy = mW * mY;
	double wx = mW * mX;
	m[0][0] = 1.0 - 2.0 * (yy + zz);
	m[0][1] = 2.0 * (xy - wz);
	m[0][2] = 2.0 *(xz + wy);
	m[1][0] = 2.0 * (xy + wz);
	m[1][1] = 1.0 - 2.0 * (xx + zz);
	m[1][2] = 2.0 * (yz - wx);
	m[2][0] = 2.0 * (xz - wy);
	m[2][1] = 2.0 * (yz + wx);
	m[2][2] = 1.0 - 2.0 * (xx + yy);
	return m;
}

void Quaternion::fromMatrix(const Matrix3& rot)
{
	float ww = (1.0 / 4.0) * (rot[0][0] + rot[1][1] + rot[2][2] + 1.0);
	float xx = (1.0 / 4.0) * (1.0 + rot[0][0] - rot[1][1] - rot[2][2]);
	float yy = (1.0 / 4.0) * (1.0 - rot[0][0] + rot[1][1] - rot[2][2]);
	float zz = (1.0 / 4.0) * (1.0 - rot[0][0] - rot[1][1] + rot[2][2]);
	float list [4] = {xx, yy, zz, ww};
	
	for(int i = 1;i < 5; i++){
       if(list[0] < list[i])
           list[0] = list[i];
    }
	float largest = list[0];

	if(largest ==  ww){
		mW = sqrt(ww);
		mX = (1.0 / 4.0) * (rot[2][1] - rot[1][2]) * (1.0 / mW);
		mY = (1.0 / 4.0) * (rot[0][2] - rot[2][0])* (1.0 / mW);
		mZ = (1.0 / 4.0) * (rot[1][0] - rot[0][1])* (1.0 / mW);
	} else if( largest == xx){
		mX = sqrt(xx);
		mY = (1.0 / 4.0) * (rot[1][0] + rot[0][1]) * (1.0 / mX);
		mZ = (1.0 / 4.0) * (rot[0][2] + rot[2][0]) * (1.0 / mX);
		mW = (1.0 / 4.0) * (rot[2][1] - rot[1][2]) * (1.0 / mX);

	} else if (largest == yy){
		mY = sqrt(yy);
		mX = (1.0 / 4.0) * (rot[1][0] + rot[0][1]) * (1.0 / mY);
		mZ = (1.0 / 4.0) * (rot[1][2] + rot[2][1]) * (1.0 / mY);
		mW = (1.0 / 4.0) * (rot[0][2] - rot[2][0]) * (1.0 / mY);

	} else if (largest == zz){
		mZ = sqrt(zz);
		mX = (1.0 / 4.0) * (rot[0][2] + rot[2][0]) * (1.0 / mZ);
		mY = (1.0 / 4.0) * (rot[1][2] + rot[2][1]) * (1.0 / mZ);
		mW = (1.0 / 4.0) * (rot[1][0] - rot[0][1]) * (1.0 / mZ);
	}

}

}