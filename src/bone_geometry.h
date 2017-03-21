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
};

struct Bone {
    // Bone data structure
    Bone(glm::vec4 s, glm::vec4 e){
        start = s;
        end = e;
        tangent = glm::vec4 (glm::normalize(glm::vec3(end-start)),0);
        int normalInd = abs(tangent.x) < abs(tangent.y) ? 0 : 1;
        normalInd = abs(tangent[normalInd]) < abs(tangent.z) ? normalInd : 2;
        normal = glm::vec4 (0,0,0,1);
        normal[normalInd] = 1;
        normal = glm::cross(tangent,normal);
        normal /= glm::length(normal);
        bd = glm::normalize(glm::cross(tangent,normal));
    }

    glm::vec4 start;
    glm::vec4 end;
    glm::vec4 tangent;
    glm::vec4 normal;
    glm::vec4 bd; // Binormal direction

};


struct Skeleton {
	// FIXME: create skeleton and bone data structures
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
		return 0;
		// FIXME: return number of bones in skeleton
	}
	glm::vec3 getCenter() const { return 0.5f * glm::vec3(bounds.min + bounds.max); }
private:
	void computeBounds();
	void computeNormals();
};

#endif
