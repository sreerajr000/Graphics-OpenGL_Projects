/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once

//#include <aiAnim.h>
#include <vector>
#include <map>
#include <assimp/scene.h>

class Mesh;
// Each bone has a animation channel which describes how it's transformed over time
class AnimationChannel
{
public:
	typedef std::vector<aiMatrix4x4> aiMatrix4x4Vector;

	AnimationChannel(const aiNodeAnim* nodeAnim, double ticksPerSecond);
	
	// Calculates a new m_currentTransform based on the given time
	void calculate(double time);
	// Finds the correct bone in the argument mesh and sets the local transformation to m_currentTransform
	void updateTransform(Mesh& mesh);
	// Fetches this channel's bone and sets the matrix at the correct index (same index that bone has in the mesh's bone-vector)
	void setAbsoluteTransform(Mesh& mesh, aiMatrix4x4Vector& matrices);
	const std::string& getName() const { return m_name; }
private:
	void updatePosition(double time);
	void updateRotation(double time);
	void updateScaling(double time);

	// This channel shares this name with a bone, linking them
	std::string m_name;
	double m_ticksPerSecond;
	double m_lastTime;
	// The transform calculated by function calculate
	aiMatrix4x4 m_currentTransform;
	// Position, rotation and scaling with time-base keys. Much like key-frames
	std::vector<aiVectorKey> m_positionKeys;
	std::vector<aiQuatKey> m_rotationKeys;
	std::vector<aiVectorKey> m_scalingKeys;
	size_t m_previousPositionKeyIndex;
	size_t m_previousRotationKeyIndex;
	size_t m_previousScalingKeyIndex;
};
