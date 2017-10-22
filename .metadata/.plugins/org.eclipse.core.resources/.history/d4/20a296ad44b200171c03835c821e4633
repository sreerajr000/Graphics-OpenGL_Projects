/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once

#include <vector>
#include <aiVector3D.h>
#include <aiMatrix4x4.h>
#include <aiScene.h>
#include <map>

// A bone knows it's parent and it's children. It can calculate it's world space transform by hierarchially multiplying it's local transformation with the local transformation of all it's parents. Vertices refer to bones by a list of bone influences (index in a list) and a list of weights.
class Bone
{
public:
	typedef std::map<std::string, Bone*> BoneMap;

	Bone(const aiNode* node, Bone* parent);
	bool operator==(const Bone& rightHand) const;
	// Used after copying a mesh full of bones, needs to copy children aswell. Recursive.
	void updateChildren(BoneMap& bonesByName, Bone* parent = 0);

	// Bones share name with animation channels
	const std::string& getName() { return m_name; }
	const aiMatrix4x4& getGlobalTransform() const { return m_globalTransform; }
	const aiMatrix4x4& getOffsetMatrix() const { return m_offsetMatrix; }
	void setLocalTransform(const aiMatrix4x4& localTransform) { m_localTransform = localTransform; }
	void setOffsetMatrix(const aiMatrix4x4& offMat) { m_offsetMatrix = offMat; m_offsetMatrix.Transpose(); }
	
	void addChild(Bone* child);

	// Calls calculateTransform for this object and does so recursively for all children
	void updateTransforms();
	// Calculates m_globalTransform by hierarcially multiplying m_localTransform by the local transforms of all parents
	void calculateTransform();
private:
	std::vector<Bone*>	m_children;
	Bone*				m_parent;
	std::string			m_name;
	aiMatrix4x4			m_localTransform;
	aiMatrix4x4			m_globalTransform;
	// The bone-to-world transform
	aiMatrix4x4			m_offsetMatrix;
};

