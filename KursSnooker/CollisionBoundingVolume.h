#pragma once
#include "ShaderProgram.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class CollisionBoundingVolume
{
public:
	virtual ~CollisionBoundingVolume() = default;
	virtual void Draw(glm::mat4 proj, glm::mat4 view) = 0;
	virtual bool Intersects(const CollisionBoundingVolume& other) const = 0;
	virtual bool IntersectsOBB(const class OBB& other) const = 0;
	virtual std::unique_ptr<CollisionBoundingVolume> clone() const = 0;
	virtual void UpdatePos(glm::vec3 pos) = 0;
	virtual void UpdateRotation(glm::mat4 rot) = 0;
	virtual void UpdateScale(glm::vec3 scl) = 0;
protected:
	glm::vec3 pos;
	glm::vec3 scale;
	glm::mat4 rotation;
	unsigned int VAO;
};

