#include "Sprite.h"

const float TERMINAL_VELOCITY = 3.0f;

Sprite::Sprite(glm::vec3 pos, glm::vec3 scale,
	glm::mat4 rotation, std::shared_ptr<Model<>> model)
	: scale(scale), model(model)
{
	position = pos;
	orientation = glm::quat_cast(rotation);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->CalculateDerivedData();
}

//Sprite::Sprite(const Sprite& other)
//	: pos(other.pos), rotation(other.rotation), scale(other.scale), model(other.model), g(other.g), velocity(other.velocity)
//{
//	position = other.position;
//	orientation = other.orientation
//	/*for (int i = 0; i < other.boundingVolumes.size(); i++) {
//		this->boundingVolumes.push_back(other.boundingVolumes[i]->clone());
//	}*/
//
//}

void Sprite::Draw(ShaderProgram& shaderProgram, float deltaTime, glm::mat4 proj, glm::mat4 view)
{

	shaderProgram.setMat4("model", glm::scale(this->transformMatrix, this->scale));
	model.get()->Draw(shaderProgram);

	//for (int i = 0; i < boundingVolumes.size(); i++) {
	//	/*boundingVolumes[i]->UpdatePos(pos);
	//	boundingVolumes[i]->UpdateRotation(rotation);
	//	boundingVolumes[i]->UpdateScale(scale);*/
	//	boundingVolumes[i]->Draw(proj, view);
	//}
	//shaderProgram.Use();
}

//void Sprite::AddBoundingVolume(std::unique_ptr<CollisionBoundingVolume> cbv)
//{
//	this->boundingVolumes.push_back(std::move(cbv));
//	for (int i = 0; i < boundingVolumes.size(); i++) {
//		/*boundingVolumes[i]->UpdatePos(pos);
//		boundingVolumes[i]->UpdateRotation(rotation);
//		boundingVolumes[i]->UpdateScale(scale);*/
//	}
//}


//glm::vec3 Sprite::GetPosition()
//{
//	return pos;
//}

//bool Sprite::CheckCollision(const Sprite& other)
//{
//	for (int i = 0; i < this->boundingVolumes.size(); i++) {
//		for (int j = 0; j < other.boundingVolumes.size(); j++) {
//			if (this->boundingVolumes[i]->Intersects(*other.boundingVolumes[j].get())) {
//				std::cout << "Intersection detected, baby!\n";
//			}
//		}
//	}
//	return false;
//}
