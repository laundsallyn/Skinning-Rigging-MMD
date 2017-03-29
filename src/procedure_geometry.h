#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>
#include "bone_geometry.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
// FIXME: Add functions to generate the bone mesh.
void create_linemesh(LineMesh&, Skeleton);
void create_default(LineMesh&);
void create_cylinder(LineMesh&, Skeleton, int);
void create_coordinate(LineMesh&, Skeleton, int);

#endif
