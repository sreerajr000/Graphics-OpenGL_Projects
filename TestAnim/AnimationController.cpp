/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#include "AnimationController.h"

using namespace std;

AnimationController::AnimationController(const aiScene* assimpScene)
{
	for(size_t animIndex = 0; animIndex < assimpScene->mNumAnimations; ++animIndex)
	{
		m_animations[assimpScene->mAnimations[animIndex]->mName.data] = Animation(assimpScene->mAnimations[animIndex]);
	}

	m_currentAnimation = &m_animations.begin()->second;
}

void AnimationController::precalculate(std::vector<Mesh*>& meshes)
{
	for(auto animIter = m_animations.begin(); animIter != m_animations.end(); ++animIter)
	{
		animIter->second.precalculate(meshes);
	}
}

void AnimationController::update(float time, std::vector<Mesh*>& meshes)
{
	if(m_currentAnimation)
		m_currentAnimation->update(time, meshes);
}
