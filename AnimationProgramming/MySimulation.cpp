#include "MySimulation.h"

Transform Transform::operator*(Transform const& other) const
{
	Transform result;

	result.m_rotation = m_rotation * other.m_rotation;
	result.m_position = m_position + (m_rotation.rotate(other.m_position));

	return result;
}


void MySimulation::Init()
{
	constexpr char* anim = "ThirdPersonRun.anim";
	m_frameCount = GetAnimKeyCount(anim);
	int boneCount = GetSkeletonBoneCount();
	m_bones.reserve(boneCount);

	for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
	{
		const char* name = GetSkeletonBoneName(boneIndex);
		if (name[0] == 'i' && name[1] == 'k')
			continue;

		Bone newBone;
		newBone.m_parentIndex = GetSkeletonBoneParentIndex(boneIndex);
		GetSkeletonBoneLocalBindTransform(boneIndex,
			newBone.m_bindPoseTransform.m_position.m_x,
			newBone.m_bindPoseTransform.m_position.m_y,
			newBone.m_bindPoseTransform.m_position.m_z,
			newBone.m_bindPoseTransform.m_rotation.m_w,
			newBone.m_bindPoseTransform.m_rotation.m_x,
			newBone.m_bindPoseTransform.m_rotation.m_y,
			newBone.m_bindPoseTransform.m_rotation.m_z);

		for (int frameIndex = 0; frameIndex < m_frameCount; frameIndex++)
		{
			Transform transform;
			GetAnimLocalBoneTransform(anim, boneIndex, frameIndex,
				transform.m_position.m_x,
				transform.m_position.m_y,
				transform.m_position.m_z,
				transform.m_rotation.m_w,
				transform.m_rotation.m_x,
				transform.m_rotation.m_y,
				transform.m_rotation.m_z);

			newBone.m_transforms.push_back(transform);
		}

		m_bones.push_back(newBone);
	}
}

void MySimulation::Update(float DeltaTimeSeconds)
{
	printf(" fps: %d    \r", (int)(1.0f / DeltaTimeSeconds));

	if (DeltaTimeSeconds > 0.1f)
	{
		DeltaTimeSeconds = 1.0f / 60.0f;
	}

	//step1(DeltaTimeSeconds);
	step2(DeltaTimeSeconds);

	DrawMarkers();
}

void MySimulation::DrawMarkers()
{
	// X axis
	DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);
	// Y axis
	DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);
	// Z axis
	DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);
}

void MySimulation::DrawLines(LibMath::Vector3 const& start, 
	LibMath::Vector3 const& end, LibMath::Vector3 const& color)
{
	DrawLine(start.m_x, start.m_y, start.m_z, 
		end.m_x, end.m_y, end.m_z, 
		color.m_x, color.m_y, color.m_z);
}

void MySimulation::step1(float DeltaTimeSeconds)
{
	LibMath::Vector3 color(1.0f, 0.0f, 1.0f);

	LibMath::Vector3 offset(0, -100, 0);
	std::vector<Transform> currentPose;
	currentPose.resize(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		if (bone.m_parentIndex >= 0)
		{
			currentPose[i] = currentPose[bone.m_parentIndex] * bone.m_bindPoseTransform;
		}
		else
		{
			currentPose[i] = bone.m_bindPoseTransform;
		}
	}

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		if (bone.m_parentIndex >= 0)
		{
			LibMath::Vector3 position = currentPose[i].m_position;
			LibMath::Vector3 positionParent = currentPose[bone.m_parentIndex].m_position;
			DrawLines(position + offset, positionParent + offset, color);
		}
	}
}

void MySimulation::step2(float DeltaTimeSeconds)
{
	LibMath::Vector3 color(1.0f, 0.0f, 1.0f);

	LibMath::Vector3 offset(0, -100, 0);

	m_animTime += DeltaTimeSeconds;
	int frameIndex = int(m_animTime * 30.f) % m_frameCount;

	std::vector<Transform> currentPose;
	currentPose.resize(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal = bone.m_bindPoseTransform * bone.m_transforms[frameIndex];

		if (bone.m_parentIndex >= 0)
		{
			currentPose[i] = currentPose[bone.m_parentIndex] * animatedLocal;
		}
		else
		{
			currentPose[i] = animatedLocal;
		}
	}

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		if (bone.m_parentIndex >= 0)
		{
			LibMath::Vector3 position = currentPose[i].m_position;
			LibMath::Vector3 positionParent = currentPose[bone.m_parentIndex].m_position;
			DrawLines(position + offset, positionParent + offset, color);
		}
	}
}

int main()
{
	MySimulation simulation;
	Run(&simulation, 1400, 800);

	return 0;
}