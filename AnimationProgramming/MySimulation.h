#ifndef MYSIMULATION_H
#define MYSIMULATION_H

#include "Simulation.h"

#include "LibMath/Vector/Vector3.h"
#include "LibMath/Quaternion.h"

#include <vector>

struct Transform
{
	LibMath::Vector3		m_position;
	LibMath::Quaternion		m_rotation;

	Transform operator*(Transform const&) const;
};

struct Bone
{
	Transform				m_bindPoseTransform;
	// list of position per keyframe
	std::vector<Transform>	m_transforms;
	int						m_parentIndex = -1;
};


class MySimulation : public ISimulation
{
public:

	void Init() override;
	void Update(float DeltaTimeSeconds) override;

	void DrawMarkers();
	void DrawLines(LibMath::Vector3 const&, LibMath::Vector3 const&, LibMath::Vector3 const&);

	void step1(float DeltaTimeSeconds);
	void step2(float DeltaTimeSeconds);
	void step3(float DeltaTimeSeconds);
	void step4(float DeltaTimeSeconds);
	void step5(float DeltaTimeSeconds);


private:
	float m_animTime = 0;
	int m_frameCount = 0;
	std::vector<Bone> m_bones;
};


#endif // !MYSIMULATION_H

