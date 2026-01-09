#include "MySimulation.h"

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
		1.0f - 2.0f * (y * y + z * z),
		2.0f * (x * y + w * z),
		2.0f * (x * z - w * y),
		0.0f,

		// Column 1
		2.0f * (x * y - w * z),
		1.0f - 2.0f * (x * x + z * z),
		2.0f * (y * z + w * x),
		0.0f,

		// Column 2
		2.0f * (x * z + w * y),
		2.0f * (y * z - w * x),
		1.0f - 2.0f * (x * x + y * y),
		0.0f,

		// Column 3 (translation)
		p.m_x,
		p.m_y,
		p.m_z,
		1.0f
	);
}

Transform Transform::operator*(Transform const& other) const
{
	Transform result;

	result.m_rotation = m_rotation * other.m_rotation;
	result.m_position = m_position + (m_rotation.rotate(other.m_position));

	return result;
}


void MySimulation::Init()
{
	constexpr char* walkAnim = "ThirdPersonWalk.anim";
	constexpr char* runAnim = "ThirdPersonRun.anim";
	m_walkFrameCount = GetAnimKeyCount(walkAnim);
	m_runFrameCount = GetAnimKeyCount(runAnim);
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
			newBone.m_bindPoseTransform.m_position.m_y, newBone.m_bindPoseTransform.m_position.m_z,
			newBone.m_bindPoseTransform.m_rotation.m_w, newBone.m_bindPoseTransform.m_rotation.m_x,
			newBone.m_bindPoseTransform.m_rotation.m_y, newBone.m_bindPoseTransform.m_rotation.m_z);

		for (int frameIndex = 0; frameIndex < m_walkFrameCount; frameIndex++)
		{
			Transform transform;
			GetAnimLocalBoneTransform(walkAnim, boneIndex, frameIndex,
				transform.m_position.m_x, transform.m_position.m_y, transform.m_position.m_z,
				transform.m_rotation.m_w, transform.m_rotation.m_x, transform.m_rotation.m_y, transform.m_rotation.m_z);

			newBone.m_walkTransforms.push_back(transform);
		}
		for (int frameIndex = 0; frameIndex < m_runFrameCount; frameIndex++)
		{
			Transform transform;
			GetAnimLocalBoneTransform(runAnim, boneIndex, frameIndex,
				transform.m_position.m_x, transform.m_position.m_y, transform.m_position.m_z,
				transform.m_rotation.m_w, transform.m_rotation.m_x, transform.m_rotation.m_y, transform.m_rotation.m_z);

			newBone.m_runTransforms.push_back(transform);
		}

		if (newBone.m_parentIndex >= 0)
		{
			newBone.m_globalBindPoseTransform = m_bones[newBone.m_parentIndex].m_globalBindPoseTransform * newBone.m_bindPoseTransform;
		}
		else
		{
			newBone.m_globalBindPoseTransform = newBone.m_bindPoseTransform;
		}

		newBone.m_inverseBindPoseMatrix = newBone.m_globalBindPoseTransform;
		newBone.m_inverseBindPoseMatrix.inverse();

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

	//DeltaTimeSeconds /= 10;

	//step1(DeltaTimeSeconds);
	//step2(DeltaTimeSeconds);
	//step3(DeltaTimeSeconds);
	//step4(DeltaTimeSeconds);
	step5(DeltaTimeSeconds);

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
			LibMath::Vector3 position = bone.m_globalBindPoseTransform.m_position;
			LibMath::Vector3 positionParent = m_bones[bone.m_parentIndex].m_globalBindPoseTransform.m_position;
			DrawLines(position + offset, positionParent + offset, color);
		}
	}
}

