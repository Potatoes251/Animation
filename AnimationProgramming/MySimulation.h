#ifndef MYSIMULATION_H
#define MYSIMULATION_H

#include "Simulation.h"

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

class MySimulation : public ISimulation
{
public:

	void	Init() override;
	void	Update(float DeltaTimeSeconds) override;

	void	initBoneTransforms(Bone& bone, int boneIndex, char const* walkAnim, char const* runAnim) const;


	void	DrawMarkers();
	void	DrawLines(LibMath::Vector3 const&, LibMath::Vector3 const&, LibMath::Vector3 const&);

	void	step1(float DeltaTimeSeconds);
	void	step2(float DeltaTimeSeconds);
	void	step3(float DeltaTimeSeconds);
	void	step4(float DeltaTimeSeconds);
	void	step5(float DeltaTimeSeconds);

	void	blend(float blendingTime);

	Pose	evaluateAnimation(float frameTime, int frameCount, bool useWalk) const;
	Pose	blendPoses(const Pose& source, const Pose& destination, float t) const;
	void	applyPose(const Pose& pose);

private:
	bool				m_walk = true;
	bool				m_blending = false;
	float				m_animTime = 0;
	int					m_walkFrameCount = 0;
	int					m_runFrameCount = 0;
	std::vector<Bone>	m_bones;
};


#endif // !MYSIMULATION_H

