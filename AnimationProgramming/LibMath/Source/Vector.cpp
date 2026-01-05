#include "LibMath/Vector.h"
#include "LibMath/Arithmetic.h"

LibMath::Vector2::Vector2() : m_x(0), m_y(0) {}

LibMath::Vector2::Vector2(float val) : m_x(val), m_y(val) {}

LibMath::Vector2::Vector2(float x, float y) : m_x(x), m_y(y) {}

LibMath::Vector2::Vector2(Vector2 const& other) : m_x(other.m_x), m_y(other.m_y) {}

LibMath::Vector2::Vector2(Vector2 const&& other) noexcept : m_x(other.m_x), m_y(other.m_y) {}

float& LibMath::Vector2::operator[](int index)
{
	if (index == 0)
	{
		return m_x;
	}
	else if (index == 1)
	{
		return m_y;
	}
	else
	{
		throw std::out_of_range("Invalid index: Vector2 only supports indices 0 and 1.");
	}
}

float LibMath::Vector2::operator[](int index) const
{
	if (index == 0)
	{
		return m_x;
	}
	else if (index == 1)
	{
		return m_y;
	}
	else
	{
		throw std::out_of_range("Invalid index: Vector2 only supports indices 0 and 1.");
	}
}

float LibMath::Vector2::magnitude() const
{
	return LibMath::squareRoot(m_x * m_x + m_y * m_y);
}

float LibMath::Vector2::magnitudeSquared() const
{
	return (m_x * m_x + m_y * m_y);
}

float LibMath::Vector2::dotProduct(Vector2 const& other) const
{
	return (m_x * other.m_x + m_y * other.m_y);
}

float LibMath::Vector2::crossProduct(Vector2 const& other) const
{
	return m_x * other.m_y - m_y * other.m_x;
}

LibMath::Vector2& LibMath::Vector2::operator=(Vector2 const& other)
{
	m_x = other.m_x;
	m_y = other.m_y;

	return *this;
}

LibMath::Vector2& LibMath::Vector2::operator=(Vector2 const&& other) noexcept
{
	m_x = other.m_x;
	m_y = other.m_y;

	return *this;
}

LibMath::Vector2 LibMath::Vector2::operator-()
{
	return Vector2(-m_x, -m_y);
}

LibMath::Vector2 LibMath::Vector2::operator+(Vector2 const& other) const
{
	return Vector2(m_x + other.m_x, m_y + other.m_y);
}

LibMath::Vector2 LibMath::Vector2::operator-(Vector2 const& other) const
{
	return Vector2(m_x - other.m_x, m_y - other.m_y);
}

LibMath::Vector2 LibMath::Vector2::operator*(float val) const
{
	return Vector2(m_x * val, m_y * val);
}

LibMath::Vector2 LibMath::Vector2::operator/(float val) const
{
	return Vector2(m_x / val, m_y / val);
}

LibMath::Vector2& LibMath::Vector2::operator+=(Vector2 const other)
{
	m_x += other.m_x;
	m_y += other.m_y;

	return *this;
}

LibMath::Vector2& LibMath::Vector2::operator-=(Vector2 const other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;

	return *this;
}

LibMath::Vector2& LibMath::Vector2::operator*=(float val)
{
	m_x *= val;
	m_y *= val;

	return *this;
}

LibMath::Vector2& LibMath::Vector2::operator/=(float val)
{
	m_x /= val;
	m_y /= val;

	return *this;
}

bool LibMath::Vector2::operator==(Vector2 const& other) const
{
	return (this->m_x == other.m_x && this->m_y == other.m_y);
}

bool LibMath::Vector2::isLongerThan(Vector2 const& other) const
{
	return magnitudeSquared() > other.magnitudeSquared();
}

bool LibMath::Vector2::isShorterThan(Vector2 const& other) const
{
	return magnitudeSquared() < other.magnitudeSquared();
}

bool LibMath::Vector2::isUnitVector() const
{
	return 1 == magnitudeSquared();
}

