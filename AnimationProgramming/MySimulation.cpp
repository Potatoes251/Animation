#include "MySimulation.h"

Transform& Transform::operator*(Transform& other)
{
	Transform result;

	result.m_rotation = m_rotation * other.m_rotation;
	result.m_position = m_position + (m_rotation.rotate(other.m_position));

	return result;
}


void MySimulation::Init()
{
	int boneCount = GetSkeletonBoneCount();
	bones.reserve(boneCount);

	for (int i = 0; i < boneCount; i++)
	{
		const char* name = GetSkeletonBoneName(i);
		if (name[0] == 'i' && name[1] == 'k')
			continue;

		Bone newBone;
		newBone.parentIndex = GetSkeletonBoneParentIndex(i);
		GetSkeletonBoneLocalBindTransform(i,
			newBone.bindPoseTransform.m_position.m_x,
			newBone.bindPoseTransform.m_position.m_y,
			newBone.bindPoseTransform.m_position.m_z,
			newBone.bindPoseTransform.m_rotation.m_w,
			newBone.bindPoseTransform.m_rotation.m_x,
			newBone.bindPoseTransform.m_rotation.m_y,
			newBone.bindPoseTransform.m_rotation.m_z);

		if (newBone.parentIndex >= 0 && newBone.parentIndex < i)
			newBone.bindPoseTransform = bones[newBone.parentIndex].bindPoseTransform * newBone.bindPoseTransform;

		bones.push_back(newBone);
	}
}

void MySimulation::Update(float DeltaTimeSeconds)
{
	printf(" fps: %d    \r", (int)(1.0f / DeltaTimeSeconds));

	if (DeltaTimeSeconds > 0.1f)
	{
		DeltaTimeSeconds = 1.0f / 60.0f;
	}

	step1(DeltaTimeSeconds);

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
	for (Bone& bone : bones)
	{
		if (bone.parentIndex >= 0)
		{
			LibMath::Vector3 position = bone.bindPoseTransform.m_position;
			LibMath::Vector3 positionParent = bones[bone.parentIndex].bindPoseTransform.m_position;
			DrawLines(position + offset, positionParent + offset, color);
		}
	}
}

void MySimulation::step2(float DeltaTimeSeconds)
{
	LibMath::Vector3 color(1.0f, 0.0f, 1.0f);

	LibMath::Vector3 offset(0, -100, 0);
	for (Bone& bone : bones)
	{
		if (bone.parentIndex >= 0)
		{

		}
	}
}

int main()
{
	MySimulation simulation;
	Run(&simulation, 1400, 800);

	return 0;
}