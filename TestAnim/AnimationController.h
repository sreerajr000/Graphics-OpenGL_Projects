/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once
#include <assimp/scene.h>
//#include <aiScene.h>
#include <map>
#include <vector>
#include "Animation.h"

struct aiScene;
class Bone;
class Mesh;
// Holds a map of all animations and can quickly switch between different animations
class AnimationController
{
public:
	typedef std::map<std::string, Animation> AnimationMap;

	AnimationController(const aiScene* assimpScene);
	void reset() { m_currentAnimation = &m_animations.begin()->second; }
	// Updates the current animation. Time should be the length of the last frame in seconds
	void update(float time, std::vector<Mesh*>& meshes);
	// Precalculates all animations
	void precalculate(std::vector<Mesh*>& meshes);

private:
	AnimationMap	m_animations;
	Animation*		m_currentAnimation;
};
