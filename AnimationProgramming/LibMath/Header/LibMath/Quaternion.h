#ifndef LIBMATH_QUATERNION_H_
#define LIBMATH_QUATERNION_H_

#include "Angle/Radian.h"
#include "Vector/Vector3.h"

namespace LibMath
{
	class Quaternion
	{
	public:
		Quaternion();										// set all component to 0
		Quaternion(float, float, float, float);				// set all component individually
		Quaternion(Quaternion const&);						// copy all component
		Quaternion(Radian, Radian, Radian);					// create rotation from euler angles
		Quaternion(Radian, Vector3);						// create rotation from an angle and an axis
		~Quaternion() = default;

		float				magnitude() const;
		float				magnitudeSquared() const;
		bool				isUnit() const;
		Quaternion&			normalize();
		Quaternion&			conjugate();
		Quaternion			inverse() const;
		Vector3 			rotate(Vector3 const&) const;

		float&			operator[](int);					// return this quaternion component value
		float			operator[](int) const;				// return this quaternion component value

		bool			operator==(Quaternion const&) const;

		Quaternion		operator+(Quaternion const&) const;
		Quaternion		operator*(Quaternion const&) const;
		Quaternion		operator*(float) const;

		static Quaternion	slerp(Quaternion, Quaternion, float);
		static Quaternion	identity();						// return a valid quaternion with a rotation of 0

		float m_x;
		float m_y;
		float m_z;
		float m_w;
	};
}

#endif // !LIBMATH_QUATERNION_H_
