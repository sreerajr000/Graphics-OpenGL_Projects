/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#include "Bone.h"

using namespace std;

Bone::Bone(const aiNode* node, Bone* parent):
m_name(node->mName.data),
m_localTransform(node->mTransformation),
m_parent(parent)
{
	m_localTransform.Transpose();
}

void Bone::updateChildren(BoneMap& bonesByName, Bone* parent)
{
	m_parent = parent;

	for(size_t childIndex = 0; childIndex < m_children.size(); ++childIndex)
	{
		Bone* oldChild = m_children[childIndex];
		Bone* newChild = bonesByName.at(oldChild->getName());
		newChild->updateChildren(bonesByName, this);
		m_children[childIndex] = newChild;
	}
}

bool Bone::operator==(const Bone& rightHand) const
{
	return m_name == rightHand.m_name;
}

void Bone::addChild(Bone* child)
{
	m_children.push_back(child);
}

void Bone::updateTransforms()
{
	calculateTransform();
	for(size_t childIndex = 0; childIndex < m_children.size(); ++childIndex)
	{
		m_children[childIndex]->updateTransforms();
	}
}

void Bone::calculateTransform()
{
	m_globalTransform = m_localTransform;
	Bone* parent = m_parent;
	while(parent)
	{
		m_globalTransform *= parent->m_localTransform;
		parent = parent->m_parent;
	}
}