LibMath::Vector2& LibMath::Vector2::normalize()
{
	if (!isUnitVector())
	{
		float magnitude = this->magnitude();

		m_x = m_x / magnitude;
		m_y = m_y / magnitude;
	}
	return *this;
}

LibMath::Radian LibMath::Vector2::angleBetween(Vector2 const& other) const
{
	float dot = dotProduct(other);
	float magnitudes = magnitude() * other.magnitude();

	if (magnitudes == 0.0)
	{
		throw std::runtime_error("Cannot calculate angle with a zero-length vector.");
	}

	float cosTheta = dot / magnitudes;
	return Radian(acos(cosTheta));
}

LibMath::Vector2 LibMath::Vector2::projectOnto(Vector2 const& other) const
{
	float otherMagnitudeSquared = other.magnitudeSquared();

	float scalar = dotProduct(other) / otherMagnitudeSquared;

	return Vector2(scalar * other.m_x, scalar * other.m_y);
}

LibMath::Vector2 LibMath::Vector2::reflectOnto(const Vector2& other) const
{
	Vector2 projection = projectOnto(other);
	return Vector2(2 * projection.m_x - m_x, 2 * projection.m_y - m_y);
}

LibMath::Vector2 LibMath::Vector2::lerp(Vector2 const& A, Vector2 const& B, float t)
{
	return A + (B - A) * t;
}




LibMath::Vector3::Vector3() : m_x(0), m_y(0), m_z(0) {}

LibMath::Vector3::Vector3(float val) : m_x(val), m_y(val), m_z(val) {}

LibMath::Vector3::Vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

LibMath::Vector3::Vector3(Vector2 const& other, float z) : m_x(other.m_x), m_y(other.m_y), m_z(z) {}

LibMath::Vector3::Vector3(Vector3 const& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}

LibMath::Vector3 LibMath::Vector3::zero()
{
	return Vector3();
}

LibMath::Vector3 LibMath::Vector3::one()
{
	return Vector3(1);
}

LibMath::Vector3 LibMath::Vector3::up()
{
	return Vector3(0, 1, 0);
}

LibMath::Vector3 LibMath::Vector3::down()
{
	return Vector3(0, -1, 0);
}

LibMath::Vector3 LibMath::Vector3::left()
{
	return Vector3(-1, 0, 0);
}

LibMath::Vector3 LibMath::Vector3::right()
{
	return Vector3(1, 0, 0);
}

LibMath::Vector3 LibMath::Vector3::front()
{
	return Vector3(0, 0, 1);
}

LibMath::Vector3 LibMath::Vector3::back()
{
	return Vector3(0, 0, -1);
}

LibMath::Vector3& LibMath::Vector3::operator=(Vector3 const& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;

	return *this;
}

float& LibMath::Vector3::operator[](int index)
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
	else
	{
		throw std::out_of_range("Invalid index: Vector2 only supports indices 0, 1 and 2.");
	}
}

float LibMath::Vector3::operator[](int index) const
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
	else
	{
		throw std::out_of_range("Invalid index: Vector2 only supports indices 0, 1 and 2.");
	}
}

LibMath::Radian LibMath::Vector3::angleFrom(Vector3 const& other) const
{
	float dotProduct = dot(other);
	float magnitudes = magnitude() * other.magnitude();

	if (magnitudes == 0.0)
	{
		throw std::runtime_error("Cannot calculate angle with a zero-length vector.");
	}

	float cosTheta = dotProduct / magnitudes;
	return Radian(acos(cosTheta));
}

LibMath::Vector3 LibMath::Vector3::cross(Vector3 const& other) const
{
	float x = m_y * other.m_z - m_z * other.m_y;
	float y = m_z * other.m_x - m_x * other.m_z;
	float z = m_x * other.m_y - m_y * other.m_x;
	return Vector3(x, y, z);
}

