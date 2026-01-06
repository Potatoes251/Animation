#include "LibMath/Quaternion.h"
#include "LibMath/Arithmetic.h"

LibMath::Quaternion::Quaternion()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_w = 0;
}

LibMath::Quaternion::Quaternion(float x, float y, float z, float w)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

LibMath::Quaternion::Quaternion(Quaternion const& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;
	m_w = other.m_w;
}

LibMath::Quaternion::Quaternion(Radian angle, Vector3 axis)
{
	if (axis.magnitudeSquared() <= g_epsilon)
	{
		*this = identity();
		return;
	}
	axis.normalize();

	float halfAngle = angle.radian() * 0.5f;

	float sin = std::sin(halfAngle);
	float cos = std::cos(halfAngle);

	m_x = axis.m_x * sin;
	m_y = axis.m_y * sin;
	m_z = axis.m_z * sin;
	m_w = cos;
}

float LibMath::Quaternion::magnitude() const
{
	return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

float LibMath::Quaternion::magnitudeSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

bool LibMath::Quaternion::isUnit() const
{
	return fabs(magnitudeSquared() - 1.f) < g_epsilon;
}

LibMath::Quaternion& LibMath::Quaternion::normalize()
{
	float mag = magnitude();
	
	if (mag <= g_epsilon)
	{
		*this = identity();
		return *this;
	}


	m_x /= mag;
	m_y /= mag;
	m_z /= mag;
	m_w /= mag;

	return *this;
}

LibMath::Quaternion& LibMath::Quaternion::conjugate()
{
	m_x = -m_x;
	m_y = -m_y;
	m_z = -m_z;

	return *this;
}

LibMath::Quaternion LibMath::Quaternion::inverse() const
{
	Quaternion inverse = *this;
	if (isUnit())
	{
		return inverse.conjugate();
	}

	float invMagSquared = 1 / magnitudeSquared();

	inverse.m_x = -m_x * invMagSquared;
	inverse.m_y = -m_y * invMagSquared;
	inverse.m_z = -m_z * invMagSquared;
	inverse.m_w = m_w * invMagSquared;

	return inverse;
}

LibMath::Vector3 LibMath::Quaternion::rotate(Vector3 const& vec) const
{
	Quaternion qv(vec.m_x, vec.m_y, vec.m_z, 0.0f);

	Quaternion result = (*this) * qv * inverse();

	return Vector3(
		result.m_x,
		result.m_y,
		result.m_z
	);
}

float& LibMath::Quaternion::operator[](int index)
{
	if (index == 0)
	{
		return m_x;
	}
	else if (index == 1)
	{
		return m_y;
	}
	else if (index == 2)
	{
		return m_z;
	}
	else if (index == 3)
	{
		return m_w;
	}
	else
	{
		throw std::out_of_range("Invalid index: Quaternion only supports indices 0, 1, 2 and 3.");
	}
}

float LibMath::Quaternion::operator[](int index) const
{
	if (index == 0)
	{
		return m_x;
	}
	else if (index == 1)
	{
		return m_y;
	}
	else if (index == 2)
	{
		return m_z;
	}
	else if (index == 3)
	{
		return m_w;
	}
	else
	{
		throw std::out_of_range("Invalid index: Quaternion only supports indices 0, 1, 2 and 3.");
	}
}

bool LibMath::Quaternion::operator==(Quaternion const& other) const
{
	return fabs(m_x - other.m_x) < g_epsilon &&
		fabs(m_y - other.m_y) < g_epsilon &&
		fabs(m_z - other.m_z) < g_epsilon &&
		fabs(m_w - other.m_w) < g_epsilon;
}

LibMath::Quaternion LibMath::Quaternion::operator+(Quaternion const& other) const
{
	return Quaternion(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

LibMath::Quaternion LibMath::Quaternion::operator*(Quaternion const& other) const
{
	return Quaternion(
		m_w * other.m_x + m_x * other.m_w + m_y * other.m_z - m_z * other.m_y,
		m_w * other.m_y - m_x * other.m_z + m_y * other.m_w + m_z * other.m_x,
		m_w * other.m_z + m_x * other.m_y - m_y * other.m_x + m_z * other.m_w,
		m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z);
}

LibMath::Quaternion LibMath::Quaternion::slerp(Quaternion from, Quaternion to, float alpha)
{
	// Clamp interpolation factor
	alpha = clamp(alpha, 0.0f, 1.0f);

	// Compute the cosine of the angle between the two quaternions
	float dot =
		from.m_x * to.m_x +
		from.m_y * to.m_y +
		from.m_z * to.m_z +
		from.m_w * to.m_w;

	// Take shortest path
	if (dot < 0.0f)
	{
		dot = -dot;
		to.m_x = -to.m_x;
		to.m_y = -to.m_y;
		to.m_z = -to.m_z;
		to.m_w = -to.m_w;
	}

	if (dot > 0.9995f)
	{
		Quaternion result(
			from.m_x + alpha * (to.m_x - from.m_x),
			from.m_y + alpha * (to.m_y - from.m_y),
			from.m_z + alpha * (to.m_z - from.m_z),
			from.m_w + alpha * (to.m_w - from.m_w)
		);
		return result.normalize();
	}

	float theta0 = std::acos(dot);
	float theta = theta0 * alpha;

	float sinTheta = std::sin(theta);
	float sinTheta0 = std::sin(theta0);

	float s0 = std::cos(theta) - dot * sinTheta / sinTheta0;
	float s1 = sinTheta / sinTheta0;

	return Quaternion(
		s0 * from.m_x + s1 * to.m_x,
		s0 * from.m_y + s1 * to.m_y,
		s0 * from.m_z + s1 * to.m_z,
		s0 * from.m_w + s1 * to.m_w
	);
}

LibMath::Quaternion	LibMath::Quaternion::identity()
{
	return Quaternion(0, 0, 0, 1);
}