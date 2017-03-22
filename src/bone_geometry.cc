#include "config.h"
#include "bone_geometry.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <glm/gtx/io.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

/*
 * For debugging purpose.
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	size_t count = std::min(v.size(), static_cast<size_t>(10));
	for (size_t i = 0; i < count; ++i) os << i << " " << v[i] << "\n";
	os << "size = " << v.size() << "\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& bounds)
{
	os << "min = " << bounds.min << " max = " << bounds.max;
	return os;
}



// FIXME: Implement bone animation.


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::loadpmd(const std::string& fn)
{
	MMDReader mr;
	mr.open(fn);
	mr.getMesh(vertices, faces, vertex_normals, uv_coordinates);
	computeBounds();
	mr.getMaterial(materials);

	// FIXME: load skeleton and blend weights from PMD file
	//        also initialize the skeleton as needed
	int id = 0;
	glm::vec3 offset;
	int parent;

	while (mr.getJoint(id, offset, parent)) {
		//create Joints with data
		std::cout << "  id = " << id << std::endl;
		std::cout << "    parentID = " << parent << std::endl;
		Joint j(id, offset, parent);

		skeleton.joints.push_back(j);

		++id;
	}
	std::cout << "Number of Joints found: " << id << std::endl;
	std::cout << "Joint List size: " << skeleton.joints.size() << std::endl;

	// If joint.parent == -1, that joint is cannot represent a bone
	// bone is based off end joint -> 

	skeleton.bones.resize(skeleton.joints.size());

	for (int n = skeleton.joints.size() - 1; n > 0; --n) {
		// jointList[n];
		Joint* p = &(skeleton.joints[n]);
		skeleton.constructBone(*p);
		//Bone b(p->parent, *p);

		// bones.push_back(b);
		// (p->parent).addBone(b);

		// std::cout << n << " offset (";
		// std::cout << p->offset[0] << ", ";
		// std::cout << p->offset[1] << ", ";
		// std::cout << p->offset[2] << ")";
		// std::cout << " to joint " << p->parent << std::endl;
	}
}

void Mesh::updateAnimation()
{
	animated_vertices = vertices;
	// FIXME: blend the vertices to animated_vertices, rather than copy
	//        the data directly.
}


void Mesh::computeBounds()
{
	bounds.min = glm::vec3(std::numeric_limits<float>::max());
	bounds.max = glm::vec3(-std::numeric_limits<float>::max());
	for (const auto& vert : vertices) {
		bounds.min = glm::min(glm::vec3(vert), bounds.min);
		bounds.max = glm::max(glm::vec3(vert), bounds.max);
	}
}

void Skeleton::constructBone(Joint j) {
	if (j.parent < 0 || bones[j.id] != nullptr) {
		return;
	}

	constructBone(joints[j.parent]);
	bones[j.id] = new Bone(joints[j.parent], j);
	bones[j.parent]->addChild(bones[j.id]);
	return;
}

void Bone::addChild(Bone* child) {
	children.push_back(child);
	child->parent = this;
}