float LibMath::Vector3::distanceFrom(Vector3 const& other) const
{
	return LibMath::squareRoot((m_x - other.m_x) * (m_x - other.m_x) + (m_y - other.m_y) * (m_y - other.m_y) + (m_z - other.m_z) * (m_z - other.m_z));
}

float LibMath::Vector3::distanceSquaredFrom(Vector3 const& other) const
{
	return (m_x - other.m_x) * (m_x - other.m_x) + (m_y - other.m_y) * (m_y - other.m_y) + (m_z - other.m_z) * (m_z - other.m_z);
}

float LibMath::Vector3::distance2DFrom(Vector3 const& other) const
{
	return LibMath::squareRoot((m_x - other.m_x) * (m_x - other.m_x) + (m_y - other.m_y) * (m_y - other.m_y));
}

float LibMath::Vector3::distance2DSquaredFrom(Vector3 const& other) const
{
	return (m_x - other.m_x) * (m_x - other.m_x) + (m_y - other.m_y) * (m_y - other.m_y);
}

float LibMath::Vector3::dot(Vector3 const& other) const
{
	return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z);
}

bool LibMath::Vector3::isLongerThan(Vector3 const& other) const
{
	return magnitudeSquared() > other.magnitudeSquared();
}

bool LibMath::Vector3::isShorterThan(Vector3 const& other) const
{
	return magnitudeSquared() < other.magnitudeSquared();
}

bool LibMath::Vector3::isUnitVector() const
{
	return magnitudeSquared() == 1;
}

float LibMath::Vector3::magnitude() const
{
	return LibMath::squareRoot(m_x * m_x + m_y * m_y + m_z * m_z);
}

float LibMath::Vector3::magnitudeSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

LibMath::Vector3& LibMath::Vector3::normalize()
{
	float mag = magnitude();
	m_x /= mag;
	m_y /= mag;
	m_z /= mag;

	return *this;
}

void LibMath::Vector3::projectOnto(Vector3 const& other)
{
	float otherMagnitudeSquared = other.magnitudeSquared();

	if (otherMagnitudeSquared == 0.0f)
	{
		throw std::runtime_error("Cannot project onto a zero-length vector.");
	}

	float scalar = dot(other) / otherMagnitudeSquared;

	m_x = scalar * other.m_x;
	m_y = scalar * other.m_y;
	m_z = scalar * other.m_z;
}

void LibMath::Vector3::reflectOnto(Vector3 const& other)
{
	float oldX = m_x;
	float oldY = m_y;
	float oldZ = m_z;

	projectOnto(other);

	m_x = 2 * m_x - oldX;
	m_y = 2 * m_y - oldY;
	m_z = 2 * m_z - oldZ;
}

void LibMath::Vector3::rotate(Radian roll, Radian pitch, Radian yaw)
{
	// Match glm's yawPitchRoll function
	float tmp_ch = cos(yaw);
	float tmp_sh = sin(yaw);
	float tmp_cp = cos(pitch);
	float tmp_sp = sin(pitch);
	float tmp_cb = cos(roll);
	float tmp_sb = sin(roll);

	float xNew = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
	float yNew = tmp_sb * tmp_cp;
	float zNew = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;

	float xNew2 = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
	float yNew2 = tmp_cb * tmp_cp;
	float zNew2 = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;

	float xNew3 = tmp_sh * tmp_cp;
	float yNew3 = -tmp_sp;
	float zNew3 = tmp_ch * tmp_cp;

	// Multiply vector by rotation matrix (column-major order):
	float newX = xNew * m_x + xNew2 * m_y + xNew3 * m_z;
	float newY = yNew * m_x + yNew2 * m_y + yNew3 * m_z;
	float newZ = zNew * m_x + zNew2 * m_y + zNew3 * m_z;

	m_x = newX;
	m_y = newY;
	m_z = newZ;
}

