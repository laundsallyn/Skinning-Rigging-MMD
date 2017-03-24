#ifndef BONE_GEOMETRY_H
#define BONE_GEOMETRY_H

#include <ostream>
#include <vector>
#include <map>
#include <limits>
#include <glm/glm.hpp>
#include <mmdadapter.h>

struct BoundingBox {
	BoundingBox()
		: min(glm::vec3(-std::numeric_limits<float>::max())),
		max(glm::vec3(std::numeric_limits<float>::max())) {}
	glm::vec3 min;
	glm::vec3 max;
};

struct Joint {
	// FIXME: Implement your Joint data structure.
	// Note: PMD represents weights on joints, but you need weights on
	//       bones to calculate the actual animation.
	Joint() {}
	Joint(int i, glm::vec3 off, int p) 
		: id(i), offset(off), parent(p) {}
	int id;
	int parent;
	glm::vec3 offset;
	std::vector<int> children;
} typedef Joint;

struct Bone {
    // Bone data structure
    Bone(Joint s, Joint e){
        start = s;
        end = e;
        tangent = glm::vec3 (glm::normalize(end.offset));
        int normalInd = abs(tangent.x) < abs(tangent.y) ? 0 : 1;
        normalInd = abs(tangent[normalInd]) < abs(tangent.z) ? normalInd : 2;
        normal = glm::vec3 (0,0,0);
        normal[normalInd] = 1;
        normal = glm::cross(tangent,normal);
        normal /= glm::length(normal);
        bd = glm::normalize(glm::cross(tangent,normal));
        length = glm::length(end.offset);
        id = e.id;

        // R = [tvec nvec bvec]
        rotation = glm::mat4(glm::mat3(tangent, normal, bd));
        rotation[3][3] = 1.0f;
        // TODO: change translation matrix
        translation = glm::mat4(1.0f);
    }

    void updateRotation();
    void updateTranslation();

    Joint start;
    Joint end;
    float length;
    float weight;
    glm::vec3 tangent;
    glm::vec3 normal;
    glm::vec3 bd; // Binormal direction
    glm::mat4 translation;
    glm::mat4 rotation;
    int id;
    Bone* parent;
}typedef Bone;


struct Skeleton {
	// FIXME: create skeleton and bone data structures
	std::vector<Joint> joints;
	std::vector<Bone*>  bones;
	std::vector<SparseTuple> weights;

	void constructBone(Joint j);
};

struct Mesh {
	Mesh();
	~Mesh();
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> animated_vertices;
	std::vector<glm::uvec3> faces;
	std::vector<glm::vec4> vertex_normals;
	std::vector<glm::vec4> face_normals;
	std::vector<glm::vec2> uv_coordinates;
	std::vector<Material> materials;
	BoundingBox bounds;
	Skeleton skeleton;

	void loadpmd(const std::string& fn);
	void updateAnimation();
	int getNumberOfBones() const 
	{ 
		// number of joints minus 1?
		return skeleton.bones.size() - 1;
	}
	glm::vec3 getCenter() const { return 0.5f * glm::vec3(bounds.min + bounds.max); }
private:
	void computeBounds();
	void computeNormals();
};

#endif
