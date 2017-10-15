/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once

#include "AnimationChannel.h"

// Represents a set of bone matrices for a specific point in time. Function recalcuate generates these
struct BoneMatricesKey
{
	double time;
	std::vector<aiMatrix4x4> matrices;
};

class Mesh;
class Animation
{
public:
	typedef std::map<std::string, std::vector<BoneMatricesKey> > NamedBoneMatricesKeys;
	typedef std::vector<size_t> MeshIndexToMatrixKeyIndices;

	Animation();
	Animation(const aiAnimation* assimpAnim);
	// Selects the correct matrices from m_matricesKeysByMeshName and sets in for the mesh
	void update(float time, std::vector<Mesh*>& meshes);
	// Fills m_matricesKeysByMeshName by precalculating the matrices (frames per second can be altered, normally 60)
	void precalculate(std::vector<Mesh*>& meshes);

private:
	std::string m_name;
	// A model can have multiple meshes, each one is represented by name here (should maybe use index???)
	NamedBoneMatricesKeys m_matricesKeysByMeshName;
	MeshIndexToMatrixKeyIndices m_previousMatrixKeyIndices;
	std::vector<AnimationChannel> m_channels;
	double m_previousTime;
	double m_elapsedTime;
	double m_duration;
};