void	LibMath::Vector3::rotate(Radian angle, Vector3 const& axis)
{
	Vector3 normAxis = axis;
	normAxis.normalize();

	float cosA = cos(angle);
	float sinA = sin(angle);
	float oneMinusCos = 1.0f - cosA;

	float x = m_x;
	float y = m_y;
	float z = m_z;

	float u = normAxis.m_x;
	float v = normAxis.m_y;
	float w = normAxis.m_z;

	m_x = (u * u + (1 - u * u) * cosA) * x +
		(u * v * (oneMinusCos) - w * sinA) * y +
		(u * w * (oneMinusCos) + v * sinA) * z;

	m_y = (u * v * (oneMinusCos) + w * sinA) * x +
		(v * v + (1 - v * v) * cosA) * y +
		(v * w * (oneMinusCos) - u * sinA) * z;

	m_z = (u * w * (oneMinusCos) - v * sinA) * x +
		(v * w * (oneMinusCos) + u * sinA) * y +
		(w * w + (1 - w * w) * cosA) * z;
}

void	LibMath::Vector3::scale(Vector3 const& factor)
{
	m_x *= factor.m_x;
	m_y *= factor.m_y;
	m_z *= factor.m_z;
}

std::string		LibMath::Vector3::string() const
{
	return "{" + std::to_string(m_x) + "," + std::to_string(m_y) + "," + std::to_string(m_z) + "}";
}

std::string		LibMath::Vector3::stringLong() const
{
	return "Vector3{ x:" + std::to_string(m_x) + ", y:" + std::to_string(m_y) + ", z:" + std::to_string(m_z) + " }";
}


void	LibMath::Vector3::translate(Vector3 const& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
}

LibMath::Vector3	LibMath::Vector3::lerp(Vector3 const& A, Vector3 const& B, float t)
{
	return A + (B - A) * t;
}



bool	LibMath::operator==(Vector3 const& v1, Vector3 const& v2)
{
	return v1.m_x == v2.m_x && 
		v1.m_y == v2.m_y && 
		v1.m_z == v2.m_z;
}

bool LibMath::operator!=(Vector3 const& v1, Vector3 const& v2)
{
	return v1.m_x != v2.m_x ||
		v1.m_y != v2.m_y || 
		v1.m_z != v2.m_z;
}

LibMath::Vector3 LibMath::operator-(Vector3 vec)
{
	return Vector3(-vec.m_x, -vec.m_y, -vec.m_z);
}

LibMath::Vector3 LibMath::operator*(Vector3 const& v, float val)
{
	return Vector3(v.m_x * val, v.m_y * val, v.m_z * val);
}

LibMath::Vector3 LibMath::operator/(Vector3 const& v, float val)
{
	return Vector3(v.m_x / val, v.m_y / val, v.m_z / val);
}

LibMath::Vector3& LibMath::operator*=(Vector3& v, float val)
{
	v.m_x *= val;
	v.m_y *= val;
	v.m_z *= val;

	return v;
}

LibMath::Vector3& LibMath::operator/=(Vector3& v, float val)
{
	v.m_x /= val;
	v.m_y /= val;
	v.m_z /= val;

	return v;
}

LibMath::Vector3 LibMath::operator+(Vector3 v1, Vector3 const& v2)
{
	return Vector3(v1.m_x + v2.m_x, v1.m_y + v2.m_y, v1.m_z + v2.m_z);
}

LibMath::Vector3 LibMath::operator-(Vector3 v1, Vector3 const& v2)
{
	return Vector3(v1.m_x - v2.m_x, v1.m_y - v2.m_y, v1.m_z - v2.m_z);
}

LibMath::Vector3 LibMath::operator*(Vector3 v1, Vector3 const& v2)
{
	return Vector3(v1.m_x * v2.m_x, v1.m_y * v2.m_y, v1.m_z * v2.m_z);
}

LibMath::Vector3 LibMath::operator/(Vector3 v1, Vector3 const& v2)
{
	return Vector3(v1.m_x / v2.m_x, v1.m_y / v2.m_y, v1.m_z / v2.m_z);
}

