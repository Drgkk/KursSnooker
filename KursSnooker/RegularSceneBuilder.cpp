#include "RegularSceneBuilder.h"

inline glm::mat3 setInertiaTensorCoeffs(
	float ix, float iy, float iz,
	float ixy = 0.0f, float ixz = 0.0f, float iyz = 0.0f
) {
	glm::mat3 I(0.0f);

	I[0][0] = ix;
	I[1][1] = iy;
	I[2][2] = iz;

	I[1][0] = I[0][1] = -ixy;
	I[2][0] = I[0][2] = -ixz;
	I[2][1] = I[1][2] = -iyz;

	return I;
}

inline glm::mat3 setBlockInertiaTensor(const glm::vec3& halfSizes, float mass) {
	glm::vec3 sq = halfSizes * halfSizes;
	float ix = 0.3f * mass * (sq.y + sq.z);
	float iy = 0.3f * mass * (sq.x + sq.z);
	float iz = 0.3f * mass * (sq.x + sq.y);

	return setInertiaTensorCoeffs(ix, iy, iz);
}


RegularSceneBuilder::RegularSceneBuilder(ShaderProgram& spritesShader, ShaderProgram& lightsShader, glm::vec3 skyboxColor)
	: spritesShader(spritesShader), lightsShader(lightsShader), skyboxColor(skyboxColor)
{
	this->Reset();
}

void RegularSceneBuilder::BuildSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
	glm::mat4 rotation) const
{
	std::shared_ptr<Sprite> sprite = CreateSprite(path, pos, scale, rotation);
	this->scene.get()->AddSprite(sprite);
}

std::shared_ptr<Sprite> RegularSceneBuilder::CreateSprite(std::string const& path, glm::vec3 pos, glm::vec3 scale,
	glm::mat4 rotation) const
{
	ObjectAssimpParser<> objectAssimpParser;

	
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(scale, std::make_shared<Model<>>(path, objectAssimpParser));

	sprite->SetPosition(pos);
	sprite->SetOrientation(glm::quat_cast(rotation));

	return sprite;
}

void RegularSceneBuilder::AddBox(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, glm::vec3 halfSize, ShaderProgram& shaderProgram,
	float inverseMass, float linearDamping, float angularDamping, glm::vec3 velocity, glm::vec3 angularVelocity)
{
	glm::mat4 offset = glm::mat4(1.0f);
	offset[0] = glm::vec4(axisX, 0.0f);
	offset[1] = glm::vec4(axisY, 0.0f);
	offset[2] = glm::vec4(axisZ, 0.0f);
	offset = glm::translate(offset, relativePos);
	std::unique_ptr<Box> box = std::make_unique<Box>(shaderProgram, offset, halfSize, this->scene->GetSprites().back());
	box->body->SetInverseMass(inverseMass);
	box->body->SetLinearDamping(linearDamping);
	box->body->SetAngularDamping(angularDamping);
	box->body->SetVelocity(velocity);
	box->body->SetRotation(angularVelocity);
	glm::mat3 it = setBlockInertiaTensor(halfSize, box->body->GetMass());
	box->body->SetInertiaTensor(it);
	box->body->CalculateDerivedData();
	box->CalculateInternals();

	if(inverseMass > 0.00001f)
		box->body->SetAcceleration(glm::vec3(0.0f, -1.5f, 0.0f));
	else
		box->body->SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
	box->body->SetAwake(true);
	box->body->SetCanSleep(true);

	this->scene->AddCollisionBoundingVolume(std::move(box));
}

void RegularSceneBuilder::AddSphere(glm::vec3 relativePos, glm::vec3 axisX, glm::vec3 axisY, glm::vec3 axisZ, float radius, ShaderProgram& shaderProgram, float inverseMass, float linearDamping, float angularDamping, glm::vec3 velocity, glm::vec3 angularVelocity)
{
	glm::mat4 offset = glm::mat4(1.0f);
	offset[0] = glm::vec4(axisX, 0.0f);
	offset[1] = glm::vec4(axisY, 0.0f);
	offset[2] = glm::vec4(axisZ, 0.0f);
	offset = glm::translate(offset, relativePos);
	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(shaderProgram, offset, radius, this->scene->GetSprites().back());
	sphere->body->SetInverseMass(inverseMass);
	sphere->body->SetLinearDamping(linearDamping);
	sphere->body->SetAngularDamping(angularDamping);
	sphere->body->SetVelocity(velocity);
	sphere->body->SetRotation(angularVelocity);
	glm::vec3 halfSize = glm::vec3(radius, radius, radius);
	glm::mat3 it = setBlockInertiaTensor(halfSize, sphere->body->GetMass());
	sphere->body->SetInertiaTensor(it);
	sphere->body->CalculateDerivedData();
	sphere->CalculateInternals();

	if (inverseMass > 0.00001f)
		sphere->body->SetAcceleration(glm::vec3(0.0f, -1.5f, 0.0f));
	else
		sphere->body->SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
	sphere->body->SetAwake(true);
	sphere->body->SetCanSleep(true);

	this->scene->AddCollisionBoundingVolume(std::move(sphere));
}

void RegularSceneBuilder::BuildLightSource(std::shared_ptr<Sprite> sprite, std::unique_ptr<LightSourceSettings> settings) const
{
	LightSource lightSource(sprite, std::move(settings));
	this->scene.get()->AddLightSource(lightSource);
}

void RegularSceneBuilder::Reset()
{
	this->scene = std::make_unique<Scene>(spritesShader, lightsShader, skyboxColor, 700, 200);
}

std::unique_ptr<Scene> RegularSceneBuilder::Build()
{
	std::unique_ptr<Scene> pointer = std::move(this->scene);
	this->Reset();
	return pointer;
}

