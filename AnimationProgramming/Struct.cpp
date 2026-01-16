#include "Engine.h"

#include "Struct.h"
#include <LibMath/Arithmetic.h>

Transform::operator LibMath::Matrix4() const
{
	const LibMath::Quaternion& q = m_rotation;
	const LibMath::Vector3& p = m_position;

	const float x = q.m_x;
	const float y = q.m_y;
	const float z = q.m_z;
	const float w = q.m_w;

	return LibMath::Matrix4(
		// Column 0
		1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y + w * z), 2.0f * (x * z - w * y), 0.0f,
		// Column 1
		2.0f * (x * y - w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z + w * x), 0.0f,
		// Column 2
		2.0f * (x * z + w * y), 2.0f * (y * z - w * x), 1.0f - 2.0f * (x * x + y * y), 0.0f,
		// Column 3 (translation)
		p.m_x, p.m_y, p.m_z, 1.0f);
}

Transform	Transform::operator*(Transform const& other) const
{
	Transform result;

	result.m_rotation = m_rotation * other.m_rotation;
	result.m_position = m_position + (m_rotation.rotate(other.m_position));

	return result;
}

Transform interpolate(Transform const& source, Transform const& destination, float alpha)
{
	Transform result;

	result.m_position = LibMath::Vector3::lerp(source.m_position, destination.m_position, alpha);
	result.m_rotation = LibMath::Quaternion::slerp(source.m_rotation, destination.m_rotation, alpha);

	return result;
}

Animation::Animation(char const* name, int boneCount)
{
	m_frameCount = GetAnimKeyCount(name);
	m_keyFrames.resize(m_frameCount);

	for (int frameIndex = 0; frameIndex < m_frameCount; frameIndex++)
	{
		m_keyFrames[frameIndex].resize(boneCount);
		for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
		{
			Transform& transform = m_keyFrames[frameIndex][boneIndex];
			GetAnimLocalBoneTransform(name, boneIndex, frameIndex,
				transform.m_position.m_x, transform.m_position.m_y, transform.m_position.m_z,
				transform.m_rotation.m_w, transform.m_rotation.m_x, transform.m_rotation.m_y, transform.m_rotation.m_z);
		}
	}
}