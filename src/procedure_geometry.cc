#include "procedure_geometry.h"
#include "bone_geometry.h"
#include "config.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces)
{
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	floor_faces.push_back(glm::uvec3(0, 1, 2));
	floor_faces.push_back(glm::uvec3(2, 3, 0));
}

// FIXME: create cylinders and lines for the bones
// Hints: Generate a lattice in [-0.5, 0, 0] x [0.5, 1, 0] We wrap this
// around in the vertex shader to produce a very smooth cylinder.  We only
// need to send a small number of points.  Controlling the grid size gives a
// nice wireframe.

void create_linemesh(LineMesh& line_mesh, Skeleton skeleton){
	int j = 0;
	for(int i = 1; i < skeleton.bones.size(); ++i){
		Bone* b = skeleton.bones[i];
		line_mesh.vertices.push_back(b->getWorldCoordMat() * glm::vec4( 0.0,0.0,0.0,1));
		line_mesh.vertices.push_back(b->getWorldCoordMat() * b->rotation * glm::vec4(b->length, 0, 0,1));
		line_mesh.bone_lines.push_back(glm::uvec2(j, j+1));
		j += 2;
	}
}

void create_default(LineMesh& lm){
	lm.vertices.push_back(glm::vec4( 0.0, 0.0, 0.0, 1));
	lm.vertices.push_back(glm::vec4(10.0, 0.0, 0.0, 1));
	lm.bone_lines.push_back(glm::uvec2(0, 1));
	lm.vertices.push_back(glm::vec4( 0.0, 0.0, 0.0,1));
	lm.vertices.push_back(glm::vec4( 0.0,10.0, 0.0,1));
	lm.bone_lines.push_back(glm::uvec2(2, 3));
	lm.vertices.push_back(glm::vec4( 0.0, 0.0, 0.0,1));
	lm.vertices.push_back(glm::vec4( 0.0, 0.0,10.0,1));
	lm.bone_lines.push_back(glm::uvec2(4, 5));
}