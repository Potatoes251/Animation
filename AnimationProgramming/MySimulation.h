#ifndef MYSIMULATION_H
#define MYSIMULATION_H

#include "Simulation.h"
#include "Struct.h"

class MySimulation : public ISimulation
{
public:

	void	Init() override;
	void	Update(float DeltaTimeSeconds) override;

	void	initBones(int boneCount);


	void	DrawMarkers();
	void	DrawLine(LibMath::Vector3 const&, LibMath::Vector3 const&, LibMath::Vector3 const&);

	void	step1(float DeltaTimeSeconds);
	void	step2(float DeltaTimeSeconds);
	void	step3(float DeltaTimeSeconds);
	void	step4(float DeltaTimeSeconds);
	void	step5(float DeltaTimeSeconds);

	void	blend(float DeltaTimeSeconds, float blendingTime);

	Pose	evaluateAnimation(float frameTime, int frameCount, bool useWalk) const;
	Pose	blendPoses(const Pose& source, const Pose& destination, float t) const;
	void	applyPose(const Pose& pose);

private:
	constexpr static float	m_fps = 30.0f;

	bool					m_walk = true;
	bool					m_blending = false;
	float					m_animTime = 0;
	float					m_animActiveTimer = 0;
	float					m_phase = 0;
	std::vector<Bone>		m_bones;
	Animation				m_walkAnim;
	Animation				m_runAnim;
};


#endif // !MYSIMULATION_H

