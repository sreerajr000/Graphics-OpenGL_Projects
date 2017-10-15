/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#include "AnimationChannel.h"
#include "Mesh.h"
#include "Bone.h"
#include <iterator>

#define EPSILION 0.000001

using namespace std;

AnimationChannel::AnimationChannel(const aiNodeAnim* nodeAnim, double ticksPerSecond):
m_name(nodeAnim->mNodeName.data),
m_previousPositionKeyIndex(0),
m_previousRotationKeyIndex(0),
m_previousScalingKeyIndex(0),
m_ticksPerSecond(ticksPerSecond)
{
	copy(nodeAnim->mPositionKeys, nodeAnim->mPositionKeys + nodeAnim->mNumPositionKeys, back_inserter(m_positionKeys));
	copy(nodeAnim->mRotationKeys, nodeAnim->mRotationKeys + nodeAnim->mNumRotationKeys, back_inserter(m_rotationKeys));
	copy(nodeAnim->mScalingKeys, nodeAnim->mScalingKeys + nodeAnim->mNumScalingKeys, back_inserter(m_scalingKeys));
}

void AnimationChannel::calculate(double time)
{
	if(m_ticksPerSecond != 0.0)
		time *= m_ticksPerSecond;

	updateRotation(time);
	updateScaling(time);
	updatePosition(time);
	m_currentTransform.Transpose();
	m_lastTime = time;
}

void AnimationChannel::updateTransform(Mesh& mesh)
{
	if(!mesh.hasBone(m_name))
		return; // Skip this mesh

	Bone* bone = mesh.getBoneByName(m_name);
	bone->setLocalTransform(m_currentTransform);
}

void AnimationChannel::updateRotation(double time)
{
	// Start at previous index if time hasn't wrapped
	size_t keyIndex = m_previousRotationKeyIndex;
	if(time < m_lastTime)
	{
		keyIndex = 0;
	}
	
	while(keyIndex < m_rotationKeys.size() - 1)
	{
		if(time < m_rotationKeys[keyIndex + 1].mTime)
			break;

		++keyIndex;
	}

	size_t nextKeyIndex = keyIndex + 1;
	if(nextKeyIndex >= m_rotationKeys.size())
		nextKeyIndex = 0;

	const aiQuatKey& key = m_rotationKeys[keyIndex];
	const aiQuatKey& nextKey = m_rotationKeys[nextKeyIndex];
	double timeDiff = nextKey.mTime - key.mTime;

	aiQuaternion newRotation(1, 0, 0, 0);
	if(fabs(timeDiff) < EPSILION)
	{
		newRotation = key.mValue;
	}
	else if(timeDiff > 0.0)
	{
		double diffFactor = (time - key.mTime) / timeDiff;
		aiQuaternion::Interpolate(newRotation, key.mValue, nextKey.mValue, float(diffFactor));
	}

	m_previousRotationKeyIndex = keyIndex;
	m_currentTransform = aiMatrix4x4(newRotation.GetMatrix());
}

void AnimationChannel::updateScaling(double time)
{
	size_t keyIndex = m_previousScalingKeyIndex;
	if(time < m_lastTime)
	{
		keyIndex = 0;
	}

	while(keyIndex < m_scalingKeys.size() - 1)
	{
		if(time < m_scalingKeys[keyIndex + 1].mTime)
			break;

		++keyIndex;
	}
	
	aiVector3D newScaling(1,1,1);
	newScaling = m_scalingKeys[keyIndex].mValue;
	m_previousScalingKeyIndex = keyIndex;

	m_currentTransform.a1 *= newScaling.x;  m_currentTransform.b1 *= newScaling.x;  m_currentTransform.c1 *= newScaling.x;
	m_currentTransform.a2 *= newScaling.y;  m_currentTransform.b2 *= newScaling.y;  m_currentTransform.c2 *= newScaling.y;
	m_currentTransform.a3 *= newScaling.z;  m_currentTransform.b3 *= newScaling.z;  m_currentTransform.c3 *= newScaling.z;
}

void AnimationChannel::updatePosition(double time)
{
	size_t keyIndex = m_previousPositionKeyIndex;
	if(time < m_lastTime)
		keyIndex = 0;

	while(keyIndex < m_positionKeys.size() - 1)
	{
		if(time < m_positionKeys[keyIndex + 1].mTime)
			break;

		++keyIndex;
	}

	size_t nextKeyIndex = keyIndex + 1;
	if(nextKeyIndex >= m_positionKeys.size())
		nextKeyIndex = 0;

	const aiVectorKey& key = m_positionKeys[keyIndex];
	const aiVectorKey& nextKey = m_positionKeys[nextKeyIndex];
	double timeDiff = nextKey.mTime - key.mTime;

	aiVector3D newPosition(0, 0, 0);
	if(fabs(timeDiff) < EPSILION)
	{
		newPosition = key.mValue;
	}
	else if(timeDiff > 0.0)
	{
		double diffFactor = (time - key.mTime) / timeDiff;
		newPosition = key.mValue + (nextKey.mValue - key.mValue) * float(diffFactor);
	}

	m_previousPositionKeyIndex = keyIndex;
	m_currentTransform.a4 = newPosition.x;
	m_currentTransform.b4  = newPosition.y;
	m_currentTransform.c4  = newPosition.z;
}
