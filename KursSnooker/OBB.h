#pragma once
#include <math.h>
#include "CollisionBoundingVolume.h"
#include "Vertices.h"
#include "Indices.h"
#include "ShaderProgram.h"
#include "Camera.h"

struct OBBConfig {
	glm::vec3 Pos;
	glm::vec3 AxisX;
	glm::vec3 AxisY;
	glm::vec3 AxisZ;
	glm::vec3 HalfSize;
	ShaderProgram& shaderProgram;
};

class OBB : public CollisionBoundingVolume
{
public:
	OBB(OBBConfig cfg);
	void Draw(glm::mat4 proj, glm::mat4 view) override;
	bool Intersects(const CollisionBoundingVolume& other) const override;
	bool IntersectsOBB(const OBB& other) const override;
	std::unique_ptr<CollisionBoundingVolume> clone() const override;
	void UpdatePos(glm::vec3 pos) override;
	void UpdateRotation(glm::mat4 rot) override;
	void UpdateScale(glm::vec3 scl) override;
private:
	glm::vec3 AxisX;
	glm::vec3 AxisY;
	glm::vec3 AxisZ;
	glm::vec3 HalfSize;
	Vertices vertices;
	Indices indices;
	ShaderProgram& shaderProgram;
	bool getSeparatingPlane(const glm::vec3& RPos, const glm::vec3& Plane, const OBB& box1, const OBB& box2) const;
	void setUpDraw();
	void setUpVertices();
	void setUpIndices();
	void setUpTriangleIndices(unsigned int i1, unsigned int i2, unsigned int i3);
};

