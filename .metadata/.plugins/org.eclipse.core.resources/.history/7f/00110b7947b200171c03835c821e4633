/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include <aiScene.h>

using namespace std;

Model::Model(const aiScene* scene):
m_animationController(scene)
{
	processNode(scene->mRootNode, scene, aiMatrix4x4());
	m_animationController.precalculate(m_meshes);
}

Model::~Model()
{
	internalClear();
}

Model::Model(const Model& rightHand):
m_animationController(rightHand.m_animationController)
{
	m_animationController.reset();

	m_meshes.resize(rightHand.m_meshes.size());
	for(size_t meshIndex = 0; meshIndex < rightHand.m_meshes.size(); ++meshIndex)
		m_meshes[meshIndex] = new Mesh(*rightHand.m_meshes[meshIndex]);
}

Model& Model::operator=(const Model& rightHand)
{
	if(this != &rightHand)
	{
		internalClear();

		m_animationController = rightHand.m_animationController;
		m_animationController.reset();

		m_meshes.resize(rightHand.m_meshes.size());
		for(size_t meshIndex = 0; meshIndex < rightHand.m_meshes.size(); ++meshIndex)
			m_meshes[meshIndex] = new Mesh(*rightHand.m_meshes[meshIndex]);
	}

	return *this;
}

void Model::internalClear()
{
	for(size_t meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex)
		delete m_meshes[meshIndex];
}

void Model::processNode(const aiNode* node, const aiScene* scene, aiMatrix4x4 transform)
{
	// It's important that we keep the nodes transformation even though we don't create a mesh
	transform *= node->mTransformation;

	if(node->mNumMeshes > 0)
	{
		for(size_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
		{
			Mesh* newMesh = new Mesh(scene->mMeshes[node->mMeshes[meshIndex]], node, const_cast<aiNode*>(scene->mRootNode), transform);
			m_meshes.push_back(newMesh);
		}
	}

	for(size_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
		processNode(node->mChildren[childIndex], scene, transform);
}

void Model::draw(GLuint bonesLocation) const
{
	for(size_t meshIndex = 0; meshIndex < m_meshes.size(); ++meshIndex)
	{
		m_meshes[meshIndex]->draw(bonesLocation);
	}
}

void Model::update(float time)
{
	m_animationController.update(time, m_meshes);
}
