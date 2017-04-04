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
        tangent = glm::normalize(end.offset);
        int normalInd = abs(tangent.x) < abs(tangent.y) ? 0 : 1;
        normalInd = abs(tangent[normalInd]) < abs(tangent.z) ? normalInd : 2;
        normal = glm::vec3 (0,0,0);
        normal[normalInd] = 1;
        normal = glm::cross(tangent,normal);
        normal /= glm::length(normal);
        bd = glm::normalize(glm::cross(tangent,normal));
        length = glm::length(end.offset);
        id = e.id;
        pid = s.id;

        // R = [tvec nvec bvec]
  //       rotation = glm::mat4(glm::mat3(tangent, normal, bd));
		// rotation[3][3] = 1.0f;

        absRotation = makeRotateMat(e.offset);
        glm::mat4 pAbsRotInverse = glm::transpose(makeRotateMat(s.offset));
        if (s.parent == -1) {
        	pAbsRotInverse = glm::mat4(1.0f);
        }
        relRotation = pAbsRotInverse * absRotation;
        // TODO: change translation matrix
        glm::vec4 relOffset= pAbsRotInverse * glm::vec4(s.offset, 1);
        translation = glm::mat4(1.0f);
        translation[3][0] = relOffset.x;
        translation[3][1] = relOffset.y;
        translation[3][2] = relOffset.z;

        sRotation = relRotation;
    }

    glm::vec4 getWorldStartPoint();
    glm::vec4 getWorldEndPoint();
    glm::mat4 getAbsRotation(); // [^t ^n ^b] = R1R2...Ri
    glm::mat4 getRelRotation(); // Ri
    glm::mat4 getTranslation(); // Ti
    glm::mat4 getWorldMat();    // T1R1...TiRi
    static glm::mat4 makeRotateMat(glm::vec3 offset);
    glm::mat4 BoneToWorldRotation(); // R1R2...Ri
    glm::vec4 WorldPointFromBone(glm::vec4 p);
    glm::mat4& getDeformedRotation(); //Si

    Joint start;
    Joint end;
    float length;
    glm::vec3 tangent;
    glm::vec3 normal;
    glm::vec3 bd; // Binormal direction
    glm::mat4 translation;
    // glm::mat4 rotation;
    glm::mat4 relRotation;
    glm::mat4 absRotation;
    glm::mat4 sRotation; //deformed
    int id;
    int pid;
    Bone* parent;
}typedef Bone;


struct Skeleton {
	// FIXME: create skeleton and bone data structures
	std::vector<Joint> joints;
	std::vector<Bone*>  bones;
	// std::vector<SparseTuple> weights;

	void constructBone(int jid);
	Bone* getBone(int n);
};

class LineMesh{
public:
	std::vector<glm::vec4> vertices;
	std::vector <glm::uvec2> bone_lines;
	std::vector<glm::vec4> color;
	int currentIndex = 0;

	void clear(){
		vertices.clear();
		bone_lines.clear();
		color.clear();
		currentIndex = 0;
	}
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
	std::vector<std::vector<float> > weight_map;
	BoundingBox bounds;
	Skeleton skeleton;
	LineMesh cylinder;
	LineMesh coordinate;

	void loadpmd(const std::string& fn);
	void updateAnimation();
	int getNumberOfBones() const 
	{ 
		return skeleton.bones.size()-1;
	}
	glm::vec3 getCenter() const { return 0.5f * glm::vec3(bounds.min + bounds.max); }
	Bone* getBone(int n);
private:
	void computeBounds();
	void computeNormals();

};

#endif