LibMath::Vector3& LibMath::operator+=(Vector3& v1, Vector3 const& v2)
{
	v1.m_x += v2.m_x;
	v1.m_y += v2.m_y;
	v1.m_z += v2.m_z;
	return v1;
}

LibMath::Vector3& LibMath::operator-=(Vector3& v1, Vector3 const& v2)
{
	v1.m_x -= v2.m_x;
	v1.m_y -= v2.m_y;
	v1.m_z -= v2.m_z;
	return v1;
}

LibMath::Vector3& LibMath::operator*=(Vector3& v1, Vector3 const& v2)
{
	v1.m_x *= v2.m_x;
	v1.m_y *= v2.m_y;
	v1.m_z *= v2.m_z;
	return v1;
}

LibMath::Vector3& LibMath::operator/=(Vector3& v1, Vector3 const& v2)
{
	v1.m_x /= v2.m_x;
	v1.m_y /= v2.m_y;
	v1.m_z /= v2.m_z;
	return v1;
}

std::ostream& LibMath::operator<<(std::ostream& os, Vector3 const& vec)
{
	os << vec.string();
	return os;
}

LibMath::Vector4::Vector4() : m_x(0), m_y(0), m_z(0), m_w(0) {}

LibMath::Vector4::Vector4(float val) : m_x(val), m_y(val), m_z(val), m_w(val) {}

LibMath::Vector4::Vector4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

LibMath::Vector4::Vector4(Vector3 const& other, float w) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(w) {}

LibMath::Vector4::Vector4(Vector4 const& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(other.m_w) {}

LibMath::Vector4& LibMath::Vector4::operator=(Vector4 const& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;
	m_w = other.m_w;

	return *this;
}

LibMath::Vector4 LibMath::Vector4::operator+(Vector4 const& other) const
{
	return Vector4(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

LibMath::Vector4 LibMath::Vector4::operator*(float val) const
{
	return Vector4(m_x * val, m_y * val, m_z * val, m_w * val);
}

LibMath::Vector4 LibMath::Vector4::operator/(float val) const
{
	return Vector4(m_x / val, m_y / val, m_z / val, m_w / val);
}

LibMath::Vector4& LibMath::Vector4::operator+=(Vector4 const& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
	m_w += other.m_w;

	return *this;
}

LibMath::Vector4& LibMath::Vector4::operator*=(float val)
{
	m_x *= val;
	m_y *= val;
	m_z *= val;
	m_w *= val;

	return *this;
}

LibMath::Vector4& LibMath::Vector4::operator/=(float val)
{
	m_x /= val;
	m_y /= val;
	m_z /= val;
	m_w /= val;

	return *this;
}

float& LibMath::Vector4::operator[](int index)
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
		throw std::out_of_range("Invalid index: Vector4 only supports indices 0, 1, 2 and 3.");
	}
}

float LibMath::Vector4::operator[](int index) const
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
		throw std::out_of_range("Invalid index: Vector4 only supports indices 0, 1, 2 and 3.");
	}
}

float LibMath::Vector4::dot(Vector4 const& other) const
{
	return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z) + (m_w * other.m_w);
}

float LibMath::Vector4::magnitude() const
{
	return LibMath::squareRoot((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w));
}

float LibMath::Vector4::magnitudeSquared() const
{
	return (m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w);
}

void LibMath::Vector4::homogenize()
{
	m_x /= m_w;
	m_y /= m_w;
	m_z /= m_w;
	m_w = 1;
}

bool LibMath::Vector4::operator==(Vector4 const& other) const
{
	return m_x == other.m_x &&
		m_y == other.m_y &&
		m_z == other.m_z &&
		m_w == other.m_w;
}

bool LibMath::Vector4::operator!=(Vector4 const& other) const
{
	return m_x != other.m_x ||
		m_y != other.m_y ||
		m_z != other.m_z ||
		m_w != other.m_w;
}
