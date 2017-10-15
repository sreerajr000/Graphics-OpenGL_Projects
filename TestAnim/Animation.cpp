/*
	Author: Karl Zylinski
	Portfolio and blog: http://zylinski.se

	Use, modify and copy in any way you want
*/

#include "Animation.h"
#include "Bone.h"
#include "Mesh.h"

using namespace std;

Animation::Animation():
m_duration(0.0),
m_elapsedTime(0.0)
{
}

Animation::Animation(const aiAnimation* assimpAnim):
m_name(assimpAnim->mName.data),
m_duration(assimpAnim->mDuration),
m_elapsedTime(0.0)
{
	for(size_t chanIndex = 0; chanIndex < assimpAnim->mNumChannels; ++chanIndex)
	{
		auto assimpChan = assimpAnim->mChannels[chanIndex];
		m_channels.push_back(AnimationChannel(assimpChan, assimpAnim->mTicksPerSecond));
	}
}

void Animation::update(float time, std::vector<Mesh*>& meshes)
{
	if(time + m_elapsedTime > m_duration)
	{
		m_elapsedTime = fmod(float(time), float(m_duration)); // wrap around
	}
	else
	{
		m_elapsedTime += time;
	}

	// probably only runs once
	if(m_previousMatrixKeyIndices.size() != meshes.size())
	{
		m_previousMatrixKeyIndices.resize(meshes.size(), 0);
	}

	for(size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
	{
		std::vector<BoneMatricesKey>& matricesKeys = m_matricesKeysByMeshName[meshes[meshIndex]->getName()];

		// Get previous matrix key index if time hasn't wrapped around, saves iteration time
		size_t keyIndex = m_previousMatrixKeyIndices[meshIndex];
		if(m_elapsedTime < m_previousTime)
			keyIndex = 0;

		for(; keyIndex < matricesKeys.size(); ++keyIndex)
		{
			BoneMatricesKey& matricesKey = matricesKeys[keyIndex];
			
			if(keyIndex == matricesKeys.size() - 1 ||
				m_elapsedTime >= matricesKey.time && m_elapsedTime < matricesKeys[keyIndex + 1].time)
			{
				meshes[meshIndex]->setAbsoluteTransforms(&matricesKey.matrices[0].a1); // found current frame, set pointer to absolute tranformation matrix (only first float in first matrix, mesh knows the number of bones anyways)
				m_previousMatrixKeyIndices[meshIndex] = keyIndex;
				break;
			}
		}
	}

	m_previousTime = m_elapsedTime;
}

void Animation::precalculate(std::vector<Mesh*>& meshes)
{
	double time = 0.0;
	while(time <= m_duration)
	{
		// Go through all channels and update transforms per channel for all meshes
		for(size_t chanIndex = 0; chanIndex < m_channels.size(); ++chanIndex)
		{
			m_channels[chanIndex].calculate(time);

			for(size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
			{
				Mesh& mesh = *meshes[meshIndex];
				AnimationChannel& channel = m_channels[chanIndex];
				channel.updateTransform(mesh);
			}	
		}

		// Tell mesh to update skeleton hierarcially
		for(size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
		{
			meshes[meshIndex]->updateTransforms();
		}

		// The code below is quite complicated. It created key frames which are made up of matrices and times. Since we support multiple meshes, we need to know which matrices belong to which mesh while still working on the same key frame.
		// Used to remember which matrix we're working on per mesh (index = mesh index). -1 means no matrix-key-list has been created yet
		vector<size_t> currentMatricesIndices(meshes.size(), -1);
		for(size_t chanIndex = 0; chanIndex < m_channels.size(); ++chanIndex)
		{
			for(size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex)
			{
				Mesh& mesh = *meshes[meshIndex];
				AnimationChannel& channel = m_channels[chanIndex];

				const std::string& boneName = channel.getName();
				Bone* bone = mesh.getBoneByName(boneName);
				std::vector<BoneMatricesKey>& matrixKeys = m_matricesKeysByMeshName[mesh.getName()];

				std::vector<aiMatrix4x4>* matrices = 0;
				if(currentMatricesIndices[meshIndex] == -1)
				{
					// Create a list of matrices for this point in time, for this mesh. This code is quite confusing :(
					BoneMatricesKey newMatricesKey;
					newMatricesKey.time = time;
					const Mesh::BoneVector& bones = mesh.getBones();
					newMatricesKey.matrices.resize(bones.size());
					for(size_t boneIndex = 0; boneIndex < bones.size(); ++boneIndex)
					{
						// We need to set a transformation for _all bones_, some may not have animations, so they wouldn't be included if we skipped this step.
						newMatricesKey.matrices[boneIndex] = bones[boneIndex]->getOffsetMatrix() * bones[boneIndex]->getGlobalTransform();
					}

					currentMatricesIndices[meshIndex] = matrixKeys.size(); // will find this matrix-key-list next time, for this mesh
					matrixKeys.push_back(newMatricesKey);
					matrices = &matrixKeys.back().matrices;
				}
				else
				{
					matrices = &matrixKeys[currentMatricesIndices[meshIndex]].matrices;
				}

				// Unsure if m_transform should be multiplied with final transformation or not, it is the transformation from the mesh's node. Left out the multiplication for now. Not to be confused with a kind of position variable for the mesh, or maybe it could be used as this?
				(*matrices)[mesh.getBoneIndex(boneName)] = bone->getOffsetMatrix() * bone->getGlobalTransform();
			}	
		}

		time += 1.0/60.0; // One 60th of a second, i.e. 60 fps animation speed. Increase this to get even smoother animation at the expense of memory.
	}
}