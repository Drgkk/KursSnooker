#include "RegularSceneBuilder.h"

RegularSceneBuilder::RegularSceneBuilder(SceneConfig cfg) : cfg(cfg)
{
	this->Reset();
}

void RegularSceneBuilder::BuildSprite(SceneSpriteBuilderConfig cfg) const
{
	ObjectAssimpParser<> objectAssimpParser;
	Sprite sprite(SpriteConfig{
		.pos = cfg.pos,
		.scale = cfg.scale,
		.rotation = cfg.rotation,
		.model = std::make_shared<Model<>>(cfg.path, objectAssimpParser),
		.g = cfg.g
		});
	this->scene.get()->AddSprite(sprite);
}

void RegularSceneBuilder::BuildLightSource(SceneLightSourceBuilderConfig cfg) const
{
	LightSource lightSource(LightSourceConfig{
		.sprite = cfg.sprite,
		.ambient = cfg.ambient,
		.diffuse = cfg.diffuse,
		.specular = cfg.specular,
		.intensity = cfg.intensity,
		.settings = cfg.settings->clone()
		});
	this->scene.get()->AddLightSource(lightSource);
}

void RegularSceneBuilder::Reset()
{
	this->scene = std::make_unique<Scene>(cfg);
}

std::unique_ptr<Scene> RegularSceneBuilder::Build()
{
	std::unique_ptr<Scene> pointer = std::move(this->scene);
	this->Reset();
	return pointer;
}