void MySimulation::step2(float DeltaTimeSeconds)
{
	LibMath::Vector3 color(1.0f, 0.0f, 1.0f);

	LibMath::Vector3 offset(0, -100, 0);

	m_animTime += DeltaTimeSeconds;
	int frameIndex = int(m_animTime * 30.f) % m_walkFrameCount;

	std::vector<Transform> currentPose;
	currentPose.resize(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal = bone.m_bindPoseTransform * bone.m_walkTransforms[frameIndex];

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

void MySimulation::step3(float DeltaTimeSeconds)
{
	m_animTime += DeltaTimeSeconds;
	int frameIndex = int(m_animTime * 30.f) % m_walkFrameCount;

	std::vector<Transform> currentPose(m_bones.size());
	std::vector<LibMath::Matrix4> Matrices;
	Matrices.reserve(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal = bone.m_bindPoseTransform * bone.m_walkTransforms[frameIndex];

		if (bone.m_parentIndex >= 0)
		{
			currentPose[i] = currentPose[bone.m_parentIndex] * animatedLocal;
		}
		else
		{
			currentPose[i] = animatedLocal;
		}
		Matrices.push_back(static_cast<LibMath::Matrix4>(currentPose[i]) * bone.m_inverseBindPoseMatrix);
	}

	SetSkinningPose(Matrices[0].data(), Matrices.size());
}

void MySimulation::step4(float DeltaTimeSeconds)
{
	m_animTime += DeltaTimeSeconds;
	float frameTime = m_animTime * 30.0f;

	walk(frameTime);
}

void MySimulation::step5(float DeltaTimeSeconds)
{
	constexpr float blendingTime = .5f;
	constexpr float animDuration = 3.0f; // stay on each anim for 10 seconds

	m_animTime += DeltaTimeSeconds;
	m_animActiveTime += DeltaTimeSeconds; // new timer for how long we've been in this anim
	float frameTime = m_animActiveTime * 30.0f;

	if (m_blending)
	{
		float blendingDelta = m_animActiveTime / blendingTime;
		if (blendingDelta >= 1.0f)
		{
			m_blending = false;
			m_animActiveTime = 0.0f; // reset active timer after blending
			if (m_walk)
				walk(frameTime);
			else
				run(frameTime);
			return;
		}

		if (m_walk)
			blendIntoRun(frameTime, blendingDelta);
		else
			blendIntoWalk(frameTime, blendingDelta);
	}
	else
	{
		// Switch animation only after animDuration seconds
		if (m_animActiveTime >= animDuration)
		{
			m_animActiveTime = 0.0f;
			m_blending = true;
			m_walk = !m_walk;

			if (m_walk)
				blendIntoRun(0, 0);
			else
				blendIntoWalk(0, 0);

			return;
		}

		// Otherwise, just play current animation
		if (m_walk)
			walk(frameTime);
		else
			run(frameTime);
	}
}

void MySimulation::walk(float frameTime)
{
	int frameIndex = int(frameTime) % m_walkFrameCount;
	int nextFrameIndex = (frameIndex + 1) % m_walkFrameCount;

	float alpha = frameTime - floor(frameTime);

	std::vector<Transform> currentPose(m_bones.size());
	std::vector<LibMath::Matrix4> Matrices;
	Matrices.reserve(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal;
		animatedLocal.m_rotation = LibMath::Quaternion::slerp(
			bone.m_walkTransforms[frameIndex].m_rotation, bone.m_walkTransforms[nextFrameIndex].m_rotation, alpha);
		animatedLocal.m_position = LibMath::Vector3::lerp(
			bone.m_walkTransforms[frameIndex].m_position, bone.m_walkTransforms[nextFrameIndex].m_position, alpha);

		animatedLocal = bone.m_bindPoseTransform * animatedLocal;

		if (bone.m_parentIndex >= 0)
		{
			currentPose[i] = currentPose[bone.m_parentIndex] * animatedLocal;
		}
		else
		{
			currentPose[i] = animatedLocal;
		}
		Matrices.push_back(static_cast<LibMath::Matrix4>(currentPose[i]) * bone.m_inverseBindPoseMatrix);
	}

	SetSkinningPose(Matrices[0].data(), Matrices.size());
}

void MySimulation::run(float frameTime)
{
	int frameIndex = int(frameTime) % m_runFrameCount;
	int nextFrameIndex = (frameIndex + 1) % m_runFrameCount;

	float alpha = frameTime - floor(frameTime);

	std::vector<Transform> currentPose(m_bones.size());
	std::vector<LibMath::Matrix4> Matrices;
	Matrices.reserve(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal;
		animatedLocal.m_rotation = LibMath::Quaternion::slerp(
			bone.m_runTransforms[frameIndex].m_rotation, bone.m_runTransforms[nextFrameIndex].m_rotation, alpha);
		animatedLocal.m_position = LibMath::Vector3::lerp(
			bone.m_runTransforms[frameIndex].m_position, bone.m_runTransforms[nextFrameIndex].m_position, alpha);

		animatedLocal = bone.m_bindPoseTransform * animatedLocal;

		if (bone.m_parentIndex >= 0)
		{
			currentPose[i] = currentPose[bone.m_parentIndex] * animatedLocal;
		}
		else
		{
			currentPose[i] = animatedLocal;
		}
		Matrices.push_back(static_cast<LibMath::Matrix4>(currentPose[i]) * bone.m_inverseBindPoseMatrix);
	}

	SetSkinningPose(Matrices[0].data(), Matrices.size());
}

void MySimulation::blendIntoRun(float frameTime, float blendDelta)
{
	int walkFrameIndex = int(frameTime) % m_walkFrameCount;
	int walkNextFrameIndex = (walkFrameIndex + 1) % m_walkFrameCount;
	int runFrameIndex = int(frameTime) % m_runFrameCount;
	int runNextFrameIndex = (runFrameIndex + 1) % m_runFrameCount;

	float alpha = frameTime - floor(frameTime);

	std::vector<Transform> currentPose(m_bones.size());
	std::vector<LibMath::Matrix4> Matrices;
	Matrices.reserve(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal;
		Transform walk;
		walk.m_rotation = LibMath::Quaternion::slerp(
			bone.m_walkTransforms[walkFrameIndex].m_rotation, bone.m_walkTransforms[walkNextFrameIndex].m_rotation, alpha);
		walk.m_position = LibMath::Vector3::lerp(
			bone.m_walkTransforms[walkFrameIndex].m_position, bone.m_walkTransforms[walkNextFrameIndex].m_position, alpha);

		Transform run;
		run.m_rotation = LibMath::Quaternion::slerp(
			bone.m_runTransforms[runFrameIndex].m_rotation, bone.m_runTransforms[runNextFrameIndex].m_rotation, alpha);
		run.m_position = LibMath::Vector3::lerp(
			bone.m_runTransforms[runFrameIndex].m_position, bone.m_runTransforms[runNextFrameIndex].m_position, alpha);

		animatedLocal.m_rotation = LibMath::Quaternion::slerp(
			walk.m_rotation, run.m_rotation, blendDelta);
		animatedLocal.m_position = LibMath::Vector3::lerp(
			walk.m_position, run.m_position, blendDelta);


		animatedLocal = bone.m_bindPoseTransform * animatedLocal;

		if (bone.m_parentIndex >= 0)
		{
			currentPose[i] = currentPose[bone.m_parentIndex] * animatedLocal;
		}
		else
		{
			currentPose[i] = animatedLocal;
		}
		Matrices.push_back(static_cast<LibMath::Matrix4>(currentPose[i]) * bone.m_inverseBindPoseMatrix);
	}

	SetSkinningPose(Matrices[0].data(), Matrices.size());
}

void MySimulation::blendIntoWalk(float frameTime, float blendDelta)
{
	int walkFrameIndex = int(frameTime) % m_walkFrameCount;
	int walkNextFrameIndex = (walkFrameIndex + 1) % m_walkFrameCount;
	int runFrameIndex = int(frameTime) % m_runFrameCount;
	int runNextFrameIndex = (runFrameIndex + 1) % m_runFrameCount;

	float alpha = frameTime - floor(frameTime);

	std::vector<Transform> currentPose(m_bones.size());
	std::vector<LibMath::Matrix4> Matrices;
	Matrices.reserve(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal;
		Transform walk;
		walk.m_rotation = LibMath::Quaternion::slerp(
			bone.m_walkTransforms[walkFrameIndex].m_rotation, bone.m_walkTransforms[walkNextFrameIndex].m_rotation, alpha);
		walk.m_position = LibMath::Vector3::lerp(
			bone.m_walkTransforms[walkFrameIndex].m_position, bone.m_walkTransforms[walkNextFrameIndex].m_position, alpha);

		Transform run;
		run.m_rotation = LibMath::Quaternion::slerp(
			bone.m_runTransforms[runFrameIndex].m_rotation, bone.m_runTransforms[runNextFrameIndex].m_rotation, alpha);
		run.m_position = LibMath::Vector3::lerp(
			bone.m_runTransforms[runFrameIndex].m_position, bone.m_runTransforms[runNextFrameIndex].m_position, alpha);

		animatedLocal.m_rotation = LibMath::Quaternion::slerp(
			run.m_rotation, walk.m_rotation, blendDelta);
		animatedLocal.m_position = LibMath::Vector3::lerp(
			run.m_position, walk.m_position, blendDelta);


		animatedLocal = bone.m_bindPoseTransform * animatedLocal;

		if (bone.m_parentIndex >= 0)
		{
			currentPose[i] = currentPose[bone.m_parentIndex] * animatedLocal;
		}
		else
		{
			currentPose[i] = animatedLocal;
		}
		Matrices.push_back(static_cast<LibMath::Matrix4>(currentPose[i]) * bone.m_inverseBindPoseMatrix);
	}

	SetSkinningPose(Matrices[0].data(), Matrices.size());
}

int main()
{
	MySimulation simulation;
	Run(&simulation, 1400, 800);

	return 0;
}