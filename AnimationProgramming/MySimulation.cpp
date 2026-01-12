#include "MySimulation.h"
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
		2.0f * (x * z + w * y),	2.0f * (y * z - w * x),	1.0f - 2.0f * (x * x + y * y), 0.0f,
		// Column 3 (translation)
		p.m_x, p.m_y, p.m_z, 1.0f);
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
		initBoneTransforms(newBone, boneIndex, walkAnim, runAnim);

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
	printf(" fps: %d    \r", (int)(1 / DeltaTimeSeconds));

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

void MySimulation::initBoneTransforms(Bone& bone, int boneIndex, char const* walkAnim, char const* runAnim) const
{
	GetSkeletonBoneLocalBindTransform(boneIndex, bone.m_bindPoseTransform.m_position.m_x,
		bone.m_bindPoseTransform.m_position.m_y, bone.m_bindPoseTransform.m_position.m_z,
		bone.m_bindPoseTransform.m_rotation.m_w, bone.m_bindPoseTransform.m_rotation.m_x,
		bone.m_bindPoseTransform.m_rotation.m_y, bone.m_bindPoseTransform.m_rotation.m_z);

	for (int frameIndex = 0; frameIndex < m_walkFrameCount; frameIndex++)
	{
		Transform transform;
		GetAnimLocalBoneTransform(walkAnim, boneIndex, frameIndex,
			transform.m_position.m_x, transform.m_position.m_y, transform.m_position.m_z,
			transform.m_rotation.m_w, transform.m_rotation.m_x, transform.m_rotation.m_y, transform.m_rotation.m_z);

		bone.m_walkTransforms.push_back(transform);
	}
	for (int frameIndex = 0; frameIndex < m_runFrameCount; frameIndex++)
	{
		Transform transform;
		GetAnimLocalBoneTransform(runAnim, boneIndex, frameIndex,
			transform.m_position.m_x, transform.m_position.m_y, transform.m_position.m_z,
			transform.m_rotation.m_w, transform.m_rotation.m_x, transform.m_rotation.m_y, transform.m_rotation.m_z);

		bone.m_runTransforms.push_back(transform);
	}
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

void MySimulation::DrawLines(LibMath::Vector3 const& start, LibMath::Vector3 const& end, LibMath::Vector3 const& color)
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
			currentPose[i] = currentPose[bone.m_parentIndex] * animatedLocal;
		else
			currentPose[i] = animatedLocal;
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
	Pose pose = evaluateAnimation(frameTime, m_walkFrameCount, true);
	applyPose(pose);
}

void MySimulation::step5(float DeltaTimeSeconds)
{
	constexpr float		blendingTime = 3.f;
	constexpr float		animDuration = 3.f;

	float	frameTime = m_animTime * 30.0f;


	if (m_blending)
	{
		blend(DeltaTimeSeconds, blendingTime);
	}
	else
	{
		m_animTime += DeltaTimeSeconds;
		m_animActiveTimer += DeltaTimeSeconds;
		if (m_animActiveTimer >= animDuration)
		{
			constexpr float fps = 30.0f;
			float currentDuration = (m_walk ? m_walkFrameCount : m_runFrameCount) / fps;
			m_phase = fmod(m_animTime, currentDuration) / currentDuration;
			m_animActiveTimer = 0.0f;
			m_blending = true;
			return;
		}

		Pose pose = evaluateAnimation(frameTime,
			m_walk ? m_walkFrameCount : m_runFrameCount,
			m_walk);

		applyPose(pose);
	}
}

void MySimulation::blend(float DeltaTimeSeconds, float blendingTime)
{
	constexpr float fps = 30.f;
	float sourceDuration = (m_walk ? m_walkFrameCount : m_runFrameCount) / fps;
	float destDuration = (m_walk ? m_runFrameCount : m_walkFrameCount) / fps;

	// Playback speed = 1 / duration (normalized cycle per second)
	float sourceSpeed = 1.0f / sourceDuration;
	float destSpeed = 1.0f / destDuration;

	float blendRatio = m_animActiveTimer / blendingTime;
	blendRatio = LibMath::clamp(blendRatio, 0.0f, 1.0f);
	m_animActiveTimer += DeltaTimeSeconds;

	float blendedSpeed = LibMath::lerp(sourceSpeed, destSpeed, blendRatio);
	m_animTime += DeltaTimeSeconds * blendedSpeed;

	m_phase += blendedSpeed * DeltaTimeSeconds;
	m_phase = fmod(m_phase, 1.0f);

	//float normalizedTime = fmod(m_animTime, sourceDuration) / sourceDuration;
	//float destTime = normalizedTime * destDuration;
	float sourceTime = m_phase * sourceDuration;
	float destTime = m_phase * destDuration;

	Pose sourcePose = evaluateAnimation(sourceTime * fps,
		m_walk ? m_walkFrameCount : m_runFrameCount,
		m_walk);

	Pose destPose = evaluateAnimation(destTime * fps,
		m_walk ? m_runFrameCount : m_walkFrameCount,
		!m_walk);


	Pose finalPose = blendPoses(sourcePose, destPose, blendRatio);
	applyPose(finalPose);

	if (blendRatio >= 1.0f)
	{
		m_blending = false;
		m_animTime = destTime;
		m_animActiveTimer = 0.0f;
		m_walk = !m_walk;
	}
}


Pose	MySimulation::evaluateAnimation(float frameTime, int frameCount, bool useWalk) const
{
	int frameIndex = int(frameTime) % frameCount;
	int nextFrameIndex = (frameIndex + 1) % frameCount;
	float alpha = frameTime - floor(frameTime);

	Pose pose(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); ++i)
	{
		const Bone& bone = m_bones[i];

		Pose animTransforms = useWalk ? bone.m_walkTransforms : bone.m_runTransforms;

		Transform local;
		local.m_rotation = LibMath::Quaternion::slerp(
			animTransforms[frameIndex].m_rotation,
			animTransforms[nextFrameIndex].m_rotation,
			alpha);

		local.m_position = LibMath::Vector3::lerp(
			animTransforms[frameIndex].m_position,
			animTransforms[nextFrameIndex].m_position,
			alpha);

		local = bone.m_bindPoseTransform * local;

		if (bone.m_parentIndex >= 0)
			pose[i] = pose[bone.m_parentIndex] * local;
		else
			pose[i] = local;
	}

	return pose;
}

Pose	MySimulation::blendPoses(const Pose& source, const Pose& destination, float t) const
{
	Pose result(source.size());

	for (size_t i = 0; i < source.size(); ++i)
	{
		result[i].m_position = LibMath::Vector3::lerp(
			source[i].m_position,
			destination[i].m_position,
			t);

		result[i].m_rotation = LibMath::Quaternion::slerp(
			source[i].m_rotation,
			destination[i].m_rotation,
			t);
	}

	return result;
}

void	MySimulation::applyPose(const Pose& pose)
{
	std::vector<LibMath::Matrix4> matrices;
	matrices.reserve(pose.size());

	for (size_t i = 0; i < pose.size(); ++i)
	{
		matrices.push_back(static_cast<LibMath::Matrix4>(pose[i]) * m_bones[i].m_inverseBindPoseMatrix);
	}

	SetSkinningPose(matrices[0].data(), matrices.size());
}

