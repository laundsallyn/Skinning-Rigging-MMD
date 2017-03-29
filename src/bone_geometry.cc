#include "config.h"
#include "bone_geometry.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <glm/gtx/io.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

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

void printMat(glm::mat4 mat);

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
		// std::cout << "  id = " << id << std::endl;
		// std::cout << "    parentID = " << parent << std::endl;
		Joint j(id, offset, parent);

		skeleton.joints.push_back(j);

		++id;
	}
	mr.getJointWeights(skeleton.weights);
	std::cout << "Number of Joints found: " << id << std::endl;
	std::cout << "Joint List size: " << skeleton.joints.size() << std::endl;

	// If joint.parent == -1, that joint is cannot represent a bone
	// bone is based off end joint -> 

	skeleton.bones.resize(skeleton.joints.size());

	for (int n = skeleton.joints.size() - 1; n > 0; --n) {
		skeleton.constructBone(skeleton.joints[n]);
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
// std::cout << "Skeleton::constructBone: joint.id = " << j.id << std::endl;
	if (j.id <= 0 || bones[j.id] != nullptr) {
		return;
	}

	constructBone(joints[j.parent]);
	Bone *b = new Bone(joints[j.parent], j);
	bones[j.id] = b;
	joints[j.parent].children.push_back(j.id);
	if (j.parent > 0) {
		b->parent = bones[j.parent];
		Joint p = joints[j.parent];
		b->translation[3][0] = p.offset.x;
		b->translation[3][1] = p.offset.y;
		b->translation[3][2] = p.offset.z;
		// b->rotation = glm::inverse(b->parent->rotation) * b->rotation;
	} else {
		b->parent = nullptr;
		// translation and rotation are with respect to world coords
		Joint p = joints[j.parent];
		b->translation[3][0] = p.offset.x;
		b->translation[3][1] = p.offset.y;
		b->translation[3][2] = p.offset.z;

		// b->rotation = glm::mat4(1.0f);
	}
	return;
}

/*
 * For this bone, returns T*R of parent up to,
 * and including, this bone
 * For example, For Bone1 (no parent)
 *     returns T1*R1
 * For Bone3 (1<-2<-3)
       reutrn T1*R1*T2*R2*T3*R3
 */
glm::mat4 Bone::getCoordSys() {

	// TODO: reenable rotation
	if (parent == nullptr) {
		return translation;
	} else {
		return parent->getCoordSys() * translation;
	}
}

glm::vec4 Bone::getWorldCoordStartPoint() {
	glm::vec4 coord = glm::vec4(0, 0, 0, 1);
	if (parent == nullptr) {
		coord = translation * coord;
	} else {
		coord = parent->getCoordSys() * translation * coord;
	}
	return coord;
}

glm::vec4 Bone::getWorldCoordEndPoint() {
	glm::vec4 coord = glm::vec4(length, 0, 0, 1);
	if (parent == nullptr) {
		coord = translation * rotation * coord;
	} else {
		coord = parent->getCoordSys() * translation * rotation * coord;
	}
	return coord;
}

void printMat(glm::mat4 mat) {
	std::cout << "glm::mat4" << std::endl;
	for (int i = 0; i < 4; ++i) {
		std::cout << glm::to_string(glm::row(mat, i)) << std::endl;
	}
	std::cout << std::endl;
}