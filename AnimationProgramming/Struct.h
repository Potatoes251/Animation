#ifndef STRUCT_H
#define STRUCT_H

#include "LibMath/Vector/Vector3.h"
#include "LibMath/Quaternion.h"
#include "LibMath/Matrix/Matrix4.h"

#include <vector>

struct Transform
{
	LibMath::Vector3		m_position;
	LibMath::Quaternion		m_rotation;

	operator LibMath::Matrix4() const;

	Transform operator*(Transform const&) const;

};
Transform interpolate(Transform const&, Transform const&, float);

struct Bone
{
	Transform				m_bindPoseTransform;
	Transform				m_globalBindPoseTransform;
	LibMath::Matrix4		m_inverseBindPoseMatrix;
	// list of position per keyframe
	std::vector<Transform>	m_walkTransforms;
	std::vector<Transform>	m_runTransforms;
	int						m_parentIndex = -1;
};

using Pose = std::vector<Transform>;

struct Animation
{
	Animation() = default;
	Animation(char const* name, int boneCount);
	std::vector<Pose>	m_keyFrames;
	int					m_frameCount = 0;

	Pose& operator[](int Index) { return m_keyFrames[Index]; }
	Pose const& operator[](int Index) const { return m_keyFrames[Index]; }
};


#endif