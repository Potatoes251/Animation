#include "MySimulation.h"
#include <LibMath/Arithmetic.h>


void	MySimulation::Init()
{
	initBones(GetSkeletonBoneCount());
	m_walkAnim = Animation("ThirdPersonWalk.anim", m_bones.size());
	m_runAnim = Animation("ThirdPersonRun.anim", m_bones.size());
}

void	MySimulation::Update(float DeltaTimeSeconds)
{
	static float fpsTimer = 0.0f;
	fpsTimer += DeltaTimeSeconds;
	if (fpsTimer > 0.5f)
	{
		printf(" fps: %d    \r", int(1.0f / DeltaTimeSeconds));
		fpsTimer = 0.0f;
	}

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

void MySimulation::initBones(int boneCount)
{
	m_bones.resize(boneCount);
	for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
	{
		const char* name = GetSkeletonBoneName(boneIndex);
		if (name[0] == 'i' && name[1] == 'k')
		{
			m_bones.resize(boneIndex);
			return;
		}

		Bone& bone = m_bones[boneIndex];
		bone.m_parentIndex = GetSkeletonBoneParentIndex(boneIndex);

		GetSkeletonBoneLocalBindTransform(boneIndex, bone.m_bindPoseTransform.m_position.m_x,
			bone.m_bindPoseTransform.m_position.m_y, bone.m_bindPoseTransform.m_position.m_z,
			bone.m_bindPoseTransform.m_rotation.m_w, bone.m_bindPoseTransform.m_rotation.m_x,
			bone.m_bindPoseTransform.m_rotation.m_y, bone.m_bindPoseTransform.m_rotation.m_z);

		if (bone.m_parentIndex >= 0)
		{
			bone.m_globalBindPoseTransform = m_bones[bone.m_parentIndex].m_globalBindPoseTransform * bone.m_bindPoseTransform;
		}
		else
		{
			bone.m_globalBindPoseTransform = bone.m_bindPoseTransform;
		}

		bone.m_inverseBindPoseMatrix = bone.m_globalBindPoseTransform;
		bone.m_inverseBindPoseMatrix.inverse();
	}
}

void	MySimulation::DrawMarkers()
{
	// X axis
	::DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);
	// Y axis
	::DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);
	// Z axis
	::DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);
}

void	MySimulation::DrawLine(LibMath::Vector3 const& start, LibMath::Vector3 const& end, LibMath::Vector3 const& color)
{
	::DrawLine(start.m_x, start.m_y, start.m_z,
		end.m_x, end.m_y, end.m_z,
		color.m_x, color.m_y, color.m_z);
}

void	MySimulation::step1(float DeltaTimeSeconds)
{
	LibMath::Vector3 color(1.0f, 0.0f, 1.0f);

	LibMath::Vector3 offset(0, -100, 0);
	Pose currentPose(m_bones.size());

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		if (bone.m_parentIndex >= 0)
		{
			LibMath::Vector3 position = bone.m_globalBindPoseTransform.m_position;
			LibMath::Vector3 positionParent = m_bones[bone.m_parentIndex].m_globalBindPoseTransform.m_position;
			DrawLine(position + offset, positionParent + offset, color);
		}
	}
}

void	MySimulation::step2(float DeltaTimeSeconds)
{
	LibMath::Vector3 color(1.0f, 0.0f, 1.0f);
	LibMath::Vector3 offset(0, -100, 0);

	m_animTime += DeltaTimeSeconds;

	int frameIndex = int(m_animTime * 30.f) % m_walkAnim.m_frameCount;

	Pose currentPose(m_bones.size());
	Pose& currentKeyFrame = m_walkAnim[frameIndex];

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal = bone.m_bindPoseTransform * currentKeyFrame[i];

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
			DrawLine(position + offset, positionParent + offset, color);
		}
	}
}

