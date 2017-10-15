/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once

#include <SFML/Window/Window.hpp>
#include <gl/glew.h>
#include <vector>
#include "AnimationController.h"
#include <aiVector3D.h>
#include <aiMatrix4x4.h>
#include <map>

class Mesh;
struct aiScene;
struct aiNode;
// A model contains a animation controller and some meshes. It simply delegates the construction, updating and drawing of these objects.
class Model
{
public:
	Model(const aiScene* scene);
	Model(const Model& rightHand);
	Model& operator=(const Model& rightHand);
	~Model();
	void draw(GLuint bonesLocation) const;
	void update(float time);
private:
	void internalClear();
	void processNode(const aiNode* node, const aiScene* scene, aiMatrix4x4 transform);

	AnimationController		m_animationController;
	std::vector<Mesh*>		m_meshes;
};

