/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#pragma once

#include <gl/gl.h>

struct Vertex
{
	Vertex() : m_numBoneWeights(0) {
		m_boneInfluences[0] = 0;
		m_boneInfluences[1] = 0;
		m_boneInfluences[2] = 0;
		m_boneInfluences[3] = 0;

		m_boneWeights[0] = 0;
		m_boneWeights[1] = 0;
		m_boneWeights[2] = 0;
		m_boneWeights[3] = 0;
	}

	bool operator==(const Vertex& right)
	{
		return(	
			abs(m_position[0] - right.m_position[0]) < 0.0001 &&
			abs(m_position[1] - right.m_position[1]) < 0.0001 &&
			abs(m_position[2] - right.m_position[2]) < 0.0001 &&
			abs(m_normal[0] - right.m_normal[0]) < 0.0001 &&
			abs(m_normal[1] - right.m_normal[1]) < 0.0001 &&
			abs(m_normal[2] - right.m_normal[2]) < 0.0001 &&
			abs(m_texCoord[0] - right.m_texCoord[0]) < 0.0001 &&
			abs(m_texCoord[1] - right.m_texCoord[1]) < 0.0001
		);
	}

	GLfloat m_position[3];
	GLfloat m_normal[3];
	GLfloat m_texCoord[2];
	// influence[0] is associated with weight[0] etc
	GLint m_boneInfluences[4];
	GLfloat m_boneWeights[4];
	GLint m_numBoneWeights;
};