void	MySimulation::step3(float DeltaTimeSeconds)
{
	m_animTime += DeltaTimeSeconds;

	int frameIndex = int(m_animTime * 30.f) % m_walkAnim.m_frameCount;

	std::vector<Transform> currentPose(m_bones.size());
	std::vector<LibMath::Matrix4> Matrices;
	Matrices.reserve(m_bones.size());

	Pose& currentKeyFrame = m_walkAnim[frameIndex];

	for (size_t i = 0; i < m_bones.size(); i++)
	{
		Bone const& bone = m_bones[i];
		Transform animatedLocal = bone.m_bindPoseTransform * currentKeyFrame[i];

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

void	MySimulation::step4(float DeltaTimeSeconds)
{
	m_animTime += DeltaTimeSeconds;
	float frameTime = m_animTime * 30.0f;
	Pose pose = evaluateAnimation(frameTime, m_walkAnim.m_frameCount, true);
	applyPose(pose);
}

void	MySimulation::step5(float DeltaTimeSeconds)
{
	constexpr float		blendingTime = 2.f;
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
			float currentDuration = (m_walk ? m_walkAnim.m_frameCount : m_runAnim.m_frameCount) / m_fps;
			m_phase = fmod(m_animTime, currentDuration) / currentDuration;
			m_animActiveTimer = 0.0f;
			m_blending = true;
			return;
		}

		Pose pose = evaluateAnimation(frameTime,
			m_walk ? m_walkAnim.m_frameCount : m_runAnim.m_frameCount,
			m_walk);

		applyPose(pose);
	}
}

void	MySimulation::blend(float DeltaTimeSeconds, float blendingTime)
{
	const float sourceDuration = (m_walk ? m_walkAnim.m_frameCount : m_runAnim.m_frameCount) / m_fps;
	const float destDuration = (m_walk ? m_runAnim.m_frameCount : m_walkAnim.m_frameCount) / m_fps;

	float blendRatio = m_animActiveTimer / blendingTime;
	blendRatio = LibMath::clamp(blendRatio, 0.0f, 1.0f);
	m_animActiveTimer += DeltaTimeSeconds;

	float blendedSpeed = LibMath::lerp(1 / sourceDuration, 1 / destDuration, blendRatio);
	m_animTime += DeltaTimeSeconds * blendedSpeed;

	m_phase += DeltaTimeSeconds * blendedSpeed;

	Pose sourcePose = evaluateAnimation(m_phase * sourceDuration * m_fps,
		m_walk ? m_walkAnim.m_frameCount : m_runAnim.m_frameCount,
		m_walk);

	Pose destPose = evaluateAnimation(m_phase * destDuration * m_fps,
		m_walk ? m_runAnim.m_frameCount : m_walkAnim.m_frameCount,
		!m_walk);

	Pose finalPose = blendPoses(sourcePose, destPose, blendRatio);
	applyPose(finalPose);

	if (blendRatio >= 1.0f)
	{
		m_blending = false;
		m_animTime = m_phase * destDuration;
		m_animActiveTimer = 0.0f;
		m_walk = !m_walk;
	}
}


Pose	MySimulation::evaluateAnimation(float frameTime, int frameCount, bool useWalk) const
{
	int frameIndex = int(frameTime) % frameCount;
	int nextFrameIndex = (frameIndex + 1) % frameCount;
	float alpha = frameTime - static_cast<int>(frameTime);

	Pose pose(m_bones.size());
	Animation const& currentAnim = useWalk ? m_walkAnim : m_runAnim;

	for (size_t i = 0; i < m_bones.size(); ++i)
	{
		Bone const& bone = m_bones[i];

		Transform local;
		local = interpolate(currentAnim[frameIndex][i], currentAnim[nextFrameIndex][i], alpha);
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
		result[i] = interpolate(source[i], destination[i], t);
	}

	return result;
}

void	MySimulation::applyPose(const Pose& pose)
{
	std::vector<LibMath::Matrix4> matrices(pose.size());

	for (size_t i = 0; i < pose.size(); ++i)
	{
		matrices[i] = static_cast<LibMath::Matrix4>(pose[i]) * m_bones[i].m_inverseBindPoseMatrix;
	}

	SetSkinningPose(matrices[0].data(), matrices.size());
}

