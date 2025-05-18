#include "RegularSceneBuilder.h"

RegularSceneBuilder::RegularSceneBuilder(SceneConfig cfg) : cfg(cfg)
{
	this->Reset();
}

void RegularSceneBuilder::BuildSprite(SceneSpriteBuilderConfig cfg) const
{
	Sprite sprite = CreateSprite(cfg);
	this->scene.get()->AddSprite(sprite);
}

Sprite RegularSceneBuilder::CreateSprite(SceneSpriteBuilderConfig& cfg) const
{
	ObjectAssimpParser<> objectAssimpParser;

	std::vector<std::unique_ptr<CollisionBoundingVolume>> vec = std::move(cfg.boundingVolumes);
	
	Sprite sprite(SpriteConfig{
		.pos = cfg.pos,
		.scale = cfg.scale,
		.rotation = cfg.rotation,
		.model = std::make_shared<Model<>>(cfg.path, objectAssimpParser),
		.g = cfg.g
		});
	for (int i = 0; i < vec.size(); i++) {
		sprite.AddBoundingVolume(std::move(vec[i]));
	}

	return sprite;
}

void RegularSceneBuilder::BuildLightSource(SceneLightSourceBuilderConfig cfg) const
{
	LightSource lightSource(LightSourceConfig{
		.sprite = cfg.sprite,
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

