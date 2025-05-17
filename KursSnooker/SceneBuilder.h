#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sprite.h"
#include "LightSourceSettings.h"

class SceneBuilder
{
public:
	virtual ~SceneBuilder(){}
	struct SceneSpriteBuilderConfig {
		std::string const& path;
		glm::vec3 pos;
		glm::vec3 scale;
		glm::mat4 rotation;
		float g;
	};
	virtual void BuildSprite(SceneSpriteBuilderConfig cfg) const = 0;
	struct SceneLightSourceBuilderConfig {
		Sprite sprite;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float intensity;
		std::unique_ptr<LightSourceSettings> settings;
	};
	virtual void BuildLightSource(SceneLightSourceBuilderConfig cfg) const = 0;
};

